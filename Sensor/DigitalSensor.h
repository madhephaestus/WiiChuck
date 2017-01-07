#ifndef __DIGITALSENSOR_H__
#define __DIGITALSENSOR_H__

class DigitalSensor
{
public:
	DigitalSensor(int pin);
	virtual bool getState();

private:
	int inputPin;
};

#endif