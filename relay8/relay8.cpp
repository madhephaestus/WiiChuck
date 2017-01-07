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

#include "relay8.h"

relay8::relay8(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8)
{
	_number_of_relays=0;
	_relay_pin[0] = pin1;
	_relay_pin[1] = pin2;
	_relay_pin[2] = pin3;
	_relay_pin[3] = pin4;
	_relay_pin[4] = pin5;
	_relay_pin[5] = pin6;
	_relay_pin[6] = pin7;
	_relay_pin[7] = pin8;

	for (int i=0; i<MAX_RELAYS; i++)
	{
		if (_relay_pin[i]!=NOT_IN_USE)
		{
			pinMode(_relay_pin[i], OUTPUT);
			digitalWrite(_relay_pin[i], !OUTPUT_ON_LEVEL);
		}
		else
			if (_number_of_relays==0)
				_number_of_relays = i;
	}
	if (_number_of_relays==0)
		_number_of_relays=MAX_RELAYS;
}

int relay8::numberOfRelays()
{
	return _number_of_relays;
}

void relay8::on(int relay)
{
	if ((relay>=1) and (relay<=_number_of_relays))
	{
		digitalWrite(_relay_pin[relay-1], OUTPUT_ON_LEVEL);
	}
}

void relay8::off(int relay)
{
	if ((relay>=1) and (relay<=_number_of_relays))
	{
		digitalWrite(_relay_pin[relay-1], !OUTPUT_ON_LEVEL);
	}
}

void relay8::allOn()
{
	for (int i=1; i<=_number_of_relays; i++)
		on(i);

}

void relay8::allOff()
{
	for (int i=1; i<=_number_of_relays; i++)
		off(i);
}

void relay8::cycle(int delay_time)
{
	cycleUp(delay_time);
	cycleDown(delay_time);
}

void relay8::cycleUp(int delay_time)
{
	for (int i=1; i<=_number_of_relays; i++)
	{
		on(i);
		delay(delay_time);
		off(i);
	}
}

void relay8::cycleDown(int delay_time)
{
	for (int i=_number_of_relays; i>0; i--)
	{
		on(i);
		delay(delay_time);
		off(i);
	}
}

void relay8::chaseUpOn(int delay_time)
{
	for (int i=1; i<=_number_of_relays; i++)
	{
		on(i);
		delay(delay_time);
	}
}

void relay8::chaseUpOff(int delay_time)
{
	for (int i=1; i<=_number_of_relays; i++)
	{
		off(i);
		delay(delay_time);
	}
}

void relay8::chaseDownOn(int delay_time)
{
	for (int i=_number_of_relays; i>0; i--)
	{
		on(i);
		delay(delay_time);
	}
}

void relay8::chaseDownOff(int delay_time)
{
	for (int i=_number_of_relays; i>0; i--)
	{
		off(i);
		delay(delay_time);
	}
}

