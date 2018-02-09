#include "Nunchuck.h"

Nunchuck::Nunchuck() :
    Accessory() {
}
int Nunchuck::getJoyX() {
    return decodeInt(joyXBytes);
}
int Nunchuck::getJoyY() {
    return decodeInt(joyYBytes);
}

float Nunchuck::getRollAngle() {
    return atan2((double) getAccelX() - 511, (double) getAccelZ() - 511) * 180.0 / PI;
}
float Nunchuck::getPitchAngle() {
    return -atan2((double) getAccelY() - 511, (double) getAccelZ() - 511) * 180.0 / PI;
}
int Nunchuck::getAccelX() {
    return decodeInt(accelXBytes);
}
int Nunchuck::getAccelY() {
    return decodeInt(accelYBytes);
}
int Nunchuck::getAccelZ() {
    return decodeInt(accelZBytes);
}
boolean Nunchuck::checkButtonC() {
    return decodeBit(buttonCBits);
}
boolean Nunchuck::checkButtonZ() {
    return decodeBit(buttonZBits);
}

void Nunchuck::printInputs(Stream& stream) {
    char st[100];

    stream.print("NUNCHUCK ");
    sprintf(st,
            "  JoyX: %4d  | JoyY: %4d | Ax: %4d | Ay: %4d | Az: %4d | Buttons: ",
            getJoyX(), getJoyY(), getAccelX(), getAccelY(),getAccelZ());

    stream.print(st);

    if (checkButtonC())
        stream.print("C");
    else
        stream.print("-");
    if (checkButtonZ())
        stream.print("Z");
    else
        stream.print("-");
    stream.println();
}



unsigned int  Nunchuck::joyX::mapVar() {
    Nunchuck* c = (Nunchuck*)controller;
    return smap(c->getJoyX(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Nunchuck::joyX::printMap(Stream& stream) {
    stream.print("Nunchuck::joyX -> ");
    Mapping::printMap(stream);
}

unsigned int  Nunchuck::joyY::mapVar() {
    Nunchuck* c = (Nunchuck*)controller;
    return smap(c->getJoyY(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Nunchuck::joyY::printMap(Stream& stream) {
    stream.print("Nunchuck::joyY -> ");
    Mapping::printMap(stream);
}

unsigned int  Nunchuck::roll::mapVar() {
    Nunchuck* c = (Nunchuck*)controller;
    return smap(c->getRollAngle(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Nunchuck::roll::printMap(Stream& stream) {
    stream.print("Nunchuck::roll -> ");
    Mapping::printMap(stream);
}


unsigned int  Nunchuck::pitch::mapVar() {
    Nunchuck* c = (Nunchuck*)controller;
    return smap(c->getPitchAngle(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Nunchuck::pitch::printMap(Stream& stream) {
    stream.print("Nunchuck::pitch -> ");
    Mapping::printMap(stream);
}

unsigned int  Nunchuck::buttonC::mapVar() {
    Nunchuck* c = (Nunchuck*)controller;
    return (c->checkButtonC()) ? servoMax:servoZero;
}

void Nunchuck::buttonC::printMap(Stream& stream) {
    stream.print("Nunchuck::ButtonC -> ");
    Mapping::printMap(stream);
}

unsigned int  Nunchuck::buttonZ::mapVar() {
    Nunchuck* c = (Nunchuck*)controller;
    return (c->checkButtonZ()) ? servoMax:servoZero;
}

void Nunchuck::buttonZ::printMap(Stream& stream) {
    stream.print("Nunchuck::ButtonZ -> ");
    Mapping::printMap(stream);
}


