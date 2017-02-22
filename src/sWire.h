
#ifndef swire_h
#define swire_h
#include "Arduino.h"

class sWire {
public:
    sWire(uint8_t sda,uint8_t scl);
    sWire();
    void begin();
    void end();

    void beginTransmission(uint8_t address);
    uint8_t endTransmission(void);
    void write(uint8_t val);

    uint8_t requestFrom(uint8_t address, uint8_t size); // moves them into buf

    void readBytes(uint8_t* array,uint8_t len); // copies from ours to thiers

    void usePullUpClock(bool mode);

    void setiicpins(uint8_t sda,uint8_t scl);
private:
    uint8_t _rxbuf[8];
    // SW i2c
    uint8_t _scl_pin;
    uint8_t _sda_pin;
    boolean _use_hw;
    uint32_t _clockSpacing;
    unsigned long ackTimeout;
    boolean _usePullUpClock;

    void _clockHigh();
    void _clockLow();
    void _dataHigh();
    void _dataLow();
    void _sendStart(byte addr);
    void _sendStop();
    void _sendAck();
    void _sendNack();
    void _waitForAck();
    void _clockStallCheck();
    void _shiftOut(uint8_t val);
    uint8_t _readByte();
    void _writeByte(uint8_t value);
};

#endif
