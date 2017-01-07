#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	void setDelay(float seconds);
	float getDelay();
	void reset();
	void clear();
	void addDelay();
	void update(float dt);
	bool isTriggered();
	float getTimeRemaining();

private:
	float timer, delay;
};

#endif