#include "Nunchuck.h"
Nunchuck::Nunchuck(uint8_t data_pin, uint8_t sclk_pin) :
		Accessory(data_pin, sclk_pin) {
	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
	_joy_x_center = _rawgetJoyX() ;
	_joy_y_center = _rawgetJoyY();

}
int Nunchuck::getJoyX() {
	int JoyPos =_rawgetJoyX();
	int center = _joy_x_center;
	int min = 0;
	int max = 255;

	int value =0;

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

	return (int) ((100.0*joyDiff)/valueDiff)*(m?1:-1);
}
int Nunchuck::getJoyY() {
	return
	int JoyPos = _rawgetJoyY();
	int center = _joy_y_center;
	int min = 0;
	int max = 255;

	int value =0;

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

	return (int) ((100.0*joyDiff)/valueDiff)*(m?1:-1);
}
int Nunchuck::_rawgetJoyX() {
	return decodeInt(0,0,0,0,0,0,BYTE0,BIT0,BIT7); // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}
int Nunchuck::_rawgetJoyY() {
	return decodeInt(0,0,0,0,0,0,BYTE1,BIT0,BIT7); // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}

int Nunchuck::getRollAngle() {
	return (int) (atan2((double) getAccelX(), (double) getAccelZ()) * 180.0 / PI);
}
int Nunchuck::getPitchAngle() {
	return (int) (atan2((double) getAccelY(), (double) getAccelZ()) * 180.0 / PI);
}
int Nunchuck::getAccelX() {
	return decodeInt(0,0,0,BYTE2,BIT0,BIT7,BYTE5,BIT2,BIT3) - 512; // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}
int Nunchuck::getAccelY() {
	return decodeInt(0,0,0,BYTE3,BIT0,BIT7,BYTE5,BIT4,BIT5) - 512; // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}
int Nunchuck::getAccelZ() {
	return decodeInt(0,0,0,BYTE4,BIT0,BIT7,BYTE5,BIT6,BIT7) - 512; // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}
boolean Nunchuck::checkButtonC() {
	return decodeBit(BYTE5,BIT1,true);  // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}
boolean Nunchuck::checkButtonZ() {
	return decodeBit(BYTE5,BIT0,true);  // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}

void Nunchuck::printInputs(Stream& stream) {
	char st[100];
	stream.println("");

	stream.print("NUNCHUCK ");
	sprintf(st,
			"  JoyX: %4d%% | JoyY: %4d%% | Roll: %4d | Pitch: %4d | Buttons: ",
			getJoyX(), getJoyY(), getRollAngle(), getPitchAngle());

	stream.print(st);

	if (checkButtonC())
		stream.print("C");
	else
		stream.print("-");
	if (checkButtonZ())
		stream.print("Z");
	else
		stream.print("-");
	stream.println();
}

int Nunchuck::getAnalog(FunctionMapName name) {
	switch (name) {
	case JOY_X:
	case RIGHTSTICKX:
		return getJoyX();
	case JOY_Y:
	case RIGHTSTICKY:
		return getJoyY();

	case ROLL:
		return getRollAngle();

	case PITCH:
		return getPitchAngle();

	case ACCELX:
		return getAccelX();

	case ACCELY:
		return getAccelY();

	case ACCELZ:
		return getAccelZ();

	default:
		return 0;
	}
}
boolean Nunchuck::getDigital(ButtonMapName name) {
	switch (name) {
	case CBUTTON:
		return checkButtonC();
	case ZBUTTON:
		return checkButtonZ();
	default:
			return false;
		}
}
