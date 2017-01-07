#include "AbsoluteServo.h"
#include <Arduino.h>
#include <math.h>

AbsoluteServo::AbsoluteServo(int pin)
{
	outputPin = pin;
	servo.attach(outputPin);
	prevPos = servo.read();
}

void AbsoluteServo::setPercent(float percent)
{
	int newPos = constrain(percent * 179.f, 0, 179);
	if (prevPos == newPos || !servo.attached())
	{
		return;
	}
	servo.write(newPos);
	prevPos = newPos;
}

float AbsoluteServo::getCurrentPercent()
{
	return servo.read() / 179.f;
}

float AbsoluteServo::getTargetPercent()
{
	return prevPos / 179.f;
}