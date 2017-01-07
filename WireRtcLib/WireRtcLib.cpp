/*
 * Wire RTC Library: DS1307 and DS3231 driver library
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
 * 13Jan14/wbp - Optimized to reduce code size
 */

/*
 * DS1307 register map
 *  
 *  00h-06h: seconds, minutes, hours, day-of-week, date, month, year (all in BCD)
 *     bit 7 of seconds enables/disables clock
 *     bit 6 of hours toggles 12/24h mode (1 for 12h, 0 for 24h)
 *       when 12h mode is selected bit 5 is high for PM, low for AM
 *  07h: control
 *      bit7: OUT
 *      bit6: 0
 *      bit5: 0
 *      bit4: SQWE
 *      bit3: 0
 *      bit2: 0
 *      bit1: RS0
 *      bit0: RS1
 *  08h-3fh: 56 bytes of SRAM
 *
 * DS3231 register map
 *
 *  00h-06h: seconds, minutes, hours, day-of-week, date, month, year (all in BCD)
 *     bit 7 of seconds enables/disables clock
 *
 */

#include <avr/io.h>

#define TRUE 1
#define FALSE 0

#include "WireRtcLib.h"

#define RTC_ADDR 0x68 // I2C address
#define CH_BIT 7 // clock halt bit

// *16
// >>4
uint8_t WireRtcLib::dec2bcd(uint8_t d)
{
  return ((d/10 * 16) + (d % 10));
}

uint8_t WireRtcLib::bcd2dec(uint8_t b)
{
  return ((b/16 * 10) + (b % 16));
}

uint8_t WireRtcLib::read_byte(uint8_t offset)
{
	Wire.beginTransmission(RTC_ADDR);
	Wire.write(offset);
	Wire.endTransmission();

	Wire.requestFrom(RTC_ADDR, 1);
	
	if (Wire.available()) return Wire.read();
	return 0;
}

void WireRtcLib::write_byte(uint8_t b, uint8_t offset)
{
	Wire.beginTransmission(RTC_ADDR);
	Wire.write(offset);
	Wire.write(b);
	Wire.endTransmission();
}

void WireRtcLib::write_addr(uint8_t addr)
{
	Wire.beginTransmission(RTC_ADDR);
	Wire.write(addr);
	Wire.endTransmission();
}

WireRtcLib::WireRtcLib()
: m_is_ds1307(false)
, m_is_ds3231(false)
{}

void WireRtcLib::begin()
{
	// Attempt autodetection:
	// 1) Read and save temperature register
	// 2) Write a value to temperature register
	// 3) Read back the value
	//   equal to the one written: DS1307, write back saved value and return
	//   different from written:   DS3231
	
	uint8_t temp1 = read_byte(0x11);
	uint8_t temp2 = read_byte(0x12);
	
	write_byte(0xee, 0x11);
	write_byte(0xdd, 0x12);
	
	if (read_byte(0x11) == 0xee && read_byte(0x12) == 0xdd) {
		m_is_ds1307 = true;
		// restore values
		write_byte(temp1, 0x11);
		write_byte(temp2, 0x12);
	}
	else {
		m_is_ds3231 = true;
	}
}

// Autodetection
bool WireRtcLib::isDS1307(void) { return m_is_ds1307; }
bool WireRtcLib::isDS3231(void) { return m_is_ds3231; }

// Autodetection override
void WireRtcLib::setDS1307(void) { m_is_ds1307 = true;   m_is_ds3231 = false; }
void WireRtcLib::setDS3231(void) { m_is_ds1307 = false;  m_is_ds3231 = true;  }

