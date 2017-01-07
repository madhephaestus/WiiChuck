#ifndef __LEDGROUP_H__
#define __LEDGROUP_H__

#include "ILed.h"

class LedGroup : public ILed
{
public:
	LedGroup();
	virtual ~LedGroup();
	bool addLed(ILed* led);
	ILed* get(int index);
	virtual void setIntensity(float intensity);
	virtual void on();
	virtual void off();

protected:
	static const int MAX_LEDS = 10;
	ILed* leds[MAX_LEDS];
};

#endif