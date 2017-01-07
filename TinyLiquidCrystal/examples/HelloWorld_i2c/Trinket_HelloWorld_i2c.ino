/*
 Demonstration sketch for Adafruit i2c/SPI LCD backpack
 using MCP23008 I2C expander and the Trinket mini microcontroller

 This sketch prints "Hello World!" to the LCD
 and shows a count in seconds.  The circuit:
 
 * 5V to Trinket 5V pin
 * GND to Trinket GND pin
 * CLK to Trinket pin GPIO #0
 * DAT to Trinket pin GPIO #2
*/

// include the library code:
#include <TinyWireM.h>
#include <TinyLiquidCrystal.h>

// Connect via i2c, default address #0 (A0-A2 not jumpered)
TinyLiquidCrystal lcd(0);

void setup() {
  // set up the LCD
  lcd.begin(16, 2);            // our display has 16 cols, 2 rows
  lcd.setBacklight(HIGH);      // Turn on the backlight
  lcd.print("hello, world!");  // Print a message to the LCD.
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}

