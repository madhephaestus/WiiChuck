#include "Accessory.h"



int  Accessory::getStickXGuitar() {
	return decodeInt(stickXBytes); 
}



int  Accessory::getStickYGuitar() {
	return decodeInt(stickYBytes); 
}



int  Accessory::getWhammyBar() {
	return decodeInt(whammyBarBytes); 
}




int  Accessory::getPlusButtonGuitar() {
	return decodeBit(plusButtonBytes); 
}



int  Accessory::getMinusButtonGuitar() {
	return decodeBit(minusButtonBytes); 
}



int  Accessory::getGreenButton() {
	return decodeBit(greenButtonBytes); 
}



int  Accessory::getRedButton() {
	return decodeBit(redButtonBytes); 
}


int  Accessory::getYellowButton() {
	return decodeBit(yellowButtonBytes); 
}


int  Accessory::getBlueButton() {
	return decodeBit(blueButtonBytes); 
}


int  Accessory::getOrangeButton() {
	return decodeBit(orangeButtonBytes); 
}


int  Accessory::getStrumUp() {
	return decodeBit(strumUpBytes); 
}

int  Accessory::getStrumDown() {
	return decodeBit(strumDownBytes); 
}
void Accessory::getValuesGuitar(uint8_t * values){
	values[0]=map(getWhammyBar(),0,255,0,256);
	values[1]=0;
	values[2]=0;
	values[3]=0;
	values[4]=0;
	values[5]=0;

	values[6]=getPlusButtonGuitar()?255:(getMinusButtonGuitar()?0:128);
	values[7]=getStrumUp()?255:(getStrumDown()?0:128);
	values[8]=0;
	values[9]=0;
	values[10]=getGreenButton()?255:0;
	values[11]=getRedButton()?255:0;
	values[12]=getYellowButton()?255:0;
	values[13]=getBlueButton()?255:0;

	values[14]=getOrangeButton()?255:0;
	values[15]=getLeftGreenButton()?255:0;
	values[16]=getButtonPlus()?255:0;

	values[17]=0;
	values[18]=0;
	for(int i=0;i<WII_VALUES_ARRAY_SIZE;i++){
		if(values[i]>247){
			values[i]=255;
		}
	}
}

void  Accessory::printInputsGuitar(Stream& stream) {
	char st[100];
	sprintf(st," stick x: %4d | stick y: %4d | whammy bar: %4d | Buttons: ",getStickXGuitar(),getStickYGuitar(),getWhammyBar());
	stream.print(st);
	
	if (getPlusButtonGuitar())
		stream.print("plus button");

	if (getMinusButtonGuitar())
		stream.print("minus button");

	if (getGreenButton())
		stream.print("green button");

	if (getRedButton())
		stream.print("red button");

	if (getYellowButton())
		stream.print("yellow button");

	if (getBlueButton())
		stream.print("blue button");

	if (getOrangeButton())
		stream.print("orange button");

	if (getStrumUp())
		stream.print("strum up");

	if (getStrumDown())
		stream.print("strum down");

	stream.println("");
}
