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
 * This example turns on all three LEDs, sets keyrepeat on all buttons and read status 
 * for keyup and keydown events.
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
  keyb.setLed(LED1, 100);
  keyb.setLed(LED2, 100);
  keyb.setLed(LED3, 100);
  keyb.setKeyRepeat(BUTTON1, KEYREPEAT_SLOW);
  keyb.setKeyRepeat(BUTTON2, KEYREPEAT_MEDIUMSLOW);
  keyb.setKeyRepeat(BUTTON3, KEYREPEAT_MEDIUM);
  keyb.setKeyRepeat(BUTTON4, KEYREPEAT_MEDIUMFAST);
  keyb.setKeyRepeat(BUTTON5, KEYREPEAT_FAST);
}

void loop()
{
  uint8_t keydown = keyb.getKeyDown();
  if (keydown)
  {
    Serial.print("Keydown event: 0b");
    Serial.println(keydown, BIN);
  }
  
  uint8_t keyup = keyb.getKeyUp();
  if (keyup)
  {
    Serial.print("Keyup event: 0b");
    Serial.println(keyup, BIN);
  }
}

