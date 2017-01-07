#ifndef __PIEZOSPEAKER_H__
#define __PIEZOSPEAKER_H__

class PiezoSpeaker
{
public:
	PiezoSpeaker(int pin);
	void playTone(float pitch, float duration); //pitch from min to max, duration in seconds

private:
	int outputPin;
	static const int MIN_FREQUENCY = 5;
	static const int MAX_FREQUENCY = 4000;
};

#endif