WireRtcLib::tm* WireRtcLib::getTime(void)
{
	uint8_t rtc[9];

	// read 7 bytes starting from register 0
	// sec, min, hour, day-of-week, date, month, year
	write_addr(0); // wake up the RTC
	
	Wire.requestFrom(RTC_ADDR, 7);
	
	for(uint8_t i=0; i<7; i++) {
		if (Wire.available())
			rtc[i] = Wire.read();
		else
			break;
	}
	
	Wire.endTransmission();

	// Clear clock halt bit from read data
	rtc[0] &= ~(_BV(CH_BIT)); // clear bit

	m_tm.sec  = bcd2dec(rtc[0]);
	m_tm.min  = bcd2dec(rtc[1]);
	m_tm.hour = bcd2dec(rtc[2]);
	m_tm.mday = bcd2dec(rtc[4]);
	m_tm.mon  = bcd2dec(rtc[5]); // returns 1-12
	m_tm.year = bcd2dec(rtc[6]); // year 0-99
	m_tm.wday = bcd2dec(rtc[3]); // returns 1-7
	
	if (m_tm.hour == 0) {
		m_tm.twelveHour = 0;
		m_tm.am = 1;
	}
	else if (m_tm.hour < 12) {
		m_tm.twelveHour = m_tm.hour;
		m_tm.am = 1;
	}
	else {
		m_tm.twelveHour = m_tm.hour - 12;
		m_tm.am = 0;
	}
	
	return &m_tm;
}

void WireRtcLib::getTime_s(uint8_t* hour, uint8_t* min, uint8_t* sec)
{
	uint8_t rtc[9];

	// read 7 bytes starting from register 0
	// sec, min, hour, day-of-week, date, month, year
	write_addr(0); // wake up the RTC
	
	Wire.requestFrom(RTC_ADDR, 7);
	
	for(uint8_t i=0; i<7; i++) {
		if (Wire.available())
			rtc[i] = Wire.read();
		else
			break;
	}
	
	Wire.endTransmission();
	
	if (sec)  *sec =  bcd2dec(rtc[0]);
	if (min)  *min =  bcd2dec(rtc[1]);
	if (hour) *hour = bcd2dec(rtc[2]);
}

void WireRtcLib::setTime(WireRtcLib::tm* tm)
{
	Wire.beginTransmission(RTC_ADDR);
	Wire.write((uint8_t)0);

	// clock halt bit is 7th bit of seconds: this is always cleared to start the clock
	Wire.write(dec2bcd(tm->sec)); // seconds
	Wire.write(dec2bcd(tm->min)); // minutes
	Wire.write(dec2bcd(tm->hour)); // hours
	Wire.write(dec2bcd(tm->wday)); // day of week
	Wire.write(dec2bcd(tm->mday)); // day
	Wire.write(dec2bcd(tm->mon)); // month
	Wire.write(dec2bcd(tm->year)); // year
	
	Wire.endTransmission();
}

void WireRtcLib::setTime_s(uint8_t hour, uint8_t min, uint8_t sec)
{
	Wire.beginTransmission(RTC_ADDR);
	Wire.write((uint8_t)0);

	// clock halt bit is 7th bit of seconds: this is always cleared to start the clock
	Wire.write(dec2bcd(sec)); // seconds
	Wire.write(dec2bcd(min)); // minutes
	Wire.write(dec2bcd(hour)); // hours
	
	Wire.endTransmission();
}

// halt/start the clock
// 7th bit of register 0 (second register)
// 0 = clock is running
// 1 = clock is not running
void WireRtcLib::runClock(bool run)
{
  if (m_is_ds3231) return;
  
  uint8_t b = read_byte(0x0);

  if (run)
    b &= ~(_BV(CH_BIT)); // clear bit
  else
    b |= _BV(CH_BIT); // set bit
    
    write_byte(b, 0x0);
}

bool WireRtcLib::isClockRunning(void)
{
  if (m_is_ds3231) return true;
  
  uint8_t b = read_byte(0x0);

  if (b & _BV(CH_BIT)) return false;
  return true;
}

void WireRtcLib::getTemp(int8_t* i, uint8_t* f)
{
	uint8_t msb, lsb;
	
	*i = 0;
	*f = 0;
	
	if (m_is_ds1307) return; // only valid on DS3231
	
	// temp registers are 0x11 and 0x12
	write_addr(0x11);

	Wire.requestFrom(RTC_ADDR, 2);

	if (Wire.available() >= 2) {
		msb = Wire.read(); // integer part (in twos complement)
		lsb = Wire.read(); // fraction part
		
		// integer part in entire byte
		*i = msb;
		// fractional part in top two bits (increments of 0.25)
		*f = (lsb >> 6) * 25;
		
		// float value can be read like so:
		// float temp = ((((short)msb << 8) | (short)lsb) >> 6) / 4.0f;
	}
}

