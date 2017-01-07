/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * (C) 2011-12 Akafugu Corporation
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

#ifndef TWIDISPLAY_H
#define TWIDISPLAY_H

#if defined(ARDUINO) && ARDUINO >= 100
#  include <Arduino.h>
#else
#  include <WProgram.h>
#endif
#include <Wire.h>

#include <inttypes.h>
#include <Print.h>

class TWIDisplay : public Print
{
public:
  TWIDisplay(int addr);

  void begin();

  void changeAddress(int new_addr);
  void showAddress();
  void setBrightness(int brightness);
  void clear();

  void setRotateMode();
  void setScrollMode();

  void setDot(int position, bool on);
  void setDots(bool dot0, bool dot1, bool dot2, bool dot3);
  void setDots(bool dot0, bool dot1, bool dot2, bool dot3, bool dot4, bool dot5, bool dot6, bool dot7);
  void setApostrophe(int position, bool on);
  void setApostrophes(bool a0, bool a1, bool a2, bool a3);
  void setApostrophes(bool a0, bool a1, bool a2, bool a3, bool a4, bool a5, bool a6, bool a7);

  void setPosition(int position);

  void writeInt(int val);
  void writeChar(char val);
  void writeStr(char* val);
  void writeTemperature(int temp, char symbol);
  void writeTemperature(int temp_t, int temp_f, char symbol);
  void writeTime(int hour, int min, int sec);
  void writeTime12h(int hour, int min, int sec);  // for 12h time display
  void writeSegments(int segments);
  void writeSegments16(uint16_t segments);

  int getFirmwareRevision();
  int getDigits();
  int getSegments();

  // functions available on TWIDisplay-LCD only
  void setBeep(int val);
  void setBias(int val);

  // Inherited from Print
  virtual size_t write(uint8_t val) { writeChar((char)val); }
private:
  void  set_number(uint16_t num);
  int m_addr;
  uint8_t m_dots;
  uint8_t m_apostrophes;
  char m_data[8];

  uint8_t m_digits;
  uint8_t m_version;

  void print2(int num);
  void print2sp(int num);  // leading space if < 10
};


#endif // TWIDISPLAY_H
