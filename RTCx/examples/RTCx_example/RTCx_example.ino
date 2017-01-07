/*
 * Demonstrate the RTCx library.
 *
 * This sketch demonstrates the basic functionality of the RTCx
 * library. It can also be used to adjust and check the frequency
 * calibration settings on MCP7941x devices.
 *
 * On start-up the autoprobe feature is used to search the I2C bus to
 * find the first available real-time clock. The sketch indicates
 * which clock type (if any) has been found. If the clock is an
 * MCP7941x device the current calibration setting is printed.
 *
 * The clock can be set by writing the current unixtime (i.e., seconds
 * since 1970-01-01 00:00:00Z) prefixed by the character 'T' to
 * Serial. The string must be terminated by a NULL character, line
 * feed, or carriage return. E.g., "T1343779200" sets the clock to
 * 2012-08-01 00:00:00Z".
 *
 * The clock error, measured in seconds, can be found in a similar way
 * by sending the current unix time, prefixed by the character 'C',
 * e.g., "C1343779200".
 *
 * It is possible to set the SQW output frequency by sending "M0" (1Hz
 * output), "M1" (4096Hz output), "M2" (8192Hz output) or "M3"
 * (32768Hz output). If the clock is a MCP7941x device then sending
 * "M4" will enable the calibration mode and output a 64Hz signal
 * which is continously compensated with the frequency calibration
 * setting, thereby enabling the correct calibration value to be
 * found.
 */

#include <Wire.h>
#include <RTCx.h>

void printTm(Stream &str, struct RTCx::tm *tm)
{
  str.print(tm->tm_year + 1900);
  str.print('-');
  str.print(tm->tm_mon + 1);
  str.print('-');
  str.print(tm->tm_mday);
  str.print('T');
  str.print(tm->tm_hour);
  str.print(':');
  str.print(tm->tm_min);
  str.print(':');
  str.print(tm->tm_sec);
  str.print(" yday=");
  str.print(tm->tm_yday);
  str.print(" wday=");
  str.println(tm->tm_wday);
}

void setup(void)
{
  Serial.begin(9600);
  Wire.begin();
  // The address used by the DS1307 is also used by other devices (eg
  // MCP3424 ADC). Test for a MCP7941x device first.
  uint8_t addressList[] = {RTCx::MCP7941xAddress,
			   RTCx::DS1307Address};

  // Autoprobe to find a real-time clock.
  if (rtc.autoprobe(addressList, sizeof(addressList))) {
    // Found something, hopefully a clock.
    Serial.print("Autoprobe found ");
    switch (rtc.getDevice()) {
      case RTCx::DS1307:
	Serial.print("DS1307");
	break;
    case RTCx::MCP7941x:
      Serial.print("MCP7941x");
      break;
    default:
      // Ooops. Must update this example!
      Serial.print("unknown device");
      break;
    }
    Serial.print(" at 0x");
    Serial.println(rtc.getAddress(), HEX);
  }
  else {
    // Nothing found at any of the addresses listed.
    Serial.println("No RTCx found");
    return;
  }

  // Enable the battery backup. This happens by default on the DS1307
  // but needs to be enabled on the MCP7941x.
  rtc.enableBatteryBackup();

  // rtc.clearVBAT();
  
  // Ensure the oscillator is running.
  rtc.startClock();

  if (rtc.getDevice() == RTCx::MCP7941x) {
    Serial.print("Calibration: ");
    Serial.println(rtc.getCalibration(), DEC);
    // rtc.setCalibration(-127);
  }

  rtc.setSQW(RTCx::freq4096Hz);
}


const uint8_t bufLen = 30;
char buffer[bufLen + 1] = {'\0'};
uint8_t bufPos = 0;
unsigned long last = 0;
void loop(void)
{
  struct RTCx::tm tm;
  if (millis() - last > 2000) {
    last = millis();
    rtc.readClock(tm);
    
    printTm(Serial, &tm);
    RTCx::time_t t = RTCx::mktime(&tm);
    printTm(Serial, &tm);
    Serial.print("unixtime = ");
    Serial.println(t);
    Serial.println("-----");
  }

  while (Serial.available()) {
    char c = Serial.read();
    if ((c == '\r' || c == '\n' || c == '\0')) {
      if (bufPos <= bufLen && buffer[0] == 'C') {
	// Check time error
	buffer[bufPos] = '\0';
	RTCx::time_t pcTime = atol(&(buffer[1]));
	rtc.readClock(&tm);
	RTCx::time_t mcuTime = RTCx::mktime(&tm);
	Serial.print("MCU clock error: ");
	Serial.print(mcuTime - pcTime);
	Serial.println(" s");
	Serial.println("~~~~~");
      }
      if (bufPos <= bufLen && buffer[0] == 'T') {
	// Set time
	buffer[bufPos] = '\0';
	RTCx::time_t t = atol(&(buffer[1])); 
	RTCx::gmtime_r(&t, &tm);
	rtc.setClock(&tm);
	Serial.println("Clock set");
	Serial.println(&(buffer[0]));
	printTm(Serial, &tm);
	Serial.println("~~~~~");
      }
      if (bufPos <= bufLen && buffer[0] == 'X') {
	// Set calibration value
	buffer[bufPos] = '\0';
	if (rtc.getDevice() == RTCx::MCP7941x) {
	  int8_t oldCal = rtc.getCalibration();
	  char *endptr;
	  long cal = strtol(&(buffer[1]), &endptr, 0);
	  if (cal >= -127 && cal <= 127 && endptr == &buffer[bufPos]) {
	    Serial.print("Previous calibration: ");
	    Serial.println(oldCal, DEC);
	    Serial.print("Calibration: ");
	    Serial.println(cal, DEC);
	      rtc.setCalibration(cal);
	  }
	  else 
	    Serial.println("Bad value for calibration");
	}
	else {
	  Serial.println("Cannot set calibration: not a MCP7941x");
	}
      }
      if (bufPos <= bufLen && buffer[0] == 'M') {
	// Set SQW mode
	buffer[bufPos] = '\0';
	  char *endptr;
	  long mode = strtol(&(buffer[1]), &endptr, 0);
	  if (mode >= RTCx::freq1Hz && mode <= RTCx::freqCalibration
	      && endptr == &buffer[bufPos]) {
	    if (rtc.setSQW((RTCx::freq_t)mode)) { 
	      Serial.print("SQW: ");
	      Serial.println(mode, DEC);
	    }
	    else
	      Serial.println("Could not set SQW");
	  }
	  else 
	    Serial.println("Bad value for SQW");
      }
      bufPos = 0;
    }
    else if (bufPos < bufLen)
      // Store character
      buffer[bufPos++] = c; 
  }
}
