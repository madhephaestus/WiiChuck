#include "sWire.h"

sWire::sWire() {

    _clockSpacing = 1;
    ackTimeout = 100;
    _usePullUpClock = false;
}

sWire::sWire(uint8_t sda,uint8_t scl) {
    _sda_pin = sda;
    _scl_pin = scl;
    _clockSpacing = 1;
    ackTimeout = 100;
    _usePullUpClock = false;
}

void sWire::setiicpins(uint8_t sda,uint8_t scl) {
    _sda_pin = sda;
    _scl_pin = scl;
}
void sWire::begin() {

}
void sWire::end() {

}

void sWire::beginTransmission(uint8_t address) {
    Serial.print("s ");
    Serial.print(address,HEX);
    _sendStart((address << 1));
    _waitForAck();

}
uint8_t sWire::endTransmission(void) {
    _sendStop();
}
void sWire::write(uint8_t val) {
    Serial.print("w ");
    Serial.print(val,HEX);
    _writeByte(val);
    _waitForAck();
}

uint8_t sWire::requestFrom(uint8_t address, uint8_t size) { // moves them into buf
    _sendStart((address << 1));
    _waitForAck();

    uint8_t csize = size;
    if (csize>sizeof(_rxbuf)) csize=sizeof(_rxbuf);

    for (int i = 0; i < size; i++) {
        delayMicroseconds(40);
        _rxbuf[i]= _readByte();
        if (i < (size - 1)) _sendAck();
        else _sendNack();
    }
    _sendStop();

}
void sWire::readBytes(uint8_t* array,uint8_t len) { // copies from ours to thiers
    uint8_t csize = len;
    if (csize>sizeof(_rxbuf)) csize=sizeof(_rxbuf);
    for(int i=0; i<csize; i++) array[i]=_rxbuf[i];
}



void sWire::_sendStart(byte addr) {
    _dataHigh();
    _clockHigh();
    _dataLow();
    _clockLow();
    _shiftOut(addr);

}

void sWire::_sendStop() {
    _clockLow();
    _clockHigh();
    _dataHigh();
    pinMode(_sda_pin, INPUT);
}

void sWire::_sendNack() {
    _clockLow();
    _dataHigh();
    _clockHigh();
    _clockLow();
    pinMode(_sda_pin, INPUT);
}

void sWire::_sendAck() {
    _clockLow();
    _dataLow();
    _clockHigh();
    _clockLow();
    pinMode(_sda_pin, INPUT);
}


void sWire::_dataHigh() {
    if (_usePullUpClock) {
        pinMode(_sda_pin, INPUT);
    } else {
        pinMode(_sda_pin, OUTPUT);
        digitalWrite(_sda_pin, HIGH);
    }

}
void sWire::_dataLow() {
    pinMode(_sda_pin, OUTPUT);
    digitalWrite(_sda_pin, LOW);

}
void sWire::_clockHigh() {

    if (_usePullUpClock) {
        _clockStallCheck();
    } else {
        pinMode(_scl_pin, OUTPUT);
        digitalWrite(_scl_pin, HIGH);
    }

    if (_clockSpacing > 0)
        delayMicroseconds(_clockSpacing);

}
void sWire::_clockLow() {
    pinMode(_scl_pin, OUTPUT);
    digitalWrite(_scl_pin, LOW);
    if (_clockSpacing > 0)
        delayMicroseconds(_clockSpacing);

}

void sWire::_clockStallCheck() {
    pinMode(_scl_pin, INPUT);

    unsigned long time = millis();
    while (digitalRead(_scl_pin) != HIGH && (time + ackTimeout) < millis()) {
    }

}
void sWire::_waitForAck() {
    pinMode(_sda_pin, INPUT);
    _clockHigh();
    unsigned long time = millis();
    while (digitalRead(_sda_pin) == HIGH && (time + ackTimeout) < millis()) {
    }
    if (digitalRead(_sda_pin) == LOW) Serial.println(" ack");
    else Serial.println(" to");

    _clockLow();
}

uint8_t sWire::_readByte() {

    pinMode(_sda_pin, INPUT);

    uint8_t value = 0;
    for (int i = 0; i < 8; ++i) {
        _clockHigh();
        value |= (digitalRead(_sda_pin) << (7 - i));
        _clockLow();
    }
    Serial.print("r ");
    Serial.print(value,HEX);
    return value;
}

void sWire::_writeByte(uint8_t value) {
    _shiftOut(value);
}


void sWire::usePullUpClock(bool mode) {
    _usePullUpClock = mode;
}

void sWire::_shiftOut(uint8_t val) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if ((val & (1 << (7 - i))) == 0) {
            _dataLow();
        } else
            _dataHigh();
        _clockHigh();
        _clockLow();
    }
}
