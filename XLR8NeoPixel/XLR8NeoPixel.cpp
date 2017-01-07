/*-------------------------------------------------------------------------
  Copyright (c) 2015 Alorim Technology.  All right reserved.
  Arduino library to control a wide variety of WS2811- and WS2812-based RGB
  LED devices such as Adafruit FLORA RGB Smart Pixels and NeoPixel strips.
  Currently handles 400 and 800 KHz bitstreams and is optimized for the
  Alorium Technology XLR8 board.

 Written by Matt Weber (linkedin.com/in/mattweberdesign) of
 Alorium Technology (info@aloriumtech.com) using the same interface
 as the Adafruit_NeoPixel library by Phil Burgess.

  -------------------------------------------------------------------------
  This file is part of the XLR8 NeoPixel library.

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
  -------------------------------------------------------------------------*/

#include "XLR8NeoPixel.h"

#define CMDBUF_SIZE 64
#define COLOR_SIZE  (4*1024 - (CMDBUF_SIZE*4))
#define NEOCR   _SFR_MEM8(0xF4)
#define NEOD0   _SFR_MEM8(0xF5)
#define NEOD1   _SFR_MEM8(0xF6)
#define NEOD2   _SFR_MEM8(0xF7)
#define NEOD01  _SFR_MEM16(0xF5)
#define NEOPIN 4
#define NEOCMD 0
#define NEOPINMIN 1
#define NEOPINMAX 13  // hardware actually could go up to 15 but 14&15 aren't connected
#define NEOCMD_NOOP  0x0   // no-op, but set PinNum/BusyID (it won't autoclear)
#define NEOCMD_GTSZ  0x1   // get memsize. D2=cmd buf size (entries), D1/D0= pixel mem size (bytes)
#define NEOCMD_SHOW  0x2   // show WS2812. D2=starting cmd buffer D1/D0=length (bytes)
#define NEOCMD_SHOW2 0x3   // show WS2811. D2=starting cmd buffer D1/D0=length (bytes)
#define NEOCMD_SCOL  0x8   // set color. D2=color value, D1/D0=memory address (autoincrement)
#define NEOCMD_SCBA  0x9   // set cmd buf entry-addr. D2=cmd buf addr, D1/D0=section start addr
#define NEOCMD_SCBL  0xA   // set cmd buf entry-length. D2=cmd buf addr, D1/D0=section length
#define NEOCMD_SCBB  0xB   // set cmd buf entry-brightness. D2=cmd buf addr, D1=reserved, D0=brightness
#define NEOCMD_GCOL  0xC   // get color. D2=color value, D1/D0=memory address (autoincrement)
#define NEOCMD_GCBA  0xD   // get cmd buf entry-addr. D2=cmd buf addr, D1/D0=section start addr
#define NEOCMD_GCBL  0xE   // get cmd buf entry-length. D2=cmd buf addr, D1/D0=section length
#define NEOCMD_GCBB  0xF   // get cmd buf entry-brightness. D2=cmd buf addr, D1=reserved, D0=brightness

// Define just one of the following
//   XLR8HW594SAFE  - Workaround bug in our very first pass hardware
//   XLR8HW594LUCKY - Partial workaround. Allows interrupts during show(), but pixel data could get corrupted if you're unlucky
//   XLR8HW594NOT   - The most likely choice. Using later version of hardware. Allows interrupts during show() without any issues
//#define XLR8HW594SAFE
//#define XLR8HW594LUCKY
#define XLR8HW594NOT


uint64_t XLR8NeoPixel::allcmdbufUsed = 0;    // Static variable shared by all instances

// Constructor when length, pin and type are known at compile-time:
XLR8NeoPixel::XLR8NeoPixel(uint16_t n, uint8_t p, neoPixelType t) :
  brightness(0), endTime(0), begun(false)
{
  cmdbufStart = findAndReserveCmdbuf();
  // Default memory section to use determined from cmdbuf location
  pixelmemStart = cmdbufStart * (COLOR_SIZE/CMDBUF_SIZE);
  updateType(t);
  updateLength(n);
  setPin(p);
}

// via Michael Vogt/neophob: empty constructor is used when strand length
// isn't known at compile-time; situations where program config might be
// read from internal flash memory or an SD card, or arrive via serial
// command.  If using this constructor, MUST follow up with updateType(),
// updateLength(), etc. to establish the strand type, length and pin number!
XLR8NeoPixel::XLR8NeoPixel() :
  pin(-1), brightness(0), endTime(0), begun(false),
  numLEDs(0), numBytes(0), rOffset(1), gOffset(0), bOffset(2), wOffset(1),
  cmdbufStart(0)
