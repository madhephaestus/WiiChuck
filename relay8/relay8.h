/*
  relay8.h - Arduino and chipKit library support for relay boards
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library supports from 1 to 8 relays.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef relay8_h
#define relay8_h

#if defined(__AVR__)
  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega32U4__)
	#include "Arduino.h"
  #else
    #error "Unsupported AVR MCU!"
  #endif  
#elif defined(__PIC32MX__)
  #if defined(__32MX320F128H__) || defined(__32MX795F512L__)
	#include "Wprogram.h"
  #else
    #error "Unsupported PIC32 MCU!"
  #endif  
#elif defined(__arm__)
  #if defined(__SAM3X8E__)
	#include "Arduino.h"
  #else
    #error "Unsupported ARM MCU!"
  #endif  
#else
  #error "Unsupported MCU architecture!"
#endif

#define MAX_RELAYS			8
#define DEFAULT_DELAY_TIME	250
#define	NOT_IN_USE			9999

// OUTPUT_ON_LEVEL should be set to HIGH or LOW according to what signal switches your relays ON.
#define OUTPUT_ON_LEVEL	HIGH

class relay8
{ 
public:
	relay8(int pin1, int pin2=NOT_IN_USE, int pin3=NOT_IN_USE, int pin4=NOT_IN_USE, int pin5=NOT_IN_USE, int pin6=NOT_IN_USE, int pin7=NOT_IN_USE, int pin8=NOT_IN_USE);

	int		numberOfRelays();

	void	on(int relay);
	void	off(int relay);
	void	allOn();
	void	allOff();

	void	cycle(int delay_time=DEFAULT_DELAY_TIME);
	void	cycleUp(int delay_time=DEFAULT_DELAY_TIME);
	void	cycleDown(int delay_time=DEFAULT_DELAY_TIME);

	void	chaseUpOn(int delay_time=DEFAULT_DELAY_TIME);
	void	chaseUpOff(int delay_time=DEFAULT_DELAY_TIME);
	void	chaseDownOn(int delay_time=DEFAULT_DELAY_TIME);
	void	chaseDownOff(int delay_time=DEFAULT_DELAY_TIME);

private:
	int _relay_pin[8];
	int _number_of_relays;
};


#endif