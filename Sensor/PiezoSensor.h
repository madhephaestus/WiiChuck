#ifndef __PIEZOSENSOR_H__
#define __PIEZOSENSOR_H__

#include "BufferedSensor.h"

class PiezoSensor : public BufferedSensor
{
public:
	PiezoSensor(int pin);
	float getPercent();
	void setMin(int min);
	void setMax(int max);

private:
	int rangeMin, rangeMax;
};

#endif