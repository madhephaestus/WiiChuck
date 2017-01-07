#include "Led.h"
#include <Arduino.h>

Led::Led(int pin, bool isDigital) : outputPin(pin), isDigital(isDigital)
{
	if (0 <= pin)
	{
		pinMode(pin, OUTPUT);
	}
}

void Led::setIntensity(float intensity)
{
	if (0 > outputPin)
	{
		return;
	}
	writeToPin(outputPin, intensity);
}

void Led::on()
{
	setIntensity(1.f);
}

void Led::off()
{
	setIntensity(0.f);
}

void Led::writeToPin(int pinNumber, float intensity)
{
	intensity = constrain(intensity, 0.f, 1.f);
	if (isDigital)
	{
		digitalWrite(pinNumber, intensity >= .5f ? HIGH : LOW);
	}
	else
	{
		analogWrite(pinNumber, intensity * 255);
	}
}