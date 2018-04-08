#ifndef Drums_h
#define Drums_h

#include "Accessory.h"

#define stickXBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE0,  WII_BIT0, WII_BIT5
#define stickYBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE1,  WII_BIT0, WII_BIT5

#define softnessDataForBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE2,  WII_BIT1, WII_BIT5
#define softnessBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE3,  WII_BIT5, WII_BIT7
#define highHatDataFlagBytes	WII_BYTE2,WII_BIT7,true
#define softnessDataFlagBytes	WII_BYTE2,WII_BIT6,true

#define minusButtonBytes	WII_BYTE4,WII_BIT4,true
#define plusButtonBytes	WII_BYTE4,WII_BIT2,true

#define orangeDrumBytes	WII_BYTE5,WII_BIT7,true
#define redDrumBytes	WII_BYTE5,WII_BIT6,true
#define yellowDrumBytes	WII_BYTE5,WII_BIT5,true
#define greenDrumBytes	WII_BYTE5,WII_BIT4,true
#define blueDrummBytes	WII_BYTE5,WII_BIT3,true
#define bassPedalBytes	WII_BYTE5,WII_BIT2,true

#define bassID   27
#define redID    25
#define yellowID 17
#define blueID   15
#define orangeID 14
#define greenID  18
  
class Drums
{
	public:
	virtual void printInputsDrums(Stream& stream = Serial);
	virtual void getValuesDrums(uint8_t * values);

	virtual int getStickXDrums();
	virtual int getStickYDrums();

	virtual int getSoftnessDataFor();
	virtual int getSoftness();
	virtual int getHighHatDataFlag();
	virtual int getSoftnessDataFlag();

	virtual int getMinusButtonDrums();
	virtual int getPlusButtonDrums();

	virtual int getOrangeDrum();
	virtual int getRedDrum();
	virtual int getYellowDrum();
	virtual int getGreenDrum();
	virtual int getBlueDrumm();
	virtual int getBassPedal();

};

#endif
