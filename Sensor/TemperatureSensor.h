#ifndef __TEMPERATURESENSOR_H__
#define __TEMPERATURESENSOR_H__

#include "BufferedSensor.h"

class TemperatureSensor : public BufferedSensor
{
public:
	TemperatureSensor(int pin);
	float getCelsius();
};

#endif