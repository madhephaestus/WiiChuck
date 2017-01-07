#ifndef __LEDANIMATOR_H__
#define __LEDANIMATOR_H__

#include "ILed.h"

class LedAnimator : ILed
{
public:
	LedAnimator(ILed* led);
	void update(float dt);

	void setIntensityVelocity(float intensityVelocity);
	float getIntensityVelocity();

	//ILed Interface
	void setIntensity(float intensity);
	void on();
	void off();

protected:
	ILed* led;
	float intensity;
	float intensityVelocity;
};

#endif