void WireRtcLib::forceTempConversion(uint8_t block)
{
	if (m_is_ds1307) return; // only valid on DS3231

	// read control register (0x0E)
	uint8_t control = read_byte(0x0E);  // read control register
	control |= 0b00100000; // Set CONV bit

	// write new control register value
	write_byte(control, 0x0E);

	if (!block) return;
	
	// Temp conversion is ready when control register becomes 0
	do {
		// Block until CONV is 0
		write_addr(0x0E);
		Wire.requestFrom(RTC_ADDR, 1);
	} while (Wire.available() && (Wire.read() & 0b00100000) != 0);
}

#define DS1307_SRAM_ADDR 0x08

// SRAM: 56 bytes from address 0x08 to 0x3f (DS1307-only)
void WireRtcLib::getSram(uint8_t* data)
{
	// cannot receive 56 bytes in one go, because of the Wire library buffer limit
	// so just receive one at a time for simplicity
  	for(int i=0;i<56;i++)
		data[i] = getSramByte(i);
}

void WireRtcLib::setSram(uint8_t *data)
{
	// cannot send 56 bytes in one go, because of the Wire library buffer limit
	// so just send one at a time for simplicity
  	for(int i=0;i<56;i++)
		setSramByte(data[i], i);
}

uint8_t WireRtcLib::getSramByte(uint8_t offset)
{
	return read_byte(DS1307_SRAM_ADDR + offset);
}

void WireRtcLib::setSramByte(uint8_t b, uint8_t offset)
{
	write_byte(b, DS1307_SRAM_ADDR + offset);
}

void WireRtcLib::SQWEnable(bool enable)
{
	byte offset = 0x0E; // DS3231
	if (m_is_ds1307)
		offset = 0x07;
	uint8_t control = read_byte(offset);  // read control register
	if (m_is_ds1307) {
		if (enable)
			control |=  0b00010000; // set SQWE to 1
		else
			control &= ~0b00010000; // set SQWE to 0
	}
	else { // DS3231
		if (enable) {
			control |=  0b01000000; // set BBSQW to 1
			control &= ~0b00000100; // set INTCN to 0
		}
		else {
			control &= ~0b01000000; // set BBSQW to 0
		}
	}
	// write control back
	write_byte(control, offset);
}

void WireRtcLib::SQWSetFreq(enum RTC_SQW_FREQ freq)
{
	byte offset = 0x0E; // DS3231
	if (m_is_ds1307)
		offset = 0x07;
	uint8_t control = read_byte(offset);  // read control register
	if (m_is_ds1307) {
		control &= ~0b00000011; // Set to 0
		control |= freq; // Set freq bitmask
	}
	else { // DS3231
		control &= ~0b00011000; // Set to 0
		control |= (freq << 4); // Set freq bitmask
	}
	// write control back
	write_byte(control, offset);
}

// DS3231 only
void WireRtcLib::Osc32kHzEnable(bool enable)
{
	if (!m_is_ds3231) return;

	uint8_t status = read_byte(0x0F);  // read status

	if (enable)
		status |= 0b00001000; // set to 1
	else
		status &= ~0b00001000; // Set to 0

	// write status back
	write_byte(status, 0x0F);
}

// ALARM FUNCTIONALITY
//
// On DS1307, SRAM bytes 0 to 2 are used to store the alarm data
// On DS3232, native alarm 1 is used
//

// reset the alarm to 0:00
void WireRtcLib::resetAlarm(void)
{
	if (m_is_ds1307) {
		setSramByte(0, 0); // hour
		setSramByte(0, 1); // minute
		setSramByte(0, 2); // second
	}
	else {
		// writing 0 to bit 7 of all four alarm 1 registers disables alarm
		write_byte(0, 0x07); // second
		write_byte(0, 0x08); // minute
		write_byte(0, 0x09); // hour
		write_byte(0, 0x0a); // day
	}
}

