#ifndef __ILED_H__
#define __ILED_H__

class ILed
{
public:
	virtual void setIntensity(float intensity) = 0;
	virtual void on() = 0;
	virtual void off() = 0;
};

#endif