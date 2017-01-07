#include <Adafruit_PS2_Trackpad.h>
/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1251398431
*/

// protocol version 2 http://www.mjmwired.net/kernel/Documentation/input/alps.txt

Adafruit_PS2::Adafruit_PS2() {
}

Adafruit_PS2::Adafruit_PS2(uint8_t c, uint8_t d) {
  constructor(c, d);
}

void Adafruit_PS2::constructor(uint8_t c, uint8_t d) {
  _clk = c;
  _data = d;
}

Adafruit_PS2_Mouse::Adafruit_PS2_Mouse(void) {
}

Adafruit_PS2_Mouse::Adafruit_PS2_Mouse(uint8_t c, uint8_t d) {
  constructor(c, d);


}
Adafruit_PS2_Trackpad::Adafruit_PS2_Trackpad(uint8_t c, uint8_t d) {
  constructor(c, d);
}



boolean Adafruit_PS2_Mouse::begin(void) {

  if (! reset()) return false;

  idle();
  write(ADAFRUIT_PS2_SETPOLL); // we will poll the mouse
  read();
  delayMicroseconds(100);
  inhibit();
}


boolean Adafruit_PS2_Mouse::reset(void) {
  idle();
  if (!command(ADAFRUIT_PS2_RESET)) return false;
  if (read() != 0xAA) return false;
  if (read() != 0x00) return false;
  inhibit();

  return true;
}


boolean Adafruit_PS2_Trackpad::begin(void) {
  if (! reset()) return false;


  // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-13.html sec 13.7
  uint32_t e6 = E6Report();
  uint8_t s1, s2, s3;
  s1 = e6 >> 16;
  s2 = e6 >> 8;
  s3 = e6;

  if (s3 != 0x64) return false; // not a trackpad?
  if (s2 == 0) return false; // not a trackpad?

  Serial.print("Trackpad found with ");
  Serial.print(s2, DEC); 
  Serial.println(" buttons");

  if (s1 & 0x80)  // supports the e7 command report
    E7Report();

  if (!tapMode(false)) return false;
  Serial.println("Tap mode set");
  
  getStatus();

  if (! absoluteMode()) return false;
  Serial.println("Absolute mode set");


  //if (!command(ADAFRUIT_PS2_SETPOLL)) return false;
  //delay(1);
  //if (!command(ADAFRUIT_PS2_SETSTREAMMODE)) return false;

  inhibit();

  return true;
}

uint32_t  Adafruit_PS2_Trackpad::getStatus(void) {
  uint8_t info[3];
  Serial.print("Status: ");

  if (!command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_GETINFO)) 
    return 0;

  info[0] = read();
  Serial.print(" 0x"); Serial.print(info[0], HEX);
  info[1] = read();
  Serial.print(" 0x"); Serial.print(info[1], HEX);
  info[2] = read();
  Serial.print(" 0x"); Serial.println(info[2], HEX);

  return 1;
}

boolean Adafruit_PS2_Trackpad::tapMode(boolean enable) {
  uint8_t cmd, arg;
  if (enable) {
    cmd = ADAFRUIT_PS2_SETRATE;
    arg = 0x0A;
  } else {
    cmd = ADAFRUIT_PS2_SETRES;
    arg = 0;
  }
  
  uint8_t info[3];
  if (!command(ADAFRUIT_PS2_GETINFO)) return false;
  info[0] = read();
  Serial.println(info[0], HEX);
  info[1] = read();
  Serial.println(info[1], HEX);
  info[2] = read();
  Serial.println(info[2], HEX);

  if (!command(ADAFRUIT_PS2_DISABLE)) return false;
  if (!command(ADAFRUIT_PS2_DISABLE)) return false;
  write(cmd);
  write(arg);
  //if (read() != ADAFRUIT_PS2_ACK) return false;
  Serial.println(read(), HEX);
  read();
  return true;
}



uint32_t Adafruit_PS2_Trackpad::E6Report(void) {
  write(ADAFRUIT_PS2_SETRES);
  write(0x0); // argument for setres
  write(ADAFRUIT_PS2_SETSCALE11);
  write(ADAFRUIT_PS2_SETSCALE11);
  write(ADAFRUIT_PS2_SETSCALE11);
  if (!command(ADAFRUIT_PS2_GETINFO)) return 0x0;

  uint8_t reply0, reply1, reply2;
  reply0 = read();
  reply1 = read();
  reply2 = read();

  Serial.print("E6 Reply: 0x"); Serial.print(reply0, HEX);
  Serial.print(" 0x");  Serial.print(reply1, HEX);
  Serial.print(" 0x");  Serial.println(reply2, HEX);

  uint32_t reply = reply0;
  reply <<= 8;
  reply |= reply1;
  reply <<= 8;
  reply |= reply2;

  return reply;
}


