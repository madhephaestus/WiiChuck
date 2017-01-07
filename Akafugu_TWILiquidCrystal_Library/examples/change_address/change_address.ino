/*
 * TWILiquidCrystal: Arduino Library for Akafugu TWI/I2C LCD backpack
 * Address change: Shows how to change the slave address of a device
 *
 * After changing the address, you must reset or toggle power on your device.
 * To see the current address of a device at any time, plug in VCC and GND only,
 * and wait for several seconds. The address will be shown in the display.
 *
 * The address will be displayed as a decimal number.
 *
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

#include <Wire.h>
#include "TWILiquidCrystal.h"

// current address
#define CURRENT_ADDR 50
// new address
#define NEW_ADDR 51

LiquidCrystal lcd(CURRENT_ADDR);

void setup()
{
  Wire.begin();
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);

  lcd.print("Old:");
  lcd.print(CURRENT_ADDR);
  lcd.print(", New:");
  lcd.println(NEW_ADDR);
  
  lcd.changeAddress(NEW_ADDR);
}

void loop()
{
}
