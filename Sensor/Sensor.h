#ifndef __SENSOR_H__
#define __SENSOR_H__

class Sensor
{
public:
	Sensor(int pin);
	virtual int getReading();

protected:
	static const unsigned long MIN_READ_DELAY = 1; //milliseconds
	static unsigned long lastReadingTime;
	int inputPin;
};

#endif