/*
 WiiChuck.cpp - Arduino/chipKit library support for the WiiChuck
 Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved

 This library has been made to easily interface and use the WiiChuck with
 an Arduino or chipKit.

 You can find the latest version of the library at
 http://www.RinkyDinkElectronics.com/

 This library is free software; you can redistribute it and/or
 modify it under the terms of the CC BY-NC-SA 3.0 license.
 Please see the included documents for further information.

 Commercial use of this library requires you to buy a license that
 will allow commercial use. This includes using the library,
 modified or not, as a tool to sell products.

 The license applies to all part of the library including the
 examples and tools supplied with the library.
 */
#include "WiiChuck.h"

// Include hardware-specific functions for the correct MCU
#if defined(__AVR__)
#include "hardware/avr/HW_AVR.h"
#elif defined(__PIC32MX__)
#include "hardware/pic32/HW_PIC32.h"
#elif defined(__arm__)
#include "hardware/arm/HW_ARM.h"
#else
#include  "hardware/generic/HW_Generic.h"
#endif

/* Public */

WiiChuck::WiiChuck(uint8_t data_pin, uint8_t sclk_pin) {
	_sda_pin = data_pin;
	_scl_pin = sclk_pin;
	_callCount = 0;
	callCountBeforeReset = 1000;
	_clockSpacing = 1;
	ackTimeout = 100;
	_timeoutCount = 0;
	type = THIRDPARTYWII;
	maps = NULL;
	numMaps=0;
}

void WiiChuck::readData() {
	initBytes();
	_burstRead();
	if (maps != NULL) {
		ServoWiiControllerMap * tmp = maps;
		int i=0;
		while (tmp != NULL && i++<=numMaps) {
			// perform the mapping
			int value = performMap(tmp);
			Serial.print(" value ");
			Serial.print(value);

			tmp->myservo.write(value);
			if (tmp->next == NULL) {
				// The end of the list
				tmp = NULL; //breal the loop
			} else {
				tmp = tmp->next;
			}
		}
		Serial.println();
	}
}

int WiiChuck::getJoyX() {
	int JoyPos = _dataarray[0];
	int center = _joy_x_center;
	int min = _joy_x_min;
	int max = _joy_x_max;

	int value =0;
	// Set up the min and max values on this channel
	if(JoyPos>max){
		_joy_x_max=JoyPos;
		max=JoyPos;
	}
	if(JoyPos<min){
		_joy_x_min=JoyPos;
		min=JoyPos;
	}
	bool m=false;
	// select the bounding value to map to
	if (JoyPos < center) {
		value=min;

	} else if (JoyPos > center) {
		value=max;
		m=true;
	} else
		return 0;
	// calculate a mapping value
	float valueDiff = value -center;
	float joyDiff =  JoyPos -center;
//	Serial.print("Raw x center ");
//	Serial.print (center);
//
//	Serial.print(" Raw x val ");
//	Serial.print (JoyPos);
//	Serial.print("  ");
//	Serial.print(" Raw bound ");
//	Serial.print (value);
//	Serial.print("  ");
	return (int) ((100.0*joyDiff)/valueDiff)*(m?1:-1);

/*
	if (_dataarray[0] < _joy_x_center) {
		return -((_joy_x_center - _dataarray[0]) / (_joy_x_center / 100.0f));
	} else if (_dataarray[0] > _joy_x_center) {
		return ((_dataarray[0] - _joy_x_center)
				/ ((255 - _joy_x_center) / 100.0f));
	} else
		return 0;
*/
}

