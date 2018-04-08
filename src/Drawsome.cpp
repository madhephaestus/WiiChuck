#include "Accessory.h"


void Accessory::initBytesDrawsome() {
	Serial.println("Drawesome Init");

	// Enable Encrypted Mode.
	//enableEncryption(true);

	Serial.println("Drawesome Specific init");

	_burstRead(0x20);
	_burstRead(0x28);
	delay(100);

	_burstRead(0x30);
	_burstRead(0x38);
	delay(100);

	_burstRead(0x00);
	delay(100);

	_burstRead(0x00);
	delay(100);

	_writeRegister(0xfb,0x01);

	_burstRead(0x00);
	delay(100);

	_burstRead(0x00);
	delay(100);
}

 void Accessory::getValuesDrawsome(uint8_t * values){
		values[0]=map(getPenXPosition(),0,64,0,256);
		values[1]=map(getPenYPosition(),0,64,0,256);
		values[2]=map(getPenPressure(),0,32,0,256);
		values[3]=0;
		values[4]=0;
		values[5]=0;

		values[6]=0;
		values[7]=0;
		values[8]=0;
		values[9]=0;
		values[10]=getPenContact()?255:0;
		values[11]=0;
		values[12]=0;
		values[13]=0;

		values[14]=0;
		values[15]=0;
		values[16]=0;

		values[17]=0;
		values[18]=0;
		for(int i=0;i<WII_VALUES_ARRAY_SIZE;i++){
			if(values[i]>247){
				values[i]=255;
			}
		}
}


void Accessory::printInputsDrawsome(Stream& stream) {
	char st[100];
	stream.print("Drawsome ");
	sprintf(st,
	        "  PenX: %4d  | PenY: %4d | Force: %4d | pen?: ",
	        getPenXPosition(), getPenYPosition(), getPenPressure());

	stream.print(st);

	if (getPenContact())
		stream.print("YES ");
	else
		stream.print("--- ");

	for(byte mask = 0x80; mask; mask >>= 1){
		if(mask  & _dataarray[0])
			Serial.print('1');
		else
			Serial.print('0');
	}
	Serial.print(' ');
}

int Accessory::getPenXPosition() {
	return decodeInt(penXPositionBytes); 
}

int Accessory::getPenYPosition() {
	return decodeInt(penYPositionBytes); 
}

int Accessory::getPenPressure() {
	return decodeInt(penPressureBytes); 
}

int Accessory::getPenContact() {
	return decodeBit(penContactBytes); 
}
