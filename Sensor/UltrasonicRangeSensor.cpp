#include "UltrasonicRangeSensor.h"
#include <NewPing.h>

unsigned long UltrasonicRangeSensor::lastReadingTime = 0;

UltrasonicRangeSensor::UltrasonicRangeSensor(int triggerPin, int echoPin, int maxDistance)
{
	sensor = new NewPing(triggerPin, echoPin, maxDistance * 100); //convert from meters to centimeters
	this->maxDistance = maxDistance;
}

UltrasonicRangeSensor::~UltrasonicRangeSensor()
{
	delete sensor;
}

float UltrasonicRangeSensor::getPercent()
{
	return getMeters() / (float)maxDistance;
}

float UltrasonicRangeSensor::getMeters()
{
	return (getMicroSeconds() / (float)US_ROUNDTRIP_CM) / 100.f;
}

unsigned int UltrasonicRangeSensor::getMicroSeconds()
{
	unsigned long curTime = millis();
	unsigned long delta = curTime - lastReadingTime;
	if (delta < MIN_READ_DELAY)
	{
		delay(MIN_READ_DELAY - delta);
	}
	lastReadingTime = millis();
	return sensor->ping();
}