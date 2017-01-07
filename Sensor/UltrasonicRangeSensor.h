#ifndef __ULTRASONICRANGESENSOR_H__
#define __ULTRASONICRANGESENSOR_H__

class NewPing;

class UltrasonicRangeSensor
{
public:
	UltrasonicRangeSensor(int triggerPin, int echoPin, int maxDistance);
	virtual ~UltrasonicRangeSensor();

	float getPercent();
	float getMeters();
	unsigned int getMicroSeconds();

private:
	NewPing* sensor;
	unsigned int maxDistance;

	static const unsigned long MIN_READ_DELAY = 30; //milliseconds
	static unsigned long lastReadingTime;
};

#endif