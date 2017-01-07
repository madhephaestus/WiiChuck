
#include <Wire.h>
#include "TWILiquidCrystal.h"

uint8_t m_addr = 50;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(m_addr);

void setup() {                
  Wire.begin();
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.println("TWILCD Colortest");
  lcd.println("1234567890123456");
}

void loop() {
  unsigned int rgbColour[3];
  
  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
      lcd.setColor(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(15);
    }
  }
}
