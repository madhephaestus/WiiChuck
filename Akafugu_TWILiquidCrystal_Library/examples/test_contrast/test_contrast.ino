
#include <Wire.h>
#include "TWILiquidCrystal.h"

uint8_t m_addr = 50;

// This program tests the different Contrasts.
// You can store a preset value in EEPROM with:
// lcd.saveContrast([YOUR_VALUE])
// Your value will be read every time the device boots up.

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(m_addr);

void setup() {                
  Wire.begin();
  Serial.begin(9600);
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.println("Contrast test");
}

uint8_t contrast = 0;
bool raise = true;

void loop() {
  lcd.setCursor(0, 1);
  if(raise)
    lcd.print("Increment: ");
  else
    lcd.print("Decrement: ");
  
  lcd.print("   ");
  lcd.setCursor(11,1);
  lcd.print(contrast, DEC);
  lcd.setContrast(contrast);
  Serial.print("Contrast: ");
  Serial.println(contrast,DEC);
  delay(500);
  
  // As the digital pot has 64 levels we increase/decrease with 4 each time (256/4 == 64)
  contrast += (raise ? 4 : -4);
  if(contrast == 0 || contrast >= 252)
    raise = (raise ? false : true);
}
