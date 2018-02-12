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

class Nunchuck : public Accessory
{
public:
	Nunchuck();
	void printInputs(Stream& stream = Serial);

	int getJoyX();
	int getJoyY();

	float getRollAngle();
	float getPitchAngle();
	int getAccelX();
	int getAccelY();
	int getAccelZ();

	boolean getButtonC();
	boolean getButtonZ();

	class joyX : public Accessory::Mapping
	{
		public:
		joyX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		joyX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int  mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class joyY : public Accessory::Mapping
	{
		public:
		joyY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		joyY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int  mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class roll : public Accessory::Mapping
	{
		public:
		roll(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		roll(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int  mapVar();
		void printMap(Stream& stream = Serial);
		const int16_t myMin = -180;
		const int16_t myZero = 0;
		const int16_t myMax = 180;
	};

	class pitch : public Accessory::Mapping
	{
		public:
		pitch(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		pitch(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int  mapVar();
		void printMap(Stream& stream = Serial);
		const int16_t myMin = -180;
		const int16_t myZero = 0;
		const int16_t myMax = 180;
	};

	class buttonC : public Accessory::Mapping
	{
		public:
		buttonC(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonC(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		void printMap(Stream& stream = Serial);
		unsigned int  mapVar();
	};

	class buttonZ : public Accessory::Mapping
	{
		public:
		buttonZ(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonZ(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		void printMap(Stream& stream = Serial);
		unsigned int  mapVar();
	};
};

#endif
