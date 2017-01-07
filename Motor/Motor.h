#ifndef __MOTOR_H__
#define __MOTOR_H__

class Motor
{
public:
	Motor(int pin, bool isDigital);
	void on();
	void off();
	void setSpeed(float percent);

protected:
	int outputPin;
	bool isDigital;
};

#endif