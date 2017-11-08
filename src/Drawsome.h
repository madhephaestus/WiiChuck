#ifndef Drawsome_h
#define Drawsome_h
#include "Accessory.h"
#define penYPositionBytes	UNUSED, 0, 0, BYTE3, BIT0, BIT5, BYTE2, BIT0, BIT5
#define penXPositionBytes	UNUSED, 0, 0, BYTE1, BIT0, BIT5  , BYTE0, BIT0, BIT5
#define penPressureBytes	UNUSED, 0, 0, BYTE5, BIT0, BIT3 ,BYTE4   , 0   , 5
#define penContactBytes	BYTE5,BIT7,true
  
  
class Drawsome : public Accessory
{
  public:
  Drawsome();
  void printInputs(Stream& stream = Serial);
  
 	int getPenXPosition();
	int getPenYPosition();
	int getPenPressure();
	int getPenContact();

  
  

	  class penXPosition : public Accessory::Mapping
	  {
	    public:
	    penXPosition(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    const uint16_t myMin = 0;
	    const uint16_t myZero = 125;
	    const uint16_t myMax = 255;
	    
	  };
  

	  class penYPosition : public Accessory::Mapping
	  {
	    public:
	    penYPosition(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    const uint16_t myMin = 0;
	    const uint16_t myZero = 125;
	    const uint16_t myMax = 255;
	    
	  };
  

	  class penPressure : public Accessory::Mapping
	  {
	    public:
	    penPressure(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    const uint16_t myMin = 0;
	    const uint16_t myZero = 125;
	    const uint16_t myMax = 255;
	    
	  };
  

	  class penContact : public Accessory::Mapping
	  {
	    public:
	    penContact(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
	    unsigned int  mapVar();
	    void printMap(Stream& stream = Serial);
	    
	  };
  protected:
    void initBytes();
  
  
};
#endif  
  

