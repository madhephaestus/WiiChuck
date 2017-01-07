/*
* Arduino Learning Board Project - Tilt Sensor
*
* ALB_TiltSensor - This sketch demonstrates how to monitor a digiital input
* pin connected to a Tilt Sensor and turn on the Nano LED (D13) when a tilt
* is detected.
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

int tiltPin = 2; // Tilt sensor is connected between D2 and GND
int ledPin = 13; // Using internal LED on pin D13
int tiltValue;

void setup()
{
	pinMode(tiltPin, INPUT_PULLUP); // Configure Tilt Pin as Input w/ Pullup
	pinMode(ledPin, OUTPUT);		// Configure LED pin as Output
	digitalWrite(ledPin, LOW);		// Initially turn off LED
}

void loop()
{
	tiltValue = digitalRead(tiltPin); // Read current value from digital input
	// When the sensor is still, the two pins are connected and 
	// D2 is pulled LOW.  On motion, the switch opens and the digital input
	// will report a High value
	if (tiltValue == HIGH) {
		digitalWrite(ledPin, HIGH);  // Turn on the LED
		delay(1500);	// Delay 1.5 Seconds so we can see that the LED turned on
		digitalWrite(ledPin, LOW);   // Turn off the LED
	}
}
