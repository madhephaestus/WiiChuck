/*--------------------------------------------------------------------
 Copyright (c) 2016 Alorim Technology.  All right reserved.
 This library takes advantage of the high-performance ADC found
 on an XLR8 board. It is a 12-bit ADC (versus Arduino's 10bit)
 and can perform at higher sampling rates than Arduino.
 
 You can still use all the ADC code you already have, whether it
 is Arduino's analogRead(), or direct reads and writes of the
 AVR registers. Simply instantiating a
 
 
 Written by Matt Weber (linkedin.com/in/mattweberdesign) of
 Alorium Technology (info@aloriumtech.com)

 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with XLR8 NeoPixel.  If not, see
 <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------*/

#include <Arduino.h>

#ifndef XLR8ADC_h
#define XLR8ADC_h

// #ARDUINO_XLR8 is passed from IDE to the compiler if XLR8 is selected properly
#ifdef ARDUINO_XLR8

#define XLR8ADCR   _SFR_MEM8(0x7d)
#define AD12EN 7
class XLR8adcClass {
  public:
  // Done similar to Arduino SPI class with begin() and end() instead of
  //  constructor/destructor
  static void begin();
  inline static void set12bitMode() { XLR8ADCR |= _BV(AD12EN); }
  inline static void set10bitMode() { XLR8ADCR &= ~_BV(AD12EN); }
  inline static bool is12bitMode(return (XLR8ADCR & _BV(AD12EN)));
  inline static bool is10bitMode(return !(XLR8ADCR & _BV(AD12EN));
  static void end();
  private:
  // Placeholder to remind myself that any variables we add in the future probably
  //  would be static because it wouldn't make sense to have multiple instances of
  //  this class anyway.
  // static uint8_t placeholder;
};
#else
 #error "XLR8ADC library requires Tools->Board->XLR8xxx selection. Install boards from https://github.com/AloriumTechnology/Arduino_Boards"
#endif

#endif
