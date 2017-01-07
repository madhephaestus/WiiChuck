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
  
Passive Buzzer:
  - -> Ground
  + -> +5V
  S -> D12
  
*/

// Musical Notes
#define C4 262 // Frequency of the musical note C (4th octave)
#define E4 330 // Frequency of the musical note E
#define G4 392 // Frequency of the musical note G
#define C5 523 // Frequency of the musical note C (5th octave)

void setup()
{
	// Configure Touch Buttons as Inputs
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	
	// Configure Passive Buzzer pin as Outputs
	pinMode(12, OUTPUT);
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

	if (b1)
		tone(12, C4);
	else if (b2)
		tone(12, E4);
	else if (b3)
		tone(12, G4);
	else if (b4)
		tone(12, C5);
	else
		noTone(12);
}
