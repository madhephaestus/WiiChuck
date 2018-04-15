#include "Accessory.h"
#include <Wire.h>

Accessory::Accessory() {
	type=NUNCHUCK;
}
/**
 * Reads the device type from the controller
 */
ControllerType Accessory::getControllerType() {
	return type;
}

ControllerType Accessory::identifyController() {
	//Serial.println("Reading periph bytes");
	_burstRead(0xfa);
	//printInputs(Serial);

	if (_dataarray[4] == 0x00)
		if (_dataarray[5] == 0x00)
			return NUNCHUCK; // nunchuck

	if (_dataarray[4] == 0x01)
		if (_dataarray[5] == 0x01)
			return WIICLASSIC; // Classic Controller

	if (_dataarray[0] == 0x00)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x03)
							return GuitarHeroController; // Guitar Hero Controller

	if (_dataarray[0] == 0x01)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x03)
							return GuitarHeroWorldTourDrums; // Guitar Hero World Tour Drums

	if (_dataarray[0] == 0x03)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x03)
							return Turntable; // Guitar Hero World Tour Drums

	if (_dataarray[0] == 0x00)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x11)
							return DrumController; // Taiko no Tatsujin TaTaCon (Drum controller)

	if (_dataarray[0] == 0xFF)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x00)
						if (_dataarray[5] == 0x13)
							return DrawsomeTablet; // Drawsome Tablet

	return Unknown;
}

void Accessory::sendMultiSwitch(uint8_t iic, uint8_t sw) {
	uint8_t err = 0;
	int i = 0;
	for (; i < 10; i++) {
		Wire.beginTransmission(iic);
		Wire.write(1 << sw);
		Wire.endTransmission();
		err = Wire.endTransmission();
		if (err != 0) {
			Serial.println("sendMultiSwitch Resetting because of " + String(err));
			reset();
		}else
			return;
	}

}

void Accessory::addMultiplexer(uint8_t iic, uint8_t sw) {
	if (sw >= 8)
		return;

	_multiplexI2C = iic;
	_multiplexSwitch = sw;
}

void Accessory::switchMultiplexer() {
	if (_multiplexI2C == 0)
		return; // No multiplexer set
	sendMultiSwitch(_multiplexI2C, _multiplexSwitch);
}

void Accessory::switchMultiplexer(uint8_t iic, uint8_t sw) {
	if (sw >= 8)
		return;
#if defined(TWCR)
	if (TWCR == 0)
#endif
		Wire.begin();
	 // Start I2C if it's not running
	sendMultiSwitch(iic, sw);
}

/*
 * public function to read data
 */
boolean Accessory::readData() {
	switchMultiplexer();

	if (_burstRead()) {
		return true;
	}
	return false;
}

uint8_t* Accessory::getDataArray() {
	return _dataarray;
}

void Accessory::initBytes() {
	//Serial.println("Init Periph..");
	_writeRegister(0xF0, 0x55);
	_writeRegister(0xFB, 0x00);
	delay(100);

	type = identifyController();
	delay(100);

	if (_encrypted) {
		//Serial.println("Beginning Encrypted Comms");

		delay(100);

		_writeRegister(0xF0, 0xAA); // enable enc mode?
		delay(90);

		Accessory::_burstWriteWithAddress(0x40, _key_table_1, 8);
		Accessory::_burstWriteWithAddress(0x48, _key_table_1 + 0x8, 8);
		delay(100);

		//_writeRegister(0x40, 0x00);
	}
}

void Accessory::setDataArray(uint8_t data[6]) {
	for (int i = 0; i < 6; i++)
		_dataarray[i] = data[i];
}



