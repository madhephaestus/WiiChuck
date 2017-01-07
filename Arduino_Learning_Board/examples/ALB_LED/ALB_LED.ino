/*
* Arduino Learning Board Project - Simple LED Blink Example
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

void setup()
{
	pinMode(10, OUTPUT); // Set Digital Pin 10 as an Output
}

void loop()
{
	digitalWrite(10, HIGH); // Turn the LED ON
	delay(500); // delay (do nothing) for 500ms (1/2 of a second)
	digitalWrite(10, LOW); // Turn the LED OFF
	delay(500); // delay (do nothing) for 500ms (1/2 of a second)
}
