#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(int dataPin, int clockPin, int latchPin) : dataPin(dataPin), clockPin(clockPin), latchPin(latchPin)
{
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	digitalWrite(dataPin, LOW);
	digitalWrite(clockPin, LOW);
	digitalWrite(latchPin, LOW);
}

void ShiftRegister::send(bool data[], unsigned int length)
{
	digitalWrite(latchPin, LOW);

	for (int i = length - 1; i >= 0; i--)
	{
		digitalWrite(clockPin, LOW);
		digitalWrite(dataPin, data[i] ? HIGH : LOW);
		digitalWrite(clockPin, HIGH);
	}

	digitalWrite(clockPin, LOW);
	digitalWrite(latchPin, HIGH);
}

void ShiftRegister::send(byte data[], unsigned int length)
{
	digitalWrite(latchPin, LOW);

	for (int i = length - 1; i >= 0; i--)
	{
		for (int j = 0; j < 8; j++)
		{
			digitalWrite(clockPin, LOW);
			digitalWrite(dataPin, (data[i] & (1 << j)) ? HIGH : LOW);
			digitalWrite(clockPin, HIGH);
		}
	}
	
	digitalWrite(clockPin, LOW);
	digitalWrite(latchPin, HIGH);
}