#include "Accessory.h"
#include <Wire.h>

#include "sWire.h"
#include "Servo.h"

//#define WiiChickUseSWiic

//#ifdef WiiChickUseSWiic
//  sWire myWire;
//#else
#define myWire Wire
//#endif


Accessory::Accessory(uint8_t data_pin, uint8_t sclk_pin) {
//#ifdef WiiChickUseSWiic
  myWire.setiicpins(data_pin,sclk_pin);
//#endif

}
/**
 * Reads the device type from the controller
 */
ControllerType Accessory::identifyController() {
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

/*
 * public function to read data
 */
void Accessory::readData() {

	_burstRead();
  _applyMaps();
}

uint8_t* Accessory::getDataArray() {
	return _dataarray;
}

void Accessory::initBytes() {

	// improved startup procedure from http://playground.arduino.cc/Main/sWireClass
	_writeRegister(0xF0, 0x55);
	_writeRegister(0xFB, 0x00);

}

void Accessory::setDataArray(uint8_t data[6]) {
	for (int i = 0; i < 6; i++)
		_dataarray[i] = data[i];
}

void Accessory::printInputs(Stream& stream) {
	stream.print("Accessory Bytes:\t");
	for (int i = 0; i < 6; i++) {
		if (_dataarray[i] < 0x10) {
			stream.print("0");
		}
		stream.print(_dataarray[i], HEX);
		stream.print(" ");
	}
	stream.println("");
}

int Accessory::decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
		uint8_t csbbyte, uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte,
		uint8_t lsbstart, uint8_t lsbend, int16_t aMin, int16_t aMid, int16_t aMax) {
// 5 bit int split across 3 bytes. what... the... fuck... nintendo...
  bool msbflag=false,csbflag=false,lsbflag=false;
	if (msbbyte > 5)
		msbflag=true;
	if (csbbyte > 5)
		csbflag=true;
	if (lsbbyte > 5)
		lsbflag=true;

	int32_t analog = 0;
	uint32_t lpart;
	lpart = (lsbflag)?0:_dataarray[lsbbyte];
	lpart = lpart >> lsbstart;
	lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

	uint32_t cpart;
	cpart = (csbflag)?0:_dataarray[csbbyte];
	cpart = cpart >> csbstart;
	cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

	cpart = cpart << ((lsbend - lsbstart) + 1);

	uint32_t mpart;
	mpart = (lsbflag)?0:_dataarray[msbbyte];
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
  myWire.begin();

	initBytes();

	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
	
	Serial.println("Initialization Done");

}

void Accessory::_burstRead() {
	_burstReadWithAddress(0);
}

void Accessory::_burstReadWithAddress(uint8_t addr) {
	int readAmnt = 6;

		// send conversion command
		myWire.beginTransmission(I2C_ADDR);
		myWire.write(addr);
		myWire.endTransmission();

		// wait for data to be converted
		delay(1);

		// read data
		myWire.readBytes(_dataarray,
				myWire.requestFrom(I2C_ADDR, sizeof(_dataarray)));
}

void Accessory::_writeRegister(uint8_t reg, uint8_t value) {
		myWire.beginTransmission(I2C_ADDR);
		myWire.write(reg);
		myWire.write(value);
		myWire.endTransmission();
}

uint8_t Accessory::addAnalogMap(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
			uint8_t csbbyte, uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte,
			uint8_t lsbstart, uint8_t lsbend,int16_t aMin, int16_t aMid, int16_t aMax, uint8_t sMin, uint8_t sMax,
			uint8_t sZero, uint8_t sChan){
	    inputMapping* im = (inputMapping*) malloc(sizeof(inputMapping));
	    if (im==0) return -1;
	    Serial.print("Malloc'd:\t0x"); Serial.println((int)im, HEX);
	    // populate mapping struct
	    im->type = ANALOG;
	    im->aSch.aMsbbyte=msbbyte;
	    im->aSch.aMsbstart=msbstart;
	    im->aSch.aMsbend=msbend;
	    
	    im->aSch.aCsbbyte=csbbyte;
	    im->aSch.aCsbstart=csbstart;
	    im->aSch.aCsbend=csbend;
	    
	    im->aSch.aLsbbyte=lsbbyte;
	    im->aSch.aLsbstart=lsbstart;
	    im->aSch.aLsbend=lsbend;
	    
	    im->aMin=aMin;
	    im->aMid=aMid;
	    im->aMax=aMax;
	    
	    im->servoMax=sMax;
	    im->servoMin=sMin;
	    im->servoZero=sZero;
	    
	    im->sChan=sChan;
	    
	    im->servo = Servo();
	    im->servo.attach(sChan);
	    im->servo.write(sZero);
	    
	    // Add to list
	    // Are we first
	    if (_firstMap==0) _firstMap = im;
	    else {
	      // Walk the list
	      inputMapping* m=_firstMap;
	      while(m->nextMap!=0) m=m->nextMap;
	      // Ok we're at the end. Add our map.
	      m->nextMap=im;
	    }
}

