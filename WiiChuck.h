/**
 * @author Kevin Harrington, Alex Camilo
 */
#ifndef WiiChuck_h
#define WiiChuck_h
#include "Servo.h"


#define I2C_ADDR		0x52
#define I2C_ADDR_R		((I2C_ADDR << 1) + 1)
#define I2C_ADDR_W		(I2C_ADDR << 1)

#define THIRDPARTYWII 0
#define OFFICIALWII 1
#define WIICLASSIC 2

typedef enum _functionMapName {JOY_X,JOY_Y,ROLL,PITCH,ACCELX,ACCELY ,ACCELZ,
	RSPRESSURE,LSPRESSURE,LEFTSTICKX,LEFTSTICKY,RIGHTSTICKX,RIGHTSTICKY,NOFUNCTION} FunctionMapName;
	typedef enum _buttonMapName {CBUTTON,ZBUTTON,LZ,RZ,LD,RD,UD,DD,SL,H,START,X,Y,A,B,NOBUTTON} ButtonMapName;

typedef struct controllerMap {
	FunctionMapName name;
	ButtonMapName button;
   int axisMin;
   int axisMax;
   int axisCenter;
   int servoMin;
   int servoMax;
   int servoCenter;
   Servo myservo;
   struct controllerMap * next;
} ServoWiiControllerMap;


class WiiChuck {
public:
	WiiChuck(uint8_t data_pin, uint8_t sclk_pin);
	void begin();
	void readData();
	void initBytes();

	int getJoyX();
	int getJoyY();

	int getRollAngle();
	int getPitchAngle();
	int getAccelX();
	int getAccelY();
	int getAccelZ();

	boolean checkButtonC();
	boolean checkButtonZ();
	uint32_t callCountBeforeReset;
	int type;
	unsigned long ackTimeout;
	boolean leftShoulderPressed();
	boolean rightShoulderPressed();
	boolean lzPressed();
	boolean rzPressed();
	boolean leftDPressed();
	boolean rightDPressed();
	boolean upDPressed();
	boolean downDPressed();
	boolean selectPressed();
	boolean homePressed();
	boolean startPressed();
	boolean xPressed();
	boolean yPressed();
	boolean aPressed();
	boolean bPressed();
	int rightShouldPressure();
	int leftShouldPressure();
	int leftStickX();
	int leftStickY();
	int rightStickX();
	int rightStickY();
	bool printServos;

	// Create a map between controller and a servo
	void addControlMap(int servoPin, int servoMin,int servoCenter,
			int servoMax,int axisMin,int axiscenter,int axisMax,
			FunctionMapName mapName);
	void addButtonMap(int servoPin, int servoMin,
				int servoMax,
				ButtonMapName mapName);

	void _clockHigh();
	void _clockLow();
	void _dataHigh();
	void _dataLow();
	uint8_t _dataarray[8];
	boolean usePullUpClock;
	int identifyController();
private:
	ServoWiiControllerMap * maps;
	void addControlMap(int servoPin, int servoMin,int servoCenter,
				int servoMax,int axisMin,int axisCenter,int axisMax,
				FunctionMapName mapName,ButtonMapName button);
	int performMap(ServoWiiControllerMap * tmp);
	uint8_t _scl_PIN;
	uint8_t _sda_pin;
	int32_t _joy_x_center, _joy_y_center;
	boolean _use_hw;
	uint32_t _callCount;
	uint32_t _clockSpacing;
	uint32_t _timeoutCount;
	int numMaps;
	void _sendStart(byte addr);
	void _sendStop();
	void _sendAck();
	void _sendNack();
	void _waitForAck();
	void _clockStallCheck();
	uint8_t _readByte();
	void _writeByte(uint8_t value);
	void _burstRead();
	void _burstReadWithAddress(unsigned char addr);

	void _writeRegister(uint8_t reg, uint8_t value);
	void _shiftOut(uint8_t val);
	boolean _PressedRowBit(uint8_t row, uint8_t bit);

};
#endif
