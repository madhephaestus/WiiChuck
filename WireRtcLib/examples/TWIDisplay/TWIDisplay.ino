/*
 * Wire RTC Library: DS1307 and DS3231 driver library
 * Display demo: Show current time and temperature (if available) using the Akafugu 7-seg 4-digit TWI display
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
#include "WireRtcLib.h"

WireRtcLib rtc;

#define SLAVE_ADDR 0x12 // 7-seg

TWIDisplay disp(SLAVE_ADDR);

void setup()
{
  Wire.begin();
  rtc.begin();
  disp.setBrightness(255);
  disp.clear();

  if (rtc.isDS1307())
    disp.print(1307);
  else if (rtc.isDS3231())
    disp.print(3231);
  else
    disp.print("----"); // autodetection failed
    
  delay(2000);
}

void loop()
{
  for (int i = 0; i < 40; i++) {
    WireRtcLib::tm* t = rtc.getTime();

    if (rtc.isDS1307() || i < 20) {
      disp.writeTime(t->hour, t->min, t->sec);
    }
    else {
      int8_t i;
      uint8_t f;
      rtc.getTemp(&i, &f);
      disp.writeTemperature(i, f, 'C');
    }
    
    delay(200);
  }
}

