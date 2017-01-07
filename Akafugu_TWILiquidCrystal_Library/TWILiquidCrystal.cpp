/*
 * TWI LCD Character Display - Arduino Library
 * (C) 2011 Akafugu Corporation
 *
 * Based on Arduino LiquidCrystal Library
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

#include "TWILiquidCrystal.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

#define TWILCD_DEFAULT_ADDR 50

LiquidCrystal::LiquidCrystal(uint8_t addr)
: _addr(addr)
{
}

LiquidCrystal::LiquidCrystal()
: _addr(TWILCD_DEFAULT_ADDR)
{  
}

// The following constructors are left for  compatibility with the standard LiquidCrystal library only
// None of the pins are used: This library uses only SDA and SCL
LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
: _addr(TWILCD_DEFAULT_ADDR)
{
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
			     uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
: _addr(TWILCD_DEFAULT_ADDR)
{
}

LiquidCrystal::LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
: _addr(TWILCD_DEFAULT_ADDR)
{
}

LiquidCrystal::LiquidCrystal(uint8_t rs,  uint8_t enable,
			     uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
: _addr(TWILCD_DEFAULT_ADDR)
{
}

void LiquidCrystal::resetDisplay()
{
  Wire.beginTransmission(_addr);
  Wire.write(0xFF); // sending some NOP to clear input buffer on display
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.write(0xFF);
  Wire.write(0xFE); // Reset display
  Wire.endTransmission();
}

void LiquidCrystal::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  Wire.begin();
  delay(200);
  
  resetDisplay();
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);
  delay(5);
  
  _firmware_version = getFirmwareVersion();
  
  if(_firmware_version >= 2)
  {
	  Wire.beginTransmission(_addr);
	  Wire.write(0xfd); // Set cols/lines
	  Wire.write(cols);
	  Wire.write(lines);
	  Wire.endTransmission();
  }
  
  delay(5);
}

/********** high level commands, for the user! */
void LiquidCrystal::clear()
{
  Wire.beginTransmission(_addr);
  Wire.write(0x82); // clearscr
  Wire.endTransmission();
}

void LiquidCrystal::home()
{
  Wire.beginTransmission(_addr);
  Wire.write(0x91); // clearscr
  Wire.endTransmission();
}

void LiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
  Wire.beginTransmission(_addr);
  Wire.write(0x92); // gotoxy
  Wire.write(col);
  Wire.write(row);
  Wire.endTransmission();
}

// Turn the display on/off (quickly)
void LiquidCrystal::noDisplay() {
	Wire.beginTransmission(_addr);
	Wire.write(0x93); // Display off
	Wire.endTransmission();
}
void LiquidCrystal::display() {
	Wire.beginTransmission(_addr);
	Wire.write(0x94); // Display on
	Wire.endTransmission();
}

// Turns the underline cursor on/off
void LiquidCrystal::noCursor() {
	Wire.beginTransmission(_addr);
	Wire.write(0x95); // Cursor off
	Wire.endTransmission();	
}
void LiquidCrystal::cursor() {
	Wire.beginTransmission(_addr);
	Wire.write(0x96); // Cursor on
	Wire.endTransmission();
}

// Turn on and off the blinking cursor
void LiquidCrystal::noBlink() {
	Wire.beginTransmission(_addr);
	Wire.write(0x97); // Blink off
	Wire.endTransmission();
}
void LiquidCrystal::blink() {
	Wire.beginTransmission(_addr);
	Wire.write(0x98); // Blink on
	Wire.endTransmission();
}

// These commands scroll the display without changing the RAM
void LiquidCrystal::scrollDisplayLeft(void) {
	Wire.beginTransmission(_addr);
	Wire.write(0x99); // scroll left
	Wire.endTransmission();
}
void LiquidCrystal::scrollDisplayRight(void) {
	Wire.beginTransmission(_addr);
	Wire.write(0x9a); // scroll right
	Wire.endTransmission();
}

// This is for text that flows Left to Right
void LiquidCrystal::leftToRight(void) {
	Wire.beginTransmission(_addr);
	Wire.write(0x9b); // left to right mode
	Wire.endTransmission();
}

