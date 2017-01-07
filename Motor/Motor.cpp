#include "Motor.h"
#include <Arduino.h>

Motor::Motor(int pin, bool isDigital) : outputPin(pin), isDigital(isDigital)
{
	pinMode(pin, OUTPUT);
}

void Motor::on()
{
	setSpeed(1.f);
}

void Motor::off()
{
	setSpeed(0.f);
}

void Motor::setSpeed(float percent)
{
	if (0 > outputPin)
	{
		return;
	}
	if (isDigital)
	{
		digitalWrite(outputPin, percent >= .5f ? HIGH : LOW);
	}
	else
	{
		analogWrite(outputPin, percent * 255);
	}
}