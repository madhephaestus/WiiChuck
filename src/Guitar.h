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

class Guitar : public Accessory
{
	public:
	Guitar();
	void printInputs(Stream& stream = Serial);
  
 	int getStickX();
	int getStickY();
	int getWhammyBar();

	int getPlusButton();
	int getMinusButton();

	int getGreenButton();
	int getRedButton();
	int getYellowButton();
	int getBlueButton();
	int getOrangeButton();

	int getStrumUp();
	int getStrumDown();

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

	class whammyBar : public Accessory::Mapping
	{
		public:
		whammyBar(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		whammyBar(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
		const uint16_t myMin = 0;
		const uint16_t myZero = 125;
		const uint16_t myMax = 255;
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

	class greenButton : public Accessory::Mapping
	{
		public:
		greenButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		greenButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class redButton : public Accessory::Mapping
	{
		public:
		redButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		redButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class yellowButton : public Accessory::Mapping
	{
		public:
		yellowButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		yellowButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class blueButton : public Accessory::Mapping
	{
		public:
		blueButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		blueButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class orangeButton : public Accessory::Mapping
	{
		public:
		orangeButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		orangeButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class strumUp : public Accessory::Mapping
	{
		public:
		strumUp(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		strumUp(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};

	class strumDown : public Accessory::Mapping
	{
		public:
		strumDown(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
		strumDown(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};
		unsigned int mapVar();
		void printMap(Stream& stream = Serial);
	};
};

#endif  