int Accessory::decodeInt(uint8_t mmsbbyte, uint8_t mmsbstart, uint8_t mmsbend,
		uint8_t msbbyte, uint8_t msbstart, uint8_t msbend, uint8_t csbbyte,
		uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte, uint8_t lsbstart,
		uint8_t lsbend) {
// 6 bit int split across 3 bytes in 4 parts.... :(
	bool msbflag = false, csbflag = false, lsbflag = false, mmsbflag = false;
	if (msbbyte > 5)
		msbflag = true;
	if (csbbyte > 5)
		csbflag = true;
	if (lsbbyte > 5)
		lsbflag = true;
	if (mmsbbyte > 5)
		mmsbflag = true;

	uint32_t analog = 0;
	uint32_t lpart = 0;
	lpart = (lsbflag) ? 0 : _dataarray[lsbbyte];
	lpart = lpart >> lsbstart;
	lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

	uint32_t cpart = 0;
	cpart = (csbflag) ? 0 : _dataarray[csbbyte];
	cpart = cpart >> csbstart;
	cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

	cpart = cpart << ((lsbend - lsbstart) + 1);

	uint32_t mpart = 0;
	mpart = (msbflag) ? 0 : _dataarray[msbbyte];
	mpart = mpart >> msbstart;
	mpart = mpart & (0xFF >> (7 - (msbend - msbstart)));

	mpart = mpart << (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1));

	uint32_t mmpart = 0;
	mmpart = (mmsbflag) ? 0 : _dataarray[mmsbbyte];
	mmpart = mmpart >> mmsbstart;
	mmpart = mmpart & (0xFF >> (7 - (mmsbend - mmsbstart)));

	mmpart = mmpart
			<< (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1)
					+ ((msbend - msbstart) + 1));

	analog = lpart | cpart | mpart | mmpart;
	//analog = analog + offset;
	//analog = (analog*scale);

	return analog;
}

int Accessory::decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
		uint8_t csbbyte, uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte,
		uint8_t lsbstart, uint8_t lsbend) {
// 5 bit int split across 3 bytes. what... the... fuck... nintendo...
	bool msbflag = false, csbflag = false, lsbflag = false;
	if (msbbyte > 5)
		msbflag = true;
	if (csbbyte > 5)
		csbflag = true;
	if (lsbbyte > 5)
		lsbflag = true;

	uint32_t analog = 0;
	uint16_t lpart = 0;
	lpart = (lsbflag) ? 0 : _dataarray[lsbbyte];
	lpart = lpart >> lsbstart;
	lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

	uint16_t cpart = 0;
	cpart = (csbflag) ? 0 : _dataarray[csbbyte];
	cpart = cpart >> csbstart;
	cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

	cpart = cpart << ((lsbend - lsbstart) + 1);

	uint16_t mpart = 0;
	mpart = (msbflag) ? 0 : _dataarray[msbbyte];
	mpart = mpart >> msbstart;
	mpart = mpart & (0xFF >> (7 - (msbend - msbstart)));

	mpart = mpart << (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1));

	analog = lpart | cpart | mpart;
	//analog = analog + offset;
	//analog = (analog*scale);

	return analog;
}

bool Accessory::decodeBit(uint8_t byte, uint8_t bit, bool activeLow) {
	if (byte > 5)
		return false;
	uint8_t swb = _dataarray[byte];
	uint8_t sw = (swb >> bit) & 0x01;
	return activeLow ? (!sw) : (sw);
}

void Accessory::begin() {
#if defined(TWCR)
	if (TWCR == 0)
#endif
		Wire.begin();
	// Start I2C if it's not running

	switchMultiplexer();

	initBytes();
	identifyController();
	if(getControllerType()==DrawsomeTablet){
		initBytesDrawsome();
	}
	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
}

boolean Accessory::_burstRead(uint8_t addr) {
	//int readAmnt = sizeof(_dataarray);
	uint8_t err = 0;
	int i = 0;
	for (; i < 10; i++) {
		Wire.beginTransmission(WII_I2C_ADDR);
		Wire.write(addr);
		err = Wire.endTransmission();
		if (err == 0) {			// wait for data to be converted
			delayMicroseconds(175);

			// read data
			uint8_t readBytes = Wire.readBytes(_dataarray,

					Wire.requestFrom(WII_I2C_ADDR, sizeof(_dataarray)));

			if (_encrypted) {
				for (int i = 0; i < sizeof(_dataarray); i++)
					_dataarray[i] = decryptByte(_dataarray[i], addr + i);

			}
			getValues();//parse the data into readable data
			return readBytes == sizeof(_dataarray);
		}
		if(i>5)
			Serial.println("_burstRead Resetting because of " + String(err)+" repeted: "+String(i));
		reset();
	}

	return false;
}

