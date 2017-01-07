/* 
 * Arduino Learning Board Project - 4-Button Touch Sensor Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 */

/*
Four Button Touch Sensor:
  Out1 -> D2
  Out2 -> D3
  Out3 -> D4
  Out4 -> D5
*/

void setup()
{
	// Configure Touch Buttons as Inputs
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	
	Serial.begin(9600); // Initialize serial communications
}

// Define variables to hold values of buttons
int b1, b2, b3, b4;

void loop()
{
	// Read buttons (HIGH = button pushed, LOW = button not pushed)
	b1 = digitalRead(2);
	b2 = digitalRead(3);
	b3 = digitalRead(4);
	b4 = digitalRead(5);
	
	// Output values of all buttons to serial port
	Serial.print(b1);
	Serial.print(" ");
	Serial.print(b2);
	Serial.print(" ");
	Serial.print(b3);
	Serial.print(" ");
	Serial.println(b4);
	
	delay(500);
}
