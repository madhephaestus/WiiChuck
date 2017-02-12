#include "Accessory.h"
#include <Wire.h>


Accessory::Accessory(uint8_t data_pin, uint8_t sclk_pin) {
	_sda_pin = data_pin;
	_scl_pin = sclk_pin;
	_callCount = 0;
	_clockSpacing = 1;
	ackTimeout = 100;
	usePullUpClock=false;

}
/**
 * Reads the device type from the controller
 */
ControllerType Accessory::identifyController(){
	_burstReadWithAddress(0xfe);

  if (_dataarray[0] == 0x01)
    if (_dataarray[1] == 0x01)
      return WIICLASSIC; // Classic Controller

  if (_dataarray[0] == 0x00)
    if (_dataarray[1] == 0x00)
      return NUNCHUCK; // nunchuck

  // It's something else.
  _burstReadWithAddress(0xfa);

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
              return GuitarHeroWorldTourDrums; // Guitar Hero World Tour Drums

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

  return Mystery;
}

/*
 * public function to read data
 */
void Accessory::readData() {

	_burstRead();

}

uint8_t* Accessory::getDataArray(){
  return _dataarray;
}

void Accessory::setDataArray(uint8_t data[6]){
  for(int i=0; i<6; i++) _dataarray[i] = data[i];
}
  
void Accessory::printInputs(Stream& stream){
  stream.print("Accessory Bytes:\t");
  for(int i=0; i<6; i++) {
         if (_dataarray[i]<0x10) {stream.print("0");} 
         stream.print(_dataarray[i],HEX); 
         stream.print(" "); 
  }
  stream.println("");
}

int   Accessory::decodeInt(uint8_t msbbyte,uint8_t msbstart,uint8_t msbend,
                           uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                           uint8_t lsbbyte, uint8_t lsbstart, uint8_t lsbend){
// 5 bit int split across 3 bytes. what... the... fuck... nintendo...

  if (msbbyte>5) return false;
  if (csbbyte>5) return false;
  if (lsbbyte>5) return false;

  uint32_t analog=0;
  uint32_t lpart;
  lpart = _dataarray[lsbbyte];
  lpart = lpart >> lsbstart;
  lpart = lpart &  (0xFF>>(7-(lsbend-lsbstart)) ) ;
  
  uint32_t cpart;
  cpart = _dataarray[csbbyte];
  cpart = cpart >> csbstart;
  cpart = cpart &  (0xFF>>(7-(csbend-csbstart)) ) ;
  
  cpart = cpart << ((lsbend-lsbstart)+1);
  
  uint32_t mpart;
  mpart = _dataarray[msbbyte];
  mpart = mpart >> msbstart;
  mpart = mpart &  (0xFF>>(7-(msbend-msbstart)) ) ;
  
  mpart = mpart << ( ((lsbend-lsbstart)+1) + ((csbend-csbstart)+1) ) ;
  
  analog = lpart | cpart | mpart;
  
  return analog;
  
}

bool  Accessory::decodeBit(uint8_t byte, uint8_t bit, bool activeLow){
  if (byte>5) return false;
  uint8_t swb = _dataarray[byte];
  uint8_t sw   = (swb>>bit) & 0x01;
  return activeLow?(!sw):(sw);
} 

void Accessory::_sendStart(byte addr) {
	_dataHigh();
	_clockHigh();
	_dataLow();
	_clockLow();
	_shiftOut( addr);

}

void Accessory::_sendStop() {
	_clockLow();
	_clockHigh();
	_dataHigh();
	pinMode(_sda_pin, INPUT);
}

void Accessory::_sendNack() {
	_clockLow();
	_dataHigh();
	_clockHigh();
	_clockLow();
	pinMode(_sda_pin, INPUT);
}

void Accessory::_sendAck() {
	_clockLow();
	_dataLow();
	_clockHigh();
	_clockLow();
	pinMode(_sda_pin, INPUT);
}

void Accessory::_dataHigh(){
	//Serial.println("high");
	if(usePullUpClock){
		pinMode(_sda_pin, INPUT);
	}else{
		pinMode(_sda_pin, OUTPUT);
		digitalWrite(_sda_pin, HIGH);
	}

}
void Accessory::_dataLow(){
	//Serial.println("low");
	pinMode(_sda_pin, OUTPUT);
	digitalWrite(_sda_pin, LOW);

}
void Accessory::_clockHigh(){
	//Serial.println("high");
	if(usePullUpClock){
		_clockStallCheck();
	}else{
		pinMode(_scl_pin, OUTPUT);
		digitalWrite(_scl_pin, HIGH);
	}
//	pinMode(_scl_pin, OUTPUT);
//	digitalWrite(_scl_pin, HIGH);
	if (_clockSpacing > 0)delayMicroseconds(_clockSpacing);

}
void Accessory::_clockLow(){
	//Serial.println("low");
	pinMode(_scl_pin, OUTPUT);
	digitalWrite(_scl_pin, LOW);
	if (_clockSpacing > 0)delayMicroseconds(_clockSpacing);

}

