/*--------------------------------------------------------------------
  Copyright (c) 2016 Alorium Technology.  All rights reserved.
  This file is part of the Alorium Technology XLR8 Servo library.
  Written by Matt Weber (linkedin.com/in/mattweberdesign) of 
    Alorium Technology (info@aloriumtech.com) 
    
  The example shows both how to use the XLR8 Servo library and is
    easy to modify to demonstrates the advantage of using the XLR8
    board's dedicated Servo hardware. The ordinary Arduino Servo
    library depends on interrupts and in many systems the interrupt
    processing can become delayed which results in servo jitter. The
    the XLR8 servo hardware and library does not rely on processor
    interrupts and instead has a hardware timer for each pin and 
    therefore is able to drive jitter-free.
    
  The sketch runs a configurable number of servos. The one on pin 2
    is also tied to a interrupt routine that measures (roughly)
    the size of the pulse being sent. Difference from
    the programmed pulse width is calculated and statistics
    such min, max, mean, and std dev are calculated on the difference
    and reported across serial. Min and max are used to determine
    peak to peak jitter, the mean indicates if the jitter is centered
    (mean = 0) or if the servo is generally pushed in one direction 
    or the other. In case the min and max measurements are rare
    instead of typical, the std dev shows the typical error.
  Simply changing between #include <Servo.h> and #include <XLR8Servo.h>
    shows the lower jitter obtained by using the XLR8 hardware
    and library. Note that error reported would be due to
    both the generation of the servo pulse by the servo library
    and the measurement of it by the pin change interrupt handlers,
    so even though XLR8Servo generates servo pulses with no errors,
    the results reported across serial may still be non-zero. To
    get an idea of the error on the measurement side, a clean
    pulse train is sent out on pin 3 using analogWrite and is measured
    using the same method that pin 2 is using to measure the servo
    pulses. (This also generates additional interrupt handling).
  For a more phyical demo, actual servo(s) can be driven from
    one or more of the pins. To make this interesting, the servo
    pattern alternates between sweeping back and forth and holding
    steady in the center. The jitter when using Servo.h should be
    visible when holding in the center, although you can make it
    more obvious by attaching a laser light or pencil to the servo.
    With XLR8Servo.h holding in the center should be jitter-free.



  XLR8 Servo is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  XLR8 Servo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with XLR8 Servo.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#include <Servo.h>		// for a standard Uno board
//#include <XLR8Servo.h>   	// for the Alorium XLR8 board

// The control pin is used to determine the error on the
//  measurement side by sending a clean pulse train using
//  analogWrite on pin 3
// Using pins 2&3 because they have the attachInterrupt()
//  function available. To use other pins we'd need to switch 
//  to the PinChangeInt.h or other similar library.
const uint8_t controlPin = 3;
const uint8_t controlPWM = 64; // 0-255, but to ensure meaureControl() sees both edges stay within about 8-248
const uint16_t controlIdeal = clockCyclesToMicroseconds(controlPWM * 64); // timer prescaler default is 64
const uint8_t measurePin = 2; // pin where we measure servo pulses
const uint8_t numServos = 4;
// servoPins can be any pins from 4-19 (plus pin 2 for measuring)
const uint8_t servoPins[numServos] = {measurePin,6,7,8};
Servo servoDrivers[numServos];
volatile int16_t controlMinError,controlMaxError,controlNum;
volatile int32_t controlSumError;
volatile int16_t measureMinError,measureMaxError,measureNum;
volatile int32_t measureSumError;
const uint16_t scanTime = 1000; // number of ms to scan left to right
const uint16_t holdTime = 3000; // number of ms to hold after scanning before starting again
uint32_t scanStartTime = 0;
const uint16_t servo_mid = 1500;  // mid-point
const uint16_t servo_range = 300; // how far to move each direction
const uint16_t servo_min = servo_mid - servo_range; // minimum pulse width to servo
const uint16_t servo_max = servo_mid + servo_range; // maximum pulse width to servo
uint16_t measureIdeal = servo_mid;
int i;

void setup() {
  Serial.begin(115200);
  Serial.println("   Servo Error      |    Control Error");
  Serial.println("Min Max Range Avg   |  Min Max Range Avg");
  analogWrite(controlPin,controlPWM);
  for (i=0;i<numServos;i++) {
    pinMode(servoPins[i],OUTPUT);
    servoDrivers[i].attach(servoPins[i]);
  }
  clearControlStats();
  clearServoStats();
  attachInterrupt(digitalPinToInterrupt(controlPin),measureControl,CHANGE);
  attachInterrupt(digitalPinToInterrupt(measurePin),measureServo,CHANGE);
}

void loop() {
  // Periodically send stats to serial and clear them
  if ((controlNum > 1000) || (abs(controlSumError) > 32767) ||
      (measureNum > 1000) || (abs(measureSumError) > 32767)) {
    int16_t tempRange = measureMaxError-measureMinError;
    float tempAvg = measureSumError/(float)measureNum;
    Serial.print(measureMinError);Serial.print("   ");
    Serial.print(measureMaxError);Serial.print("   ");
    Serial.print(tempRange);Serial.print("     ");
    Serial.print(tempAvg);Serial.print("     |   ");
    tempRange = controlMaxError-controlMinError;
    tempAvg = controlSumError/(float)controlNum;
    Serial.print(controlMinError);Serial.print("   ");
    Serial.print(controlMaxError);Serial.print("   ");
    Serial.print(tempRange);Serial.print("     ");
    Serial.print(tempAvg);Serial.println("");
    clearControlStats();
    clearServoStats();
  }
  // Move the servos around
  uint32_t currentMillis = millis();
  uint16_t elapsed = currentMillis - scanStartTime;
  uint16_t nextServoPW = servo_mid;
  if (elapsed < scanTime) {
    // This jumps to left, scans smoothly to right, then jumps to middle
    nextServoPW = map(elapsed,0,scanTime,servo_min,servo_max);
  }
  // assume pin to be measured is first in the list, if it's in the middle
  //   of a pulse, wait for it to end so we don't goof up the measurement
  while (digitalRead(measurePin));
  measureIdeal = nextServoPW;
  // send new value to the servos
  for (i=0;i<numServos;i++) {
    servoDrivers[i].writeMicroseconds(measureIdeal);
  }
  // check if it's time to start the next cycle
  if (elapsed > (scanTime + holdTime)) {
    scanStartTime = currentMillis;
  }
}

void clearControlStats() {
  controlMinError = 10000;
  controlMaxError = -10000;
  controlSumError = 0;
  controlNum = 0;
}
void clearServoStats() {
  measureMinError = 10000;
  measureMaxError = -10000;
  measureSumError = 0;
  measureNum = 0;
}

void measureControl() {
  uint32_t now = micros();
  static uint32_t posedge;
  // Can't do digitalRead here because for some reason
  //  that shuts off analogWrite that we have running
  //  So we end up hardcoding some things here. 
  // controlPin is pin 3 which is portD pin 3 on Uno compatibles
  if (PIND & (1<<PIND3)) {
    posedge = now;
  } else {
    uint16_t pulseWidth = now - posedge; // 16b can measure pulses up to about 64ms 
    int16_t error = controlIdeal - pulseWidth; // error can be pos or neg
    if (error < controlMinError) {controlMinError = error;}
    if (error > controlMaxError) {controlMaxError = error;}
    controlSumError += error; // in loop() we try to catch this before it overflows
    controlNum++;  
  }
}
  
void measureServo() {
  uint32_t now = micros();
  static uint32_t posedge;
  // measurePin is pin 2 which is portD pin 2 on Uno compatibles
  if (PIND & (1<<PIND3)) {
    posedge = now;
  } else {
    uint32_t pulseWidth = now - posedge;
    int16_t error = measureIdeal - pulseWidth; // error can be pos or neg
    if (error < measureMinError) {measureMinError = error;}
    if (error > measureMaxError) {measureMaxError = error;}
    measureSumError += error; // in loop() we try to catch this before it overflows
    measureNum++;  
  }
}

