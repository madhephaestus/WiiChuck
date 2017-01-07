
/*--------------------------------------------------------------------
  Copyright (c) 2015 Alorim Technology.  All right reserved.
  This file is part of the Alorium Technology XLR8 NeoPixel library.
  Written by Matt Weber (linkedin.com/in/mattweberdesign) of 
    Alorium Technology (info@aloriumtech.com) 
    
  The example shows both how to use the XLR8 NeoPixel library and also
    demonstrates the advantage of using the XLR8 board's dedicated
    NeoPixel hardware. Specifically highlighted in this example is
    the ability to change the brightness of NeoPixels down and then
    back up without losing pixel color information along the way.
  Open the serial monitor and press <space>+<enter> to step through
    the displays.
  Adafruit's sweet 5x8 NeoPixel shield is used. The bottom four
    rows of the shield start with four different colors in eight
    different starting brightness levels. The top row, in blue,
    shows how far the brightness of the overall matrix will be brought
    down (e.g 6 of 8 pixels lit means we'll be taking the brightness
    down to 3/4). We're currently bringing the brightness down to half,
    but that's easy to change. The upper right pixel turns on green when
    we're paused back up at full brightness. The second pixel from the
    upper right turns on green when we dropped the brightness in a single
    step as described next.
  First, we drop to that brightness in one step to show what the
    dimmed result should look like. Then we come back up to full
    brightness, also in one step. If you're really good, at this point
    when running on an Arduino Uno or SparkFun RedBoard with the typical
    Adafruit_NeoPixel library (simply uncomment the first three lines
    of the sketch to do this), you might notice that the colors don't
    come back up to exactly the same color they started from. I can't tell
    the difference. Still, with XLR8 the colors do come back to the original.
  Second, is really dramatic. We start by restoring the pixel data to
    the original and then we drop to the target brightness in a smooth fade.
    The effect here is dramatic as truncation errors that accumulate through
    each step along the way cause the Uno/Redboard to completely turn off
    pixels that should still be shining, and to drive mixed colors such
    as orange, yellow, and violet to primary colors red, green, and blue.
    Returning to full brightness, the off pixels remain off and the color
    mix remains lost. Also, when using fadeStepSize=1, the brightness does
    not come back up at all, again because of trucation errors.
    With XLR8 (comment out the first three lines of the sketch), we smoothly
    drop to the same display that was reached with the previous single-step
    drop and we smoothly return to proper full brightness. The errors seen on
    the Uno/Redboard occur because that hardware isn't fast enough to scale the
    brightness on the fly and still maintain the strict timing requirements of
    the NeoPixels, so reducing brightness is done by reducing the values in memory.
    By contrast, XLR8 is fast enough to scale on the fly and the original pixel
    color data can remain in memory undistrubed.
  If you don't have the 5x8 matrix, essentially any NeoPixel matrix/strip/
    ring/whatever can be used.
    
    
    
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

const uint8_t rows = 5;
const uint8_t columns = 8;
const uint8_t length = rows * columns;
const uint16_t interval = 10; // fast enough to go from full to zero in a couple seconds
uint8_t wheelPos = 16; // color wheel. start mostly red, a bit green 
uint8_t minBrightness = 255;
uint8_t maxBrightness = 255;
const uint8_t fadeStepSize = 2; // How fast the fades go down and up
const uint8_t brightnessValues[] = { 128 };
const uint8_t sizeofBrightnessValues = sizeof(brightnessValues)/sizeof(brightnessValues[0]);

#define PIN 6
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
XLR8NeoPixel strip = XLR8NeoPixel(length, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();
  fill(wheelPos);
  wheelPos += columns;
}

void loop() {
  fill(wheelPos); // resets brightness to max
  uint8_t i,j;
  for (i = 0;i < sizeofBrightnessValues; i++) {
    minBrightness = brightnessValues[i];  
    // Top row gets pure color that won't get lost until brightness goes to zero
    //  use row that as thermometer to show current brightness level
    uint8_t thermLength = map(minBrightness,0,255,0,columns);
    for (j=0; j<columns;j++) {
      // Show thermometer in blue since the initial wheel colors will be red/green mix
      if (j<thermLength) {
        strip.setPixelColor(j,0,0,255);
      } else {
        strip.setPixelColor(j,0,0,0);
      } 
    }
    strip.setPixelColor(columns-1,0,255,0); // indicate full brightness
    strip.show();
    while (!Serial.available()); // wait for indication that we're ready to move on
    Serial.read();
    // One step down to min brightness and back up to show what it should look like
    strip.setBrightness(minBrightness);
    strip.setPixelColor(columns-2,0,255,0); // indicate one step drop
    strip.setPixelColor(columns-1,0,0,0); // indicate non-full brightness
    strip.show();
    while (!Serial.available()); // wait for indication that we're ready to move on
    Serial.read();
    strip.setBrightness(maxBrightness);        
    strip.setPixelColor(columns-1,0,255,0); // indicate full brightness
    strip.show();
    while (!Serial.available()); // wait for indication that we're ready to move on
    Serial.read();
    // restore the picture
    fill(wheelPos); // resets brightness to max
    // Smooth drop down to min brightness and back up
    strip.setPixelColor(columns-2,0,0,0); // indicate smooth drop
    strip.setPixelColor(columns-1,0,0,0); // indicate non-full brightness
    fadedown(minBrightness,fadeStepSize);
    while (!Serial.available()); // wait for indication that we're ready to move on
    Serial.read();
    fadeup(maxBrightness,fadeStepSize);
    strip.setPixelColor(columns-1,0,255,0); // indicate full brightness
    strip.show();
    while (!Serial.available()); // wait for indication that we're ready to move on
    Serial.read();
  }
  wheelPos += columns;
}

void fill(uint8_t wheelPos) {
  strip.setBrightness(maxBrightness);
  uint8_t i,j;
  for (i=1;i<rows;i++) {
    for (j=0;j<columns;j++) {
      // work from full brightness on left to 1/3 brightness on right
      // increment from wheelPos in row 1 to wheelPos+24
      strip.setPixelColor(i*columns+j,Wheel(wheelPos+(i-1)*columns,map(j,0,columns-1,255,85)));
    }
    // All the same color
    //strip.setPixelColor(i,Wheel(wheelPos+i));
  }
  strip.show();
}

void fadedown(uint8_t minBrightness,uint8_t step) {
  while (strip.getBrightness() >= (minBrightness+step)) {
    strip.setBrightness(strip.getBrightness() - step);
    strip.show();
    delay(interval);
  }
}
void fadeup(uint8_t maxBrightness, uint8_t step) {
  while (strip.getBrightness() <= (maxBrightness-step)) {
    strip.setBrightness(strip.getBrightness() + step);
    strip.show();
    delay(interval);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos,uint16_t brightScale) {
  WheelPos = 255 - WheelPos;
  uint8_t r,g,b;
  if(WheelPos < 85) {
    r = 255 - WheelPos * 3;
    g = 0;
    b = WheelPos * 3;
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    b = 255 - WheelPos * 3;
    r = 0;
    g = WheelPos * 3;
  } else {
    WheelPos -= 170;
    g = 255 - WheelPos * 3;
    b = 0;
    r = WheelPos * 3;
  }
  r = ((uint16_t)r * (brightScale+1)) >> 8;
  g = ((uint16_t)g * (brightScale+1)) >> 8;
  b = ((uint16_t)b * (brightScale+1)) >> 8;
  return strip.Color(r, g, b);
}