uint8_t Accessory::addDigitalMap(uint8_t byte, uint8_t bit, bool activeLow,
		uint8_t sMin, uint8_t sMax, uint8_t sZero, uint8_t sChan) {
inputMapping* im = (inputMapping*) malloc(sizeof(inputMapping));
	    if (im==0) return -1;
	    Serial.print("Malloc'd:\t0x"); Serial.println((int)im, HEX);
	    // populate mapping struct
	    im->type = DIGITAL;
	    im->dSch.dByte=byte;
	    im->dSch.dBit=bit;
	    im->dSch.dActiveLow;
	    
	    im->servoMax=sMax;
	    im->servoMin=sMin;
	    im->servoZero=sZero;
	    
	    im->sChan=sChan;
	    im->servo.attach(sChan);
	    
	    // Add to list
	    // Are we first
	    if (_firstMap==0) _firstMap = im;
	    else {
	      // Walk the list
	      inputMapping* m=_firstMap;
	      while(m->nextMap!=0) m=m->nextMap;
	      // Ok we're at the end. Add our map.
	      m->nextMap=im;
	    }
}

void Accessory::printMaps(Stream& stream) {
   stream.println("Active Maps");
   if (_firstMap==0) stream.println("/tNo Maps");
	    else {
	      // Walk the list
	      inputMapping* m=_firstMap;
	      int cnt=0;
	      do {
	        stream.print("\t["); stream.print(cnt); stream.print("]  "); stream.print("Servo: ");stream.print(m->sChan);stream.print("(");
	        stream.print(m->servoMin);stream.print(",");
	        stream.print(m->servoZero);stream.print(",");
	        stream.print(m->servoMax);stream.print(") ");
	        
	        if (m->type == ANALOG){
	        stream.print("ANALOG ");
	          if (m->aSch.aMsbbyte != UNUSED){
	            stream.print("BIT");stream.print(m->aSch.aMsbbyte);
	            stream.print("[");
	            stream.print(m->aSch.aMsbend);
	            stream.print(":");
	            stream.print(m->aSch.aMsbstart);
	            stream.print("] ");
	          }
	          if (m->aSch.aCsbbyte != UNUSED){
	            stream.print("BIT");stream.print(m->aSch.aCsbbyte);
	            stream.print("[");
	            stream.print(m->aSch.aCsbend);
	            stream.print(":");
	            stream.print(m->aSch.aCsbstart);
	            stream.print("] ");
	          }
	          if (m->aSch.aLsbbyte != UNUSED){
	            stream.print("BIT");stream.print(m->aSch.aLsbbyte);
	            stream.print("[");
	            stream.print(m->aSch.aLsbend);
	            stream.print(":");
	            stream.print(m->aSch.aLsbstart);
	            stream.print("] ");
	          }
	          stream.print("min: "); stream.print(m->aMin);
	          stream.print(" mind: "); stream.print(m->aMid);
	          stream.print(" max: "); stream.print(m->aMax);
	        
	        } else {
	          stream.print("DIGITAL ");
	          stream.print("BIT");stream.print(m->dSch.dByte);
	          stream.print("[");
	          stream.print(m->dSch.dBit);
	          stream.print("]");
	        }
	        
	        stream.println("");
	        m=m->nextMap;
	        cnt++;
	      } while(m!=0);
	    }

}

void Accessory::_applyMaps(){
  inputMapping* m=_firstMap;
  int cnt=0;
  
  if (m==0) return; // no maps. bail.
  do {
    switch(m->type){
      case ANALOG: {
        int val =   decodeInt(
          m->aSch.aMsbbyte,m->aSch.aMsbstart,m->aSch.aMsbend,        // MSB 
          m->aSch.aCsbbyte,m->aSch.aCsbstart,m->aSch.aCsbend,        // CSB 
          m->aSch.aLsbbyte,m->aSch.aLsbstart,m->aSch.aLsbend,        // LSB 
          m->aMin,m->aMid,m->aMax);                         // bounds
          
        // map to servo
        uint8_t pos=smap(val,m->aMax,m->aMid,m->aMin,m->servoMax,m->servoZero,m->servoMin);
        
        // update servo
        m->servo.write(pos);
      }
      break;
      case DIGITAL: {
        bool val = decodeBit(m->dSch.dByte,m->dSch.dBit,m->dSch.dActiveLow);
        if (val) m->servo.write(m->servoMax);
        else m->servo.write(m->servoMin);
      }
      break;
    }
    m=m->nextMap;
	  cnt++;
  } while(m!=0);

}

uint8_t Accessory::getMapCount() {

}
void Accessory::removeMaps() {
}
void Accessory::removeMap(uint8_t id) {
}

int16_t Accessory::smap(int16_t val, int16_t aMax, int16_t aMid, int16_t aMin, int16_t sMax, int16_t sZero, int16_t sMin){
  if (val>aMid) {
    return map(val,aMid,aMax,sZero,sMax);
  } else if (val<aMid) {
    return map(val,aMin,aMid,sMin,sZero);
  }
  return sZero;
}
