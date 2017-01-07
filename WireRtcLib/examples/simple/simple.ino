/*
 * Wire RTC Library: DS1307 and DS3231 driver library
 * Simple demo: Show current time and temperature in the serial console
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
#include <WireRtcLib.h>

WireRtcLib rtc;

void setup()
{
  Wire.begin();
  rtc.begin();
  Serial.begin(9600);

  if (rtc.isDS1307())
    Serial.println("Detected DS1307");
  else if (rtc.isDS3231())
    Serial.println("Detected DS3231");
  else
    Serial.println("Autodetect failed");
}

void loop()
{
  WireRtcLib::tm* t = rtc.getTime();
  Serial.print("Time: ");
  Serial.print(t->hour);
  Serial.print(":");
  Serial.print(t->min);
  Serial.print(":");
  Serial.println(t->sec);
  
  if (rtc.isDS3231()) {
    int8_t i;
    uint8_t f;
    rtc.getTemp(&i, &f);
    Serial.print("Temperature: ");
    Serial.print(i);
    Serial.print(".");
    Serial.print(f, 2);
    Serial.println("C");
  }

  delay(1000);
}
