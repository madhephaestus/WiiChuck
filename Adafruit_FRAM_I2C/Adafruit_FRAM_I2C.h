/**************************************************************************/
/*! 
    @file     Adafruit_FRAM_I2C.h
    @author   KTOWN (Adafruit Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _ADAFRUIT_FRAM_I2C_H_
#define _ADAFRUIT_FRAM_I2C_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Wire.h>

#define MB85RC_DEFAULT_ADDRESS        (0x50) /* 1010 + A2 + A1 + A0 = 0x50 default */
#define MB85RC_SLAVE_ID       (0xF8)

class Adafruit_FRAM_I2C {
 public:
  Adafruit_FRAM_I2C(void);
  
  boolean  begin(uint8_t addr = MB85RC_DEFAULT_ADDRESS);
  void     write8 (uint16_t framAddr, uint8_t value);
  uint8_t  read8  (uint16_t framAddr);
  void     getDeviceID(uint16_t *manufacturerID, uint16_t *productID);

 private:
  uint8_t i2c_addr;
  boolean _framInitialised;
};

#endif
