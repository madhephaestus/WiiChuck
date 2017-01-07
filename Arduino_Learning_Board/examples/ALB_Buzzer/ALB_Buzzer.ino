/*
* Arduino Learning Board Project - Passive Buzzer Example
*
* This example uses the tone function to create a siren
* sound on the passive buzzer.  It starts with a low frequency and
* increases to a higher frequency, then back down again and repeats
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

int buzzer_pin = 3; // What pin is the buzzer connected to
int i = 0;

void setup()
{
	pinMode(buzzer_pin, OUTPUT); // Configure the buzzer pin as an output pin
}

void loop()
{
	// Play low to high frequencies
	for(i = 25; i < 120; i++)
	{
		tone(buzzer_pin, 20 * i, 200); // Create a tone/note from 500 to 2400 Hz
		delay(20);
	}
	
	// Now play high to low frequencies
	for(i = 120; i >= 25; i--)
	{
		tone (buzzer_pin, 20 * i, 200); // Create a tone/note from 2400 Hz to 500 hz
		delay(20);
	}
}
