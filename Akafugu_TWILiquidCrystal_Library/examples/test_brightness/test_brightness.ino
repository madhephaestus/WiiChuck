
#include <Wire.h>
#include "TWILiquidCrystal.h"

uint8_t m_addr = 50;

// This program tests the different vales for brightness.
// You can store a preset value in EEPROM with:
// lcd.saveBrightness([YOUR_VALUE])
// Your value will be read every time the device boots up.

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(m_addr);

void setup() {                
  Wire.begin();
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.println("Brightness test");
}

uint8_t brightness = 0;
bool raise = true;

void loop() {
  lcd.setCursor(0, 1);
  if(raise)
    lcd.print("Increment: ");
  else
    lcd.print("Decrement: ");
  
  lcd.print("   ");
  lcd.setCursor(11,1);
  lcd.print(brightness, DEC);
  lcd.setBrightness(brightness);
  Serial.print("Brightness: ");
  Serial.println(brightness,DEC);
  delay(250);
  
  brightness += (raise ? 1 : -1);
  if(brightness == 0 || brightness >= 252)
    raise = (raise ? false : true);
}
