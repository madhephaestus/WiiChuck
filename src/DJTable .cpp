#include "DJTable.h"


DJTable::DJTable() :
    Accessory() {
}



  	unsigned int  DJTable::crossfadeSlider::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return smap(c->getCrossfadeSlider(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void DJTable::crossfadeSlider::printMap(Stream& stream){
    stream.print("DJTable::crossfadeSlider -> ");
    Mapping::printMap(stream);
  }

int DJTable::getCrossfadeSlider() {
	return decodeInt(crossfadeSliderBytes); 
}


  	unsigned int  DJTable::effectDial::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return smap(c->getEffectDial(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void DJTable::effectDial::printMap(Stream& stream){
    stream.print("DJTable::effectDial -> ");
    Mapping::printMap(stream);
  }

int DJTable::getEffectDial() {
	return decodeInt(effectDialBytes); 
}


  	unsigned int  DJTable::stickX::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return smap(c->getStickX(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void DJTable::stickX::printMap(Stream& stream){
    stream.print("DJTable::stickX -> ");
    Mapping::printMap(stream);
  }

int DJTable::getStickX() {
	return decodeInt(stickXBytes); 
}


  	unsigned int  DJTable::stickY::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return smap(c->getStickY(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void DJTable::stickY::printMap(Stream& stream){
    stream.print("DJTable::stickY -> ");
    Mapping::printMap(stream);
  }

int DJTable::getStickY() {
	return decodeInt(stickYBytes); 
}


  	unsigned int  DJTable::rightDJTable::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return smap(c->getRightDJTable(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void DJTable::rightDJTable::printMap(Stream& stream){
    stream.print("DJTable::rightDJTable -> ");
    Mapping::printMap(stream);
  }

int DJTable::getRightDJTable() {
	return decodeInt(rightDJTableBytes); 
}


  	unsigned int  DJTable::leftDJTable::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return smap(c->getLeftDJTable(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void DJTable::leftDJTable::printMap(Stream& stream){
    stream.print("DJTable::leftDJTable -> ");
    Mapping::printMap(stream);
  }

int DJTable::getLeftDJTable() {
	return decodeInt(leftDJTableBytes); 
}


  	unsigned int  DJTable::euphoriaButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getEuphoriaButton() ? servoMax:servoZero;
	}

  void DJTable::euphoriaButton::printMap(Stream& stream){
    stream.print("DJTable::euphoriaButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getEuphoriaButton() {
	return decodeBit(euphoriaButtonBytes); 
}


  	unsigned int  DJTable::plusButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getPlusButton() ? servoMax:servoZero;
	}

  void DJTable::plusButton::printMap(Stream& stream){
    stream.print("DJTable::plusButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getPlusButton() {
	return decodeBit(plusButtonBytes); 
}


  	unsigned int  DJTable::minusButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getMinusButton() ? servoMax:servoZero;
	}

  void DJTable::minusButton::printMap(Stream& stream){
    stream.print("DJTable::minusButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getMinusButton() {
	return decodeBit(minusButtonBytes); 
}


  	unsigned int  DJTable::leftGreenButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getLeftGreenButton() ? servoMax:servoZero;
	}

  void DJTable::leftGreenButton::printMap(Stream& stream){
    stream.print("DJTable::leftGreenButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getLeftGreenButton() {
	return decodeBit(leftGreenButtonBytes); 
}


  	unsigned int  DJTable::leftRedButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getLeftRedButton() ? servoMax:servoZero;
	}

  void DJTable::leftRedButton::printMap(Stream& stream){
    stream.print("DJTable::leftRedButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getLeftRedButton() {
	return decodeBit(leftRedButtonBytes); 
}


  	unsigned int  DJTable::leftBlueButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getLeftBlueButton() ? servoMax:servoZero;
	}

  void DJTable::leftBlueButton::printMap(Stream& stream){
    stream.print("DJTable::leftBlueButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getLeftBlueButton() {
	return decodeBit(leftBlueButtonBytes); 
}


  	unsigned int  DJTable::rightGreenButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getRightGreenButton() ? servoMax:servoZero;
	}

  void DJTable::rightGreenButton::printMap(Stream& stream){
    stream.print("DJTable::rightGreenButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getRightGreenButton() {
	return decodeBit(rightGreenButtonBytes); 
}


  	unsigned int  DJTable::rightRedButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getRightRedButton() ? servoMax:servoZero;
	}

  void DJTable::rightRedButton::printMap(Stream& stream){
    stream.print("DJTable::rightRedButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getRightRedButton() {
	return decodeBit(rightRedButtonBytes); 
}


  	unsigned int  DJTable::rightBlueButton::mapVar(){
	  DJTable* c = (DJTable*)controller;
	  return c->getRightBlueButton() ? servoMax:servoZero;
	}

  void DJTable::rightBlueButton::printMap(Stream& stream){
    stream.print("DJTable::rightBlueButton -> ");
    Mapping::printMap(stream);
  }

int DJTable::getRightBlueButton() {
	return decodeBit(rightBlueButtonBytes); 
}


    void DJTable::printInputs(Stream& stream) {
    char st[100];
    sprintf(st," crossfade slider: %4d | effect dial: %4d | stick x: %4d | stick y: %4d | right DJTable: %4d | left DJTable: %4d |",getCrossfadeSlider(),getEffectDial(),getStickX(),getStickY(),getRightDJTable(),getLeftDJTable());  
      stream.print(st);
    
          if (getEuphoriaButton())
        stream.print("euphoria button");

          if (getPlusButton())
        stream.print("plus button");

          if (getMinusButton())
        stream.print("minus button");

          if (getLeftGreenButton())
        stream.print("left green button");

          if (getLeftRedButton())
        stream.print("left red button");

          if (getLeftBlueButton())
        stream.print("left blue button");

          if (getRightGreenButton())
        stream.print("right green button");

          if (getRightRedButton())
        stream.print("right red button");

          if (getRightBlueButton())
        stream.print("right blue button");
      stream.println("");

    }
  

