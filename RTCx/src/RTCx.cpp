#include <avr/pgmspace.h>
#include <Wire.h>
#include "RTCx.h"

#define SECS_PER_DAY 86400L
#define SECS_PER_4_YEARS (SECS_PER_DAY * (366L + 365L + 365L + 365L))

const uint8_t RTCx::DS1307Address = 0x68;
const uint8_t RTCx::MCP7941xAddress = 0x6F;
const uint8_t RTCx::MCP7941xEepromAddress = 0x57;
  

RTCx rtc;

bool RTCx::isLeapYear(uint16_t year)
{
  // Since year is > 1900 and < 2100 can use simple method
  return (year % 4) == 0;
}

uint8_t RTCx::daysInMonth(uint16_t year, uint8_t month)
{
  uint8_t daysInMonth[13] =
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 && isLeapYear(year))
    return 29;
  return daysInMonth[month];
}

uint16_t RTCx::dayOfYear(uint16_t year, uint8_t month, uint8_t day)
{
  uint16_t doy = 0;
  uint8_t m = 1;
  while (m < month)
    doy += daysInMonth(year, m++);
  doy += day;
  return doy;
}

RTCx::time_t RTCx::mktime(struct tm *tm)
{
  // Normalise the time
  tm->tm_min += (tm->tm_sec / 60);
  tm->tm_sec = (tm->tm_sec % 60);
  if (tm->tm_sec < 0) {
    tm->tm_sec += 60;
    --(tm->tm_min);
  }

  tm->tm_hour += (tm->tm_min / 60);
  tm->tm_min = (tm->tm_min % 60);
  if (tm->tm_min < 0) {
    tm->tm_min += 60;
    --(tm->tm_hour);
  }

  tm->tm_mday += (tm->tm_hour / 24);
  tm->tm_hour = (tm->tm_hour % 24);
  if (tm->tm_hour < 0) {
    tm->tm_hour += 24;
    --(tm->tm_mday);
  }

  if (tm->tm_mon < 0 || tm->tm_mon > 11 || tm->tm_mday < 1)
    return -1;

  // Normalise the date
  while (true) {
    uint8_t dim = daysInMonth(tm->tm_year+1900, tm->tm_mon+1);
    if (tm->tm_mday > dim) {
      tm->tm_mday -= dim;
      ++(tm->tm_mon);
      if (tm->tm_mon == 12) {
	tm->tm_mon = 0;
	++(tm->tm_year);
      }
      continue;
    }
    
    if (tm->tm_mday < 1) {
      --(tm->tm_mon);
      if (tm->tm_mon == -1) {
	tm->tm_mon = 11;
	--(tm->tm_year);
      }

      uint8_t dim = daysInMonth(tm->tm_year+1900, tm->tm_mon+1);
      tm->tm_mday += dim;
      continue;
    }
    break;      
  }

  // Compute day of year
  tm->tm_yday = dayOfYear(tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday) - 1;

  uint8_t yearsSinceEpoch = tm->tm_year + 1900  - RTCX_EPOCH; 
  time_t t = (yearsSinceEpoch * 365 * SECS_PER_DAY) // Whole years, leap days excluded
    + ((yearsSinceEpoch / 4) * SECS_PER_DAY)  // Leap days in whole 4 year period
    // Leap days in partial 4 year period. Count only if in last year
    + ((yearsSinceEpoch % 4) == 3 ? SECS_PER_DAY : 0L) 
    + (tm->tm_yday * SECS_PER_DAY)           // Whole days in current year
    + (tm->tm_hour * 3600L)
    + (tm->tm_min * (uint16_t)60)
    + tm->tm_sec;
  
  // Compute day of week
  uint32_t daysSinceEpoch = (t / SECS_PER_DAY);
  tm->tm_wday = (daysSinceEpoch + 4) % 7; // 1970-01-01 was Thursday (day 4)
  return t;
}

struct RTCx::tm *RTCx::gmtime_r(const time_t *timep, struct tm *result)
{
  time_t t = *timep;
  // Find multiples of 4 years since epoch
  int8_t fourYears = (int8_t)(t / SECS_PER_4_YEARS);
  if (t < 0)
    --fourYears; // Now remaining time will be positive and must add
  result->tm_year = (fourYears * 4) + (RTCX_EPOCH - 1900); // years since 1900
  t -= (fourYears * SECS_PER_4_YEARS);

  // Split t into seconds in day and days remaining.
  int16_t days = (t / SECS_PER_DAY); // Fits into 16 bits
  time_t partialDay_s = (t % SECS_PER_DAY); // seconds
  
  // Calculate hours, minutes and seconds next so that the rest of the
  // calculations can be made in days using 16 bit arithmetic.
  result->tm_sec = (partialDay_s % 60);
  int16_t partialDay_m = partialDay_s / 60; // minutes
  result->tm_min = (partialDay_m % 60);
  result->tm_hour = partialDay_m / 60; 

