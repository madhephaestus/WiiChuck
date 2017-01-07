#include "SevenSegmentDisplay.h"

const byte SevenSegmentDisplay::NUMBER_PATTERNS[] = {
	B11111100, //0
	B01100000, //1
	B11011010, //2
	B11110010, //3
	B01100110, //4
	B10110110, //5
	B10111110, //6
	B11100000, //7
	B11111110, //8
	B11110110, //9
};

const byte SevenSegmentDisplay::DASH_PATTERN = B00000010;

SevenSegmentDisplay::SevenSegmentDisplay(byte* data) : data(data), negate(false)
{

}

SevenSegmentDisplay::SevenSegmentDisplay(byte* data, bool negate) : data(data), negate(negate)
{

}

void SevenSegmentDisplay::displayNumber(byte number, bool decimalPoint)
{
	if (0 <= number && 10 > number)
	{
		displayCustom(NUMBER_PATTERNS[number] | (decimalPoint ? B00000001 : B00000000));
	}
}

void SevenSegmentDisplay::displayDash()
{
	displayCustom(DASH_PATTERN);
}

void SevenSegmentDisplay::displayCustom(byte values)
{
	*data = negate ? ~values : values;
}

void SevenSegmentDisplay::displayCustom(bool values[], unsigned int numValues)
{
	byte newData = 0;
	for (int i = 7; i >= 0; i--)
	{
		newData |= values[i] << i;
	}
	displayCustom(newData);
}