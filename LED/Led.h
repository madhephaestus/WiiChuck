#ifndef __LED_H__
#define __LED_H__

#include "ILed.h"

class Led : public ILed
{
public:
	Led(int pin, bool isDigital);
	virtual void setIntensity(float intensity);
	virtual void on();
	virtual void off();

protected:
	int outputPin;
	bool isDigital;

	virtual void writeToPin(int pinNumber, float intensity);
};

#endif