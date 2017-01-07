#include "RangedSensor.h"
#include <Arduino.h>

RangedSensor::RangedSensor(int pin, int minReading, int maxReading)
	: Sensor(pin)
	, minReading(minReading)
	, maxReading(maxReading)
{
	
}

int RangedSensor::getNormalizedReading()
{
	return map(getReading(), minReading, maxReading, 0, 1023);
}

float RangedSensor::getPercent()
{
	return getNormalizedReading() / 1023.f;
}