#include <SPI.h>
#include "Adafruit_FRAM_SPI.h"

/* Example code for the Adafruit SPI FRAM breakout */
uint8_t FRAM_CS = 10;

//Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_CS);  // use hardware SPI

uint8_t FRAM_SCK= 13;
uint8_t FRAM_MISO = 12;
uint8_t FRAM_MOSI = 11;
//Or use software SPI, any pins!
Adafruit_FRAM_SPI fram = Adafruit_FRAM_SPI(FRAM_SCK, FRAM_MISO, FRAM_MOSI, FRAM_CS);

uint16_t          addr = 0;

void setup(void) {
  Serial.begin(9600);
  
  if (fram.begin()) {
    Serial.println("Found SPI FRAM");
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
    while (1);
  }
  
  // Read the first byte
  uint8_t test = fram.read8(0x0);
  Serial.print("Restarted "); Serial.print(test); Serial.println(" times");

  // Test write ++
  fram.writeEnable(true);
  fram.write8(0x0, test+1);
  fram.writeEnable(false);

  fram.writeEnable(true);
  fram.write(0x1, (uint8_t *)"FTW!", 5);
  fram.writeEnable(false);

  // dump the entire 8K of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 8192; a++) {
    value = fram.read8(a);
    if ((a % 32) == 0) {
      Serial.print("\n 0x"); Serial.print(a, HEX); Serial.print(": ");
    }
    Serial.print("0x"); 
    if (value < 0x1) 
      Serial.print('0');
    Serial.print(value, HEX); Serial.print(" ");
  }
}

void loop(void) {

}