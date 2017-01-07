/* SweepAndHold
 Copyright (c) 2016 Alorium Technology.  All rights reserved.
 by Matt Weber (linkedin.com/in/mattweber0) of
 Alorium Technology (info@aloriumtech.com)
 
 Demonstrates the awesomeness of the XLR8 Servo library and
  hardware. The example separately controls up to 16 servos.
  1 of them is done in a sweep pattern to give you something
  interesting to look at while the others are in a hold pattern
  where you can see if they are stable or jittery.
 Simply by changing which header file is #include'd, the example
  can be run with either the standard Arduino servo library or
  with Alorium's XLR8 servo library and hardware.
 If you'd like to put servo motors on all of pins being used
  you certainly can, but more typically I leave most of them
  unconnected and just have two actual servo motors, one on
  the pin that is sweeping and one on any of the pins that are
  holding. If you have a long enough "arm" (a laser light works
  well) you should be able to see the difference in jitter between
  the ordinary and XLR8 versions.
*/

// Use ordinary Arduino servo library
//#include <Servo.h> 
// Use XLR8 servo library and hardware
#include <XLR8Servo.h>

// While XLR8 can control up to 16 channels, the Arduino library
//  only supports up to 12, so that's the default to make it
//  easier to switch back and forth
#define NUM_SERVOS 12
 
#define SWEEP_PIN 9
// skip RX and TX pins 
#define SKIP_PINS 2
 
Servo myservos[NUM_SERVOS+SKIP_PINS];  // create servo objects to control the servos
 
int pos = 0;    // variable to store the servo position on the ones that are sweeping
 
void setup() 
{ 
  for (uint8_t i = SKIP_PINS; i < NUM_SERVOS + SKIP_PINS; i++) {
    myservos[i].attach(i);  // 
    myservos[i].write(i*8); // start all servos holding in different positions
  }
} 
 
void loop() 
{ 
  // Have one servo doing sweep pattern just to make something interesting
  //   to look at
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservos[SWEEP_PIN].write(pos);  // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservos[SWEEP_PIN].write(pos);  // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
} 

