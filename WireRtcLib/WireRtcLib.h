/*
 * Wire RTC Library: DS1307 and DS3231 driver library
 * (C) 2011-2013 Akafugu Corporation
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
 * 06Jan13/wbp - change tm int to uint8_t
 * 07Jan13/wbp - add makeTime, breakTime
 */

#ifndef WIRETRCLIB_H
#define WIRETRCLIB_H

#if defined(ARDUINO) && ARDUINO >= 100
#  include <Arduino.h>
#else
#  include <WProgram.h>
#endif
#include <Wire.h>

#include <avr/io.h>

#define DS1307_SLAVE_ADDR 0b11010000

// leap year calulator expects year argument as years offset from 1970
#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52UL)
#define SECS_YR_2000  (946684800UL) // the time at the start of y2k

typedef unsigned long time_t;

class WireRtcLib {
public:
  class tm {
    public:
    uint8_t sec;      // 0 to 59 (or 60 for occasional rare leap-seconds)
    uint8_t min;      // 0 to 59
    uint8_t hour;     // 0 to 23
    uint8_t mday;     // 1 to 31
    uint8_t mon;      // 1 to 12
    uint8_t year;     // 0-99
    uint8_t wday;     // 1-7
    // 12-hour clock data (set when READING time, ignored when SETTING time)
    bool am; // true for AM, false for PM
    uint8_t twelveHour; // 12 hour clock time
  };

private:
  bool m_is_ds1307;
  bool m_is_ds3231;
  tm m_tm;

public:
  WireRtcLib();

  /** Initialize the RTC and autodetect type (DS1307 or DS3231) */
  void begin();
  
  // Autodetection
  /** Check if the clock chip is a DS1307 */
  bool isDS1307(void);
  /** Check if the clock chip is a DS3231 */
  bool isDS3231(void);

  // Autodetection override
  /** Force set the clock chip type to DS1307 */
  void setDS1307(void);
  /** Force set the clock chip type to DS3231 */
  void setDS3231(void);

  // Get/set time
  /* Gets the current time and date from the chip
   * @return WireRtcLib::tm structure filled with time data. This data is statically allocated by the library, and should not be deleted
   */
  WireRtcLib::tm* getTime(void);

  /** Gets the current time from the chip, simplified version
   * @param hour pointer to value to store the current hour
   * @param min pointer to value to store the current minutes
   * @param sec pointer to value to store the curent seconds
   */
  void getTime_s(uint8_t* hour, uint8_t* min, uint8_t* sec);

  /** Set the time
   * @param tm Pointer to a WireRtcLib::tm structure filled with the time data to set
   *           Note that
   */
  void setTime(WireRtcLib::tm* tm);

  /* Set the time, simplified version
   * @param hour The hour to set
   * @param min  The minutes to set
   * @param sec  The seconds to set
   */
  void setTime_s(uint8_t hour, uint8_t min, uint8_t sec);

  // start/stop clock running (DS1307 only)
  void runClock(bool run);
  bool isClockRunning(void);

  // Temperature (DS3231 only)
  void getTemp(int8_t* i, uint8_t* f);
  void forceTempConversion(uint8_t block);

  // SRAM read/write (DS1307 only)
  void getSram(uint8_t* data);
  void setSram(uint8_t *data);
  uint8_t getSramByte(uint8_t offset);
  void setSramByte(uint8_t b, uint8_t offset);

  // Auxillary functions
  enum RTC_SQW_FREQ { FREQ_1 = 0, FREQ_1024, FREQ_4096, FREQ_8192 };

  void SQWEnable(bool enable);
  void SQWSetFreq(enum RTC_SQW_FREQ freq);
  void Osc32kHzEnable(bool enable);

  // Alarm functionality
  void resetAlarm(void);
  void setAlarm(WireRtcLib::tm* tm);
  void setAlarm_s(uint8_t hour, uint8_t min, uint8_t sec);
  WireRtcLib::tm* getAlarm();
  void getAlarm_s(uint8_t* hour, uint8_t* min, uint8_t* sec);
  bool checkAlarm(void);
	
	// Conversion utilities
	void breakTime(time_t time, WireRtcLib::tm* tm);  // break time_t into elements
	time_t makeTime(WireRtcLib::tm* tm);  // convert time elements into time_t

private:
  uint8_t dec2bcd(uint8_t d);
  uint8_t bcd2dec(uint8_t b);
  uint8_t read_byte(uint8_t offset);
  void write_byte(uint8_t b, uint8_t offset);
  void write_addr(uint8_t addr);
};
	
#endif // WIRETRCLIB_H

