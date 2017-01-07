/* 
 * Arduino Learning Board Project - HC-SR04 Ultrasonic Sensor Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 */

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_DS1302 to include the DS1302  functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)

#define USE_ALB_DS1302

// NOW include the main ArduinoLearningBoard library (quotes now, <Arduino...> when it's "live")
// Based on the defines above, the appropriate code will be added to the project
#include "ArduinoLearningBoard.h"

// VCC -> 5V
// GND -> GND
// CLK -> D11
// DAT -> D10
// RST -> D9

// Set pins:  CE, IO,CLK
DS1302RTC RTC(9, 10, 11);

void setup()
{
	Serial.begin(9600); // Start Serial port so we can see the results

	Serial.println("DS1302RTC Read Test");
	Serial.println("-------------------");

	Serial.println("RTC module activated");
	Serial.println();
	delay(500);

	if (RTC.haltRTC())
	{
		Serial.println("The DS1302 is stopped.  Set the time as shown");
		Serial.println("below to initialize the clock and begin running.");
		Serial.println();
	}
	
	Serial.println("To set the time, send a string in the following format:");
	Serial.println("yy,m,d,h,m,s  {Year, Month, Day, Hour, Minute, Second}");
	Serial.println("i.e. 16,9,1,11,30,00 would set the time to September 1, 2016 at 11:30:00 AM");
	delay(3000);
}

void loop()
{
	tmElements_t tm;
	time_t t;

	//check for input to set the RTC, minimum length is 12, i.e. yy,m,d,h,m,s
	if (Serial.available() >= 12)
	{
		//note that the tmElements_t Year member is an offset from 1970,
		//but the RTC wants the last two digits of the calendar year.
		//use the convenience macros from Time.h to do the conversions.
		int y = Serial.parseInt();
		if (y >= 100 && y < 1000)
			Serial.println("Error: Year must be two digits or four digits!");
		else
		{
			if (y >= 1000)
				tm.Year = CalendarYrToTm(y);
			else    //(y < 100)
				tm.Year = y2kYearToTm(y);
			tm.Month = Serial.parseInt();
			tm.Day = Serial.parseInt();
			tm.Hour = Serial.parseInt();
			tm.Minute = Serial.parseInt();
			tm.Second = Serial.parseInt();
			t = makeTime(tm);
			//use the time_t value to ensure correct weekday is set
			if(RTC.set(t) == 0)   // Success
			{
				setTime(t);
				Serial.println("RTC set.");
			}
			else
				Serial.println("RTC set failed!");
			//dump any extraneous input
			while (Serial.available() > 0) Serial.read();
		}
	}

	Serial.print("UNIX Time: ");
	Serial.print(RTC.get());

	if (! RTC.read(tm))
	{
		Serial.print("  Time = ");
		print2digits(tm.Hour);
		Serial.write(':');
		print2digits(tm.Minute);
		Serial.write(':');
		print2digits(tm.Second);
		Serial.print(", Date (D/M/Y) = ");
		Serial.print(tm.Day);
		Serial.write('/');
		Serial.print(tm.Month);
		Serial.write('/');
		Serial.print(tmYearToCalendar(tm.Year));
		Serial.print(", DoW = ");
		Serial.print(tm.Wday);
		Serial.println();
	}
	else
	{
		Serial.println("DS1302 read error!  Please check the circuitry.");
		Serial.println();
		delay(9000);
	}

	// Wait one second before repeating :)
	delay (1000);
}

void print2digits(int number)
{
	if (number >= 0 && number < 10)
		Serial.write('0');
	Serial.print(number);
}