int WiiChuck::getJoyY() {
	int JoyPos = _dataarray[1];
	int center = _joy_y_center;
	int min = _joy_y_min;
	int max = _joy_y_max;

	int value =0;
	// Set up the min and max values on this channel
	if(JoyPos>max){
		_joy_y_max=JoyPos;
		max=JoyPos;
	}
	if(JoyPos<min){
		_joy_y_min=JoyPos;
		min=JoyPos;
	}
	bool m=false;
	// select the bounding value to map to
	if (JoyPos < center) {
		value=min;

	} else if (JoyPos > center) {
		value=max;
		m=true;
	} else
		return 0;
	// calculate a mapping value
	float valueDiff = value -center;
	float joyDiff =  JoyPos -center;
//	Serial.print("Raw  center ");
//	Serial.print (center);
//
//	Serial.print(" Raw  val ");
//	Serial.print (JoyPos);
//	Serial.print("  ");
//	Serial.print(" Raw bound ");
//	Serial.print (value);
//	Serial.print("  ");
	return (int) ((100.0*joyDiff)/valueDiff)*(m?1:-1);
}

int WiiChuck::getRollAngle() {
	return (int) (atan2((double) getAccelX(), (double) getAccelZ()) * 180.0 / PI);
}

int WiiChuck::getPitchAngle() {
	return (int) (atan2((double) getAccelY(), (double) getAccelZ()) * 180.0 / PI);
}

int WiiChuck::getAccelX() {
	return ((_dataarray[2] << 2) + ((_dataarray[5] & 0x0C) >> 2)) - 512;
}

int WiiChuck::getAccelY() {
	return ((_dataarray[3] << 2) + ((_dataarray[5] & 0x30) >> 4)) - 512;
}

int WiiChuck::getAccelZ() {
	return ((_dataarray[4] << 2) + ((_dataarray[5] & 0xC0) >> 6)) - 512;;
}

boolean WiiChuck::checkButtonC() {
	int buttons = _dataarray[5] & 0x03;
	switch (buttons) {
	case 0:
		return false;
	case 1:
		return true;
	case 2:
		return true;
	case 3:
		return false;
	}
}
boolean WiiChuck::leftShoulderPressed() {
	return _PressedRowBit(0, 5);
}

boolean WiiChuck::rightShoulderPressed() {
	return _PressedRowBit(0, 1);
}

boolean WiiChuck::lzPressed() {
	return _PressedRowBit(1, 7);
}

boolean WiiChuck::rzPressed() {
	return _PressedRowBit(1, 2);
}

boolean WiiChuck::leftDPressed() {
	return _PressedRowBit(1, 1);
}

boolean WiiChuck::rightDPressed() {
	return _PressedRowBit(0, 7);
}

boolean WiiChuck::upDPressed() {
	return _PressedRowBit(1, 0);
}

boolean WiiChuck::downDPressed() {
	return _PressedRowBit(0, 6);
}

boolean WiiChuck::selectPressed() {
	return _PressedRowBit(0, 4);
}

boolean WiiChuck::homePressed() {
	return _PressedRowBit(0, 3);
}

boolean WiiChuck::startPressed() {
	return _PressedRowBit(0, 2);
}

boolean WiiChuck::xPressed() {
	return _PressedRowBit(1, 3);
}

boolean WiiChuck::yPressed() {
	return _PressedRowBit(1, 5);
}

boolean WiiChuck::aPressed() {
	return _PressedRowBit(1, 4);
}

boolean WiiChuck::bPressed() {
	return _PressedRowBit(1, 6);
}

int WiiChuck::rightShouldPressure() {
	return _dataarray[3] & 0x1f; //rightmost 5 bits
}

int WiiChuck::leftShouldPressure() {
	return ((_dataarray[2] & 0x60) >> 2) + ((_dataarray[3] & 0xe0) >> 5); //rightmost 5 bits
}

int WiiChuck::leftStickX() {
	return ((_dataarray[0] & 0x3f) >> 1); //Modified
}

int WiiChuck::leftStickY() {
	return ((_dataarray[1] & 0x3f)) >> 1; //Modified
}

int WiiChuck::rightStickX() {
	return (((_dataarray[0] & 0xc0) >> 3) + ((_dataarray[1] & 0xc0) >> 5)
			+ ((_dataarray[2] & 0x80) >> 7));

}

int WiiChuck::rightStickY() {
	return (_dataarray[2] & 0x1f);
}

