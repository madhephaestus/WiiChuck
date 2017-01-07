/*--------------------------------------------------------------------
  Copyright (c) 2015 Alorim Technology.  All right reserved.
  This file is part of the Alorium Technology XLR8 NeoPixel library.
  Written by Matt Weber (linkedin/in/mattweberdesign) of 
    Alorium Technology (info@aloriumtech.com) using the same interface
    as the Adafruit_NeoPixel library by Phil Burgess, but the XLR8
    implementation is modified to take advantage of the FPGA hardware
    acceleration available on the XLR8 board.
 
  Notable improvements include the following:
    show() does not disable interrupts, so everything else your sketch is
             doing doesn't come grinding to a halt. This is a BIG deal!
    show() speed. In addition to not locking up the processor by disabling
             interrupts, show() also returns much, much faster enabling you
             to build the next image or do other work while data is still
             being sent to the NeoPixels. This is the advantage of having
             a hardware accelerator running in parallel with software on 
             the processor.
    setBrightness() does not lose pixel information. With the old library,
             if you reduce brightness down to, for example, 32 and then
             bring the brightness back up, you may notice that the image
             doesn't come back to the original. With XLR8 it does. Try it.
    length can be up to 1280 NeoPixels. This is many times greater than
             what can be driven by an standard UNO
    data memory savings. Instead of storing the pixel color data (3 bytes
             per pixel) in the AVR's data memory, XLR8 stores it in a
             separate dedicated memory so the data memory is left available
             for other program needs.
    future possibilities. The hardware is designed in a flexible way that
             will allow for enhanced functions in the future.
  A couple of things to note:
    One thing we don't have is the getPixels() function. In the original
             library this returned a pointer to the pixel color data. As 
             described above the pixel color data is no longer stored in
             data memory, so the getPixels() function has been eliminated.
             The more commonly used getPixelColor() function is still
             available.
    If multiple instances are created, they all share the same pixel memory.
             It's possible for them to run into each other, but you'd have to
             have a pretty big design for that to happen because they default
             to using different sections of that memory as widely spaced as
             possible.
 
 Usage
 The XLR8NeoPixel library can be a drop in replacement for the standard Adafruit
   NeoPixel library. It is common for other libraries, such as Adafruit_NeoMatrix,
   to build on the Adafruit_NeoPixel library and with just three lines added to
   the top of your sketch, it is possible to get the XLR8 advantages used by those 
   libraries as well. For example,
        // 3 new lines added
        #include <XLR8NeoPixel.h>
        #define Adafruit_NeoPixel XLR8NeoPixel
        #define ADAFRUIT_NEOPIXEL_H
        // Existing lines kept
        #include <Adafruit_GFX.h>
        #include <Adafruit_NeoMatrix.h>
        #include <Adafruit_NeoPixel.h>
  As explanation, working from the bottom up:
  -Including Adafruit_NeoPixel.h remains because other library files that we
    are using but are not modifying (Adafruit_NeoMatrix.h in this case) have
    #include <Adafruit_NeoPixel.h>. The way Arduino handles compiling and linking
    generally requires that lower level #includes also be included in the sketch.
  -Including Adafruit_NeoMatrix remains because that is the library that this
    particular sketch is using
  -Including Adafruit_GFX remains for the same reason that it was there initially.
    It is included by Adafruit_NeoMatrix, and the way Arduino handles compiling and
    linking generally requires that lower level #includes also be included in the sketch.
  -Even though we are still including the Adafruit_NeoPixel.h, adding
    #define ADAFRUIT_NEOPIXEL_H causes that header file to appear empty instead of
    having the actual Adafruit library code.
  -By adding #define Adafruit_NeoPixel XLR8NeoPixel, any place in your sketch, or
    in the libraries that your sketch is using, where an Adafruit_NeoPixel string
    is instantiated, it will instantiate an XLR8NeoPixel string instead.
  -And of course, the XLR8NeoPixel library is included. If you are coding only for
    the XLR8 hardware, this is the only line you would need.


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


#ifndef XLR8NEOPIXEL_H
#define XLR8NEOPIXEL_H


// #ARDUINO_XLR8 is passed from IDE to the compiler if XLR8 is selected properly
#ifdef ARDUINO_XLR8

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

// The order of primary colors in the NeoPixel data stream can vary
// among device types, manufacturers and even different revisions of
// the same item.  The third parameter to the XLR8NeoPixel
// constructor encodes the per-pixel byte offsets of the red, green
// and blue primaries (plus white, if present) in the data stream --
// the following #defines provide an easier-to-use named version for
// each permutation.  e.g. NEO_GRB indicates a NeoPixel-compatible
// device expecting three bytes per pixel, with the first byte
// containing the green value, second containing red and third
// containing blue.  The in-memory representation of a chain of
// NeoPixels is the same as the data-stream order; no re-ordering of
// bytes is required when issuing data to the chain.

// Bits 5,4 of this value are the offset (0-3) from the first byte of
// a pixel to the location of the red color byte.  Bits 3,2 are the
// green offset and 1,0 are the blue offset.  If it is an RGBW-type
// device (supporting a white primary in addition to R,G,B), bits 7,6
// are the offset to the white byte...otherwise, bits 7,6 are set to
// the same value as 5,4 (red) to indicate an RGB (not RGBW) device.
// i.e. binary representation:
// 0bWWRRGGBB for RGBW devices
// 0bRRRRGGBB for RGB

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W          R          G          B
#define NEO_RGB  ((0 << 6) | (0 << 4) | (1 << 2) | (2))
#define NEO_RBG  ((0 << 6) | (0 << 4) | (2 << 2) | (1))
#define NEO_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2))
#define NEO_GBR  ((2 << 6) | (2 << 4) | (0 << 2) | (1))
#define NEO_BRG  ((1 << 6) | (1 << 4) | (2 << 2) | (0))
#define NEO_BGR  ((2 << 6) | (2 << 4) | (1 << 2) | (0))

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3))
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2))
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3))
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2))
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1))
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1))

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3))
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2))
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3))
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2))
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1))
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1))

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3))
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2))
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3))
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2))
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1))
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1))

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0))
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0))
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0))
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0))
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0))
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0))

// Add NEO_KHZ400 to the color order value to indicate a 400 KHz
// device.  All but the earliest v1 NeoPixels expect an 800 KHz data
// stream, this is the default if unspecified.  Can disable
// the NEO_KHZ400 line to save a little space.

#define NEO_KHZ800 0x0000 // 800 KHz datastream
#define NEO_KHZ400 0x0100 // 400 KHz datastream

// If 400 KHz support is enabled, the third parameter to the constructor
// requires a 16-bit value (in order to select 400 vs 800 KHz speed).
// If only 800 KHz is enabled, an 8-bit value
// is sufficient to encode pixel color order, saving some space.

#ifdef NEO_KHZ400
typedef uint8_t  neoPixelType;
#else
typedef uint16_t neoPixelType;
#endif

class XLR8NeoPixel {

 public:

  // Constructor: number of LEDs, pin number, LED type
  XLR8NeoPixel(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800);
  XLR8NeoPixel(void);
  ~XLR8NeoPixel();

  void
    begin(void),
    show(void),
    setPin(uint8_t p),
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w),
    setPixelColor(uint16_t n, uint32_t c),
    setAllPixels(uint8_t r, uint8_t g, uint8_t b),
    setAllPixels(uint32_t c),
    setBrightness(uint8_t),
    clear(),
    updateLength(uint16_t n),
    updateType(neoPixelType t);
  uint8_t
    getBrightness(void) const;
  uint16_t
    numPixels(void) const;
  static uint32_t
    Color(uint8_t r, uint8_t g, uint8_t b),
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  uint32_t
    getPixelColor(uint16_t n) const;
  inline bool
    canShow(void) { return (micros() - endTime) >= 50L; }

 private:
  // Perhaps eventually move some of these functions to public?
  void setColorByte(uint16_t n, uint8_t v) const;
  uint8_t getColorByte(uint16_t n) const;
  int findAndReserveCmdbuf(void);
  void setMyCmdbuf(uint8_t i);
  void freeMyCmdbuf(uint8_t i);
  bool testMyCmdbuf(uint8_t i) const;
  void setAllCmdbuf(uint8_t i);
  void freeAllCmdbuf(uint8_t i);
  bool testAllCmdbuf(uint8_t i) const;
  bool testAndSetAllCmdbuf(uint8_t i);
  void freeAllMyCmdbufs();

  boolean
#ifdef NEO_KHZ400  // If 400 KHz NeoPixel support enabled...
    is800KHz,      // ...true if 800 KHz pixels
#endif
    begun;         // true if begin() previously called
  uint16_t
    numLEDs,       // Number of RGB LEDs in strip
    numBytes,      // Size of 'pixels' buffer below (3 or 4 bytes/pixel)
    pixelmemStart; // where in pixel data memory this instance starts
  int8_t
    pin;           // Output pin number (-1 if not yet set)
  uint8_t
    brightness,
    rOffset,       // Index of red byte within each 3- or 4-byte pixel
    gOffset,       // Index of green byte
    bOffset,       // Index of blue byte
    wOffset,       // Index of white byte (same as rOffset if no white)
    cmdbufStart;   // if multiple instances, give them each separate sections of cmd buf
  uint32_t
    endTime;       // Latch timing reference
  uint64_t
    mycmdbufUsed;   // One bit per cmdbuf location showing which ones I'm using
  static uint64_t
    allcmdbufUsed;    // All instances combined of which cmdbuf locations used
};

#else
#error "XLR8NeoPixel library requires Tools->Board->XLR8xxx selection. Install boards from https://github.com/AloriumTechnology/Arduino_Boards"
#endif

#endif // XLR8NeoPixel_H
