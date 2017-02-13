#include "Nunchuck.h"
Nunchuck::Nunchuck(uint8_t data_pin, uint8_t sclk_pin) :
		Accessory(data_pin, sclk_pin) {

}
int Nunchuck::getJoyX() {
	return decodeInt(0,0,0,0,0,0,BYTE0,BIT0,BIT7); // see http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
}
int Nunchuck::getJoyY() {
	return 0;
}

int Nunchuck::getRollAngle() {
	return 0;
}
int Nunchuck::getPitchAngle() {
	return 0;
}
int Nunchuck::getAccelX() {
	return decodeInt(0,0,0,BYTE2,BIT0,BIT7,BYTE5,BIT4,BIT5);
}
int getAccelY() {
	return 0;
}
int getAccelZ() {
	return 0;
}
boolean Nunchuck::checkButtonC() {
	return decodeBit(BYTE5,BIT1,true); 
}
boolean Nunchuck::checkButtonZ() {
	return false;
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
	switch (tmp->name) {
	case JOY_X:
		return getJoyX();
	case JOY_Y:
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
	return false;
}
