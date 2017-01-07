VoltageReference [![Build Status](https://travis-ci.org/rlogiacco/VoltageReference.svg?branch=master)](https://travis-ci.org/rlogiacco/VoltageReference)
================

Many projects need to know the voltage present at their Vcc pin (also referred as 5V or 3V3), sometimes to monitor their own battery level (voltage gets lower during discharge), sometimes to level up analog reads (reading an analog pin implies a voltage comparison).

Most Atmel micro controllers embed an internal voltage reference which can be used for these purposes and this library helps to simplify operating with them.

There is no wiring required as this library operates with internal micro controller components only, but if you want to calibrate your board/microcontroller (highly recommended) two wires and a multimeter will be necessary.

Contributions are welcome under the [Apache Public License version 2.0 (http://www.apache.org/licenses/LICENSE-2.0.html).

For wiring instructions for calibration please refer to the [sample breadboard] (https://raw.githubusercontent.com/rlogiacco/VoltageReference/master/examples/EEPROMCalibration/calibration.png).

This library is largely based on [Coding Badly](http://forum.arduino.cc/index.php?action=profile;u=10859)'s and [Scott Daniels](http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/)' work: I just wrapped their code in an easy to use library.


Usage
============

This library comprises only one class `VoltageReference` which requires to be initialized by invoking it's `begin()` function which can optionally accept a calibration value (more on calibration later on) in two formats: either as an unsigned long or as three single bytes (to ease calibration value storage into internal EEPROM).

```
VoltageReference vRef = VoltageReference();

void setup() {
  vRef.begin();
}

void loop() {
  int vcc = vRef.readVcc();
  // vcc is the voltage at Vcc pin
  
  int v11 = vRef.internalValue();
  // v11 is the real internal voltage reference value
  
  int a0 = analogRead(A0);
  
  // usually to convert a0 to millivolts
  int oldPinV = a0 / 1024 * 5000; // for a 3.3V board use 3300

  // more precise result can be obtained with
  int betterPinV = a0 / 1024 * vcc;
  
  // when playing with low voltages
  analogReference(INTERNAL);
  int a1 = analogRead(A1);

  // usually to convert a1 to millivolts
  int oldPinLowV = a1 / 1024 * 1100;
  
  // the most accurate low voltage readings are obtained with
  int accuratePinLowV = a1 / 1024 * v11;
}
```

Calibration
============

The principle is to use the internal micro controller voltage reference of 1.1V to calculate the voltage powering the board and then use that as a reference instead of assuming your board is powered by a 5V or a 3.3V source: the latter is a big assumption in many situations, especially those where your voltage source is a battery (which produces a variable voltage).

While this usually already produces a better result than just assuming your board is powered perfectly and stable, this still introduces some variations as the internal micro controller voltage reference is subject to construction differences (impurities in the materials and that kind of stuff): to improve a little more your readings accuracy a per board/microcontroller calibration step is strongly recommended.

Using a multimeter (better the multimeter accuracy, better the calibration, better analog readings) you need to measure the voltage difference between the Vcc/5V pin and GND pin when your board is powered but with no load (nothing connected to either of those two pins). 
Let's say your multimeter reads 4.87V.

Now load the EEPROMCalibration example sketch onto your board/micro controller and run it opening a connection to the serial port: you should obtain the following output.

```
--- MENU ---
  R to read Vcc
  L to load calibration from EEPROM
  S to store calibration into EEPROM
  C to clear calibration from EEPROM
  dddd (4 digits) to calibrate for mV
  A to print EEPROM calibration start address (length 3)
  Adddd (4 digits) to set EEPROM calibration start address (length 3)
  H prints this help
```

Now send `R` and let's compare the voltage measured by your micro controller with the one measured by your multimeter: if they match you got lucky as both your micro controller and your multimeter have the same error (yeah, none of them will be 100% accurate, trust me)!

In case they differ slightly then here we can step in and calibrate the micro controller so that it will closely match your multimeter in the future (if you used a decent multimeter that should improve your readings).

By the menu above, you just send on serial the 4 digits representing your multimeter reading in millivolts: 4.97 * 1000 = 4970. You should get something like the following on the serial console, with the last number being a little higher or lower than the displayed one:

```
Calibrating for Vcc 4970mV
Calibration value is 1123220
```

Bingo! That is the number you should provide as input parameter to the `VoltageReference::begin()` function ***for this specific board/micro controller***. 

You can use the calibration sketch to store this value into the internal micro controller EEPROM memory and read it from there in the future: just send `S` on the serial console to have it stored in the default location (using the very last 3 bytes of the internal flash memory).
