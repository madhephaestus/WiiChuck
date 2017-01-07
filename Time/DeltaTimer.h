#ifndef __DELTATIMER_H__
#define __DELTATIMER_H__

class DeltaTimer
{
public:
	DeltaTimer();

	float updateDt();

private:
	unsigned long prevTime;
};

#endif