// set the alarm to hour:min:sec
void WireRtcLib::setAlarm_s(uint8_t hour, uint8_t min, uint8_t sec)
{
	if (m_is_ds1307) {
		setSramByte(hour, 0); // hour
		setSramByte(min,  1); // minute
		setSramByte(sec,  2); // sec 
	}
	else {
		/*
		 *  07h: A1M1:0  Alarm 1 seconds
		 *  08h: A1M2:0  Alarm 1 minutes
		 *  09h: A1M3:0  Alarm 1 hour (bit6 is am/pm flag in 12h mode)
		 *  0ah: A1M4:1  Alarm 1 day/date (bit6: 1 for day, 0 for date)
		 *  Sets alarm to fire when hour, minute and second matches
		 */
		write_byte(dec2bcd(sec),  0x07); // second
		write_byte(dec2bcd(min),  0x08); // minute
		write_byte(dec2bcd(hour), 0x09); // hour
		write_byte(0b10000001,         0x0a); // day (upper bit must be set)

		// clear alarm flag
		uint8_t val = read_byte(0x0f);
		write_byte(val & ~0b00000001, 0x0f);
	}
}

void WireRtcLib::setAlarm(WireRtcLib::tm* tm)
{
	if (!tm) return;
	setAlarm_s(tm->hour, tm->min, tm->sec);
}

// get the currently set alarm
void WireRtcLib::getAlarm_s(uint8_t* hour, uint8_t* min, uint8_t* sec)
{
	if (m_is_ds1307) {
		if (hour) *hour = getSramByte(0);
		if (min)  *min  = getSramByte(1);
		if (sec)  *sec  = getSramByte(2);
	}
	else {
		*sec  = bcd2dec(read_byte(0x07) & ~0b10000000);
		*min  = bcd2dec(read_byte(0x08) & ~0b10000000);
		*hour = bcd2dec(read_byte(0x09) & ~0b10000000);
	}
}

WireRtcLib::tm* WireRtcLib::getAlarm()
{
	uint8_t hour, min, sec;

	getAlarm_s(&hour, &min, &sec);
	m_tm.hour = hour;
	m_tm.min = min;
	m_tm.sec = sec;
	return &m_tm;
}

// check whether or not the alarm is going off
// must be polled more than once a second
bool WireRtcLib::checkAlarm(void)
{
	if (m_is_ds1307) {
		uint8_t hour = getSramByte(0);
		uint8_t min  = getSramByte(1);
		uint8_t sec  = getSramByte(2);

		uint8_t cur_hour, cur_min, cur_sec;
		getTime_s(&cur_hour, &cur_min, &cur_sec);
		
		if (cur_hour == hour && cur_min == min && cur_sec == sec)
			return true;
		return false;
	}
	else {
		// Alarm 1 flag (A1F) in bit 0
		uint8_t val = read_byte(0x0f);

		// clear flag when set
		if (val & 1)
			write_byte(val & ~0b00000001, 0x0f);
			
		return val & 1 ? 1 : 0;
	}
}

static const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // january is month 0

void WireRtcLib::breakTime(time_t time, WireRtcLib::tm* tm)
{
// break the given time_t into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  uint8_t year;
  uint8_t month, monthLength;
  unsigned long days;
  
  tm->sec = time % 60;
  time /= 60; // now it is minutes
  tm->min = time % 60;
  time /= 60; // now it is hours
  tm->hour = time % 24;
  time /= 24; // now it is days
  tm->wday = ((time + 4) % 7) + 1;  // Sunday is day 1 
  
  year = 0;  
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm->year = year; // year is offset from 1970 
  
  days -= LEAP_YEAR(year) ? 366 : 365;
  time  -= days; // now it is days in this year, starting at 0
  
  days=0;
  month=0;
  monthLength=0;
  for (month=0; month<12; month++) {
    if (month==1) { // february
      if (LEAP_YEAR(year)) {
        monthLength=29;
      } else {
        monthLength=28;
      }
    } else {
      monthLength = monthDays[month];
    }
    
    if (time >= monthLength) {
      time -= monthLength;
    } else {
        break;
    }
  }
  tm->mon = month + 1;  // jan is month 1  
  tm->mday = time + 1;     // day of month
}

time_t WireRtcLib::makeTime(WireRtcLib::tm* tm){   
// assemble time elements into time_t 
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
  
  int i;
  time_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= tm->year*(SECS_PER_DAY * 365);
  for (i = 0; i < tm->year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < tm->mon; i++) {
    if ( (i == 2) && LEAP_YEAR(tm->year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (tm->mday-1) * SECS_PER_DAY;
  seconds+= tm->hour * SECS_PER_HOUR;
  seconds+= tm->min * SECS_PER_MIN;
  seconds+= tm->sec;
  return seconds; 
}
