#include "Sensor.h"
#include <Arduino.h>

unsigned long Sensor::lastReadingTime = 0;

Sensor::Sensor(int pin) : inputPin(pin)
{
	pinMode(inputPin, INPUT);
}

int Sensor::getReading()
{
	unsigned long curTime = millis();
	unsigned long delta = curTime - lastReadingTime;
	if (delta < MIN_READ_DELAY)
	{
		delay(MIN_READ_DELAY - delta);
	}
	lastReadingTime = millis();
	return analogRead(inputPin);
}