void Accessory::_clockStallCheck(){
	pinMode(_scl_pin, INPUT);

	unsigned long time = millis();
	while (digitalRead(_scl_pin) != HIGH && (time + ackTimeout) < millis()) {
	}
//	if ((time + ackTimeout) < millis()) {
//		_timeoutCount++;
//		if (_timeoutCount > 10) {
//			_timeoutCount = 0;
//			Serial.println("Stall reset");
//			begin();
//		}
//	}
}
void Accessory::_waitForAck() {
	pinMode(_sda_pin, INPUT);
	_clockHigh();
	unsigned long time = millis();
	while (digitalRead(_sda_pin) == HIGH && (time + ackTimeout) < millis()) {
	}
//	if ((time + ackTimeout) < millis()) {
//		_timeoutCount++;
//		if (_timeoutCount > 10) {
//			_timeoutCount = 0;
//			Serial.println("Timeout reset");
//			begin();
//		}
//	}
	_clockLow();
//	delayMicroseconds(75);
}

uint8_t Accessory::_readByte() {
	pinMode(_sda_pin, INPUT);

	uint8_t value = 0;
	//_clockLow();
	for (int i = 0; i < 8; ++i) {
		_clockHigh();
		value |= (digitalRead(_sda_pin) << (7 - i));
		_clockLow();
	}
	return value;
}

void Accessory::_writeByte(uint8_t value) {
	_shiftOut( value);
}
void Accessory::initBytes() {

		// improved startup procedure from http://playground.arduino.cc/Main/AccessoryClass
		_writeRegister(0xF0, 0x55);
		_writeRegister(0xFB, 0x00);

}

void Accessory::_shiftOut( uint8_t val) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if((val & (1 << (7 - i))) == 0) {
			_dataLow() ;
		}else
			_dataHigh();
		_clockHigh();
		_clockLow();
	}
}

void Accessory::begin()
{


	_use_hw = false;
	if (	(_sda_pin == SDA) and (_scl_pin == SCL))
	{
		_use_hw = true;
		  Wire.begin();
		  Serial.println("Starting Wire");

	}
	initBytes();
	//Serial.println("Init sent, reading");

	delay(100);
	_burstRead();
	//Serial.println("re-reading");
	delay(100);
	_burstRead();
	Serial.println("Initialization Done");

}

void Accessory::_burstRead(){
	_burstReadWithAddress(0);
}

void Accessory::_burstReadWithAddress(uint8_t addr)
{
	int readAmnt =6;

	if (_use_hw)
	{
		 // send conversion command
		  Wire.beginTransmission(I2C_ADDR);
		  Wire.write(addr);
		  Wire.endTransmission();

		  // wait for data to be converted
		  delay(1);

		  // read data
		  Wire.readBytes(_dataarray, Wire.requestFrom(I2C_ADDR, sizeof(_dataarray)));

	}else{
	// send conversion command
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		_writeByte(addr);
		_waitForAck();
		_sendStop();
		// wait for data to be converted
		  delay(1);
		_sendStart(I2C_ADDR_R);
		_waitForAck();

		for (int i=0; i<readAmnt; i++)
		{
			delayMicroseconds(40);
			_dataarray[i] = _readByte()  ;
			if (i<(readAmnt-1))
				_sendAck();
			else
				_sendNack();
		}
		_sendStop();
	}
}

void Accessory::_writeRegister(uint8_t reg, uint8_t value)
{
	if (_use_hw)
	{
		//Serial.println("Writing reg");
		  Wire.beginTransmission(I2C_ADDR);
		  Wire.write(reg);
		  Wire.write(value);
		  Wire.endTransmission();
		  //Serial.println("Writing reg done");
	}else{
	//Serial.println("Sending start");
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		//Serial.println("First byte");
		_writeByte(reg);
		_waitForAck();
		//Serial.println("Seconde byte");
		_writeByte(value);
		//Serial.println("waiting");
		_waitForAck();
		//Serial.println("stopping");
		_sendStop();
		//Serial.println("done");
	}
}

