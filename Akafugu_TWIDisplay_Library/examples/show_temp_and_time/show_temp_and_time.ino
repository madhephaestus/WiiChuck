/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * clock and temp test: Shows how to use the clock and temperature functionality
 *
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
  disp.begin();
}

void loop()
{
  // for the test, just "fake" the time and temperature
  byte temp = 25;
  static byte hour = 12;
  static byte min = 0;
  static byte sec = 0;
  
  for (int i = 0; i < 10; i++) {
    // show time for 5 seconds then temperature for 5 seconds
    if (i < 5) disp.writeTime(hour, min, sec);
    else disp.writeTemperature(temp, 'C');
    
    // increase time
    sec++;
    if (sec == 60) { sec = 0; min++; }
    if (min == 60) { min = 0; hour++; }
    if (hour == 24) { hour = min = sec = 0; }
    
    delay(1000); 
  }
}
