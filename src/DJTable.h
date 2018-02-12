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

class DJTable : public Accessory
{
public:
	DJTable();
	void printInputs(Stream& stream = Serial);

 	int getCrossfadeSlider();
	int getEffectDial();

	int getStickX();
	int getStickY();

	int getRightDJTable();
	int getLeftDJTable();

	int getEuphoriaButton();
	int getPlusButton();
	int getMinusButton();

	int getLeftGreenButton();
	int getLeftRedButton();
	int getLeftBlueButton();
	int getRightGreenButton();
	int getRightRedButton();
	int getRightBlueButton();

	class crossfadeSlider : public Accessory::Mapping
	{
		public:
		crossfadeSlider(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		crossfadeSlider(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class effectDial : public Accessory::Mapping
	{
		public:
		effectDial(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		effectDial(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class stickX : public Accessory::Mapping
	{
		public:
		stickX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		stickX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class stickY : public Accessory::Mapping
	{
		public:
		stickY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		stickY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class rightDJTable : public Accessory::Mapping
	{
		public:
		rightDJTable(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		rightDJTable(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class leftDJTable : public Accessory::Mapping
	{
		public:
		leftDJTable(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		leftDJTable(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
	};

	class euphoriaButton : public Accessory::Mapping
	{
		public:
		euphoriaButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		euphoriaButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class plusButton : public Accessory::Mapping
	{
		public:
		plusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		plusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class minusButton : public Accessory::Mapping
	{
		public:
		minusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		minusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class leftGreenButton : public Accessory::Mapping
	{
		public:
		leftGreenButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		leftGreenButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		
	};

	class leftRedButton : public Accessory::Mapping
	{
		public:
		leftRedButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		leftRedButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class leftBlueButton : public Accessory::Mapping
	{
		public:
		leftBlueButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		leftBlueButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class rightGreenButton : public Accessory::Mapping
	{
		public:
		rightGreenButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		rightGreenButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class rightRedButton : public Accessory::Mapping
	{
		public:
		rightRedButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		rightRedButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class rightBlueButton : public Accessory::Mapping
	{
		public:
		rightBlueButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		rightBlueButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	}; 
};

#endif
