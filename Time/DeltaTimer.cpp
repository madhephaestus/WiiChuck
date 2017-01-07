#include "DeltaTimer.h"
#include <Arduino.h>

DeltaTimer::DeltaTimer()
{
	prevTime = 0;
}

float DeltaTimer::updateDt()
{
	unsigned long curTime = millis();
	float dt = (curTime - prevTime) / 1000.f;
	prevTime = curTime;
	return dt;
}