void Accessory::_writeRegister(uint8_t reg, uint8_t value) {
	//Serial.print("W ");
	//Serial.print(reg,HEX);
	//Serial.print(": ");
	//Serial.println(value,HEX);
	uint8_t err = 0;
	int i = 0;
	for (; i < 10; i++) {
		Wire.beginTransmission(WII_I2C_ADDR);
		Wire.write(reg);
		Wire.write(value);
		err = Wire.endTransmission();
		if (err != 0) {
			Serial.println("_writeRegister Resetting because of " + String(err)+" repeted: "+String(i));
			reset();
		}else
			return;
	}

}

void Accessory::_burstWriteWithAddress(uint8_t addr, uint8_t* arr,
		uint8_t size) {
	//Serial.print("W ");
	//Serial.print(addr,HEX);
	//Serial.print(": ");
	//for (int i=0; i<size; i++) {//Serial.print(arr[i],HEX);//Serial.print(" ");
	//}
	//Serial.println("");
	uint8_t err = 0;
	int i = 0;
	for (; i < 3; i++) {
		Wire.beginTransmission(WII_I2C_ADDR);
		Wire.write(addr);
		for (int i = 0; i < size; i++)
			Wire.write(arr[i]);
		err = Wire.endTransmission();
		if (err != 0) {
			Serial.println("_burstWriteWithAddress Resetting because of " + String(err)+" repeted: "+String(i));
			reset();
		}else
			return;
	}

}

void Accessory::reset(){
#if defined(ARDUINO_ARCH_ESP32)
	Wire.reset();
#endif
}

void Accessory::enableEncryption(bool enc) {
	_encrypted = enc;
}



int Accessory::smap(int16_t val, int16_t aMax, int16_t aMid, int16_t aMin,
		int16_t sMax, int16_t sZero, int16_t sMin) {
	int mapv = sZero;
	if (val > aMid) {
		mapv = map(val, aMid, aMax, sZero, sMax);
	} else if (val < aMid) {
		mapv = map(val, aMin, aMid, sMin, sZero);
	}
//Serial.print(val);Serial.print(" ");Serial.println(mapv);

	return mapv;
}



uint8_t Accessory::decryptByte(uint8_t byte, uint8_t address) {
//return (byte ^ _key_table_1[address % 8]) + _key_table_1[(address % 8)+0x08];
	return (byte ^ 0x97) + 0x97;
}

void Accessory::printInputs(Stream& stream) {
	switch(getControllerType()){
	case WIICLASSIC:
		printInputsClassic(stream);
		break;
	case GuitarHeroController:
		printInputsGuitar(stream);
		break;
	case GuitarHeroWorldTourDrums:
		printInputsDrums(stream);
		break;
	case DrumController:
		printInputsDrums(stream);
		break;
	case DrawsomeTablet:
		printInputsDrawsome(stream);
		break;
	case Turntable:
		printInputsDj(stream);
		break;
	case NUNCHUCK:
		printInputsNunchuck(stream);
		break;
	default:
		stream.println("Unknown controller!");
		break;

	}
}

uint8_t * Accessory::getValues(){
	switch(getControllerType()){
	case WIICLASSIC:
		getValuesClassic(values);
		break;
	case GuitarHeroController:
		getValuesGuitar(values);
		break;
	case GuitarHeroWorldTourDrums:
		getValuesDrums(values);
		break;
	case DrumController:
		getValuesDrums(values);
		break;
	case DrawsomeTablet:
		getValuesDrawsome(values);
		break;
	case Turntable:
		getValuesDj(values);
		break;
	case NUNCHUCK:
	default:
		getValuesNunchuck(values);
		break;

	}
	return values;
};

