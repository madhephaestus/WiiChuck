
#include <Wire.h>
#include "TWILiquidCrystal.h"

uint8_t m_addr = 50;

// Show which firmware version is on the device.

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(m_addr);

void setup() {                
  Wire.begin();
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  uint8_t version = lcd.getFirmwareVersion();
  Serial.print("Firmware Version: ");
  Serial.println(version,DEC);
}

void loop() {}
