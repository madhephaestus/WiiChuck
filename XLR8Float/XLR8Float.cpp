/*--------------------------------------------------------------------
 Copyright (c) 2015 Alorim Technology.  All right reserved.
 This file is part of the Alorium Technology XLR8 Floating Point library.
 Written by Matt Weber (linkedin.com/in/mattweberdesign) of
   Alorium Technology (info@aloriumtech.com)
 Uses the floating point hardware in Alorium Technology's XLR8 board
  (which happens to be otherwise Arduino Uno compatible) to significantly
  accelerate some floating point functions.

 
 
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

#include "XLR8Float.h"

//#if defined(ARDUINO_AVR_UNO) && !defined(XLR8_DISABLE_FLOAT)
#if defined(ARDUINO_XLR8) && !defined(XLR8_DISABLE_FLOAT)

#ifdef XLR8_OLD_ADDRESSES
 #define XFCTRL   _SFR_IO8(0x10)
 #define XFSTAT   _SFR_IO8(0x11)
 #define XFR0     _SFR_IO8(0x0C)
 #define XFR1     _SFR_IO8(0x0D)
 #define XFR2     _SFR_IO8(0x0E)
 #define XFR3     _SFR_IO8(0x0F)
 #define XFR01    _SFR_IO16(0x0C)
 #define XFR23    _SFR_IO16(0x0E)
 // control register command value definitions for bits 4:0
 #define XFGO_BIT 6
 #define XFADD_CMD  (1 | (1 << XFGO_BIT))
 #define XFSUB_CMD  (2 | (1 << XFGO_BIT))
 #define XFMULT_CMD (3 | (1 << XFGO_BIT))
 #define XFDIV_CMD  (4 | (1 << XFGO_BIT))
 // status register bit positions
 #define XFDONE_BIT 7
 #define XFERROR_BIT 6
#else
 #define XFCTRL   _SFR_IO8(0x10)
 #define XFSTAT   _SFR_IO8(0x11)
 #define XFR0     _SFR_IO8(0x38)
 #define XFR1     _SFR_IO8(0x39)
 #define XFR2     _SFR_IO8(0x3A)
 #define XFR3     _SFR_IO8(0x3B)
 #define XFR01    _SFR_IO16(0x38)
 #define XFR23    _SFR_IO16(0x3A)
 // control register command value definitions for bits 4:0
 #define XFGO_BIT 6
 #define XFADD_CMD  (1 | (1 << XFGO_BIT))
 #define XFMULT_CMD (2 | (1 << XFGO_BIT))
 #define XFDIV_CMD  (3 | (1 << XFGO_BIT))
 // status register bit positions
 #define XFDONE_BIT 7
 #define XFERROR_BIT 6
#endif

#define INVERTSIGN() \
  __asm__ __volatile__ ( \
  "subi	r21, 0x80" "\n\t" \
  :: \
 );

#define SENDXLR8CMD(val) \
    __asm__ __volatile__ ( \
        "ldi r26, %[cmd]" "\n\t" \
        "out %[ctrl], r26" "\n\t" \
        ::[ctrl] "I" (_SFR_IO_ADDR(XFCTRL)),[cmd] "M" (val) \
    );
#define COPYXLR8RESULT() \
    __asm__ __volatile__ ( \
    "in r22, %[rslt0]" "\n\t" \
    "in r23, %[rslt1]" "\n\t" \
    "in r24, %[rslt2]" "\n\t" \
    "in r25, %[rslt3]" "\n\t" \
    ::[rslt0] "I" (_SFR_IO_ADDR(XFR0)), \
      [rslt1] "I" (_SFR_IO_ADDR(XFR1)), \
      [rslt2] "I" (_SFR_IO_ADDR(XFR2)), \
      [rslt3] "I" (_SFR_IO_ADDR(XFR3))  \
);

float __attribute__ ((noinline)) xlr8FloatAdd (float opa, float opb) {
    // Becase operands were loaded before this function call, the
    //  results are already done and we can immediately load
    //  the result reg without waiting for the done status to set.
    // Using assembly instead of C code to copy results because the compiled C
    //  code ends up brining XFR* into r19:18 and then copying from
    //  there to the output registers r25:22. Also, the compiler thinks
    //  the data being copied from XFR* is integer data and we would
    //  need to use a union or other trick to get it viewed as float for
    //  the function return value
    SENDXLR8CMD(XFADD_CMD)
    COPYXLR8RESULT()
}
float __attribute__ ((noinline)) xlr8FloatSub (float opa, float opb) {
  INVERTSIGN()  
    // Give time for updated operand to propogate through function
    __asm__ __volatile__ ("nop"  "\n\t" ::);
    __asm__ __volatile__ ("nop"  "\n\t" ::);
    SENDXLR8CMD(XFADD_CMD)
    COPYXLR8RESULT()
}
float __attribute__ ((noinline)) xlr8FloatMult (float opa, float opb) {
    SENDXLR8CMD(XFMULT_CMD)
    COPYXLR8RESULT()
}
float __attribute__ ((noinline)) xlr8FloatDiv (float opa, float opb) {
    SENDXLR8CMD(XFDIV_CMD)
    while (!(XFSTAT & _BV(XFDONE_BIT))); // div is slower, hang here until we're done
    if (XFSTAT & _BV(XFERROR_BIT)) {
      return opa / opb; // if no div unit, do it the slow old fashioned way
    } else {
      COPYXLR8RESULT()
    }
}

// Took idea from http://lolengine.net/blog/2011/3/20/understanding-fast-float-integer-conversions
//  and modified it (a lot).
float xlr8FloatFromInt(uint16_t opa) {
  union { float f; uint32_t i; } u; u.f = 8388608.0f; u.i |= opa; // set exponent to 2^24
  return xlr8FloatAdd(u.f,-8388608.0f);
}
float xlr8FloatFromInt(int16_t opa) {
  if (!(opa & _BV(15))) { // positive number
    return xlr8FloatFromInt((uint16_t)opa);
  } else {
    union { float f; uint32_t i; } u; 
    u.f = -8388608.0f; // set exponent to 2^24, and set sign bit
    uint16_t tmp = ~((uint16_t)opa);// ones complement the input. Casting first prevents unnecessary sign extension instructions
    u.i |= tmp; // using ones instead of 2s complement and adjusting after lets this work for -32768 as well
    return xlr8FloatAdd(u.f,8388607.0f); // adjust subtract amount by one because we did ones complement instead of twos 
  }
}

#endif
