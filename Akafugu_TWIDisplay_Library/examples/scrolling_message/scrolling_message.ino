/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * (C) 2011-12 Akafugu Corporation
 *
 * Scrolling message
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
  disp.setScrollMode();
  disp.clear();
}

void loop()
{
  char message[] = "This is a long message    ";
  
  for (int i = 0; i < strlen(message); i++) {
    disp.print(message[i]);
    delay(250);
  }
  
  delay(500);
}
