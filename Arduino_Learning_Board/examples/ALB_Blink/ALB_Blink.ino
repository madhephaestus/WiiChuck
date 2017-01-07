/*
* Arduino Learning Board Project - Simple LED Blink Example
*
* ALB_Blink - This very simple sketch demonstrates how to configure D13
* as an output and blink the on-board LED.  There is an LED on the Arduino
* Nano connected to D13 that we can control with our code
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/
void setup()
{
	pinMode(13, OUTPUT); // Set Digital Pin 13 as an Output
}

void loop()
{
	digitalWrite(13, HIGH); // Turn the LED ON
	delay(500); // delay (do nothing) for 500ms (1/2 of a second)
	digitalWrite(13, LOW); // Turn the LED OFF
	delay(500); // delay (do nothing) for 500ms (1/2 of a second)
}
