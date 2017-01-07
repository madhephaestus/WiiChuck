/* 
 * Arduino Learning Board Project - Dual Axis Joystick Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 */

#define Vx A0 // Define / Equate "Vx" with A0, the pin where Vx is connected
#define Vy A1 // Define / Equate "Vy" with A1, the pin where Vy is connected
#define Button A2 // Define / Equate Button with A2, the pin where the button is connected

void setup()
{
	pinMode(Vx, INPUT); // Configure Vx (A0) as an Input
	pinMode(Vy, INPUT); // Configure Vy (A1) as an Input
	pinMode(Button, INPUT_PULLUP); // Configure Button (A2) as an Input, internally "pulled-up" to 5V
	                               // Note, we're configuring an Analog input as digital input
	                               // which is perfectly fine.  I did this to make the wiring easier
	                               // and keep all of the wires on the same side of the board

	Serial.begin(9600); // Initialize Serial Port at 9600 baud to display the results
}

void loop()
{
	int x, y, btn;
	
	x = analogRead(Vx);	// Read the analog value of Vx (Analog Values are from 0-1023 which equate to 0V to 5V)
	y = analogRead(Vy); // Read the analog value of Vy
	btn = digitalRead(Button); // Read the button.  When the button is open (unpushed),
	                           // the input will read High (+5V)
	                           // When the button is closed (pressed), the input pin
	                           // is connected to ground and will read Low (0V)
	
	Serial.print(x);	// Print the X value to the serial port
	Serial.print("\t"); // Print a Tab character 
	Serial.print(y); 	// Print the Y value
	Serial.print("\t"); // Print a Tab
	Serial.println(btn); // Print the value of the Btn (0=Pushed, 1 = Not Pushed)
	
	delay(250); // Delay 250ms so the results don't print too quickly
}
