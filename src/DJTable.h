#ifndef DJTable_h
#define DJTable_h
#include "Accessory.h"


#define crossfadeSliderBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE2,  BIT1, BIT4
#define effectDialBytes	UNUSED,     0,     0,   BYTE2 ,        5,     6, BYTE3,  BIT5, BIT7
#define stickXBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE0,  BIT0, BIT5
#define stickYBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE1,  BIT0, BIT5
#define rightDJTableBytes	2,0,0,BYTE0,     6,     7,    BYTE1,        6,     7, BYTE2,  BIT7, BIT7
#define leftDJTableBytes	UNUSED,     0,     0,    BYTE4,        0,     0, BYTE3,  BIT0, BIT4
#define euphoriaButtonBytes	BYTE5,BIT4,true
#define plusButtonBytes	BYTE4,BIT2,true
#define minusButtonBytes	BYTE4,BIT4,true
#define leftGreenButtonBytes	BYTE5,BIT3,true
#define leftRedButtonBytes	BYTE4,BIT5,true
#define leftBlueButtonBytes	BYTE5,BIT7,true
#define rightGreenButtonBytes	BYTE5,BIT5,true
#define rightRedButtonBytes	BYTE4,BIT1,true
#define rightBlueButtonBytes	BYTE5,BIT2,true
  
  
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


