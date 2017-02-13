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

typedef enum _functionMapName {
	JOY_X,
	JOY_Y,
	ROLL,
	PITCH,
	ACCELX,
	ACCELY,
	ACCELZ,
	RSPRESSURE,
	LSPRESSURE,
	LEFTSTICKX,
	LEFTSTICKY,
	RIGHTSTICKX,
	RIGHTSTICKY,
	NOFUNCTION
} FunctionMapName;
typedef enum _buttonMapName {
	CBUTTON, ZBUTTON, LZ, RZ, LD, RD, UD, DD, SL, H, START, X, Y, A, B, NOBUTTON
} ButtonMapName;

typedef struct _inputMapping {
	inType type;

	// digital in params
	uint8_t dByte;
	uint8_t dBit;
	bool dActiveLow;

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

	// Analog Scaling info
	uint8_t aMax;
	uint8_t aZero;
	uint8_t aMin;

	// Servo Scaling Info
	uint8_t sevoMax;
	uint8_t servoZero;
	uint8_t servoMin;

	Servo servo;

	struct _inputMapping* nextMap;
} inputMapping;

class Accessory {
public:
	Accessory(uint8_t data_pin, uint8_t sclk_pin);

	uint8_t* getDataArray();
	void setDataArray(uint8_t data[6]);

	void printInputs(Stream& stream);

	void begin();
	void readData();

	void usePullUpClock(bool mode);

	// mapping funcs
	uint8_t addAnalogMap(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
			uint8_t csbbyte, uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte,
			uint8_t lsbstart, uint8_t lsbend, uint8_t sMin, uint8_t sMax,
			uint8_t sZero, uint8_t sChan);

	uint8_t addDigitalMap(uint8_t byte, uint8_t bit, bool activeLow,
			uint8_t sMin, uint8_t sMax, uint8_t sZero, uint8_t sChan);

	void printMaps(Stream& stream);
	uint8_t getMapCount();
	void removeMaps();
	void removeMap(uint8_t id);

	ControllerType identifyController();
	int getAnalog(FunctionMapName name);
	boolean getDigital(ButtonMapName name);
protected:
	// allow sub classes to view the data
	uint8_t _dataarray[8];
private:

	// Data Parsing
	int decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
			uint8_t csbbyte, uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte,
			uint8_t lsbstart, uint8_t lsbend);
	bool decodeBit(uint8_t byte, uint8_t bit, bool activeLow);

	// SW i2c
	uint8_t _scl_pin;
	uint8_t _sda_pin;
	boolean _use_hw;
	uint32_t _clockSpacing;
	unsigned long ackTimeout;
	boolean _usePullUpClock;

	void _clockHigh();
	void _clockLow();
	void _dataHigh();
	void _dataLow();
	void _sendStart(byte addr);
	void _sendStop();
	void _sendAck();
	void _sendNack();
	void _waitForAck();
	void _clockStallCheck();
	void _shiftOut(uint8_t val);
	uint8_t _readByte();
	void _writeByte(uint8_t value);

	// Controller Register Transactions

	void _burstRead();
	void _burstReadWithAddress(uint8_t addr);
	void _writeRegister(uint8_t reg, uint8_t value);
	void initBytes();

	// Mapping
	inputMapping* first;
	uint8_t mapCount;

};

#endif
