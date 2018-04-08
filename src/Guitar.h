#ifndef Guitar_h
#define Guitar_h

#include "Accessory.h"

#define stickXBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE0,  WII_BIT0, WII_BIT5
#define stickYBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE1,  WII_BIT0, WII_BIT5
#define whammyBarBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE3,  WII_BIT0, WII_BIT4

#define plusButtonBytes	 WII_BYTE4,WII_BIT2,true
#define minusButtonBytes	 WII_BYTE4,WII_BIT4,true

#define greenButtonBytes	 WII_BYTE5,WII_BIT4,true
#define redButtonBytes	 WII_BYTE5,WII_BIT6,true
#define yellowButtonBytes	 WII_BYTE5,WII_BIT3,true
#define blueButtonBytes	 WII_BYTE5,WII_BIT5,true
#define orangeButtonBytes	 WII_BYTE5,WII_BIT7,true

#define strumUpBytes	 WII_BYTE5,WII_BIT0,true
#define strumDownBytes	 WII_BYTE4,WII_BIT6,true

class Guitar
{
	public:
	virtual void printInputsGuitar(Stream& stream = Serial);
	virtual void getValuesGuitar(uint8_t * values);

 	virtual int getStickXGuitar();
	virtual int getStickYGuitar();
	virtual int getWhammyBar();

	virtual int getPlusButtonGuitar();
	virtual int getMinusButtonGuitar();

	virtual int getGreenButton();
	virtual int getRedButton();
	virtual int getYellowButton();
	virtual int getBlueButton();
	virtual int getOrangeButton();

	virtual int getStrumUp();
	virtual int getStrumDown();

};

#endif  