#ifdef NEO_KHZ400
  , is800KHz(true)
#endif
{
  cmdbufStart = findAndReserveCmdbuf();
  // Default memory section to use determined from cmdbuf location
  pixelmemStart = cmdbufStart * (COLOR_SIZE/CMDBUF_SIZE);
}

XLR8NeoPixel::~XLR8NeoPixel() {
  freeAllMyCmdbufs();
  if(pin >= 0) {
    while(!canShow()); // make sure full transfer to NeoPixel is done before shutting off
    pinMode(pin, INPUT);
  }
}

void XLR8NeoPixel::begin(void) {
  if(pin >= 0) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  begun = true;
}

void XLR8NeoPixel::updateLength(uint16_t n) {
  // Allocate new data -- note: ALL PIXELS ARE CLEARED
  numBytes = n * ((wOffset == rOffset) ? 3 : 4);
  // Check hardware capacity. Get size returns cmdbuf size (entries)
  //   in D2 and pixel color memory size (bytes) in D1/D0
  NEOCR = NEOCMD_GTSZ << NEOCMD;
  if (NEOD01 >= numBytes) {
    clear();
    numLEDs = n;
  } else {
    numLEDs = numBytes = 0;
  }
}

void XLR8NeoPixel::updateType(neoPixelType t) {
  boolean oldThreeBytesPerPixel = (wOffset == rOffset); // false if RGBW

  wOffset = (t >> 6) & 0b11; // See notes in header file
  rOffset = (t >> 4) & 0b11; // regarding R/G/B/W offsets
  gOffset = (t >> 2) & 0b11;
  bOffset =  t       & 0b11;
#ifdef NEO_KHZ400
  is800KHz = (t < 256);      // 400 KHz flag is 1<<8
#endif

  // If bytes-per-pixel has changed (and pixel data was previously
  // allocated), re-allocate to new size.  Will clear any data.
  if(numBytes) {
    boolean newThreeBytesPerPixel = (wOffset == rOffset);
    if(newThreeBytesPerPixel != oldThreeBytesPerPixel) updateLength(numLEDs);
  }
}

