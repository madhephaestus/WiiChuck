/*
* Arduino Learning Board Project - I2C Address Scanner Example
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_LCD_I2C to include the LCD functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)
#define USE_ALB_I2C_Scan // LCD Library

// NOW include the main ArduinoLearningBoard library
// Based on the defines above, the appropriate modules will be added to the project
#include <ArduinoLearningBoard.h>

ALB_I2C_Scan i2c_scan;

void setup()
{
	// Open serial port at 9600 baud for debugging
	Serial.begin(9600);

	i2c_scan.scan();
}

void loop()
{
}
