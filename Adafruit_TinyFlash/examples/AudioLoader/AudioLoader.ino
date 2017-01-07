// Winbond serial flash loader.  Runs on normal Arduino (e.g. Uno,
// NOT Trinket), works with 'AudioXfer' Processing code on host PC.

#include <Adafruit_TinyFlash.h>
#include <SPI.h>

#define LED A0

// Blink LED (not pin 13) to indicate flash error
void error(uint32_t d) {
  for(;;) {
    digitalWrite(LED, HIGH);
    delay(d);
    digitalWrite(LED, LOW);
    delay(d);
  }
}

void setup() {
  Adafruit_TinyFlash flash;
  uint8_t            buffer[256];
  int                c, index      = 0;
  uint32_t           capacity,
                     address       = 0,
                     bytesExpected = 0x7FFFFFFF,
                     bytesReceived = 0;

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(57600);
  Serial.println("HELLO");      // ACK to host

  capacity = flash.begin();
  Serial.println(capacity);     // Chip size to host
  if(!capacity) error(250);     // Fast blink

  while(!Serial.find("ERASE")); // Wait for ERASE command from host
  if(!flash.eraseChip()) {
    Serial.println("ERROR");
    error(500);                 // Slow blink
  }
  Serial.println("READY");      // ACK to host

  for(;;) {
    // Buffer data until a full page is ready or last packet arrives.
    if((c = Serial.read()) >= 0) {
      buffer[index++] = c;

      // Bytes 3-6 indicate number of samples to follow
      if(++bytesReceived == 6) {
        bytesExpected = (((uint32_t)buffer[2] << 24) |
                         ((uint32_t)buffer[3] << 16) |
                         ((uint32_t)buffer[4] <<  8) |
                                    buffer[5]      ) + 6;
      }

      if((index >= sizeof(buffer)) || (bytesReceived >= bytesExpected)) {
        if(flash.writePage(address, buffer)) {
          digitalWrite(LED, HIGH);
          Serial.print('.');
        } else {
          Serial.print('X');
        }
        address += sizeof(buffer);
        digitalWrite(LED, LOW);
        if((address >= capacity) || (bytesReceived >= bytesExpected)) break;
        index = 0;
      }
    }
  }
}

void loop() { }

