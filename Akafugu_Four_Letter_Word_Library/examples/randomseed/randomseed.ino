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

// RANDOMSEED example - Shows how to change the random seed to get a different sequence
// of four letter words each time the program runs

#include <Wire.h>
#include <FourLetterWord.h>

FourLetterWord flw;

void setup()
{
#ifdef __AVR_ATmega32U4__
  // For Leonardo, wait until serial port is opened, otherwise random seed will not be printed
  while  (!Serial) {}
#endif
  Serial.begin(9600);
  Wire.begin();
  
  // This will only work if A1 is not connected: Reading an analog value from an unconnected
  // (floating) input will return a random value suitable for use as the random number seed
  pinMode(A1, INPUT);
  int seed = analogRead(A1);
  
  Serial.print("Random seed: ");
  Serial.println(seed);
  
  flw.begin(seed);
  
  if (flw.hasEeprom())
    Serial.println("EEPROM found");
  else
    Serial.println("No EEPROM");

  // Changing the random number seed will only change the sequence of words, not
  // the starting point. To randomize the starting point too, call getWord a few
  // times and discard the values
  for (uint8_t i = 0; i < 5; i++)
    flw.getWord();
}

void loop()
{
  char* temp = flw.getWord();
  Serial.println(temp);  
  delay(500);
}