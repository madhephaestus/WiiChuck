/*
 * Four Letter Word Generator
 * (C) 2012 Akafugu Corporation
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
 
// LCD example
// Prints Four Letter words to an LCD display connected via TWILCD
// Can also be used with a normal LCD

#include <Wire.h>
#include <FourLetterWord.h>

FourLetterWord flw;

// change to LiquidCrystal.h to use with normal LiquidCrystal library
//#include <LiquidCrystal.h>
#include <TWILiquidCrystal.h>

uint8_t m_addr = 50;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(m_addr);

void setup() { 
#ifdef __AVR_ATmega32U4__
  // On Arduino Leonardo, the board can be connected directly to pins 2-5
  // For other boards, such as UNO where I2C is on different pins, it is
  // neccesary to add wires
  pinMode (5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
#endif
  
  Wire.begin();
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.println("Four Letter Word!");
  
  flw.begin();
}

void loop() {
  lcd.setCursor(6, 1);
  lcd.print(flw.getWord());
  delay(500);
}
