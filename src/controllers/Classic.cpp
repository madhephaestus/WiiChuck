#include "Classic.h"

Classic::Classic(uint8_t data_pin, uint8_t sclk_pin) :
    Accessory(data_pin, sclk_pin) {

}




uint16_t Classic::joyXLeft::mapVar() {
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


uint16_t Classic::joyXRight::mapVar() {
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


uint16_t Classic::joyYLeft::mapVar() {
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


uint16_t Classic::joyYRight::mapVar() {
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


uint16_t Classic::triggerLeft::mapVar() {
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


uint16_t Classic::triggerRight::mapVar() {
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


uint16_t Classic::padDown::mapVar() {
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


uint16_t Classic::padUp::mapVar() {
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


uint16_t Classic::padLeft::mapVar() {
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


uint16_t Classic::buttonX::mapVar() {
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


uint16_t Classic::buttonY::mapVar() {
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


uint16_t Classic::buttonA::mapVar() {
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


uint16_t Classic::buttonB::mapVar() {
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


uint16_t Classic::buttonMinus::mapVar() {
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


uint16_t Classic::buttonHome::mapVar() {
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


uint16_t Classic::buttonPlus::mapVar() {
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



