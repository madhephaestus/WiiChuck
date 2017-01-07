/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * (C) 2011-12 Akafugu Corporation
 *
 * Segments: Shows how to show custom data on the display
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

#include <Wire.h>
#include <TWIDisplay.h>

#define SLAVE_ADDR 18

TWIDisplay disp(SLAVE_ADDR);

void setup()
{
  Wire.begin();
  
  disp.begin();
  disp.setRotateMode();
  disp.clear();
  disp.setBrightness(255);
}

void loop()
{
  uint8_t segments_a = 1;
  uint8_t segments_b = 0b00100000;
  
  while (1) {
    disp.writeSegments(segments_a);
    disp.writeSegments(segments_b);
    disp.writeSegments(segments_a);
    disp.writeSegments(segments_b);

    delay(70);
    
    // rotate segments_a towards the left
    // this will create a counterclockwise spinning animation
    segments_a <<= 1;
  
    if (segments_a == 0b01000000)
      segments_a = 1;

    // rotate segments_b towards the right
    // this will create a clockwise spinning animation
    segments_b >>= 1;
  
    if (segments_b == 0)
      segments_b = 0b00100000;
  }  
}
