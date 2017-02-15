#ifndef Accessory_h
#define Accessory_h

#include "Arduino.h"
#include <Servo.h>



#define I2C_ADDR		0x52
#define I2C_ADDR_R		((I2C_ADDR << 1) + 1)
#define I2C_ADDR_W		(I2C_ADDR << 1)

#define BYTE0 0
#define BYTE1 1
#define BYTE2 2
#define BYTE3 3
#define BYTE4 4
#define BYTE5 5
#define UNUSED 255

#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7

//#define THIRDPARTYWII 0
//#define OFFICIALWII 1
//#define WIICLASSIC 2
typedef enum _controllertype {
	Unknown,
	NUNCHUCK,
	WIICLASSIC,
	GuitarHeroController,
	GuitarHeroWorldTourDrums,
	DrumController,
	DrawsomeTablet,
	Turntable
} ControllerType;

typedef enum _inType {
	ANALOG, DIGITAL
} inType;

typedef struct _analogMapBits {
  	// analog in params
	uint8_t aMsbbyte;
	uint8_t aMsbstart;
	uint8_t aMsbend;

	uint8_t aCsbbyte;
	uint8_t aCsbstart;
	uint8_t aCsbend;

	uint8_t aLsbbyte;
	uint8_t aLsbstart;
	uint8_t aLsbend;
} analogMapBits;

typedef struct _digitalMapBits {
	// digital in params
	uint8_t dByte;
	uint8_t dBit;
	bool dActiveLow;

} digitalMapBits;

typedef struct _inputMapping {
	inType type;

	// digital in params
  digitalMapBits dSch;
	// analog in params
  analogMapBits aSch;

	//Input Scaling info
	int16_t aMin;
	int16_t aMid;
	int16_t aMax;
	
	// Servo Scaling Info
	uint8_t servoMax;
	uint8_t servoZero;
	uint8_t servoMin;

	Servo servo;
	uint8_t sChan;

	struct _inputMapping* nextMap;
} inputMapping;

#define dataSize 6

class Accessory {
public:
	Accessory(uint8_t data_pin, uint8_t sclk_pin);

	uint8_t* getDataArray();
	void setDataArray(uint8_t data[dataSize]);

	void printInputs(Stream& stream);

	void begin();
	void readData();



	
	int decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
			            uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
			            uint8_t lsbbyte,uint8_t lsbstart, uint8_t lsbend);
			            
	bool decodeBit(uint8_t byte, uint8_t bit, bool activeLow);

	void printMaps(Stream& stream);
	uint8_t getMapCount();
	void removeMaps();
	void removeMap(uint8_t id);

	ControllerType identifyController();
	static int16_t smap(int16_t val,int16_t aMax, int16_t aMid, int16_t aMin, int16_t sMax, int16_t sZero,int16_t sMin);
	
	
	class Mapping {
	  public:
	  Mapping();
	  Mapping(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min);
	  virtual uint16_t mapVar();
	  virtual void printMap(Stream& stream);
	  	// Data Parsing

	  Mapping* next;
	  Accessory* controller;
	  
	  void addServo(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min);
	  void update();
	  protected:
	  Servo servo;
	  uint8_t channel;
	  
	  uint8_t servoMax;
	  uint8_t servoZero;
	  uint8_t servoMin;
	  
	  
	  
	}; 
	uint8_t addMap(Mapping* m);
protected:
	// allow sub classes to view the data
	uint8_t _dataarray[8];

	

	
	Mapping* firstMap;
	
	

	
private:


	// Controller Register Transactions

	void _burstRead();
	void _burstReadWithAddress(uint8_t addr);
	void _writeRegister(uint8_t reg, uint8_t value);
	void initBytes();

	// Mapping
	inputMapping* _firstMap;
	uint8_t mapCount;
	
	void _applyMaps();


};

#endif
