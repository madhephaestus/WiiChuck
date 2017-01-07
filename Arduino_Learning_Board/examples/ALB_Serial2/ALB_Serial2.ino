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

int i = 0; // Declare a variable "i" and initialize it to 0

void setup()
{
	Serial.begin(9600); // Start serial communications at 9600 baud
	Serial.println("Hello World!");
}

void loop()
{
	Serial.print("i = "); // Print "i = " to the serial port
	Serial.println(i);    // Print the value of "i"
	i = i + 1;			  // Add one to "i" (so it's higher for the next time)
	delay(1000);		  // delay 1 second
}
