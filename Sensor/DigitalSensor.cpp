#include "DigitalSensor.h"
#include <Arduino.h>

DigitalSensor::DigitalSensor(int pin) : inputPin(pin)
{
	pinMode(pin, INPUT);
}

bool DigitalSensor::getState()
{
	return HIGH == digitalRead(inputPin);
}