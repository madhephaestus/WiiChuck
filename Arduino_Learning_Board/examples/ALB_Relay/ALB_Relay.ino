/*
* Arduino Learning Board Project - 5V LED Module Example
*
* ALB_Relay - This sketch demonstrates how to configure D2 as an output
* and control the 5V Relay.  There is an LED on the Arduino
* Nano connected to D13 that will mirror the actions of the relay
*
* Switch 1 (Built into the Learning Board) is used as an Input to
* control the relay
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

int button; // Current State of the Button (HIGH = Unpushed, LOW = Pushed)

void setup()
{
	pinMode(2, OUTPUT); // Set Digital Pin 2 as an Output (Relay Control)
	pinMode(3, INPUT_PULLUP); // Set Digital Pin 3 as an Input (From the Button)
	pinMode(13, OUTPUT); // Set Digital Pin 13 as an Output (Nano LED)
}

void loop()
{
	button = digitalRead(3); // Read from D3
	
	// Reverse the value of button (! = "Not", so !button = "Not" Button)
	// If Button is HIGH (or 1), !button will be LOW (or 0)
	button = !button;
	digitalWrite(2, button); // Set the relay control to the button
	digitalWrite(13, button); // Mirror the button and relay on the Nano LED
	
	delay(250); // Delay for 250ms to reduce the bounce in the relay
}
