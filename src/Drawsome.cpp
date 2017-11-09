#include "Drawsome.h"
Drawsome::Drawsome() :
    Accessory() {
    

}

void Drawsome::initBytes() {
    Serial.println("Drawesome Init");
        // Enable Encrypted Mode.
        //enableEncryption(true);
        Accessory::initBytes();
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
void Drawsome::printInputs(Stream& stream) {
    char st[100];
    stream.print("NUNCHUCK ");
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
    Accessory::printInputs(stream);
}

  	unsigned int  Drawsome::penXPosition::mapVar(){
	  Drawsome* c = (Drawsome*)controller;
	  return smap(c->getPenXPosition(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Drawsome::penXPosition::printMap(Stream& stream){
    stream.print("Drawsome::penXPosition -> ");
    Mapping::printMap(stream);
  }

int Drawsome::getPenXPosition() {
	return decodeInt(penXPositionBytes); 
}


  	unsigned int  Drawsome::penYPosition::mapVar(){
	  Drawsome* c = (Drawsome*)controller;
	  return smap(c->getPenYPosition(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Drawsome::penYPosition::printMap(Stream& stream){
    stream.print("Drawsome::penYPosition -> ");
    Mapping::printMap(stream);
  }

int Drawsome::getPenYPosition() {
	return decodeInt(penYPositionBytes); 
}


  	unsigned int  Drawsome::penPressure::mapVar(){
	  Drawsome* c = (Drawsome*)controller;
	  return smap(c->getPenPressure(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Drawsome::penPressure::printMap(Stream& stream){
    stream.print("Drawsome::penPressure -> ");
    Mapping::printMap(stream);
  }

int Drawsome::getPenPressure() {
	return decodeInt(penPressureBytes); 
}


  	unsigned int  Drawsome::penContact::mapVar(){
	  Drawsome* c = (Drawsome*)controller;
	  return c->getPenContact() ? servoMax:servoZero;
	}

  void Drawsome::penContact::printMap(Stream& stream){
    stream.print("Drawsome::penContact -> ");
    Mapping::printMap(stream);
  }

int Drawsome::getPenContact() {
	return decodeBit(penContactBytes); 
}


