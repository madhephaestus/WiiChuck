/*
  Volume3.h - Library for tone() with 10-bit volume control.
  Created by Connor Nishijima, November 13th 2016.
  Released under the GPLv3 license.
*/
#ifndef volume3_h
#define volume3_h

#include "Arduino.h"

class Volume
{
  public:
	void tone(uint8_t pin, uint16_t frequency, uint16_t volume);
	void noTone();
};

extern Volume vol;

#endif
