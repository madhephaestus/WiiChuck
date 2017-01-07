/*
  volume2.cpp - Library for generating various waveforms with 256 volume levels.
  Created by Connor Nishijima, September 1st 2016.
  Released under the GPLv3 license.
*/

#include "volume2.h"

uint8_t voiceEnabled = false;
uint8_t voiceType; // can be SINE, SQUARE, PWM_12, etc.
uint8_t val; // Current output level
uint32_t toneMillis; // Used to compute millis() when Timer0 is 64x fast

uint8_t toneTable[16];
uint8_t toneIndex;

uint8_t highTable[64];
uint8_t highIndex;

// Standard resolution samples, good up to ~3,000 Hz
uint8_t const squareRef[16]  = {255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t const sawRef[16]     = {0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240};
uint8_t const triRef[16]     = {0, 32, 64, 96, 128, 160, 192, 224, 255, 224, 192, 160, 128, 96, 64, 32};
uint8_t const sineRef[16]    = {0, 10, 37, 79, 128, 176, 218, 245, 255, 245, 218, 176, 128, 79, 37, 10};
uint8_t const pwm12Ref[16]   = {255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t const pwm25Ref[16]   = {255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// High resolution samples, good up to ~750 Hz

uint8_t const sineHighRef[64] = {
	128,140,152,165,176,188,198,208,
	218,226,234,240,245,250,253,254,
	255,254,253,250,245,240,234,226,
	218,208,198,188,176,165,152,140,
	128,115,103,90,79,67,57,47,
	37,29,21,15,10,5,2,1,
	0,1,2,5,10,15,21,29,
	37,47,57,67,79,90,103,115
};

uint8_t const triHighRef[64] = {
	8,16,24,32,40,48,56,64,
	72,80,88,96,104,112,120,128,
	136,144,152,160,168,176,184,192,
	200,208,216,224,232,240,248,255,
	248,240,232,224,216,208,200,192,
	184,176,168,160,152,144,136,128,
	120,112,104,96,88,80,72,64,
	56,48,40,32,24,16,8,0
};

uint8_t const sawHighRef[64] = {
	0, 4, 8, 12, 16, 20, 24, 28,
	32,36, 40, 44, 48, 52, 56, 60,
	64, 68, 72, 76, 80, 84, 88, 92,
	96,	100, 104, 108, 112, 116, 120, 124,
	128, 132, 136, 140, 144, 148, 152, 156,
	160, 164, 168, 172, 176, 180, 184, 188,
	192, 196, 200, 204, 208, 212, 216, 220,
	224, 228, 232, 236, 240, 244, 248, 255
};

// Custom samples, good up to ~1500 Hz
uint8_t customRef[32] = {
  162, 183, 47, 45, 30, 182, 27, 102,
  114, 51, 147, 152, 255, 17, 77, 178,
  8, 73, 128, 132, 192, 57, 253, 203,
  82, 142, 83, 220, 227, 212, 163, 52
};
uint8_t customTable[32];
uint8_t customIndex;

float noiseVol;

Volume::Volume() {
};

void Volume::tone(uint16_t frequency, uint8_t voiceT, uint8_t volume) {
  cli();

  TCCR0B = (TCCR0B & 0b11111000) | 0x01;
  voiceType = voiceT;

  if (voiceType == CUSTOM) {
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    OCR1A = F_CPU / (1 * frequency * 32) - 1;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 1 prescaler
    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  }
  else if (voiceType == NOISE) {
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    OCR1A = F_CPU / (1 * 1000) - 1;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 1 prescaler
    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  }
  else if (voiceType == SINE_HIGH || voiceType == TRIANGLE_HIGH || voiceType == SAWTOOTH_HIGH) {
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    OCR1A = F_CPU / (1 * frequency * 64) - 1;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 1 prescaler
    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  }
  else {
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; // initialize counter value to 0
    OCR1A = F_CPU / (1 * frequency * 16) - 1;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12, CS11 and CS10 bits for 1 prescaler
    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
  }

  voiceEnabled = true;
  float vol = float(volume) / float(255);

  if (voiceType == SQUARE) {
    for (byte i = 0; i < 16; i++) {
      toneTable[i] = squareRef[i] * vol;
    }
  }
  else if (voiceType == SAWTOOTH) {
    for (byte i = 0; i < 16; i++) {
      toneTable[i] = sawRef[i] * vol;
    }
  }
  else if (voiceType == TRIANGLE) {
    for (byte i = 0; i < 16; i++) {
      toneTable[i] = triRef[i] * vol;
    }
  }
  else if (voiceType == SINE) {
    for (byte i = 0; i < 16; i++) {
      toneTable[i] = sineRef[i] * vol;
    }
  }
  else if (voiceType == PWM_12) {
    for (byte i = 0; i < 16; i++) {
      toneTable[i] = pwm12Ref[i] * vol;
    }
  }
  else if (voiceType == PWM_25) {
    for (byte i = 0; i < 16; i++) {
      toneTable[i] = pwm25Ref[i] * vol;
    }
  }
  else if (voiceType == CUSTOM) {
    for (byte i = 0; i < 32; i++) {
      customTable[i] = customRef[i] * vol;
    }
  }
  else if (voiceType == SINE_HIGH) {
    for (byte i = 0; i < 64; i++) {
      highTable[i] = sineHighRef[i] * vol;
    }
  }
  else if (voiceType == TRIANGLE_HIGH) {
    for (byte i = 0; i < 64; i++) {
      highTable[i] = triHighRef[i] * vol;
    }
  }
  else if (voiceType == SAWTOOTH_HIGH) {
    for (byte i = 0; i < 64; i++) {
      highTable[i] = sawHighRef[i] * vol;
    }
  }
  else if (voiceType == NOISE) {
    noiseVol = vol;
  }

  sei();
}

void Volume::noTone() {
  // TIMER 1 for interrupt frequency 1 Hz:
  cli(); // stop interrupts
  voiceEnabled = false;
  analogWrite(5, 0);
  TCCR0B = (TCCR0B & 0b11111000) | 0x03;

  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 1 Hz increments
  OCR1A = 65535;
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 256 prescaler
  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  // disable timer compare interrupt
  TIMSK1 &= (0 << OCIE1A); // Disable timer1 ISR
  sei(); // allow interrupts
}

void Volume::delay(uint32_t duration) {
  uint32_t tStart = toneMillis;
  uint32_t tEnd = tStart + duration;
  while (toneMillis < tEnd) {
    ::delay(0);
    // WAIT
  }
}
uint32_t Volume::millis(){
	return toneMillis;
}

void Volume::setCustomWave(byte arr[]) {
  for (byte i = 0; i < 32; i++) {
    customRef[i] = arr[i];
  }
}

byte Volume::getCustomSample(byte pos) {
  return customRef[pos];
}

byte rng() {
  static unsigned int y = 0;
  y += toneMillis; // seeded with changing number
  y ^= y >> 7;
  y ^= y << 7;
  return (y);
}

ISR(TIMER1_COMPA_vect) {
  toneMillis = ::millis()/64;

  if (voiceEnabled) {
    if (voiceType == NOISE) {
      val = rng() * noiseVol;
    }
    else if (voiceType == CUSTOM) {
      customIndex++;
      if (customIndex == 32) {
        customIndex = 0;
      }
      val = customTable[customIndex];
    }
	else if (voiceType == SINE_HIGH || voiceType == TRIANGLE_HIGH || voiceType == SAWTOOTH_HIGH) {
      highIndex++;
      if (highIndex == 64) {
        highIndex = 0;
      }
      val = highTable[highIndex];
    }
    else {
      toneIndex++;
      if (toneIndex == 16) {
        toneIndex = 0;
      }
      val = toneTable[toneIndex];
    }
    analogWrite(5, val);
  }
}
