/*
 * LEDStrip - Arduino driver for HL1606-based LED strips
 * Thanks to: John M Cohn
 * Copyright (c) 2009, Synoptic Labs
 * All rights reserved.
 
    Some higher level commands added by ladyada

 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SYNOPTIC LABS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SYNOPTIC LABS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef HL1606strip_h
#define HL1606strip_h

#include <inttypes.h>

enum {
  OFF,  // 0b00
  ON,   // 0b01
  UP,   // 0b10
  DOWN, // 0b11
  NONCMD 
};

#define LATCH (_BV(7))
#define SPEED2X (_BV(6))

// colors, each bit one LED
#define BLACK 0b000
#define WHITE 0b111
#define RED 0b100
#define YELLOW 0b110
#define GREEN 0b010
#define TEAL 0b011
#define BLUE 0b001
#define VIOLET 0b101

class HL1606strip
{
  private:
    uint8_t _dPin;
    uint8_t _sPin;
    uint8_t _latchPin;
    uint8_t _clkPin;
    uint8_t _faderEnabled;
    unsigned int _faderPulseHalfWidth;
    unsigned int _faderPulseNewHalfWidth;
    unsigned long _faderPulseNextEdge;

    // we will control up to 255 LEDs!
    uint8_t *_leds;
    uint8_t _numLEDs;
  public:

    HL1606strip(int, int, int, int, uint8_t);
    HL1606strip(int, int, int, uint8_t);

    // some higher level commands we added
    void setLEDcolor(uint8_t n, uint8_t color);
    uint8_t getLEDcolor(uint8_t n);
    void writeStrip(void);
    uint8_t numLEDs(void);

    uint8_t rgbPush(uint8_t, uint8_t, uint8_t);
    uint8_t rgbPush2X(uint8_t, uint8_t, uint8_t);
    void pushCmd(uint8_t);
    void blankPush();
    void latch();
    void sleep();
    void wakeup();
    void faderSpeedSet(unsigned int);
    unsigned int faderSpeedGet();
    void faderCrank();
    void sPulse();
};

#endif
