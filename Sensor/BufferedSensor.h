#ifndef __BUFFEREDSENSOR_H__
#define __BUFFEREDSENSOR_H__

#include "Sensor.h"
#include "SampleBuffer.h"

class BufferedSensor : public Sensor
{
public:
	BufferedSensor(int pin);
	virtual int getReading();

private:
	IntSampleBuffer buffer;
};

#endif