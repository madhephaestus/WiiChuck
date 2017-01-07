/*
  volume2.h - Library for generating various waveforms with 256 volume levels.
  Created by Connor Nishijima, September 1st 2016.
  Released under the GPLv3 license.
*/
#ifndef volume2_h
#define volume2_h

#include "Arduino.h"

// WAVEFORM NAME CONSTANTS
const uint8_t SQUARE = 0;
const uint8_t SAWTOOTH = 1;
const uint8_t TRIANGLE = 2;
const uint8_t SINE = 3;
const uint8_t PWM_12 = 4;
const uint8_t PWM_25 = 5;
const uint8_t NOISE = 6;
const uint8_t CUSTOM = 7;
const uint8_t SINE_HIGH = 8;
const uint8_t TRIANGLE_HIGH = 9;
const uint8_t SAWTOOTH_HIGH = 10;

class Volume
{
  public:
    Volume();
    void tone(uint16_t frequency, uint8_t voiceT, uint8_t volume);
    void noTone();
	void setCustomWave(byte arr[]);
	byte getCustomSample(byte pos);
    void delay(uint32_t duration);
	uint32_t millis();
};

#endif
