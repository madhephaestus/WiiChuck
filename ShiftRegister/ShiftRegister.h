#ifndef __SHIFTREGISTER_H__
#define __SHIFTREGISTER_H__

#include <Arduino.h>

class ShiftRegister
{
public:
	ShiftRegister(int dataPin, int clockPin, int latchPin);
	void send(bool data[], unsigned int length); //each element of the array is a single bit, length is number of elements in array
	void send(byte data[], unsigned int length); //each element of the array is 8 bits, length is number of elements in array

private:
	int dataPin, clockPin, latchPin;
};

#endif