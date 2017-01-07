/*
 * TWIKeyboard: Arduino Library for Akafugu TWI/I2C Keyboard and LED controller
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

#ifndef TWIKEYBOARD_H
#define TWIKEYBOARD_H

#if defined(ARDUINO) && ARDUINO >= 100
#  include <Arduino.h>
#else
#  include <WProgram.h>
#endif
#include <Wire.h>

#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define BUTTON4 3
#define BUTTON5 4
#define BUTTON6 5
#define BUTTON7 6

#define LED1 0
#define LED2 1
#define LED3 2
#define LED4 3
#define LED5 4

#define KEYREPEAT_STOP 0
#define KEYREPEAT_SLOW 1
#define KEYREPEAT_MEDIUMSLOW 2
#define KEYREPEAT_MEDIUM 3
#define KEYREPEAT_MEDIUMFAST 4
#define KEYREPEAT_FAST 5

class TWIKeyboard
{
public:
  TWIKeyboard(int addr);

  /** Reset everything to power-on state. Should be called in the setup function */
  void begin();

  /** Change the TWI address of the keyboard. Once the address has been changed, power must be toggled before it will respond to the new address
   * @param new_addr The new TWI address to set
  */
  void changeAddress(int new_addr);
  
  /** Turns off all leds */
  void clearLeds();
  
  /** Set a Led to a specified brightness
   * @param no Number of the Led to change brighness for. Use LED1, LED2 or LED3 for the built-in Leds, and LED4 or LED5 for the external Leds.
   * @param brightness The desired brightness from 0 (off) to 100 (max)
   *
   * If the Led is currently pulsing, the pulsing will be turned off.
   */
  void setLed(uint8_t no, uint8_t brightness);
  
  /** Set a Led to pulse
   * @param no Number of the Led to set to pulse. LED1 to LED5.
   * @param on Set to true to turn pulsing on, or false to turn off. When turning dimming off, the Led will be turned off.
   */
  void pulseLed(uint8_t no, bool on);

  /** Dim or increase brightness of a Led
   * @param no Number of the led to dim or increase brightness off. LED1 to LED5.
   * @param brightness The desired brightness. If the Led is currently above this brightness, it will be gradually dimmed. If it is less, brightness will gradually increase.
   *
   * If the Led is currently pulsing, the brightness will be dimmed or boosted until it matches the current value, and then pulsing will be turned off. 
   */
  void dimLed(uint8_t no, uint8_t brightness);

  /** Set whether or not a key has key repeat funtionalty when being held down
   * @param button The button to change, BUTTON1 to BUTTON7
   * @mode mode Sets the desired key repeat state
   *   KEYREPEAT_STOP No key repeat
   *   KEYREPEAT_SLOW Slow key repeat rate
   *   KEYREPEAT_MEDIUMSLOW Medium-slow repeat rate
   *   KEYREPEAT_MEDIUM Medium repeat rate
   *   KEYREPEAT_MEDIUMFAST Medium-fast repeat rate
   *   KEYREPEAT_FAST Fast repeat rate
   */
  void setKeyRepeat(uint8_t button, uint8_t mode);
  
  /** Get key up event state for all keys */
  uint8_t getKeyUp();
  
  /** Get key down event state for all keys*/
  uint8_t getKeyDown();
private:
  int m_addr;
};

#endif // TWIKEYBOARD_H
