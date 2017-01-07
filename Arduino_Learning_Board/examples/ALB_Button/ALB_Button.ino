/*
* Arduino Learning Board Project - Reading a Learning Board Button
*
* ALB_Button - This relatively simple sketch demonstrates how to configure D12
* as an Input to read a button.  It also configures D13 as an output to blink
* both the on-board LED and an LED hooked to D13 in response to the button
* being pressed.
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

// Learning Board LED is connected to D13 and Ground
// Learning Board Button is connected to D12 and Ground

int blink = true; // Should we be blinking, start with Yes
int ledState = LOW; // ledState used to set the current state of the LED
unsigned long previousMillis = 0; // Hold the last time the LED was updated

void setup()
{
	pinMode(12, INPUT_PULLUP); // Set Digital Pin 12 as an Input with a Pullup to 5V
	pinMode(13, OUTPUT); // Set Digital Pin 13 as an Output
}

void loop()
{
	// See if button is being pushed
	// BUtton UN-Pushed returns high (pull-up)
	// Button PUSHED returns LOW (switch connects D12 to Ground)
	// Notice the double == in the line below
	// - a single = sets the two sides equal (not what we want)
	// - a double == Compares the two sides
	if (digitalRead(12) == LOW)
	{
		// Button is pressed, wait until it's released
		while (digitalRead(12) == LOW)
			; // Do nothing while we wait
		blink = !blink; // Reverse the value of blink (if it was true, make it false, and vice-versa)
		ledState = LOW;
		digitalWrite(13, ledState);
		delay(200); // Delay for 200ms to ignore any "bounce" in the switch
	}

	if (blink)
	{
		if (millis() - previousMillis >= 500)
		{
			previousMillis = millis(); // Store the last time you blinked the LED

			// if the LED is off turn it on and vice-versa:
			if (ledState == LOW)
			{
				ledState = HIGH;
			}
			else
			{
				ledState = LOW;
			}

			digitalWrite(13, ledState);
		}
	}
}
