#include "Drums.h"
Drums::Drums() :
    Accessory() {
}



  	unsigned int  Drums::stickX::mapVar(){
	  Drums* c = (Drums*)controller;
	  return smap(c->getStickX(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Drums::stickX::printMap(Stream& stream){
    stream.print("Drums::stickX -> ");
    Mapping::printMap(stream);
  }

int Drums::getStickX() {
	return decodeInt(stickXBytes); 
}


  	unsigned int  Drums::stickY::mapVar(){
	  Drums* c = (Drums*)controller;
	  return smap(c->getStickY(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Drums::stickY::printMap(Stream& stream){
    stream.print("Drums::stickY -> ");
    Mapping::printMap(stream);
  }

int Drums::getStickY() {
	return decodeInt(stickYBytes); 
}


int Drums::getSoftnessDataFor() {
	return decodeInt(softnessDataForBytes); 
}



int Drums::getSoftness() {
	return decodeInt(softnessBytes); 
}


int Drums::getHighHatDataFlag() {
	return decodeBit(highHatDataFlagBytes); 
}


int Drums::getSoftnessDataFlag() {
	return decodeBit(softnessDataFlagBytes); 
}


  	unsigned int  Drums::minusButton::mapVar(){
	  Drums* c = (Drums*)controller;
	  return c->getMinusButton() ? servoMax:servoZero;
	}

  void Drums::minusButton::printMap(Stream& stream){
    stream.print("Drums::minusButton -> ");
    Mapping::printMap(stream);
  }

int Drums::getMinusButton() {
	return decodeBit(minusButtonBytes); 
}


  	unsigned int  Drums::plusButton::mapVar(){
	  Drums* c = (Drums*)controller;
	  return c->getPlusButton() ? servoMax:servoZero;
	}

  void Drums::plusButton::printMap(Stream& stream){
    stream.print("Drums::plusButton -> ");
    Mapping::printMap(stream);
  }

int Drums::getPlusButton() {
	return decodeBit(plusButtonBytes); 
}


  	unsigned int  Drums::orangeDrum::mapVar(){
	  Drums* c = (Drums*)controller;
	  // Is this event for us?
	  if (c->getSoftnessDataFor()==orangeID) return smap(0x7-(c->getSoftness()),7,0,0,servoMax,servoZero,servoMin);
	  else return servoZero;
	}

  void Drums::orangeDrum::printMap(Stream& stream){
    stream.print("Drums::orangeDrum -> ");
    Mapping::printMap(stream);
  }

int Drums::getOrangeDrum() {
	return decodeBit(orangeDrumBytes); 
}


  	unsigned int  Drums::redDrum::mapVar(){
	  Drums* c = (Drums*)controller;
	  if (c->getSoftnessDataFor()==redID) return smap(0x7-(c->getSoftness()),7,0,0,servoMax,servoZero,servoMin);
	  else return servoZero;
	}

  void Drums::redDrum::printMap(Stream& stream){
    stream.print("Drums::redDrum -> ");
    Mapping::printMap(stream);
  }

int Drums::getRedDrum() {
	return decodeBit(redDrumBytes); 
}


  	unsigned int  Drums::yellowDrum::mapVar(){
	  Drums* c = (Drums*)controller;
	  if (c->getSoftnessDataFor()==yellowID) return smap(0x7-(c->getSoftness()),7,0,0,servoMax,servoZero,servoMin);
	  else return servoZero;
	}

  void Drums::yellowDrum::printMap(Stream& stream){
    stream.print("Drums::yellowDrum -> ");
    Mapping::printMap(stream);
  }

int Drums::getYellowDrum() {
	return decodeBit(yellowDrumBytes); 
}


  	unsigned int  Drums::greenDrum::mapVar(){
	  Drums* c = (Drums*)controller;
	  	  if (c->getSoftnessDataFor()==greenID) return smap(0x7-(c->getSoftness()),7,0,0,servoMax,servoZero,servoMin);
	  else return servoZero;
	}

  void Drums::greenDrum::printMap(Stream& stream){
    stream.print("Drums::greenDrum -> ");
    Mapping::printMap(stream);
  }

int Drums::getGreenDrum() {
	return decodeBit(greenDrumBytes); 
}


  	unsigned int  Drums::blueDrumm::mapVar(){
	  Drums* c = (Drums*)controller;
	  
	  	  if (c->getSoftnessDataFor()==blueID) return smap(0x7-(c->getSoftness()),7,0,0,servoMax,servoZero,servoMin);
	  else return servoZero;
	}

  void Drums::blueDrumm::printMap(Stream& stream){
    stream.print("Drums::blueDrumm -> ");
    Mapping::printMap(stream);
  }

int Drums::getBlueDrumm() {
	return decodeBit(blueDrummBytes); 
}


  	unsigned int  Drums::bassPedal::mapVar(){
	  Drums* c = (Drums*)controller;
	  	  if (c->getSoftnessDataFor()==bassID) return smap(0x7-(c->getSoftness()),7,0,0,servoMax,servoZero,servoMin);
	  else return servoZero;
	}

  void Drums::bassPedal::printMap(Stream& stream){
    stream.print("Drums::bassPedal -> ");
    Mapping::printMap(stream);
  }

int Drums::getBassPedal() {
	return decodeBit(bassPedalBytes); 
}


    void Drums::printInputs(Stream& stream) {
    char st[100];
    sprintf(st," stick x: %4d | stick y: %4d | ",getStickX(),getStickY());  
  
    stream.print(st);
    
    if (getSoftnessDataFlag()){
      switch(getSoftnessDataFor()){
        case 27:
        {
          stream.print("bass pedal:\t");
          stream.print(getSoftness());
        break;
        }
        case 25:{
        stream.print("red drum:\t");
        stream.print(getSoftness());
        break;
        }
        case 17:{
        stream.print("yellow drum:\t");
        stream.print(getSoftness());
        break;
        }
        case 15:{
        stream.print("blue drumm:\t");
        stream.print(getSoftness());
        break;
        }
        case 14:{
        stream.print("orange drum:\t");
        stream.print(getSoftness());
        break;
        }
        case 18:{
        stream.print("green drum:\t");
        stream.print(getSoftness());
        break;
        }
        
      }
       if (getHighHatDataFlag()){
        stream.print("high hat:\t");
        stream.print(getSoftness()); }
      
    }



          if (getMinusButton())
        stream.print("minus button");

          if (getPlusButton())
        stream.print("plus button");


    stream.println("");
    }
    

