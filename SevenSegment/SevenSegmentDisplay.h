#ifndef __SEVENSEGMENTDISPLAY_H__
#define __SEVENSEGMENTDISPLAY_H__

#include <Arduino.h>

class SevenSegmentDisplay
{
public:
	SevenSegmentDisplay(byte* data);
	SevenSegmentDisplay(byte* data, bool negate);
	void displayNumber(byte number, bool decimalPoint=false);
	void displayDash();
	void displayCustom(byte values);
	void displayCustom(bool values[], unsigned int numValues);

private:
	byte* data;
	bool negate;
	static const byte NUMBER_PATTERNS[];
	static const byte DASH_PATTERN;
};

#endif