  if (days >= (365 + 365 + 366)) {
    // Third year in a four year block is a leap year
    days -= (365 + 365 + 366);
    result->tm_year += 3;
  }
  else
    while (days >= 365) {
      days -= 365;
      ++(result->tm_year);
    }

  // days is now the day of year
  result->tm_yday = days;
  result->tm_mon = 0;
  result->tm_mday = 1 + days;

  while (true) {
    uint8_t dim = daysInMonth(result->tm_year+1900, result->tm_mon+1);
    if (result->tm_mday > dim) {
      result->tm_mday -= dim;
      ++(result->tm_mon);
    }
    else
      break;
  }

  // Compute day of week
  uint16_t daysSinceEpoch = (*timep / 86400L);
  result->tm_wday = (daysSinceEpoch + 4) % 7; // 1970-01-01 was Thursday (day 4)
  return result;
}


RTCx::RTCx(void) : address(RTCx::DS1307Address), device(RTCx::DS1307)
{
  ;
}

// Determine probable device from address
RTCx::RTCx(uint8_t a)
  : address(a)
{
  switch (address) {
  case RTCx::DS1307Address:
    device = RTCx::DS1307;
    break;
  case RTCx::MCP7941xAddress:
    device = RTCx::MCP7941x;
    break;
  default:
    // Guess DS1307
    device = RTCx::DS1307;
    break;
  }
}

RTCx::RTCx(uint8_t a, device_t d)
  : address(a), device(d)
{
  ;
}

/* Autoprobe for a real-time clock, given a list of addresses to
 * check. Set the register address to zero before attempting to read a
 * byte; the MCP7941x will return a NACK if the register address is
 * not within its valid range.
 */
bool RTCx::autoprobe(uint8_t *addressList, uint8_t len) 
{
  for (uint8_t i = 0; i < len; ++i) {
    // Ensure register address is valid
    Wire.beginTransmission(addressList[i]);
    Wire.write(uint8_t(0));
    Wire.endTransmission();

    // Try reading a byte
    Wire.requestFrom(addressList[i], (uint8_t)1);
    if (Wire.available()) {
      *this = RTCx(addressList[i]);
      return true;
    }
  }
  return false;
}

void RTCx::stopClock(void) const
{
  uint8_t s = readData(0);
  switch (device) {
  case DS1307:
    s |= 0x80;
    break;
  case MCP7941x:
    s &= 0x7f;
    break;
  }
  writeData(0, s);
}

// Start the clock, if it isn't running already
void RTCx::startClock(void) const
{
  uint8_t s = readData(0);
  uint8_t s2 = s;
  switch (device) {
  case MCP7941x:
    s2 |= 0x80; // Enable start bit
    break;
  case DS1307:
    s2 &= 0x7f; // Clear clock halt
    break;
  }

  if (s != s2)
    writeData(0, s2);
}


/* Read a time from the clock. The same function is also used to read
 * the alarms as the register layout is essentially identical but with
 * week day and year omitted.
 */
bool RTCx::readClock(struct tm *tm, timeFunc_t func) const
{
  // Find which register to read from
  uint8_t sz = 0;
  uint8_t reg = getRegister(func, sz);
  
  if (sz == 0)
    return false; // not supported

  if (device == MCP7941x &&
      (func == TIME_POWER_FAILED || func == TIME_POWER_RESTORED))
    return readTimeSaver(tm, sz);

  while (true) {
    // Reset the register pointer
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(address, sz);
    tm->tm_sec = bcdToDec(Wire.read() & 0x7f);
    tm->tm_min = bcdToDec(Wire.read() & 0x7f);
    uint8_t h = Wire.read();
    if (h & 0x40) {
      // Twelve hour mode
      tm->tm_hour = bcdToDec(h & 0x1f);
      if (h & 0x20)
	tm->tm_hour += 12; // Seems notation for AM/PM is user-defined
    }
    else 
      tm->tm_hour = bcdToDec(h & 0x3f);
    tm->tm_wday = (Wire.read() & 0x03) - 1; // Clock uses [1..7]
    tm->tm_mday = bcdToDec(Wire.read() & 0x3f);
    tm->tm_mon = bcdToDec(Wire.read() & 0x1f) - 1; // Clock uses [1..12]
    if (sz == 7)
      tm->tm_year = bcdToDec(Wire.read()) + 100; // Assume 21st century
    else
      tm->tm_year = (RTCX_EPOCH - 1900);
    tm->tm_yday = -1;
    Wire.endTransmission();

    if ((func != TIME) || (tm->tm_sec == bcdToDec(readData(0) & 0x7f)))
      break;
  }
  return true;
}


