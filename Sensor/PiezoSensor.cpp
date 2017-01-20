#include "PiezoSensor.h"
#include <Arduino.h>

PiezoSensor::PiezoSensor(int pin) : BufferedSensor(pin), rangeMin(0), rangeMax(1023) {}

float PiezoSensor::getPercent()
{
	return min(max(float(getReading() - rangeMin) / float(rangeMax - rangeMin), 0.f), 1.f);
}

void PiezoSensor::setMin(int min)
{
	rangeMin = min;
}

void PiezoSensor::setMax(int max)
{
	rangeMax = max;
}