#include "Guitar.h"
Guitar::Guitar() :
    Accessory() {
}



  	unsigned int  Guitar::stickX::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return smap(c->getStickX(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Guitar::stickX::printMap(Stream& stream){
    stream.print("Guitar::stickX -> ");
    Mapping::printMap(stream);
  }

int Guitar::getStickX() {
	return decodeInt(stickXBytes); 
}


  	unsigned int  Guitar::stickY::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return smap(c->getStickY(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Guitar::stickY::printMap(Stream& stream){
    stream.print("Guitar::stickY -> ");
    Mapping::printMap(stream);
  }

int Guitar::getStickY() {
	return decodeInt(stickYBytes); 
}


  	unsigned int  Guitar::whammyBar::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return smap(c->getWhammyBar(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
	}

  void Guitar::whammyBar::printMap(Stream& stream){
    stream.print("Guitar::whammyBar -> ");
    Mapping::printMap(stream);
  }

int Guitar::getWhammyBar() {
	return decodeInt(whammyBarBytes); 
}


  	unsigned int  Guitar::plusButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getPlusButton() ? servoMax:servoZero;
	}

  void Guitar::plusButton::printMap(Stream& stream){
    stream.print("Guitar::plusButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getPlusButton() {
	return decodeBit(plusButtonBytes); 
}


  	unsigned int  Guitar::minusButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getMinusButton() ? servoMax:servoZero;
	}

  void Guitar::minusButton::printMap(Stream& stream){
    stream.print("Guitar::minusButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getMinusButton() {
	return decodeBit(minusButtonBytes); 
}


  	unsigned int  Guitar::greenButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getGreenButton() ? servoMax:servoZero;
	}

  void Guitar::greenButton::printMap(Stream& stream){
    stream.print("Guitar::greenButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getGreenButton() {
	return decodeBit(greenButtonBytes); 
}


  	unsigned int  Guitar::redButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getRedButton() ? servoMax:servoZero;
	}

  void Guitar::redButton::printMap(Stream& stream){
    stream.print("Guitar::redButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getRedButton() {
	return decodeBit(redButtonBytes); 
}


  	unsigned int  Guitar::yellowButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getYellowButton() ? servoMax:servoZero;
	}

  void Guitar::yellowButton::printMap(Stream& stream){
    stream.print("Guitar::yellowButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getYellowButton() {
	return decodeBit(yellowButtonBytes); 
}


  	unsigned int  Guitar::blueButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getBlueButton() ? servoMax:servoZero;
	}

  void Guitar::blueButton::printMap(Stream& stream){
    stream.print("Guitar::blueButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getBlueButton() {
	return decodeBit(blueButtonBytes); 
}


  	unsigned int  Guitar::orangeButton::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getOrangeButton() ? servoMax:servoZero;
	}

  void Guitar::orangeButton::printMap(Stream& stream){
    stream.print("Guitar::orangeButton -> ");
    Mapping::printMap(stream);
  }

int Guitar::getOrangeButton() {
	return decodeBit(orangeButtonBytes); 
}


  	unsigned int  Guitar::strumUp::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getStrumUp() ? servoMax:servoZero;
	}

  void Guitar::strumUp::printMap(Stream& stream){
    stream.print("Guitar::strumUp -> ");
    Mapping::printMap(stream);
  }

int Guitar::getStrumUp() {
	return decodeBit(strumUpBytes); 
}


  	unsigned int  Guitar::strumDown::mapVar(){
	  Guitar* c = (Guitar*)controller;
	  return c->getStrumDown() ? servoMax:servoZero;
	}

  void Guitar::strumDown::printMap(Stream& stream){
    stream.print("Guitar::strumDown -> ");
    Mapping::printMap(stream);
  }

int Guitar::getStrumDown() {
	return decodeBit(strumDownBytes); 
}

void Guitar::printInputs(Stream& stream) {

    char st[100];
    sprintf(st," stick x: %4d | stick y: %4d | whammy bar: %4d | Buttons: ",getStickX(),getStickY(),getWhammyBar());  
    stream.print(st);
    
          if (getPlusButton())
        stream.print("plus button");

          if (getMinusButton())
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


