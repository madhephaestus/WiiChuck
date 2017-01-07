/*
 * Four Letter Word Generator
 * (C) 2012-13 Akafugu Corporation
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

// SIMPLE example - Prints Four Letter Words to serial
// The default mode of the four letter word generator is censored mode
// where expletives will be skipped

#include <Wire.h>
#include <FourLetterWord.h>

FourLetterWord flw;

void setup()
{


  Serial.begin(9600);
  Wire.begin();
  flw.begin();
  
  if (flw.hasEeprom())
    Serial.println("EEPROM found");
  else
    Serial.println("No EEPROM");
}

void loop()
{
  char* temp = flw.getWord();
  Serial.println(temp);  
  delay(500);
}
