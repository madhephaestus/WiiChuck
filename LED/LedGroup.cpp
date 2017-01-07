#include "LedGroup.h"
#include <Arduino.h>

LedGroup::LedGroup()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		leds[i] = NULL;
	}
}

LedGroup::~LedGroup()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			delete leds[i];
		}
	}
}

bool LedGroup::addLed(ILed* led)
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL == leds[i])
		{
			leds[i] = led;
			return true;
		}
	}
	return false;
}

ILed* LedGroup::get(int index)
{
	if (index < 0 || index >= MAX_LEDS || NULL == leds[index])
	{
		return NULL;
	}
	return leds[index];
}

void LedGroup::setIntensity(float intensity)
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			leds[i]->setIntensity(intensity);
		}
	}
}

void LedGroup::on()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			leds[i]->on();
		}
	}
}

void LedGroup::off()
{
	for (int i = 0; i < MAX_LEDS; i++)
	{
		if (NULL != leds[i])
		{
			leds[i]->off();
		}
	}
}