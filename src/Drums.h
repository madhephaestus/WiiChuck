#ifndef Drums_h
#define Drums_h
#include "Accessory.h"
#define stickXBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE0,  BIT0, BIT5
#define stickYBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE1,  BIT0, BIT5
#define softnessDataForBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE2,  BIT1, BIT5
#define softnessBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE3,  BIT5, BIT7
#define highHatDataFlagBytes	BYTE2,BIT7,true
#define softnessDataFlagBytes	BYTE2,BIT6,true
#define minusButtonBytes	BYTE4,BIT4,true
#define plusButtonBytes	BYTE4,BIT2,true
#define orangeDrumBytes	BYTE5,BIT7,true
#define redDrumBytes	BYTE5,BIT6,true
#define yellowDrumBytes	BYTE5,BIT5,true
#define greenDrumBytes	BYTE5,BIT4,true
#define blueDrummBytes	BYTE5,BIT3,true
#define bassPedalBytes	BYTE5,BIT2,true

#define bassID   27
#define redID    25
#define yellowID 17
#define blueID   15
#define orangeID 14
#define greenID  18
  
class Drums : public Accessory
{
  public:
  Drums();
  void printInputs(Stream& stream = Serial);
  
 	int getStickX();
	int getStickY();
	int getSoftnessDataFor();
	int getSoftness();
	int getHighHatDataFlag();
	int getSoftnessDataFlag();
	int getMinusButton();
	int getPlusButton();
	int getOrangeDrum();
	int getRedDrum();
	int getYellowDrum();
	int getGreenDrum();
	int getBlueDrumm();
	int getBassPedal();

  
  

	  class stickX : public Accessory::Mapping
	  {
	    public:
	    stickX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    stickX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    const uint16_t myMin = 0;
	    const uint16_t myZero = 125;
	    const uint16_t myMax = 255;
	    
	  };
  

	  class stickY : public Accessory::Mapping
	  {
	    public:
	    stickY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    stickY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    const uint16_t myMin = 0;
	    const uint16_t myZero = 125;
	    const uint16_t myMax = 255;
	    
	  };

	  class minusButton : public Accessory::Mapping
	  {
	    public:
	    minusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    minusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class plusButton : public Accessory::Mapping
	  {
	    public:
	    plusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    plusButton(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class orangeDrum : public Accessory::Mapping
	  {
	    public:
	    orangeDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    orangeDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class redDrum : public Accessory::Mapping
	  {
	    public:
	    redDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    redDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class yellowDrum : public Accessory::Mapping
	  {
	    public:
	    yellowDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    yellowDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class greenDrum : public Accessory::Mapping
	  {
	    public:
	    greenDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    greenDrum(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class blueDrumm : public Accessory::Mapping
	  {
	    public:
	    blueDrumm(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    blueDrumm(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  

	  class bassPedal : public Accessory::Mapping
	  {
	    public:
	    bassPedal(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min,cooldown) {};
	    bassPedal(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  
  
  
};
#endif  

