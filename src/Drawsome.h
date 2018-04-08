#ifndef Drawsome_h
#define Drawsome_h

#include "Accessory.h"

#define penYPositionBytes	WII_UNUSED, 0, 0, WII_BYTE3, WII_BIT0, WII_BIT5, WII_BYTE2, WII_BIT0, WII_BIT5
#define penXPositionBytes	WII_UNUSED, 0, 0, WII_BYTE1, WII_BIT0, WII_BIT5  , WII_BYTE0, WII_BIT0, WII_BIT5
#define penPressureBytes	WII_UNUSED, 0, 0, WII_BYTE5, WII_BIT0, WII_BIT3 ,WII_BYTE4   , 0   , 5
#define penContactBytes	WII_BYTE5,WII_BIT7,true

class Drawsome
{
	public:
	virtual void printInputsDrawsome(Stream& stream = Serial);
	virtual void getValuesDrawsome(uint8_t * values);

	virtual int getPenXPosition();
	virtual int getPenYPosition();
	virtual int getPenPressure();
	virtual int getPenContact();


	virtual void initBytesDrawsome();
};

#endif
