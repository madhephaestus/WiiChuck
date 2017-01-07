/*
 * Arduino Learning Board Project - Servo Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 *
 */

// Brown -> GND
// Red -> 5V
// Orange -> D2

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_Servo to include the Servo functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)
#define USE_ALB_Servo

// NOW include the main ArduinoLearningBoard library
// Based on the defines above, the appropriate modules will be added to the project
#include "ArduinoLearningBoard.h"

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup()
{
	myservo.attach(2);  // attaches the servo on pin 2 to the servo object
	myservo.write(90);  // Center Servo
	delay(3000);        // Delay 3 seconds
}

void loop()
{
	for(pos = 0; pos <= 180; pos++) // goes from 0 degrees to 180 degrees
	{
		myservo.write(pos);         // tell servo to go to position in variable 'pos'
		delay(10);                  // waits 10ms for the servo to reach the position
	}
	for(pos = 180; pos >= 0; pos--) // goes from 180 degrees to 0 degrees
	{
		myservo.write(pos);         // tell servo to go to position in variable 'pos'
		delay(10);                  // waits 10ms for the servo to reach the position
	}
	
}
