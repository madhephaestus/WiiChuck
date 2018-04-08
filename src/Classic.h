#ifndef Classic_h
#define Classic_h

#include "Accessory.h"

#define joyXLeftBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE0,  WII_BIT0, WII_BIT5
#define joyXRightBytes	WII_BYTE0,   WII_BIT6,  WII_BIT7,    WII_BYTE1,     WII_BIT6,  WII_BIT7, WII_BYTE2,  WII_BIT7, WII_BIT7
#define joyYLeftBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE1,  WII_BIT0, WII_BIT5
#define joyYRightBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE2,  WII_BIT0, WII_BIT4

#define triggerLeftBytes	WII_UNUSED,     0,     0,    WII_BYTE2,     WII_BIT5,  WII_BIT6, WII_BYTE3,  WII_BIT5, WII_BIT7
#define triggerRightBytes	WII_UNUSED,     0,     0,   WII_UNUSED,        0,     0, WII_BYTE3,  WII_BIT0, WII_BIT4

#define padRightBytes	WII_BYTE4,WII_BIT7,true
#define padDownBytes	WII_BYTE4,WII_BIT6,true
#define padUpBytes	WII_BYTE5,WII_BIT0,true
#define padLeftBytes	WII_BYTE5,WII_BIT1,true

#define buttonXBytes	WII_BYTE5,WII_BIT3,true
#define buttonYBytes	WII_BYTE5,WII_BIT5,true
#define buttonABytes	WII_BYTE5,WII_BIT4,true
#define buttonBBytes	WII_BYTE5,WII_BIT6,true

#define buttonMinusBytes	WII_BYTE4,WII_BIT4,true
#define buttonHomeBytes	WII_BYTE4,WII_BIT3,true
#define buttonPlusBytes	WII_BYTE4,WII_BIT2,true

#define buttonZRBytes	WII_BYTE5,WII_BIT2,true
#define buttonZLBytes	WII_BYTE5,WII_BIT7,true

class Classic {
public:
	//Classic();
	virtual void printInputsClassic(Stream& stream = Serial)=0;
	virtual void getValuesClassic(uint8_t * values)=0;

	virtual int getJoyXLeft()=0;
	virtual int getJoyXRight()=0;
	virtual int getJoyYLeft()=0;
	virtual int getJoyYRight()=0;

	virtual int getTriggerLeft()=0;
	virtual int getTriggerRight()=0;

	virtual int getPadRight()=0;
	virtual int getPadDown()=0;
	virtual int getPadUp()=0;
	virtual int getPadLeft()=0;
	virtual int getButtonX()=0;
	virtual int getButtonY()=0;
	virtual int getButtonA()=0;
	virtual int getButtonB()=0;

	virtual int getButtonMinus()=0;
	virtual int getButtonHome()=0;
	virtual int getButtonPlus()=0;

	virtual int getButtonZLeft()=0;
	virtual int getButtonZRight()=0;
};

#endif
