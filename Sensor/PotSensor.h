#ifndef __POTSENSOR_H__
#define __POTSENSOR_H__

#include "Sensor.h"

class PotSensor : public Sensor
{
public:
	PotSensor(int pin);
	float getPercent();
	float getDegrees();
	float getRadians();
};

#endif