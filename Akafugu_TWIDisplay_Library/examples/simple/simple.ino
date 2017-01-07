/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * Simple demo: Count to 9999
 * (C) 2011 Akafugu Corporation
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
  
  Serial.begin(9600);
  Serial.println("Serial Display Test");

  disp.begin();
  disp.setRotateMode();
  disp.clear();
  disp.setBrightness(255);
}

void loop()
{
  // count to 1000
  for (int i = 0; i <= 9999; i++) {
    disp.writeInt(i);
    delay(100);
  }  
}