// The show() function is able to be completely different on XLR8 than it is
//  on a standard ATmega-based Arduino. The processor can simply call show()
//  and go on its merry way. No need to handle tricky timing, no need to turn
//  off interrupts, no need to destroy RGB information when reducing brightness,
//  no need to hog all kinds of precious data memory space. All of that is
//  handled and fixed by the dedicated XLR8 hardware.
// The XLR8 NeoPixel hardware provides the ability to drive multiple separate NeoPixel
//    rings/strips/arrays/whatever.
//    A total of 3840Bytes of memory (four 1024x8 memories minus some overhead) is available.
//     In 3 color mode (the most common), to this is 1280 pixels; in 4 color mode it is
//     960 pixels. The hardware doesn't do anything to prevent multiple instances
//     of this class from stepping on each others memory, but it tries to help by
//     giving them different default cmdbuf and memory locations.
//   IO regs to access this hardware are:
//     control : 4 bit pin number/busy id
//             : 4 bit command
//     data2/1/0 :  8 bits each
//   For the show operation, in its simplest form, we first ensure there isn't a
//     show() operation already running, then we write a single command
//     buffer location with the starting address of the data to send, the length in
//     bytes, and the desired brightness. Then we do the show command for the desired
//     length and pin number starting from that command buffer location.
void XLR8NeoPixel::show(void) {
  if(!numBytes) return;
  
  // Data latch = 50+ microsecond pause in the output stream.  Rather than
  // put a delay at the end of the function, the ending time is noted and
  // the function will simply hold off (if needed) on issuing the
  // subsequent round of data until the latch time has elapsed.  This
  // allows the mainline code to start generating the next frame of data
  // rather than stalling for the latch.
  while(!canShow());
  // endTime is a private member (rather than global var) so that mutliple
  // instances on different pins can be quickly issued in succession (each
  // instance doesn't delay the next).
  
  // Check that another instance of this class isn't currently using the hardware
  while((NEOCR >> NEOPIN) & 0xF); // wait for pin/busy to go to zero
  
  // Set the pin number
  NEOCR = (pin << NEOPIN) | (NEOCMD_NOOP << NEOCMD);
  // Set cmd buf entry-address of pixel data
  NEOD01 = pixelmemStart;
  NEOD2  = cmdbufStart;
  NEOCR  = (NEOCMD_SCBA << NEOCMD);
  // Set cmd buf entry-length of pixel data
  NEOD01 = numBytes;
  NEOD2  = cmdbufStart;
  NEOCR  = (NEOCMD_SCBL << NEOCMD);
  // Set brightness
  NEOD0  = brightness;
  NEOD2  = cmdbufStart;
  NEOCR  = (NEOCMD_SCBB << NEOCMD);
  // start the show
  NEOD01 = numBytes;
  NEOD2  = cmdbufStart;
#ifdef NEO_KHZ400 // 800 KHz check needed only if 400 KHz support enabled
  if(is800KHz) {
#endif
    NEOCR  = (pin << NEOPIN) | (NEOCMD_SHOW << NEOCMD);
#ifdef XLR8HW594SAFE
    noInterrupts();
    while((NEOCR >> NEOPIN) & 0xF); // Workaround hardware bug. wait for pin/busy to go to zero
    interrupts();
    endTime = micros();
#endif
#ifdef XLR8HW594LUCKY
    while((NEOCR >> NEOPIN) & 0xF); // Workaround hardware bug. wait for pin/busy to go to zero
    endTime = micros();
#endif
#ifdef XLR8HW594NOT
    endTime = micros() + 10*numBytes; // 1.25us per bit * 8 bits per byte = 10us per byte
#endif
#ifdef NEO_KHZ400 // 800 KHz check needed only if 400 KHz support enabled
  } else {
    NEOCR  = (pin << NEOPIN) | (NEOCMD_SHOW << NEOCMD);
 #ifdef XLR8HW594SAFE
    noInterrupts();
    while((NEOCR >> NEOPIN) & 0xF); // Workaround hardware bug. wait for pin/busy to go to zero
    interrupts();
    endTime = micros();
 #endif
 #ifdef XLR8HW594LUCKY
    while((NEOCR >> NEOPIN) & 0xF); // Workaround hardware bug. wait for pin/busy to go to zero
    endTime = micros();
 #endif
 #ifdef XLR8HW594NOT
    endTime = micros() + 20*numBytes; // 2.5us per bit * 8 bits per byte = 20us per byte
 #endif
  }
#endif
}

// Set the output pin number
void XLR8NeoPixel::setPin(uint8_t p) {
  if(begun && (pin >= 0)) {
    while(!canShow()); // make sure full transfer to NeoPixel is done before shutting off
    pinMode(pin, INPUT); // if switching pins, shut off the previous one
  }
  if((p >= NEOPINMIN) && (p <= NEOPINMAX)) {
    pin = p;
    if(begun) {
      pinMode(p, OUTPUT);
      digitalWrite(p, LOW);
    }
  }
}

// Just set one part of the color
void XLR8NeoPixel::setColorByte(uint16_t n, uint8_t v) const {
  uint16_t memloc = pixelmemStart + n;
  if (memloc >= COLOR_SIZE) {memloc -= COLOR_SIZE;}
  NEOD2 = v;
  NEOD01 = memloc;
  NEOCR = NEOCMD_SCOL << NEOCMD;
}
// Just get one part of the color
uint8_t XLR8NeoPixel::getColorByte(uint16_t n) const {
  uint16_t memloc = pixelmemStart + n;
  if (memloc >= COLOR_SIZE) {memloc -= COLOR_SIZE;}
  NEOD01 = memloc;
  NEOCR = NEOCMD_GCOL << NEOCMD;
  return NEOD2;
}

// Set pixel color from separate R,G,B components:
void XLR8NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  //while((NEOCR >> NEOPIN) & 0xF) == pin); // If we're currently doing show(), let it finish?
  if(n < numLEDs) {
    uint16_t p;
    if(wOffset == rOffset) { // Is an RGB-type strip
      p = n * 3;    // 3 bytes per pixel
    } else {                 // Is a WRGB-type strip
      p = n * 4;    // 4 bytes per pixel
      setColorByte(p+wOffset,0);        // But only R,G,B passed -- set W to 0
    }
    setColorByte(p+rOffset,r);    // R,G,B always stored
    setColorByte(p+gOffset,g);
    setColorByte(p+bOffset,b);
  }
}

