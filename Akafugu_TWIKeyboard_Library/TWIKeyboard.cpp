/*
 * TWIKeyboard: Arduino Library for Akafugu TWI/I2C Keyboard and LED controller
 * (C) 2011 Akafugu Corporation
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 */

#include "TWIKeyboard.h"

#if defined(ARDUINO) && ARDUINO < 100
#  define write send
#  define read  receive
#endif

TWIKeyboard::TWIKeyboard(int addr)
	: m_addr(addr)
{
}

void TWIKeyboard::changeAddress(int new_addr)
{
	Wire.beginTransmission(m_addr);
	Wire.write(0x81); // change address
	Wire.write(new_addr);
	Wire.endTransmission();
}

void TWIKeyboard::clearLeds()
{
	// Send clear command
	Wire.beginTransmission(m_addr);
	Wire.write(0x82); // clear
	Wire.endTransmission();	
}

void TWIKeyboard::begin()
{
	// Clear the buffer
	Wire.beginTransmission(m_addr);
	Wire.write(0xFF);
	Wire.write(0xFF);
	Wire.write(0xFF);
	Wire.write(0xFF);
	Wire.write(0xFE); // reset
	Wire.endTransmission();	
}

/* --- */

void TWIKeyboard::setLed(uint8_t no, uint8_t brightness)
{
	Wire.beginTransmission(m_addr);
	Wire.write(0x83);
	Wire.write(no);
	Wire.write(brightness);
	Wire.endTransmission();
}

void TWIKeyboard::pulseLed(uint8_t no, bool on)
{
	Wire.beginTransmission(m_addr);
	Wire.write(0x84);
	Wire.write(no);
	Wire.write((uint8_t)on);
	Wire.endTransmission();
}

void TWIKeyboard::dimLed(uint8_t no, uint8_t brightness)
{
	Wire.beginTransmission(m_addr);
	Wire.write(0x85);
	Wire.write(no);
	Wire.write(brightness);
	Wire.endTransmission();
}

/* ---- */

void TWIKeyboard::setKeyRepeat(uint8_t button, uint8_t mode)
{
	Wire.beginTransmission(m_addr);
	Wire.write(0x92);
	Wire.write(button);
	Wire.write(mode);
	Wire.endTransmission();
}

uint8_t TWIKeyboard::getKeyUp()
{
	uint8_t rdata = 0;
	Wire.beginTransmission(m_addr);
	Wire.write(0x90);
	Wire.endTransmission();
	Wire.requestFrom(m_addr, 1);
	if (Wire.available()) rdata = Wire.read();
	return rdata;
}

uint8_t TWIKeyboard::getKeyDown()
{
	uint8_t rdata = 0;
	Wire.beginTransmission(m_addr);
	Wire.write(0x91);
	Wire.endTransmission();
	Wire.requestFrom(m_addr, 1);
	if (Wire.available()) rdata = Wire.read();
	return rdata;
}

