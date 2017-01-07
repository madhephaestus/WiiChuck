/*
* Arduino Learning Board Project - Serial Port Example
*
* ALB_Serial - This simple sketch demonstrates how to use the serial port
* to send messages back to the computer.  This is often used to send status
* or diagnostic information back to the computer to make troubleshooting
* a new program easier.
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

void setup()
{
	// Start serial communications at 9600 baud
	Serial.begin(9600);

	// Print "Hello World!" and move to the next line
	Serial.println("Hello World!");
}

void loop()
{
}
