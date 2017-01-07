#include <Wire.h>
#include "Adafruit_FRAM_I2C.h"

/* Example code for the Adafruit I2C FRAM breakout */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5.0V DC
   Connect GROUND to common ground */
   
Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();
uint16_t          framAddr = 0;

void setup(void) {
  Serial.begin(9600);
  
  if (fram.begin()) {  // you can stick the new i2c addr in here, e.g. begin(0x51);
    Serial.println("Found I2C FRAM");
  } else {
    Serial.println("No I2C FRAM found ... check your connections\r\n");
    while (1);
  }
  
  // Read the first byte
  uint8_t test = fram.read8(0x0);
   Serial.print("Restarted "); Serial.print(test); Serial.println(" times");
  // Test write ++
   fram.write8(0x0, test+1);
   
  // dump the entire 32K of memory!
  uint8_t value;
  for (uint16_t a = 0; a < 32768; a++) {
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