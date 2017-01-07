# Arduino library for the Pololu A4990 Dual Motor Driver Shield

Version: 2.0.0 <br>
Release date: 2016-08-18 <br>
[![Build Status](https://travis-ci.org/pololu/a4990-motor-shield.svg?branch=master)](https://travis-ci.org/pololu/a4990-motor-shield) <br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for an
[Arduino-compatible controller](https://www.pololu.com/arduino) that
interfaces with the
[Pololu A4990 Dual Motor Driver Shield for Arduino](https://www.pololu.com/catalog/product/2512).
It makes it simple to drive two brushed, DC motors.

## Getting started

### Hardware

The
[Pololu A4990 Dual Motor Driver Shield for Arduino](https://www.pololu.com/catalog/product/2512)
can be purchased on Pololu's website. Before continuing, careful
reading of the product page is recommended.

#### Compatible Arduino boards

This shield should work with all Arduino boards and clones that behave
like a standard Arduino board. We have specifically tested this shield
(using this Arduino library) with:

* Arduino Uno R3
* Arduino Leonardo
* Arduino Mega 2560
* Arduino Due
* Arduino Duemilanove (ATmega328P)

This library configures Timer 1 on the Uno R3, Leonardo, and
Duemilanove to generate a 20 kHz PWM frequency for the motors. The
library uses analogWrite on any board that does not use an ATmega168,
ATmega328P or ATmega32U4. On the Mega 2560 and Due, analogWrite
generates 489 Hz and 1 kHz PWM frequencies, respectively, with the
default timer configuration.


### Software

If you are using version 1.6.2 or later of the
[Arduino software (IDE)](https://www.arduino.cc/en/Main/Software), you can use
the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then
   "Manage Libraries...".
2. Search for "A4990MotorShield".
3. Click the A4990MotorShield entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the
   [latest release archive from GitHub](https://github.com/pololu/a4990-motor-shield/releases)
   and decompress it.
2. Rename the folder "lps-arduino-xxxx" to "A4990MotorShield".
3. Drag the "A4990MotorShield" folder into the "libraries" directory inside your
   Arduino sketchbook directory. You can view your sketchbook location by
   opening the "File" menu and selecting "Preferences" in the Arduino IDE. If
   there is not already a "libraries" folder in that location, you should make
   the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Example program

An example sketch is available that shows how to use the library.  You
can access it from the Arduino IDE by opening the "File" menu,
selecting "Examples", and then selecting "A4990MotorShield".  If
you cannot find these examples, the library was probably installed
incorrectly and you should retry the installation instructions above.

### Demo

The demo ramps motor 1 from stopped to full speed forward, ramps down
to full speed reverse, and back to stopped. Then, it does the same
with the other motor.

## Documentatation

- `void setM1Speed(int speed)` <br> Set speed and direction for
  motor 1. Speed should be between -400 and 400. The motors brake at 0
  speed. Positive speeds correspond to motor current flowing from M1A
  to M1B. Negative speeds correspond to motor current flowing from M1B
  to M1A.
- `void setM2Speed(int speed)` <br> Set speed and direction for
  motor 2. Speed should be between -400 and 400. The motors brake at 0
  speed. Positive speeds correspond to motor current flowing from M2A
  to M2B. Negative speeds correspond to motor current flowing from M2B
  to M2A.
- `void setSpeeds(int m1Speed, int m2Speed)` <br> Set speed and
  direction for motor 1 and 2.
- `void flipM1(bool flip)` <br> Flip the direction meaning of the
  speed passed to the setSpeeds function for motor 1.  The default
  direction corresponds to flipM1(false) having been called.
- `void flipM2(bool flip)` <br> Flip the direction meaning of the
  speed passed to the setSpeeds function for motor 2.  The default
  direction corresponds to flipM2(false) having been called.
- `bool getFault()` <br> Returns true if pin 6 is being driven low by
  one of the error flag outputs.

## Version history

* 2.0.0 (2016-08-18): Updated library to work with the Arduino Library Manager.
* 1.0.0 (2014-08-15): Original release.
