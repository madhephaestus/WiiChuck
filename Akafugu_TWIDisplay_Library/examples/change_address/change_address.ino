/*
 * TWIDisplay: Arduino Library for Akafugu TWI/I2C serial displays
 * (C) 2011-12 Akafugu Corporation
 *
 * Address change: Shows how to change the slave address of a device
 *
 * After changing the address, you must reset or toggle power on your device.
 * To see the current address of a device at any time, plug in VCC and GND only,
 * and wait for several seconds. The address will be shown in the display.
 *
 * The address will be displayed as a decimal number, prefixed by the letter A,
 * for instance A018, which is the default address (0x12 in HEX)
 *
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

// current address
#define CURRENT_ADDR 18
// new address
#define NEW_ADDR 19

TWIDisplay disp(CURRENT_ADDR);

void setup()
{
  Wire.begin();
  disp.begin();
  disp.setRotateMode();
  disp.clear();

  disp.writeStr("----");
  disp.changeAddress(NEW_ADDR);
}

void loop()
{
}