bool RTCx::setClock(const struct tm *tm, timeFunc_t func) const
{
  // Find which register to read from
  uint8_t sz = 0;
  uint8_t reg = getRegister(func, sz);
  
  if (sz == 0)
    return false; // not supported

  if (func == TIME)
    stopClock();

  uint8_t osconEtc = 0;
  if (device == MCP7941x)
    // Preserve OSCON, VBAT, VBATEN on MCP7941x
    osconEtc = readData((uint8_t)0x03) & 0x38;
  
  // Write everything *except* the second
  Wire.beginTransmission(address);
  Wire.write(reg + 1);
  Wire.write(decToBcd(tm->tm_min));
  Wire.write(decToBcd(tm->tm_hour)); // Forces 24h mode
  Wire.write(decToBcd(tm->tm_wday + 1) | osconEtc);
  Wire.write(decToBcd(tm->tm_mday));
  Wire.write(decToBcd(tm->tm_mon + 1)); // leap year read-only on MCP7941x
  Wire.write(decToBcd(tm->tm_year % 100));
  Wire.endTransmission();

  if (func == TIME)
    startClock(decToBcd(tm->tm_sec));
  else
    writeData(reg, decToBcd(tm->tm_sec));
  return true;
}


bool RTCx::setSQW(freq_t f) const
{
  switch (device) {
  case DS1307:
    if (f <= freq32768Hz) {
      writeData(0x07, uint8_t(0x10) | (f & uint8_t(0x03)));
      return true;
    }
    break;
  case MCP7941x:
    uint8_t ctrl = readData(0x07) & uint8_t(0xf8);
    ctrl |= f;
    ctrl |=0x40; // Enable square wave
    writeData(0x07, ctrl);
    return true;
  }
  return false;
}


void RTCx::enableBatteryBackup(bool enable) const
{
  if (device == MCP7941x) {
    stopClock();
    uint8_t d = readData((uint8_t)0x03);
    if (enable)
      d |= 0x08;
    else
      d &= 0xf7;
    writeData((uint8_t)0x03, d);
    uint8_t s = readData((uint8_t)0);
    startClock(s);
  }
}

void RTCx::clearVBAT(void) const
{
  if (device == MCP7941x) {
    stopClock();
    uint8_t d = readData((uint8_t)0x03);
    d &= 0xef;
    writeData((uint8_t)0x03, d);
    uint8_t s = readData((uint8_t)0);
    startClock(s);
  }
}


int8_t RTCx::getCalibration(void) const
{
  if (device == MCP7941x) {
    // Convert from signed magnitude to two's complement.
    uint8_t d = readData(0x08);
    int8_t r = d & 0x7Fu;
    return (d & 0x80u ? -r : r); 
  }
  else
    return 0;
}


bool RTCx::setCalibration(int8_t cal) const
{
  if (device == MCP7941x) {
    // Convert two's complement to signed magnitude.
    if (cal == -128)
      cal = -127; // Out of range, use next best value.
    uint8_t d;
    if (cal < 0)
      d = 0x80u | (uint8_t)(-cal);
    else
      d = cal;
    writeData(0x08, d);
    return true;
  }
  return false;
}


uint8_t RTCx::bcdToDec(uint8_t b)
{
  return ( ((b >> 4)*10) + (b%16) );
}


uint8_t RTCx::decToBcd(uint8_t b)
{
  return ( ((b/10) << 4) + (b%10) );
}


void RTCx::startClock(uint8_t bcdSec) const
{
  switch (device) {
  case MCP7941x:
    bcdSec |= 0x80; // Enable start bit
    break;
  case DS1307:
    bcdSec &= 0x7f;
    break;
  }
  writeData(0, bcdSec);
}


uint8_t RTCx::readData(uint8_t reg) const
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, uint8_t(1));
  uint8_t value = Wire.read();
  Wire.endTransmission();
  return value;
}


void RTCx::writeData(uint8_t reg, uint8_t value) const
{
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}


uint8_t RTCx::getRegister(timeFunc_t func, uint8_t &sz) const
{
  const uint8_t regTable[2][5] = {
    {0, 0xff, 0xff, 0xff, 0xff}, // DS1307
    {0, 0x0a, 0x11, 0x18, 0x1C}, // MCP7941x
  };
  const uint8_t szTable[2][5] = {
    {7, 0, 0, 0, 0}, // DS1307
    {7, 6, 6, 4, 4}, // MCP7941x
  };
  sz = szTable[device][func];
  return regTable[device][func];
  //return pgm_read_byte((regTable[device][func]));
}


bool RTCx::readTimeSaver(struct tm *tm, uint8_t sz) const
{
  Wire.requestFrom(address, sz);
  tm->tm_sec = 0;
  tm->tm_min = bcdToDec(Wire.read() & 0x7f);
  tm->tm_hour = bcdToDec(Wire.read() & 0x3f);
  tm->tm_wday = 0;
  tm->tm_mday = bcdToDec(Wire.read() & 0x3f);
  uint8_t wdayMonth = Wire.read();
  tm->tm_mon = bcdToDec(wdayMonth & 0x1f) - 1; // Clock uses [1..12]
  tm->tm_wday = (wdayMonth >> 5) - 1; // Clock uses [1..7]
  tm->tm_year = (RTCX_EPOCH - 1900); // not stored
  tm->tm_yday = -1;
  Wire.endTransmission();
  return true;
}



  





