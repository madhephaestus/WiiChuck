#ifndef __ABSOLUTESERVO_H__
#define __ABSOLUTESERVO_H__

#include <Servo.h>

class AbsoluteServo
{
public:
	AbsoluteServo(int pin);
	void setPercent(float percent);
	float getCurrentPercent();
	float getTargetPercent();

private:
	int outputPin;
	int prevPos;
	Servo servo;
};

#endif