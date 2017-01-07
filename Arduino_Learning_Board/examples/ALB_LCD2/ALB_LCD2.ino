/* 
 * Arduino Learning Board Project - I2C LCD Display Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 */

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_LCD_I2C to include the LCD functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)
#define USE_ALB_LCD_I2C // LCD Library

// NOW include the main ArduinoLearningBoard library
// Based on the defines above, the appropriate modules will be added to the project
#include "ArduinoLearningBoard.h"

ALB_LCD_I2C lcd; // Instantiating with no arguments defaults to Address 0x27, 16 Columns, 2 Rows

int counter = 0;

void setup()
{
	lcd.init(); // Initialize the LCD
	lcd.clear(); // Clear the LCD (set cursor to 0, 0)
	lcd.backlight(); // Turn on the LCD backlight
	lcd.print("Arduino Learning"); // Print "Arduino Learning" on the first line
	lcd.setCursor(5, 1); // Move the cursor to (x, y)  (y=1 is 2nd line)
	lcd.print("Board!"); // Print "Board!" in the middle of the 2nd line
	
	delay(3000); // Wait 3 seconds
	lcd.clear(); // Clear Display
}

void loop()
{
	lcd.setCursor(0, 0);
	lcd.print("counter = "); // Display "counter ="
	lcd.print(counter);
	
	counter++;  // Increment the counter value
	if (counter > 9999) {
		counter = 0; // Reset counter to 0 after it reaches 9999
		lcd.clear();
	}
	delay(150); // Sleep 100 ms (otherwise it will count too fast)
}
