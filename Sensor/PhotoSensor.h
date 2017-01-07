#ifndef __PHOTOSENSOR_H__
#define __PHOTOSENSOR_H__

#include "BufferedSensor.h"

class PhotoSensor : public BufferedSensor
{
public:
	PhotoSensor(int pin);
	float getPercent();
	void setMin(int min);
	void setMax(int max);

private:
	int min, max;
};

#endif