// Arduino RBD Servo Library v1.0.1 - Control many servos.
// https://github.com/alextaujenis/RBD_Servo
// Copyright 2015 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <RBD_Servo.h> // https://github.com/alextaujenis/RBD_Servo

namespace RBD {
  Servo::Servo(int pin, unsigned long pulse_min, unsigned long pulse_max) {
    _pin       = pin;
    _pulse_min = pulse_min;
    _pulse_max = pulse_max;
    pinMode(_pin, OUTPUT);
    _waypoint  = micros();
  }

  void Servo::setPulseInterval(unsigned long value) {
    _interval = value * 1000; // convert millis to micros
  }

  void Servo::setDegreesOfRotation(int value) {
    if(value >= 0 && value <= 360) {
      _degrees_of_rotation = value;
    }
  }

  void Servo::moveToDegrees(int value) {
    if(value >= 0 && value <= _degrees_of_rotation) {
      _active  = true;
      _changed = true;
      _target_degrees = value;
    }
  }

  void Servo::update() {
    // check if the 20 ms pulse interval has expired, then reset the timer
    if(micros() - _waypoint >= _interval) {
      _waypoint = micros();
    }

    // check if the pulse timer has time left
    if(micros() - _waypoint <= _targetPulseTime()) {
      // don't move inactive servo on start for safety, only starts moving after first call to moveToDegrees()
      // also track the pin state with a bool so we only make one call to digitalWrite()
      if(_active && !_pin_is_high) {
        digitalWrite(_pin, HIGH);
        _pin_is_high = true;
      }
    }
    else {
      if(_pin_is_high) {
        digitalWrite(_pin, LOW);
        _pin_is_high = false;
      }
    }
  }


  // private

  unsigned long Servo::_targetPulseTime() {
    // only compute pulse time when degrees change
    if(_changed) {
      _changed = false;
      // map(value, from_low, from_high, to_low, to_high)
      _target_pulse_time = (unsigned long) map(_target_degrees, 0, _degrees_of_rotation, _pulse_min, _pulse_max);
    }
    return _target_pulse_time;
  }
}