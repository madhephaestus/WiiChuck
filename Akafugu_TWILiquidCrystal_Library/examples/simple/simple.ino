
#include <Wire.h>
#include "TWILiquidCrystal.h"

uint8_t m_addr = 50;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(m_addr);

void setup() {                
  Wire.begin();
  
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(13, OUTPUT);
  
  delay(100);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.println("Hello world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}