uint32_t Adafruit_PS2_Trackpad::E7Report(void) {
  write(ADAFRUIT_PS2_SETRES);
  write(0x0); // argument for setres
  write(ADAFRUIT_PS2_SETSCALE21);
  write(ADAFRUIT_PS2_SETSCALE21);
  write(ADAFRUIT_PS2_SETSCALE21);
  if (!command(ADAFRUIT_PS2_GETINFO)) return 0x0;

  read();

  uint8_t reply0, reply1, reply2;
  reply0 = read();
  reply1 = read();
  reply2 = read();

  Serial.print("E7 Reply: 0x");  Serial.print(reply0, HEX);
  Serial.print(" 0x");  Serial.print(reply1, HEX);
  Serial.print(" 0x");  Serial.println(reply2, HEX);

  uint32_t reply = reply0;
  reply <<= 8;
  reply |= reply1;
  reply <<= 8;
  reply |= reply2;

  return reply;
}


boolean Adafruit_PS2_Trackpad::absoluteMode(void) {
  // alps magic knock, 4 disables before an enable
  if (!command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_DISABLE) ||
      !command(ADAFRUIT_PS2_ENABLE))
    return false;

  if (! command(ADAFRUIT_PS2_SETPOLL)) return false;
  delayMicroseconds(100);
  return true;
}


boolean Adafruit_PS2_Trackpad::readData(void) {
  //write(0x0F);
  uint8_t packet[6];
  for (uint8_t x=0; x<6; x++) {
    packet[x] = read();
    //Serial.print("0x"); Serial.print(packet[x], HEX); Serial.print("\t");
  }

  if (packet[0] != 0xF8) return false;


  gesture = finger = left = right = middle = false;
  if (packet[3] & 0x1) left = true;
  if (packet[3] & 0x2) right = true;
  if (packet[3] & 0x4) middle = true;
  if (packet[2] & 0x2) finger = true;
  if (packet[2] & 0x1) gesture = true;

  if (gesture) {
    //http://marc.info/?l=linux-kernel&m=110708138225873
    //Serial.print("  Gesture!  ");
  }
  x = packet[1] | ((packet[2] & 0x78) << 4);
  y = packet[4] | ((packet[3] & 0x70) << 3);
  z = packet[5];
  
  // Serial.println();

  return true;
}

boolean Adafruit_PS2_Mouse::readData(void) {
  if (!command(ADAFRUIT_PS2_READDATA)) return false;

  status = read();

  middle = right = left = false;
  if (status & 0x04) middle = true;
  if (status & 0x02) right = true;
  if (status & 0x01) left = true;

  x = read();
  y = read();
}

uint8_t Adafruit_PS2_Mouse::readID(void) {
  if (!command(ADAFRUIT_PS2_GETDEVICEID)) return -1;

  return read();
}

boolean Adafruit_PS2::command(uint8_t cmd)  {
  write(cmd);
  return (read() == ADAFRUIT_PS2_ACK);
}


uint8_t Adafruit_PS2::read(void) {
  uint16_t d = 0;
  
  idle();
  delayMicroseconds(50);
  // wait for clock line to drop

  // start bit + 8 bits data + parity + stop = 11 bits  
  for (uint8_t i=0; i<11; i++) {
    while (digitalRead(_clk));
    if (digitalRead(_data))
      d |= _BV(i);
    while (!digitalRead(_clk));
  }
  inhibit();
  // drop start bit
  d >>= 1;
  //Serial.print("d = 0x");
  //Serial.println(d, HEX);
  // check parity & stop bit later?
  return d & 0xFF;
  
}

void Adafruit_PS2::write(uint8_t x)  {
  uint16_t tosend = x;
  uint8_t parity = 1;
  
  for (uint8_t i=0; i<8; i++) {
    parity ^= (tosend >> i);
  }
  if (parity & 0x1)
    tosend |= 0x100;

  idle();
  delayMicroseconds(300);
  low(_clk);
  delayMicroseconds(100);
  low(_data);
  delayMicroseconds(10);

  // we pull the clock line up to indicate we're ready
  high(_clk);
  // wait for the device to acknowledge by pulling it down
  while (digitalRead(_clk));
  
  for (uint8_t i=0; i < 9; i++) {
    if (tosend & 0x1)
      high(_data);
    else
      low(_data);
    
    // the clock lines are driven by the -DEVICE- so we wait
    while (!digitalRead(_clk));
    while (digitalRead(_clk));

    tosend >>= 1;
  }  

  // send stop bit (high)
  high(_data);
  delayMicroseconds(50);
  while (digitalRead(_clk));
  // wait for mouse to switch modes
  while (!digitalRead(_clk) || !digitalRead(_data));

  // inhibit any more data (we will poll!)
  inhibit();
}



void Adafruit_PS2::inhibit(void) {
  high(_data);
  low(_clk);
}

void Adafruit_PS2::idle(void) {
  high(_clk);
  high(_data);
}

// pull high
void Adafruit_PS2::high(uint8_t p) {
  pinMode(p, INPUT);
  digitalWrite(p, HIGH);
}
// drive low
void Adafruit_PS2::low(uint8_t p) {
  digitalWrite(p, LOW);
  pinMode(p, OUTPUT);
}