// Set pixel color from 'packed' 32-bit RGB color:
void XLR8NeoPixel::setPixelColor(uint16_t n, uint32_t c) {
  if(n < numLEDs) {
    uint8_t
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
    uint16_t p;
    if(wOffset == rOffset) {
      p = n * 3;    // 3 bytes per pixel
    } else {
      p = n * 4;    // 4 bytes per pixel
      uint8_t w = (uint8_t)(c >> 24);
      setColorByte(p+wOffset,w);
    }
    setColorByte(p+rOffset,r);
    setColorByte(p+gOffset,g);
    setColorByte(p+bOffset,b);
  }
}

// Set entire strip color from separate R,G,B components:
void XLR8NeoPixel::setAllPixels(uint8_t r, uint8_t g, uint8_t b) {
  setAllPixels(Color(r,g,b));
}

// Convenience function:
// Set entire strip color from 'packed' 32-bit RGB color:
void XLR8NeoPixel::setAllPixels(uint32_t c) {
  uint8_t temp_wOffset;
  if (wOffset == rOffset) {temp_wOffset = 3;}
  uint32_t colors_in_order = ((c >> 16 & 0xFF) << 8*rOffset) | 
                             ((c >> 8  & 0xFF) << 8*gOffset) | 
                             ((c       & 0xFF) << 8*bOffset) | 
                             ((c >> 24 & 0xFF) << 8*temp_wOffset); 
  //while((NEOCR >> NEOPIN) & 0xF) == pin); // If we're currently doing show(), let it finish?
  NEOD01 = pixelmemStart; // start address, autoincrements, and autowraps
  for(uint16_t i=0; i<numLEDs; i++) {
    NEOD2 = (colors_in_order & 0xFF);
    NEOCR = NEOCMD_SCOL << NEOCMD;
    NEOD2 = ((colors_in_order >> 8) & 0xFF);
    NEOCR = NEOCMD_SCOL << NEOCMD;
    NEOD2 = ((colors_in_order >> 16) & 0xFF);
    NEOCR = NEOCMD_SCOL << NEOCMD;
    if(wOffset != rOffset) {    // 4 bytes per pixel
      NEOD2 = ((colors_in_order >> 24) & 0xFF);
      NEOCR = NEOCMD_SCOL << NEOCMD;
    }
  }
}

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t XLR8NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Convert separate R,G,B,W into packed 32-bit WRGB color.
// Packed format is always WRGB, regardless of LED strand color order.
uint32_t XLR8NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Query color from previously-set pixel (returns packed 32-bit RGB value)
uint32_t XLR8NeoPixel::getPixelColor(uint16_t n) const {
  if(n >= numLEDs) return 0; // Out of bounds, return no color.
  uint16_t p;
  uint8_t r,g,b,w;
  if(wOffset == rOffset) {
    p = n * 3;    // 3 bytes per pixel
    w = 0;
  } else {
    p = n * 4;    // 4 bytes per pixel
    w = getColorByte(p+wOffset);
  }
  r = getColorByte(p+rOffset);
  g = getColorByte(p+gOffset);
  b = getColorByte(p+bOffset);
  return ((uint32_t)w << 24) |
         ((uint32_t)r << 16) |
         ((uint32_t)g <<  8) |
          (uint32_t)b;
}

uint16_t XLR8NeoPixel::numPixels(void) const {
  return numLEDs;
}

// Adjust output brightness; 0=darkest (off), 255=brightest.  This does
// NOT immediately affect what's currently displayed on the LEDs.  The
// next call to show() will refresh the LEDs at this level.
// In contrast to the Adafruit_NeoPixel library, this is not "lossy".
// Other NeoPixel libraries are lossy because they can't adjust brightness
// on the fly and still meet the tight timing in the WS2811/WS2812, so
// the have to modify the rgb values in memory, and in changing it there
// you lose part of the original rgb value. By using XLR8's dedicated
// hardware, brightness can be scaled on the fly and therefore the full
// rgb values can be kept in memory.
void XLR8NeoPixel::setBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  // Just store here for now, write to hardware when show() is called
  brightness = b + 1;
}

//Return the brightness value
uint8_t XLR8NeoPixel::getBrightness(void) const {
  return brightness - 1;
}

