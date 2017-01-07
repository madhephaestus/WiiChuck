#include "PhotoSensor.h"

PhotoSensor::PhotoSensor(int pin) : BufferedSensor(pin)
{
	min = 0;
	max = 1023;
}

float PhotoSensor::getPercent()
{
	return float(getReading() - min) / float(max - min);
}

void PhotoSensor::setMin(int min)
{
	this->min = min;
}

void PhotoSensor::setMax(int max)
{
	this->max = max;
}