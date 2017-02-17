uint16_t Classic::joyXLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->joyXLeft(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyXLeft::printMap(Stream& stream) {
    stream.print("Classic::joyXLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getjoyXLeft() {
    return decodeInt(joyXLeftBytes);
}


uint16_t Classic::joyXRight::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->joyXRight(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyXRight::printMap(Stream& stream) {
    stream.print("Classic::joyXRight -> ");
    Mapping::printMap(stream);
}

int Classic::getjoyXRight() {
    return decodeInt(joyXRightBytes);
}


uint16_t Classic::joyYLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->joyYLeft(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyYLeft::printMap(Stream& stream) {
    stream.print("Classic::joyYLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getjoyYLeft() {
    return decodeInt(joyYLeftBytes);
}


uint16_t Classic::joyYRight::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->joyYRight(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::joyYRight::printMap(Stream& stream) {
    stream.print("Classic::joyYRight -> ");
    Mapping::printMap(stream);
}

int Classic::getjoyYRight() {
    return decodeInt(joyYRightBytes);
}


uint16_t Classic::triggerLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->triggerLeft(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::triggerLeft::printMap(Stream& stream) {
    stream.print("Classic::triggerLeft -> ");
    Mapping::printMap(stream);
}

int Classic::gettriggerLeft() {
    return decodeInt(triggerLeftBytes);
}


uint16_t Classic::triggerRight::mapVar() {
    Classic* c = (Classic*)controller;
    return smap(c->triggerRight(),myMax,myZero,myMin,servoMax,servoZero,servoMin);
}

void Classic::triggerRight::printMap(Stream& stream) {
    stream.print("Classic::triggerRight -> ");
    Mapping::printMap(stream);
}

int Classic::gettriggerRight() {
    return decodeInt(triggerRightBytes);
}


uint16_t Classic::padRight::mapVar() {
    Classic* c = (Classic*)controller;
    return c->padRight() ? servoMax,sercoZero;
}

void Classic::padRight::printMap(Stream& stream) {
    stream.print("Classic::padRight -> ");
    Mapping::printMap(stream);
}

int Classic::getpadRight() {
    return decodeInt(padRightBytes);
}


uint16_t Classic::padDown::mapVar() {
    Classic* c = (Classic*)controller;
    return c->padDown() ? servoMax,sercoZero;
}

void Classic::padDown::printMap(Stream& stream) {
    stream.print("Classic::padDown -> ");
    Mapping::printMap(stream);
}

int Classic::getpadDown() {
    return decodeInt(padDownBytes);
}


uint16_t Classic::padUp::mapVar() {
    Classic* c = (Classic*)controller;
    return c->padUp() ? servoMax,sercoZero;
}

void Classic::padUp::printMap(Stream& stream) {
    stream.print("Classic::padUp -> ");
    Mapping::printMap(stream);
}

int Classic::getpadUp() {
    return decodeInt(padUpBytes);
}


uint16_t Classic::padLeft::mapVar() {
    Classic* c = (Classic*)controller;
    return c->padLeft() ? servoMax,sercoZero;
}

void Classic::padLeft::printMap(Stream& stream) {
    stream.print("Classic::padLeft -> ");
    Mapping::printMap(stream);
}

int Classic::getpadLeft() {
    return decodeInt(padLeftBytes);
}


uint16_t Classic::buttonX::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonX() ? servoMax,sercoZero;
}

void Classic::buttonX::printMap(Stream& stream) {
    stream.print("Classic::buttonX -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonX() {
    return decodeInt(buttonXBytes);
}


uint16_t Classic::buttonY::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonY() ? servoMax,sercoZero;
}

void Classic::buttonY::printMap(Stream& stream) {
    stream.print("Classic::buttonY -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonY() {
    return decodeInt(buttonYBytes);
}


uint16_t Classic::buttonA::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonA() ? servoMax,sercoZero;
}

void Classic::buttonA::printMap(Stream& stream) {
    stream.print("Classic::buttonA -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonA() {
    return decodeInt(buttonABytes);
}


uint16_t Classic::buttonB::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonB() ? servoMax,sercoZero;
}

void Classic::buttonB::printMap(Stream& stream) {
    stream.print("Classic::buttonB -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonB() {
    return decodeInt(buttonBBytes);
}


uint16_t Classic::buttonMinus::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonMinus() ? servoMax,sercoZero;
}

void Classic::buttonMinus::printMap(Stream& stream) {
    stream.print("Classic::buttonMinus -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonMinus() {
    return decodeInt(buttonMinusBytes);
}


uint16_t Classic::buttonHome::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonHome() ? servoMax,sercoZero;
}

void Classic::buttonHome::printMap(Stream& stream) {
    stream.print("Classic::buttonHome -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonHome() {
    return decodeInt(buttonHomeBytes);
}


uint16_t Classic::buttonPlus::mapVar() {
    Classic* c = (Classic*)controller;
    return c->buttonPlus() ? servoMax,sercoZero;
}

void Classic::buttonPlus::printMap(Stream& stream) {
    stream.print("Classic::buttonPlus -> ");
    Mapping::printMap(stream);
}

int Classic::getbuttonPlus() {
    return decodeInt(buttonPlusBytes);
}

