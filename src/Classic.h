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

class Classic : public Accessory
{
public:
	Classic();
	void printInputs(Stream& stream = Serial);

	int getJoyXLeft();
	int getJoyXRight();
	int getJoyYLeft();
	int getJoyYRight();

	int getTriggerLeft();
	int getTriggerRight();

	int getPadRight();
	int getPadDown();
	int getPadUp();
	int getPadLeft();
	int getButtonX();
	int getButtonY();
	int getButtonA();
	int getButtonB();

	int getButtonMinus();
	int getButtonHome();
	int getButtonPlus();

	int getButtonZLeft();
	int getButtonZRight();

	class joyXLeft : public Accessory::Mapping
	{
		public:
		joyXLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		joyXLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class joyXRight : public Accessory::Mapping
	{
		public:
		joyXRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		joyXRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class joyYLeft : public Accessory::Mapping
	{
		public:
		joyYLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		joyYLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class joyYRight : public Accessory::Mapping
	{
		public:
		joyYRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		joyYRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class triggerLeft : public Accessory::Mapping
	{
		public:
		triggerLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		triggerLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class triggerRight : public Accessory::Mapping
	{
		public:
		triggerRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		triggerRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class padRight : public Accessory::Mapping
	{
		public:
		padRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		padRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class padDown : public Accessory::Mapping
	{
		public:
		padDown(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		padDown(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class padUp : public Accessory::Mapping
	{
		public:
		padUp(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		padUp(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class padLeft : public Accessory::Mapping
	{
		public:
		padLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		padLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonX : public Accessory::Mapping
	{
		public:
		buttonX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonY : public Accessory::Mapping
	{
		public:
		buttonY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonA : public Accessory::Mapping
	{
		public:
		buttonA(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonA(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonB : public Accessory::Mapping
	{
		public:
		buttonB(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonB(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonMinus : public Accessory::Mapping
	{
		public:
		buttonMinus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonMinus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonHome : public Accessory::Mapping
	{
		public:
		buttonHome(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonHome(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonPlus : public Accessory::Mapping
	{
		public:
		buttonPlus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonPlus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonZLeft : public Accessory::Mapping
	{
		public:
		buttonZLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonZLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class buttonZRight : public Accessory::Mapping
	{
		public:
		buttonZRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		buttonZRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};
};

#endif