void XLR8NeoPixel::clear() {
  //while((NEOCR >> NEOPIN) & 0xF) == pin); // If we're currently doing show(), let it finish?
  NEOD2 = 0;
  NEOD01 = pixelmemStart; // start address, autoincrements, and autowraps
  for(uint16_t i=0; i<numBytes; i++) {
    NEOCR = NEOCMD_SCOL << NEOCMD;
  }
}

//Return cmdbuf location spaced furthest away from currently
//  allocated ones. And reserve it right away as well
//Returns the location found, or -1 if no locations available
int XLR8NeoPixel::findAndReserveCmdbuf(void)  {
  const uint8_t initialCmdbufOrder[] = { 0,32,48,16,24,56,40, 8};
  const uint8_t sizeofCmdbufOrder = sizeof(initialCmdbufOrder)/sizeof(initialCmdbufOrder[0]);
  uint8_t indexCmdbufOrder = 0;
  int cmdbufTemp = 0;
  int cmdbufFound = -1;
  for (indexCmdbufOrder = 0;
       (indexCmdbufOrder < sizeofCmdbufOrder) && (cmdbufFound == -1);
       indexCmdbufOrder++) {
    cmdbufTemp = initialCmdbufOrder[indexCmdbufOrder];
    if (!testAndSetAllCmdbuf(cmdbufTemp)) { // if it was free, we'll use it
      cmdbufFound = cmdbufTemp;
      setMyCmdbuf(cmdbufFound);
    }
  }
  // If don't find it in the first few default locations, run an algorithm
  //  to find a spot that's furthest removed from the existing spots.
  //  In allcmdbufUsed, 1's are used spots, 0's are free. To find
  //  furthest from a used spot, "grow" the used spots by or'ing
  //  with neighbors until full, then backup one step.
  // Alorithm not implemented yet. Just search in order starting at the end.
  for (cmdbufTemp = 63; (cmdbufFound == -1) && (cmdbufTemp >= 0); cmdbufTemp--) {
    if (!testAndSetAllCmdbuf(cmdbufTemp)) { // if it was free, we'll use it
      cmdbufFound = cmdbufTemp;
      setMyCmdbuf(cmdbufFound);
    }
  }
  return cmdbufTemp;
}
void XLR8NeoPixel::setMyCmdbuf(uint8_t i)  {
  mycmdbufUsed  |= (1ULL << i);
}
void XLR8NeoPixel::freeMyCmdbuf(uint8_t i)  {
  mycmdbufUsed &= ~(1ULL << i);
}
bool XLR8NeoPixel::testMyCmdbuf(uint8_t i) const {
  return (mycmdbufUsed >> i) & 1;
}
void XLR8NeoPixel::setAllCmdbuf(uint8_t i)  {
  XLR8NeoPixel::allcmdbufUsed  |= (1ULL << i);
}
void XLR8NeoPixel::freeAllCmdbuf(uint8_t i)  {
  XLR8NeoPixel::allcmdbufUsed &= ~(1ULL << i);
}
bool XLR8NeoPixel::testAllCmdbuf(uint8_t i) const {
  return (XLR8NeoPixel::allcmdbufUsed >> i) & 1;
}
// for cases driving multiple neopixel strings (have multiple
//   instances of this class) ensure each instance gets
//   its own section of cmdbuf by briefly disabling interrupts
//   to do atomic test and set
// returns result of the test (true if location was already reserved)
bool XLR8NeoPixel::testAndSetAllCmdbuf(uint8_t i) {
  if (i >= sizeof(XLR8NeoPixel::allcmdbufUsed)*8) {return true;} // using uint64_t for flags so any indices > 63 are invalid
  uint8_t oldSREG = SREG;
  cli();
  // Check hardware capacity. Get size returns cmdbuf size (entries)
  //   in D2 and pixel color memory size (bytes) in D1/D0
  NEOCR = NEOCMD_GTSZ << NEOCMD;
  if (NEOD2 <= i) {return true;}
  // Looks okay, go ahead and do the test and set
  bool alreadySet = testAllCmdbuf(i);
  setAllCmdbuf(i);
  SREG = oldSREG; // reenable interrupts
  return alreadySet;
}
void XLR8NeoPixel::freeAllMyCmdbufs() {
  uint8_t oldSREG = SREG;
  cli();
  XLR8NeoPixel::allcmdbufUsed &= ~mycmdbufUsed;
  mycmdbufUsed = 0;
  SREG = oldSREG; // reenable interrupts
}
  