boolean WiiChuck::checkButtonZ() {

	int buttons = _dataarray[5] & 0x03;
	switch (buttons) {
	case 0:
		return true;
	case 1:
		return false;
	case 2:
		return true;
	case 3:
		return false;
	}
}

// Create a map between controller and a servo
void WiiChuck::addControlMap(int servoPin, int servoMin, int servoCenter,
		int servoMax, int axisMin, int axisCenter, int axisMax,
		FunctionMapName mapName) {

	addControlMap(servoPin, servoMin, servoCenter, servoMax, axisMin,
			axisCenter, axisMax, mapName, NOBUTTON);
}
void WiiChuck::addButtonMap(int servoPin, int servoMin, int servoMax,
		ButtonMapName mapName) {
	addControlMap(servoPin, servoMin, 0, servoMax, 0, 0, 1, NOFUNCTION,
			mapName);
}

/* Private */

void WiiChuck::addControlMap(int servoPin, int servoMin, int servoCenter,
		int servoMax, int axisMin, int axisCenter, int axisMax,
		FunctionMapName mapName, ButtonMapName button) {
	ServoWiiControllerMap* newMap = new ServoWiiControllerMap();

	newMap->axisMin = axisMin;
	newMap->axisMax = axisMax;
	newMap->axisCenter = axisCenter;
	newMap->servoMin = servoMin;
	newMap->servoMax = servoMax;
	newMap->servoCenter = servoCenter;
	newMap->myservo.attach(servoPin);
	newMap->myservo.write(servoCenter);
	newMap->name = mapName;
	newMap->button = button;
	newMap->next = NULL;
//	Serial.print("Adding servo  = ");
//	Serial.print(servoPin);
//	Serial.println();
	if (maps == NULL) {
		maps = newMap;
	} else {
		ServoWiiControllerMap * tmp = maps;
		while (tmp != NULL) {
			if (tmp->next == NULL) {
				tmp->next = newMap;

				tmp = NULL; //break the loop
			} else {
				tmp = tmp->next;
			}
		}
	}
	numMaps++;

}

int WiiChuck::performMap(ServoWiiControllerMap * tmp) {
	if (tmp->name !=NOFUNCTION) {
		//Serial.println("Function");
		int value = 0;
		switch (tmp->name) {
		case JOY_X:
			value = getJoyX();
			break;
		case JOY_Y:
			value = getJoyY();
			break;
		case ROLL:
			value = getRollAngle();
			break;
		case PITCH:
			value = getPitchAngle();
			break;
		case ACCELX:
			value = getAccelX();
			break;
		case ACCELY:
			value = getAccelY();
			break;
		case ACCELZ:
			value = getAccelZ();
			break;
		case RSPRESSURE:
			value = rightShouldPressure();
			break;
		case LSPRESSURE:
			value = leftShouldPressure();
			break;
		case LEFTSTICKX:
			value = leftStickX();
			break;
		case LEFTSTICKY:
			value = leftStickY();
			break;
		case RIGHTSTICKX:
			value = rightStickX();
			break;
		case RIGHTSTICKY:
			value = rightStickY();
			break;
//		default:
//			Serial.println("No function");
		}
		int serv=0,axis=0;
		if(value>tmp->axisCenter){
			axis = tmp->axisMax;
			serv=tmp->servoMax;
		}else{
			axis = tmp->axisMin;
			serv=tmp->servoMin;
		}
		float axisRange =(float)(value-tmp->axisCenter);
		float valueRange =(float)(axis -tmp->axisCenter);
		float servoRange =serv -tmp->servoCenter;
		int servoIncremt =(int) (servoRange*axisRange/valueRange);
//		Serial.print(" incoming ");
//		Serial.print(value);
//		Serial.print(" axisRange ");
//		Serial.print(axisRange);
//		Serial.print(" valueRange ");
//		Serial.print(valueRange);
//		Serial.print(" servoRange ");
//		Serial.print(servoRange);
//		Serial.print(" servoIncremt ");
//		Serial.print(servoIncremt);
		return tmp->servoCenter+servoIncremt;
	}
	if (tmp->button != NOBUTTON) {
		//Serial.println("Button");
		boolean value = false;
		switch (tmp->button) {
		case CBUTTON:
			value = checkButtonC();
			break;
		case ZBUTTON:
			value = checkButtonZ();
			break;
		case LZ:
			value = lzPressed();
			break;
		case RZ:
			value = rzPressed();
			break;
		case LD:
			value = leftDPressed();
			break;
		case RD:
			value = rightDPressed();
			break;
		case UD:
			value = upDPressed();
			break;
		case DD:
			value = downDPressed();
			break;
		case SL:
			value = selectPressed();
			break;
		case H:
			value = homePressed();
			break;
		case START:
			value = startPressed();
			break;
		case X:
			value = xPressed();
			break;
		case Y:
			value = yPressed();
			break;

		case A:
			value = aPressed();
			break;

		case B:
			value = bPressed();
			break;


		}
		return value?tmp->servoMax:tmp->servoMin;
	}
}
boolean WiiChuck::_PressedRowBit(byte row, byte bit) {
	byte mask = (1 << bit);
	return !(_dataarray[row + 4] & mask);
}

