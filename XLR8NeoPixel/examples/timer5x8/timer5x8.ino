
/*--------------------------------------------------------------------
  Copyright (c) 2015 Alorim Technology.  All right reserved.
  This file is part of the Alorium Technology XLR8 NeoPixel library.
  Written by Matt Weber (linkedin.com/in/mattweberdesign) of 
    Alorium Technology (info@aloriumtech.com) 
    
  The example shows both how to use the XLR8 NeoPixel library and also
    demonstrates the advantage of using the XLR8 board's dedicated
    NeoPixel hardware. Specifically highlighted in this example is
    the ability to drive NeoPixels without having interrupts disabled.
  Adafruit's sweet 5x8 NeoPixel shield is used to display the number of
    seconds since the sketch began running. A 60 LED NeoPixel strip is
    attached to it to make things look neat. Together we're running 100 LEDs.
    If you run the sketch on an Arduino Uno or SparkFun RedBoard with
    the typical Adafruit_NeoPixel library (simply uncomment the first
    three lines of the sketch to do this), and have a stopwatch handy,
    you'll see that the typical library loses time because timer interrupts
    get missed while the Adafruit library is focused on sending color
    data to the NeoPixels. With the XLR8 hardware running separately and
    in parallel, the microcontroller is free to continue processing
    interrupts or doing whatever else it finds fun and interesting and
    time is not lost. (Note, if you've only got a 5x8 shield and not the
    60 LED strip, the sketch can still be run without modifications. The
    color data that would be sent the strip simply falls out onto the floor.
    But the bits are really small and shouldn't cause you any trouble :).
  Connect the strip's power and ground to +5V (not Vin!!!) and ground, 
    and the signal pin and its ground to the shield's dout and another
    ground.

  XLR8 NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  XLR8 NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with XLR8 NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

// To try this sketch with the Adafruit libraries, uncomment the following
//  three lines. It's kind of like the method described in XLR8NeoPixel.h
//  for replacing Adafruit libraries with XLR8, only done in reverse.
//#include <Adafruit_NeoPixel.h>
//#define XLR8NeoPixel Adafruit_NeoPixel
//#define XLR8NEOPIXEL_H
#include <XLR8NeoPixel.h>

// Monochrome bitmap of numbers in 4wide, 5tall font
#define B_0000 (0x00)
#define B_0001 (0x01)
#define B_0010 (0x02)
#define B_0011 (0x03)
#define B_0100 (0x04)
#define B_0101 (0x05)
#define B_0110 (0x06)
#define B_0111 (0x07)
#define B_1000 (0x08)
#define B_1001 (0x09)
#define B_1010 (0x0A)
#define B_1011 (0x0B)
#define B_1100 (0x0C)
#define B_1101 (0x0D)
#define B_1110 (0x0E)
#define B_1111 (0x0F)
const uint8_t digitsBitmap[11][5] ={{B_0110,
                                     B_1001,
                                     B_1001, // zero
                                     B_1001,
                                     B_0110},
                                    {B_0010,
                                     B_0110,
                                     B_0010, // one
                                     B_0010,
                                     B_0111},
                                    {B_0110,
                                     B_1001,
                                     B_0010, // two
                                     B_0100,
                                     B_1111},
                                    {B_1111,
                                     B_0001,
                                     B_0010, // three
                                     B_1001,
                                     B_0110},
                                    {B_1000,
                                     B_1000,
                                     B_1010, // four
                                     B_1111,
                                     B_0010},
                                    {B_1111,
                                     B_1000,
                                     B_1110, // five
                                     B_0001,
                                     B_1110},
                                    {B_0110,
                                     B_1000,
                                     B_1110, // six
                                     B_1001,
                                     B_0110},
                                    {B_1111,
                                     B_0001,
                                     B_0010, // seven
                                     B_0100,
                                     B_0100},
                                    {B_0110,
                                     B_1001,
                                     B_0110, // eight
                                     B_1001,
                                     B_0110},
                                    {B_0110,
                                     B_1001,
                                     B_0111, // nine
                                     B_0001,
                                     B_0110},
                                    {B_1111,
                                     B_1000,
                                     B_1110, // overflow
                                     B_1000,
                                     B_1000}};
const uint8_t rows = 5;
const uint8_t cols = 8;
const uint8_t stripLength = 60;
const uint16_t interval = 16; // Update string every 16ms to cascade nicely down 60 LED strip in 1 second
unsigned long previousMillis = 0;
uint8_t wheelPos = 0; // color wheel
uint32_t backgroundColor = 0;

#define PIN 6
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
XLR8NeoPixel strip = XLR8NeoPixel(rows*cols+stripLength, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  backgroundColor = strip.Color(0,0,0);
  strip.begin();
  //strip.setAllPixels(backgroundColor);  // Adafruit library doesn't have this function yet
  strip.setBrightness(100); // or whatever you like to start with
  previousMillis = millis();
}

void loop() {
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time we updated the LEDs 
    previousMillis = currentMillis;
    // Get current number of seconds, truncated to
    //  bottom two digits, and milliseconds
    unsigned long seconds = currentMillis/1000;
    unsigned long milliseconds = currentMillis - (1000*seconds);
    uint8_t twodigits = seconds % 100;
    uint8_t stripPosition = map(milliseconds,0,999,0,255);
    updateDisplayWithCount(twodigits,stripPosition,wheelPos);
  }
}

void updateDisplayWithCount(uint8_t count, uint8_t stripPos, uint8_t wheelPos) {
  uint8_t topdigit = count/10;
  uint8_t botdigit = count-(10*topdigit);
  if (topdigit > 9) { // Print OF for overflow
    topdigit = 0;
    botdigit = 10;
  }
  // Find the color we want to use
  uint32_t color = Wheel(wheelPos);
  // write pixel memory. 5x8 matrix is first then strip
  uint8_t i,j;
  for (i=0;i<5;i++) { // 5 rows
    for (j=0;j<4;j++) { // columns for top digit
      if (digitsBitmap[topdigit][i] >> (3-j) & 1) {
        strip.setPixelColor(i*cols+j,color);
      } else {
        strip.setPixelColor(i*cols+j,backgroundColor);
      }
    }
    for (j=4;j<8;j++) { // columns for bot digit
      if (digitsBitmap[botdigit][i] >> (7-j) & 1) {
        strip.setPixelColor(i*cols+j,color);
      } else {
        strip.setPixelColor(i*cols+j,backgroundColor);
      }
    }
  }
  // Then the strip. To make the effect look smooth, have
  // 3-4 LEDs lit with the center one brighter and the others
  // somewhat dimmer
  for (i=0;i<stripLength;i++) {
    int16_t difference = (i << 2) - stripPos;
    uint16_t distance = abs(difference);
    // Fade the colors by cutting the RGB values in half for
    //  each unit of distance (which is 1/4 of a pixel) from
    //  stripPos.
    if (distance > 8) {distance = 8;} // No need to look wider than 8
    uint32_t colorFade = color;
    for (j=0; j<distance; j++) {
      colorFade = (colorFade & 0xFEFEFEFE) >> 1;
    }
    strip.setPixelColor(rows*cols+i,colorFade);
  }
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
