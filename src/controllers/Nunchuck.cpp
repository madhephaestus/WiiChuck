#include "Nunchuck.h"
Nunchuck::Nunchuck(uint8_t data_pin, uint8_t sclk_pin):Accessory( data_pin,  sclk_pin){

}
int Nunchuck::getJoyX() {
	return 0;
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
	return 0;
}
int getAccelY() {
	return 0;
}
int getAccelZ() {
	return 0;
}
boolean Nunchuck::checkButtonC() {
	return false;
}
boolean Nunchuck::checkButtonZ() {
	return false;
}

void Nunchuck::printInputs(Stream& stream){
	char st[100];
  stream.print("Accessory Bytes:\t");
  for(int i=0; i<6; i++) {
         if (_dataarray[i]<0x10) {stream.print("0");}
         stream.print(_dataarray[i],HEX);
         stream.print(" ");
  }

  stream.println("");

  stream.print("NUNCHUCK ");
	stream.print("JoyX BYTE: ");
	//stream.print(_dataarray[0], BIN) ;
	stream.print(_dataarray[0], HEX);
	stream.print("  JoyY BYTE: ");
	//stream.print(_dataarray[1], BIN) ;
	stream.print(_dataarray[1], HEX);
	sprintf(st,
			"  JoyX: %4d%% | JoyY: %4d%% | Roll: %4d | Pitch: %4d | Buttons: ",
			getJoyX(), getJoyY(), getRollAngle(),
			getPitchAngle());

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