void WiiChuck::_sendStart(byte addr) {
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, LOW);
	_shiftOut(_sda_pin, _scl_pin, addr);
}

void WiiChuck::_sendStop() {
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_sda_pin, HIGH);
	pinMode(_sda_pin, INPUT);
}

void WiiChuck::_sendNack() {
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, HIGH);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void WiiChuck::_sendAck() {
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	digitalWrite(_sda_pin, LOW);
	digitalWrite(_scl_pin, HIGH);
	digitalWrite(_scl_pin, LOW);
	pinMode(_sda_pin, INPUT);
}

void WiiChuck::_waitForAck() {
	pinMode(_sda_pin, INPUT);
	digitalWrite(_scl_pin, HIGH);
	unsigned long time = millis();
	while (digitalRead(_sda_pin) == HIGH && (time + ackTimeout) < millis()) {
	}
	if ((time + ackTimeout) < millis()) {
		_timeoutCount++;
		if (_timeoutCount > 10) {
			_timeoutCount = 0;
			//Serial.println("Timeout reset");
			begin();
		}
	}
	digitalWrite(_scl_pin, LOW);
}

uint8_t WiiChuck::_readByte() {
	pinMode(_sda_pin, INPUT);

	uint8_t value = 0;
	uint8_t currentBit = 0;

	for (int i = 0; i < 8; ++i) {
		digitalWrite(_scl_pin, HIGH);
		currentBit = digitalRead(_sda_pin);
		value |= (currentBit << 7 - i);
		if (_clockSpacing > 0)
			delayMicroseconds(_clockSpacing);
		digitalWrite(_scl_pin, LOW);
		if (_clockSpacing > 0)
			delayMicroseconds(_clockSpacing);
	}
	return value;
}

void WiiChuck::_writeByte(uint8_t value) {
	pinMode(_sda_pin, OUTPUT);
	_shiftOut(_sda_pin, _scl_pin, value);
}
void WiiChuck::initBytes() {
	switch (type) {
	case THIRDPARTYWII:
		// improved startup procedure from http://playground.arduino.cc/Main/WiiChuckClass
		_writeRegister(0xF0, 0x55);
		_writeRegister(0xFB, 0x00);
		break;
	case OFFICIALWII:
	case WIICLASSIC:

		break;
//	default:
//		Serial.println("Error, specify a controller type");
	}
	_writeRegister(0x40, 0x00);
}

void WiiChuck::_shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t val) {
	uint8_t i;

	for (i = 0; i < 8; i++) {
		digitalWrite(dataPin, (val & (1 << (7 - i))) == 0 ? 0 : 1);
		digitalWrite(clockPin, HIGH);
		if (_clockSpacing > 0)
			delayMicroseconds(_clockSpacing);
		digitalWrite(clockPin, LOW);
		if (_clockSpacing > 0)
			delayMicroseconds(_clockSpacing);
	}
}

