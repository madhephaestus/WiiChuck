/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * (C) 2011-12 Akafugu Corporation
 *
 * Segments_16: Shows how to show custom data on the display (for TWIDisplay LCD 8-digit)
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

// This example is meant for TWIDisplay LCD 8-digit
// It will not run on TWIDisplay LED 4-digit
#include <Wire.h>
#include <TWIDisplay.h>

#define SLAVE_ADDR 18

TWIDisplay disp(SLAVE_ADDR);

void setup()
{
  Wire.begin();

  disp.begin();

  int rev = disp.getDigits();
  
  disp.setRotateMode();
  disp.clear();
  disp.setBrightness(255);
}

void loop()
{
  uint16_t segments_1 = 0b10000000;
  uint16_t segments_2 = 0b10000000000000;
  
  for (uint8_t i = 0; i < 7; i++) {
    disp.writeSegments16(segments_1);
    disp.writeSegments16(segments_2);
    disp.writeSegments16(segments_1);
    disp.writeSegments16(segments_2);
    disp.writeSegments16(segments_1);
    disp.writeSegments16(segments_2);
    disp.writeSegments16(segments_1);
    disp.writeSegments16(segments_2);

    // shift bytes to make segments spin towards the right and left
    segments_1 <<= 1;    
    segments_2 >>= 1;    
    delay(70);
  }  
}