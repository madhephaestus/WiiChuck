/*
 * Wire RTC Library: DS1307 and DS3231 driver library
 * Alarm demo: Sets the alarm one minute into the future and waits for it to expire
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
    
  Serial.print("Current Time: ");
  Serial.print(t->hour);
  Serial.print(":");
  Serial.print(t->min);
  Serial.print(":");
  Serial.println(t->sec);
  
  int a_hour = t->hour;
  int a_min  = t->min + 1;
  int a_sec  = t->sec;

  if (a_min == 60) {
    a_min == 0;
    a_hour++;
  }
  
  if (a_hour == 24) {
    a_hour = 0;
    a_min = 1;
  }

  Serial.print("Setting alarm to: ");
  Serial.print(a_hour);
  Serial.print(":");
  Serial.print(a_min);
  Serial.print(":");
  Serial.println(a_sec);
  
  rtc.setAlarm(a_hour, a_min, a_sec);
  
  while (1) {
    if (rtc.checkAlarm()) {
      Serial.println("ALARM!");
      break;
    }
    
    delay(200);
  }
}

