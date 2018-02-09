#include "Classic.h"

Classic::Classic() :
    Accessory() {

}




unsigned int  Classic::joyXLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->getJoyXLeft(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyXLeft::printMap(Stream& stream) {
    stream.print("Classic::joyXLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getJoyXLeft() {
    return decodeInt(joyXLeftBytes);
}


unsigned int  Classic::joyXRight::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->getJoyXRight(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyXRight::printMap(Stream& stream) {
    stream.print("Classic::joyXRight -> ");
    Mapping::printMap(stream);
}

int Classic::getJoyXRight() {
    return decodeInt(joyXRightBytes);
}


unsigned int  Classic::joyYLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->getJoyYLeft(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyYLeft::printMap(Stream& stream) {
    stream.print("Classic::joyYLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getJoyYLeft() {
    return decodeInt(joyYLeftBytes);
}


unsigned int  Classic::joyYRight::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->getJoyYRight(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyYRight::printMap(Stream& stream) {
    stream.print("Classic::joyYRight -> ");
    Mapping::printMap(stream);
}

int Classic::getJoyYRight() {
    return decodeInt(joyYRightBytes);
}


unsigned int  Classic::triggerLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->getTriggerLeft(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::triggerLeft::printMap(Stream& stream) {
    stream.print("Classic::triggerLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getTriggerLeft() {
    return decodeInt(triggerLeftBytes);
}


unsigned int  Classic::triggerRight::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->getTriggerRight(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::triggerRight::printMap(Stream& stream) {
    stream.print("Classic::triggerRight -> ");
    Mapping::printMap(stream);
}

int Classic::getTriggerRight() {
    return decodeInt(triggerRightBytes);
}
void Classic::padRight::printMap(Stream& stream) {
    stream.print("Classic::padRight -> ");
    Mapping::printMap(stream);
}

int Classic::getPadRight() {
    return decodeBit(padRightBytes);
}


unsigned int  Classic::padDown::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getPadDown() ? servoMax:servoZero;
}

void Classic::padDown::printMap(Stream& stream) {
    stream.print("Classic::padDown -> ");
    Mapping::printMap(stream);
}

int Classic::getPadDown() {
    return decodeBit(padDownBytes);
}


unsigned int  Classic::padUp::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getPadUp() ? servoMax:servoZero;
}

void Classic::padUp::printMap(Stream& stream) {
    stream.print("Classic::padUp -> ");
    Mapping::printMap(stream);
}

int Classic::getPadUp() {
    return decodeBit(padUpBytes);
}


unsigned int  Classic::padLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getPadLeft() ? servoMax:servoZero;
}

void Classic::padLeft::printMap(Stream& stream) {
    stream.print("Classic::padLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getPadLeft() {
    return decodeBit(padLeftBytes);
}


unsigned int  Classic::buttonX::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonX() ? servoMax:servoZero;
}

void Classic::buttonX::printMap(Stream& stream) {
    stream.print("Classic::buttonX -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonX() {
    return decodeBit(buttonXBytes);
}


unsigned int  Classic::buttonY::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonY() ? servoMax:servoZero;
}

void Classic::buttonY::printMap(Stream& stream) {
    stream.print("Classic::buttonY -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonY() {
    return decodeBit(buttonYBytes);
}


unsigned int  Classic::buttonA::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonA() ? servoMax:servoZero;
}

void Classic::buttonA::printMap(Stream& stream) {
    stream.print("Classic::buttonA -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonA() {
    return decodeBit(buttonABytes);
}


unsigned int  Classic::buttonB::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonB() ? servoMax:servoZero;
}

void Classic::buttonB::printMap(Stream& stream) {
    stream.print("Classic::buttonB -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonB() {
    return decodeBit(buttonBBytes);
}


unsigned int  Classic::buttonMinus::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonMinus() ? servoMax:servoZero;
}

void Classic::buttonMinus::printMap(Stream& stream) {
    stream.print("Classic::buttonMinus -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonMinus() {
    return decodeBit(buttonMinusBytes);
}


unsigned int  Classic::buttonHome::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonHome() ? servoMax:servoZero;
}

void Classic::buttonHome::printMap(Stream& stream) {
    stream.print("Classic::buttonHome -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonHome() {
    return decodeBit(buttonHomeBytes);
}


unsigned int  Classic::buttonPlus::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonPlus() ? servoMax:servoZero;
}

void Classic::buttonPlus::printMap(Stream& stream) {
    stream.print("Classic::buttonPlus -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonPlus() {
    return decodeBit(buttonPlusBytes);
}



unsigned int  Classic::buttonZLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonZLeft() ? servoMax:servoZero;
}

void Classic::buttonZLeft::printMap(Stream& stream) {
    stream.print("Classic::buttonZLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonZLeft() {
    return decodeBit(buttonZLBytes);
}



unsigned int  Classic::buttonZRight::mapVar() {
    Classic* c = (Classic*)controller;
    return c->getButtonZRight() ? servoMax:servoZero;
}

void Classic::buttonZRight::printMap(Stream& stream) {
    stream.print("Classic::buttonZRight -> ");
    Mapping::printMap(stream);
}

int Classic::getButtonZRight() {
    return decodeBit(buttonZRBytes);
}

    void Classic::printInputs(Stream& stream) {
    char st[100];
    sprintf(st," jxL: %4d | jxR: %4d | jyL: %4d | jyR: %4d | trigL: %4d | trigR: %4d |",getJoyXLeft(),getJoyXRight(),getJoyYLeft(),getJoyYRight(),getTriggerLeft(),getTriggerRight());
   stream.print(st);
    
          if (getPadRight())
        stream.print("pad right");

          if (getPadDown())
        stream.print("pad down");

          if (getPadUp())
        stream.print("pad up");

          if (getPadLeft())
        stream.print("pad left");

          if (getButtonX())
        stream.print("button x");

          if (getButtonY())
        stream.print("button y");

          if (getButtonA())
        stream.print("button a");

          if (getButtonB())
        stream.print("button b");

          if (getButtonMinus())
        stream.print("button minus");

          if (getButtonHome())
        stream.print("button home");

          if (getButtonPlus())
        stream.print("button plus");

          if (getButtonZLeft())
        stream.print("button ZL");

          if (getButtonZRight())
        stream.print("button ZR");

    stream.println("");
    }



