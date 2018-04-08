#ifndef DJTable_h
#define DJTable_h

#include "Accessory.h"

#define crossfadeSliderBytes	WII_UNUSED,	 0,	 0,   WII_UNUSED,		0,	 0, WII_BYTE2,  WII_BIT1, WII_BIT4
#define effectDialBytes	WII_UNUSED,	 0,	 0,   WII_BYTE2 ,		5,	 6, WII_BYTE3,  WII_BIT5, WII_BIT7

#define stickXBytes	WII_UNUSED,	 0,	 0,   WII_UNUSED,		0,	 0, WII_BYTE0,  WII_BIT0, WII_BIT5
#define stickYBytes	WII_UNUSED,	 0,	 0,   WII_UNUSED,		0,	 0, WII_BYTE1,  WII_BIT0, WII_BIT5

#define rightDJTableBytes	2,0,0,WII_BYTE0,	 6,	 7,	WII_BYTE1,		6,	 7, WII_BYTE2,  WII_BIT7, WII_BIT7
#define leftDJTableBytes	WII_UNUSED,	 0,	 0,	WII_BYTE4,		0,	 0, WII_BYTE3,  WII_BIT0, WII_BIT4

#define euphoriaButtonBytes	WII_BYTE5,WII_BIT4,true
#define plusButtonBytes	WII_BYTE4,WII_BIT2,true
#define minusButtonBytes	WII_BYTE4,WII_BIT4,true

#define leftGreenButtonBytes	WII_BYTE5,WII_BIT3,true
#define leftRedButtonBytes	WII_BYTE4,WII_BIT5,true
#define leftBlueButtonBytes	WII_BYTE5,WII_BIT7,true
#define rightGreenButtonBytes	WII_BYTE5,WII_BIT5,true
#define rightRedButtonBytes	WII_BYTE4,WII_BIT1,true
#define rightBlueButtonBytes	WII_BYTE5,WII_BIT2,true

class DJTable
{
public:
	virtual void printInputsDj(Stream& stream = Serial)=0;
	virtual void getValuesDj(uint8_t * values)=0;


 	virtual int getCrossfadeSlider();
	virtual int getEffectDial();

	virtual int getStickX();
	virtual int getStickY();

	virtual int getRightDJTable();
	virtual int getLeftDJTable();

	virtual int getEuphoriaButton();
	virtual int getPlusButton();
	virtual int getMinusButton();

	virtual int getLeftGreenButton();
	virtual int getLeftRedButton();
	virtual int getLeftBlueButton();
	virtual int getRightGreenButton();
	virtual int getRightRedButton();
	virtual int getRightBlueButton();
};

#endif
