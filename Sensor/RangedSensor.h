#ifndef __RANGEDSENSOR_H__
#define __RANGEDSENSOR_H__

#include "Sensor.h"

class RangedSensor : public Sensor
{
public:
	RangedSensor(int pin, int minReading, int maxReading);
	int getNormalizedReading();
	float getPercent();

private:
	int minReading;
	int maxReading;
};

#endif