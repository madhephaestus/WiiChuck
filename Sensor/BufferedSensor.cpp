#include "BufferedSensor.h"
#include <Arduino.h>

BufferedSensor::BufferedSensor(int pin) : Sensor(pin) {}

int BufferedSensor::getReading()
{
	buffer.addSample(Sensor::getReading());
	return buffer.getAverage();
}