/*************************************************** 
  This is a library for the Adafruit TPA2016D2 Class D Amplifier Breakout

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1712

  This amplifier uses I2C to communicate, 2 pins are required to interface

  Check out the links above for our tutorials and wiring diagrams 

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_TPA2016.h>

Adafruit_TPA2016::Adafruit_TPA2016(void) {
}


boolean Adafruit_TPA2016::begin() {
  Wire.begin();

  return true;
}

// Set the gain in dB!
void Adafruit_TPA2016::setGain(int8_t g) {
  if (g > 30) g = 30;
  if (g < -28) g = -28;

  write8(TPA2016_GAIN, g);
}

// for querying the gain, returns in dB
int8_t Adafruit_TPA2016::getGain(void) {

  return read8(TPA2016_GAIN);
}

// Turn on/off right and left channels
void Adafruit_TPA2016::enableChannel(boolean r, boolean l) {

  uint8_t setup = read8(TPA2016_SETUP);
  if (r)
    setup |= TPA2016_SETUP_R_EN;
  else 
    setup &= ~TPA2016_SETUP_R_EN;
  if (l)
    setup |= TPA2016_SETUP_L_EN;
  else 
    setup &= ~TPA2016_SETUP_L_EN;

  write8(TPA2016_SETUP, setup);
}

// Set to OFF, 1:2, 1:4 or 1:8
void Adafruit_TPA2016::setAGCCompression(uint8_t x) {
  if (x > 3) return; // only 2 bits!
  
  uint8_t agc = read8(TPA2016_AGC);
  agc &= ~(0x03);  // mask off bottom 2 bits
  agc |= x;        // set the compression ratio.
  write8(TPA2016_AGC, agc);
}

void Adafruit_TPA2016::setReleaseControl(uint8_t release) {
  if (release > 0x3F) return; // only 6 bits!

  write8(TPA2016_REL, release);
}

void Adafruit_TPA2016::setAttackControl(uint8_t attack) {
  if (attack > 0x3F) return; // only 6 bits!

  write8(TPA2016_ATK, attack);
}

void Adafruit_TPA2016::setHoldControl(uint8_t hold) {
  if (hold > 0x3F) return; // only 6 bits!

  write8(TPA2016_HOLD, hold);
}

// Turn on power limiter
void Adafruit_TPA2016::setLimitLevelOn(void) {
  uint8_t agc = read8(TPA2016_AGCLIMIT);
  agc &= ~(0x80);  // mask off top bit
  write8(TPA2016_AGCLIMIT, agc);
}

// Turn off power limiter
void Adafruit_TPA2016::setLimitLevelOff(void) {
  uint8_t agc = read8(TPA2016_AGCLIMIT);
  agc |= 0x80;  // turn on top bit
  write8(TPA2016_AGCLIMIT, agc);
}

// Set limit levels
void Adafruit_TPA2016::setLimitLevel(uint8_t limit) {
  if (limit > 31) return;

  uint8_t agc = read8(TPA2016_AGCLIMIT);

  agc &= ~(0x1F);  // mask off bottom 5 bits
  agc |= limit;        // set the limit level.

  write8(TPA2016_AGCLIMIT, agc);
}

void Adafruit_TPA2016::setAGCMaxGain(uint8_t x) {
  if (x > 12) return; // max gain max is 12 (30dB)
  
  uint8_t agc = read8(TPA2016_AGC);
  agc &= ~(0xF0);  // mask off top 4 bits
  agc |= (x << 4);        // set the max gain
  write8(TPA2016_AGC, agc);
}

/************************************************************************/

// Read 1 byte from the i2c bus at 'address'
uint8_t Adafruit_TPA2016::read8(uint8_t address)
{
  uint8_t data;

  Wire.beginTransmission(TPA2016_I2CADDR);
#if ARDUINO >= 100
  Wire.write(address);
#else
  Wire.send(address);
#endif
  Wire.endTransmission();

  Wire.requestFrom(TPA2016_I2CADDR, 1);
  while(!Wire.available());

#if ARDUINO >= 100
  return Wire.read();
#else
  return Wire.receive();
#endif
}

// write 1 byte
void Adafruit_TPA2016::write8(uint8_t address, uint8_t data)
{
  Wire.beginTransmission(TPA2016_I2CADDR);
#if ARDUINO >= 100
  Wire.write(address);
  Wire.write(data);  
#else
  Wire.send(address);
  Wire.send(data);  
#endif
  Wire.endTransmission();
}