// This is for text that flows Right to Left
void LiquidCrystal::rightToLeft(void) {
	Wire.beginTransmission(_addr);
	Wire.write(0x9c); // right to left mode
	Wire.endTransmission();
}

// This will 'right justify' text from the cursor
void LiquidCrystal::autoscroll(void) {
	Wire.beginTransmission(_addr);
	Wire.write(0x9d); // autoscroll on
	Wire.endTransmission();
}

// This will 'left justify' text from the cursor
void LiquidCrystal::noAutoscroll(void) {
	Wire.beginTransmission(_addr);
	Wire.write(0x9e); // autoscroll off
	Wire.endTransmission();
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[]) {
	delay(5);
	Wire.begin();
	Wire.beginTransmission(_addr);
	Wire.write(0x9f); // create custom character
	Wire.write(location&0x7); // we only have 8 locations 0-7
	
	for (int i=0; i<8; i++)
		Wire.write(charmap[i]);
	
	Wire.endTransmission();
	delay(25);
}

void LiquidCrystal::saveContrast(uint8_t value)
{
  Wire.beginTransmission(_addr);
  Wire.write(0xd0); // set contrast
  Wire.write(value);
  Wire.endTransmission();
  delay(5); // Wait for the device to activate the setting
}

void LiquidCrystal::setContrast(uint8_t value)
{
  Wire.beginTransmission(_addr);
  Wire.write(0xd1); // set contrast
  Wire.write(value);
  Wire.endTransmission();
  delay(5); // Wait for the device to activate the setting
}

void LiquidCrystal::saveBrightness(uint8_t value)
{
  Wire.beginTransmission(_addr);
  Wire.write(0x80); // set brightness
  Wire.write(value);
  Wire.endTransmission();
  delay(5); // Wait for the device to activate the setting
}

void LiquidCrystal::setBrightness(uint8_t value)
{
  Wire.beginTransmission(_addr);
  if(_firmware_version >= 3)
	Wire.write(0xd3); // set brightness
  else // Before version 3 there was only saveBrightness
	Wire.write(0x80); // set brightness
  Wire.write(value);
  Wire.endTransmission();
  delay(5); // Wait for the device to activate the setting
}

void LiquidCrystal::saveColor(uint8_t R, uint8_t G, uint8_t B)
{
  if(_firmware_version >= 4)
  {
    Wire.beginTransmission(_addr);
    Wire.write(0xd5); // save RGB
    Wire.write(R);
    Wire.write(G);
    Wire.write(B);
    Wire.endTransmission();
    delay(5); // Wait for the eeprom to be written
  }
}

void LiquidCrystal::setColor(uint8_t R, uint8_t G, uint8_t B)
{
  if(_firmware_version >= 4)
  {
    Wire.beginTransmission(_addr);
    Wire.write(0xd6); // set RGB
    Wire.write(R);
    Wire.write(G);
    Wire.write(B);
    Wire.endTransmission();
  }
}

uint8_t LiquidCrystal::getFirmwareVersion()
{
	uint8_t rdata = 0;
	Wire.beginTransmission(_addr);
	Wire.write(0x8a);
	Wire.endTransmission();
	Wire.requestFrom(_addr, (uint8_t)1);
	if (Wire.available()) rdata = Wire.read();
	return rdata;
}

/*********** mid level commands, for sending data/cmds */

inline void LiquidCrystal::command(uint8_t value) {
  Wire.beginTransmission(_addr);
  Wire.write(0xa3); // send command
  Wire.write(value);
  Wire.endTransmission();
}

inline size_t LiquidCrystal::write(uint8_t value) {
  Wire.beginTransmission(_addr);
  Wire.write(0xa4); // send data
  Wire.write(value);
  Wire.endTransmission();
  return 1;
}

inline void LiquidCrystal::write_raw_data(uint8_t value) {
  Wire.beginTransmission(_addr);
  Wire.write(0xa5); // send raw data
  Wire.write(value);
  Wire.endTransmission();
}

