/*
 MCP41xxx.cpp
 MCP41xxx driver for Arduino
 
 Copyright 2015-2016 Max Chan <max@maxchan.info>
 */

#include "AD5241.h"

AD5241::AD5241(TwoWire &wire, uint8_t address)
{
    _wire = wire;
	_address = address;
	_digitalOutputs = 0;
}

AD5241::AD5241(uint8_t address)
{
	_wire = Wire;
	_address = address;
	_digitalOutputs = 0;
}

void AD5241::begin(void)
{
    // Prepare I2C hardware.
    _wire.begin();
}

void AD5241::analogWrite(uint16_t value)
{
	analogWrite(0, value);
}

void AD5241::analogWrite(uint8_t gate, uint16_t value)
{
	// Prepare I2C message
	uint8_t cmd = (gate) ? 0x80 : 0x00;
	cmd |= _digitalOutputs << 3;
	if (value == DAC_SHUTDOWN) cmd |= _BV(5);
	if (value == DAC_RESET) cmd |= _BV(6);

	_wire.beginTransmission(_address);
	_wire.write(cmd);
	if (value != DAC_SHUTDOWN && value != DAC_RESET) _wire.write(value);
	_wire.endTransmission(true);
}

void AD5241::digitalWrite(uint8_t pin, uint8_t value)
{
	if (value) _digitalOutputs |= _BV(!pin);
	else _digitalOutputs &= ~_BV(!pin);

	uint8_t cmd = _digitalOutputs << 3;
	cmd &= 0x1f;

	_wire.beginTransmission(_address);
	_wire.write(cmd);
	_wire.write(value);
	_wire.endTransmission(true);
}

