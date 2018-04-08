#ifndef Nunchuck_h
#define Nunchuck_h

#include "Accessory.h"

// see http://wiibrew.org/wiki/Wiimote/Extension_Nunchuck
#define joyXBytes     WII_UNUSED, 0, 0, WII_UNUSED ,0   , 0   , WII_BYTE0, WII_BIT0, WII_BIT7
#define joyYBytes     WII_UNUSED, 0, 0, WII_UNUSED ,0   , 0   , WII_BYTE1, WII_BIT0, WII_BIT7
#define accelXBytes   WII_UNUSED, 0, 0, WII_BYTE2  ,WII_BIT0, WII_BIT7, WII_BYTE5, WII_BIT2, WII_BIT3
#define accelYBytes   WII_UNUSED, 0, 0, WII_BYTE3  ,WII_BIT0, WII_BIT7, WII_BYTE5, WII_BIT4, WII_BIT5
#define accelZBytes   WII_UNUSED, 0, 0, WII_BYTE4  ,WII_BIT0, WII_BIT7, WII_BYTE5, WII_BIT6, WII_BIT7

#define buttonCBits   WII_BYTE5,WII_BIT1,true
#define buttonZBits   WII_BYTE5,WII_BIT0,true

class Nunchuck
{
public:
	//Nunchuck();
	virtual void printInputsNunchuck(Stream& stream = Serial);
	virtual void getValuesNunchuck(uint8_t * values);

	virtual int getJoyX();
	virtual int getJoyY();

	virtual float getRollAngle();
	virtual float getPitchAngle();
	virtual int getAccelX();
	virtual int getAccelY();
	virtual int getAccelZ();

	virtual boolean getButtonC();
	virtual boolean getButtonZ();

};

#endif
