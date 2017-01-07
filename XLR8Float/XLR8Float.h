/*--------------------------------------------------------------------
 Copyright (c) 2015 Alorim Technology.  All right reserved.
 This file is part of the Alorium Technology XLR8 Floating Point library.
 Written by Matt Weber (linkedin.com/in/mattweberdesign) of
 Alorium Technology (info@aloriumtech.com)

 Uses the floating point hardware in Alorium Technology's XLR8 board
  (which happens to be otherwise Arduino Uno compatible) to significantly
  accelerate some floating point functions.

 Usage:
  c = xlr8FloatAdd(a,b);    // performs c = a + b
  c = xlr8FloatSub(a,b);    // performs c = a - b
  c = xlr8FloatMult(a,b);   // performs c = a * b
  c = xlr8FloatDiv(a,b);    // performs c = a / b
  b = xlr8FloatFromInt(a);// performs b = (float)a
        (in xlr8FloatFromInt (a) can be 8 or 16 bit, signed or unsigned)
 
 More complex usage:
   d = a + b*c;
  can be replaced by 2 statements using a temp variable:
   float tmp = xlr8FloatMult(b,c);
   d = xlr8FloatAdd( tmp , a)
  Or the function calls can be nested:
   d = xlr8FloatAdd( xlr8FloatMult(b,c) , a)
 
 Note that it is slightly more efficient (because of how avr-gcc assigns
 general purpose registers for subroutine calls), to have the output
 of one function call be the first operand of the second function call. For
 example:
   d = xlr8FloatAdd( xlr8FloatMult(b,c) , a)
 The line above is likely to be slightly faster than the line below, simply
 because of which registers get used for the operands and results.
   d = xlr8FloatAdd( a, xlr8FloatMult(b,c) )

 
 
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library.  If not, see
 <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------*/

#include <Arduino.h>

#ifndef XLR8FLOAT_h
#define XLR8FLOAT_h

//#if defined(ARDUINO_AVR_UNO) && !defined(XLR8_DISABLE_FLOAT)
#if defined(ARDUINO_XLR8) && !defined(XLR8_DISABLE_FLOAT)
// noinline attribute used to (help) ensure opa and opb get copied to 
//   in regs 25:22 and 21:18 where the hardware expects to get them
float __attribute__ ((noinline)) xlr8FloatAdd (float opa, float opb);
float __attribute__ ((noinline)) xlr8FloatSub (float opa, float opb);
float __attribute__ ((noinline)) xlr8FloatMult (float opa, float opb);
float __attribute__ ((noinline)) xlr8FloatDiv (float opa, float opb);
float xlr8FloatFromInt (uint16_t opa);
float xlr8FloatFromInt (int16_t opa);

#else

// XLR8Float not implemented for selected device, do orinary operations
//  so we don't cause compile errors. Make them static inline to
//  eliminate overhead
static inline float xlr8FloatAdd (float opa, float opb) {return opa + opb;}
static inline float xlr8FloatSub (float opa, float opb) {return opa - opb;}
static inline float xlr8FloatMult (float opa, float opb){return opa * opb;}
static inline float xlr8FloatDiv (float opa, float opb) {return opa / opb;}
static inline float xlr8FloatFromInt (uint16_t opa) {return (float)opa;}
static inline float xlr8FloatFromInt (int16_t opa) {return (float)opa;}


#endif
#endif
