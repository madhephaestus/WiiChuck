/*
 * TWIKeyboard: Arduino Library for Akafugu TWI/I2C Keyboard and LED controller
 * Simple demo: 
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

/*
 * This example dim up all three LEDs on keydown event and dim down them on keyup. 
 */

#include <Wire.h>
#include <TWIKeyboard.h>

#define SLAVE_ADDR 0x10

TWIKeyboard keyb(SLAVE_ADDR);

void setup()
{
  Wire.begin();
  
  Serial.begin(9600);
  Serial.println("Keyboard Test");

  keyb.begin();
  keyb.clearLeds(); // Turn of all leds
}

void loop()
{
  uint8_t keydown = keyb.getKeyDown();
  if (keydown)
  {
    Serial.print("Keydown event: 0b");
    Serial.println(keydown, BIN);
    keyb.dimLed(LED1, 100); // Brigtness of a led is a value between 0-100
    keyb.dimLed(LED2, 100);
    keyb.dimLed(LED3, 100);
  }
  
  uint8_t keyup = keyb.getKeyUp();
  if (keyup)
  {
    Serial.print("Keyup event: 0b");
    Serial.println(keyup, BIN);
    keyb.dimLed(LED1, 0);
    keyb.dimLed(LED2, 0);
    keyb.dimLed(LED3, 0);
  }
}

