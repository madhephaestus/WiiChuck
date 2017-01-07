/*
  Volume3.cpp - Library for tone() with 10-bit volume control.
  Created by Connor Nishijima, November 13th 2016.
  Released under the GPLv3 license.
*/

#include "Volume3.h"
#include "TimerOne.h"

volatile uint8_t pin1 = 0;
volatile uint8_t state1 = 0;
volatile uint16_t tick1 = -1;
volatile uint16_t period1 = 0;
volatile uint16_t volume1 = 0;

volatile bool toneRun1 = false;

const byte timerPin1 = 9;
const byte timerPin2 = 10;

void toneBegin(byte pin) {
  if (pin != pin1) {
    if (pin == timerPin1 || pin == timerPin2) {
      pin1 = pin;
      Timer1.initialize(10);
      Timer1.pwm(pin1, 0);
      Timer1.setPwmDuty(pin1, 0);
      state1 = 0;
      tick1 = -1;
      period1 = 0;
      volume1 = 0;
      toneRun1 = false;
    }
    else {
      return;
    }
  }
}

void cycle() {
  tick1++;
  if (tick1 >= period1) {
    tick1 = 0;
    state1 = !state1;
    uint16_t vol1 = state1 * volume1;
    Timer1.setPwmDuty(pin1, vol1);
  }
}

void Volume::noTone() {
  Timer1.stop();
  Timer1.setPeriod(2040);
  digitalWrite(pin1,LOW);
  pin1 = 0;
  tick1 = -1;
  period1 = 0;
  volume1 = 0;
  toneRun1 = false;
}

void Volume::tone(uint8_t pin, uint16_t frequency, uint16_t volume){
  if (frequency <= 4186) {
    Timer1.detachInterrupt();
    toneBegin(pin);
    if (frequency < 2) {
      frequency = 2;
    }

    period1 = 100000 / (frequency * 2) + 1;
    volume1 = volume;
    if (toneRun1 == false) {
      toneRun1 = true;
      state1 = 0;
      tick1 = 0;
    }
    Timer1.attachInterrupt(cycle);
  }
}
