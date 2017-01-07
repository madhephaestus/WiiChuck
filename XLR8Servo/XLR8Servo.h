/*--------------------------------------------------------------------
  Copyright (c) 2015 Alorim Technology.  All right reserved.
  This file is part of the Alorium Technology XLR8 Servo library.
  Written by Matt Weber (linkedin.com/in/mattweberdesign) of 
    Alorium Technology (info@aloriumtech.com) using the same interface as
    the Arduino Servo library by Michael Margolis, but the XLR8
    implementation is modified to take advantage of the FPGA hardware
    acceleration available on the XLR8 board.
 
  Notable improvements compared to the Arduino library and hardware
  include the following:
    Reduced jitter
    PWM on pins 9 and 10 is not lost
    16 bit timer function is not lost

  Usage
  The XLR8Servo library is a drop in replacement for the standard
    Arduino Servo library which is well documented at
    (https://www.arduino.cc/en/Reference/Servo).
    To get the XLR8 advantages, simply change
      #include <Servo.h>
    to
      #include <XLR8Servo.h>

  If the servo library is deeper in your code (included and used by other
    libraries) it only takes two lines in your sketch to get the XLR8
    advantages used by those libraries as well. For example:
        // 2 new lines added
        #include <XLR8Servo.h>
        #define Servo_h
        // Existing lines kept
        #include <OtherLibrary.h>
        #include <Servo.h>
  As explanation, working from the bottom up:
  -Including Servo.h remains because other library files that we
    are using but are not modifying (OtherLibrary.h in this case) have
    #include <Servo.h>. The way Arduino handles compiling and linking
    generally requires that lower level #includes also be included in the sketch.
  -Including OtherLibraryremains because that is the library that this
    particular sketch is using
  -Even though we are still including the Servo.h, adding
    #define Servo_h causes that header file to appear empty instead of
    having the actual old servo library code.
  -And of course, the XLR8Servo library is included. If you are coding only for
    the XLR8 hardware, this is the only line you would need.

 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library.  If not, see
 <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------*/

/* 
  A servo is activated by creating an instance of the Servo class passing 
  the desired pin to the attach() method.
  The servos are pulsed in the background using the value most recently 
  written using the write() method.

  In contrast to the standard Servo library, this library does not use the
   AVR timers and does not disable the analogWrite function on any pins.

  The methods are:

    Servo - Class for manipulating servo motors connected to Arduino pins.

    attach(pin )  - Attaches a servo motor to an i/o pin.
    attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
    default min is 544, max is 2400  
 
    write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
    writeMicroseconds() - Sets the servo pulse width in microseconds 
    read()      - Gets the last written servo pulse width as an angle between 0 and 180. 
    readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
    attached()  - Returns true if there is a servo attached. 
    detach()    - Stops an attached servos from pulsing its i/o pin. 
 */

#ifndef XLR8SERVO_H
#define XLR8SERVO_H

// #ARDUINO_XLR8 is passed from IDE to the compiler if XLR8 is selected properly
#ifdef ARDUINO_XLR8


#include <inttypes.h>

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached

#define MAX_SERVOS             20     // with current hardware design up to 16 can be independant

#define INVALID_SERVO         255     // flag indicating an invalid servo index

typedef struct  {
  uint8_t nbr        :6 ;             // a pin number from 0 to 63 (although above MAX_SERVOS doesn't get used)
  uint8_t isActive   :1 ;             // true if this channel is enabled, pin not pulsed if false 
} ServoPin_t   ;  

typedef struct {
  ServoPin_t Pin;
  volatile unsigned int microseconds;
} servo_t;

class Servo
{
public:
  Servo();
  uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
  uint8_t attach(int pin, int min, int max); // as above but also sets min and max values for writes. 
  void detach();
  void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
  void writeMicroseconds(int value); // Write pulse width in microseconds 
  int read();                        // returns current pulse width as an angle between 0 and 180 degrees
  int readMicroseconds();            // returns current pulse width in microseconds for this servo
  bool attached();                   // return true if this servo is attached, otherwise false 
private:
   uint8_t servoIndex;               // index into the channel data for this servo
   int8_t min;                       // minimum is this value times 4 added to MIN_PULSE_WIDTH    
   int8_t max;                       // maximum is this value times 4 added to MAX_PULSE_WIDTH   
};

#else
#error "XLR8Servo library requires Tools->Board->XLR8xxx selection. Install boards from https://github.com/AloriumTechnology/Arduino_Boards"
#endif

#endif
