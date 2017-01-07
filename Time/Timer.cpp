#include "Timer.h"
#include <Arduino.h>

Timer::Timer() : timer(0), delay(0)
{

}

void Timer::setDelay(float iDelay)
{
	delay = iDelay;
}

float Timer::getDelay()
{
	return delay;
}

void Timer::reset()
{
	timer = delay;
}

void Timer::clear()
{
	timer = 0.f;
}

void Timer::addDelay()
{
	timer += delay;
}

void Timer::update(float dt)
{
	timer -= dt;
}

bool Timer::isTriggered()
{
	return 0 >= timer;
}

float Timer::getTimeRemaining()
{
	return timer;
}