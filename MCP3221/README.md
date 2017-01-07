
# MCP3221 DRIVER

[![Platform Badge](https://img.shields.io/badge/platform-Arduino-orange.svg)](https://www.arduino.cc/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![SemVer](https://img.shields.io/badge/SemVer-1.0.0-brightgreen.svg)](http://semver.org/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![SemVer](https://img.shields.io/badge/eagle-pcb-yellow.svg)](https://cadsoft.io/)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

The MCP3221 is a 12-Bit Single-Channel ADC with hardware I2C interface.

This library contains a complete driver for the MCP3221 exposing all its available features. The library also contains configurable functions for obtaining either data or voltage reading from the device, as well as applying smoothing methods (Rolling-Average / Exponential-Moving-Average) to the said data/voltage readings. In addition, the library offers a built-in mechanism for calculating input from either 5V or 12V sources (the latter requiring a hardware voltage divider as the AIN pin of the MCP3221 cannot take more than 5.5V).

[MCP3221 DATASHEET](http://ww1.microchip.com/downloads/en/DeviceDoc/21732D.pdf)

## Repository Contents

- **MCP3221.h** - Library Header file.  
- **MCP3221.cpp** - Library Compilation file.  
- **/utility**   
  - **MCP3221InfoStr.h** - Header file containing a functional extention of the library to include generating printable information String (see Note #2 below).  
  - **MCP3221ComStr.h** - Header file containing a functional extention of the library to include generating a printable I2C Communication Result String (see Note #3 below).  
  - **MCP3221_PString.h** - Header file for PString class (lighter alternative to String class).  
  - **MCP3221_PString.cpp** - Compilation file for PString class (lighter alternative to String class).  
- **/examples**   
  - **/MCP3221_Test**  
    - **MCP3221_Test.ino** - A basic sketch for testing whether the MCP3221 is hooked-up and operating correctly.  
  - **/MCP3221_Usage**  
    - **MCP3221_Usage.ino** - A much more extensive sketch offering a complete usage illustration, as well as a rubust testing mechanism.  
  - **/MCP3221_Info**  
    - **MCP3221_Info.ino** - A short sketch showing how to generate a Printable Device Information String with the MCP3221's current settings.  
  - **/MCP3221_I2C_Status**
    - **MCP3221_I2C_Status.ino** - A short sketch for verifying I2C communication has been established between the controller (master) and the MCP3221 (slave).  
- **/extras**
  - **License.txt** - A cope of the end-user license agreement.  
  - **/eagle**
    - **MCP3221.sch** - Schematic file for the MCP3221 breakout board.
    - **MCP3221.brd** - Board layout for the MCP3221 breakout board.
- **keywords.txt** - Keywords for this library which will be highlighted in sketches within the Arduino IDE. 
- **library.properties** - General library properties for the Arduino's IDE (>1.5) Library Package Manager.
- **README.md** - The readme file for this library.
- **library.json** - JSON file for the Arduino's IDE (>1.5) Package Manager.

## HOOK-UP
* __PIN 1 (VCC/VREF)__ - Serves asimultaniously as Power Supply input and Voltage Reference for the ADC. Connect 5V output of Arduino's' or other equivalent power source (5.5V max). However, if using external power source, remember to connect all GND's together.
* __PIN 2 (GND)__ - Connect to Arduino GND
* __PIN 3 (VIN)__ - Connect input voltage to be measured (RANGE: GND - VCC, Min: 0V, Max: 5.5V). For higher voltage readings (e.g. Min: 8.444V, Max: 12.881V), use a voltage divider (Resistor 1: 10K; Resisor 2: 4K7)
* __PIN 4 (SDA)__ - Connect to Arduino's PIN A4 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
* __PIN 5 (SCL)__ - Connect to Arduino's PIN A5 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
* __DECOUPING__: Minimal decoupling consists of a 0.1uF Ceramic Capacitor between the VCC & GND PINS. For improved performance, add a 1uF and a 10uF Ceramic Capacitors as well across these pins.

## GENERAL NOTES

1) __I2C Communications Library Dependency__

This library depends on the Arduino IDE's native [Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire) library for I2C communication between the Arduino (Master) and the MCP3221 (Slave). 

2) __Device Information String__

It possible to extend the MCP3221 Library to include a function for generating a pritable device information string showing all the relevant details about the devices current Configuration, Limit & Hysteresis settings. As the additional functionality comes at the cost of increased memory footprint, it was implemented as an optional add-on rather than added directly to the core MCP3221 Library. See the [MCP3221_Info](https://github.com/nadavmatalon/MCP3221/blob/master/examples/MCP3221_Info/MCP3221_Info.ino) example sketch for detailed explanation and an actual usage demo.

3) __Device I2C Communications String__

It is also possible to extend the MCP3221 Library to include a function for generating a pritable I2C Communications string showing the result of each I2C transaction in a human-friendly way, something that may be useful, for example, during debugging sessions. As the additional functionality comes at the cost of increased memory footprint, it was implemented as an optional add-on rather than added directly to the core MCP3221 Library. See the [MCP3221_I2C_Status](https://github.com/nadavmatalon/MCP3221/blob/master/examples/MCP3221_I2C_Status/MCP3221_I2C_Status.ino) example sketch for detailed explanation and an actual usage demo.

## I2C ADDRESSES

Each MCP3221 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking on the package itself):

|PART NO.         | BIN      | HEX  | DEC | MARKING |
|-----------------|----------|------|-----|---------|
| MCP3221A0T-E/OT | 01001000 | 0x48 | 72  | GE      |
| MCP3221A1T-E/OT | 01001001 | 0x49 | 73  | GH      |
| MCP3221A2T-E/OT | 01001010 | 0x4A | 74  | GB      |
| MCP3221A3T-E/OT | 01001000 | 0x4B | 75  | GC      |
| MCP3221A4T-E/OT | 01001100 | 0x4C | 76  | GD      |
| MCP3221A5T-E/OT | 01001101 | 0x4D | 77  | GA      |
| MCP3221A6T-E/OT | 01001110 | 0x4E | 78  | GF      |
| MCP3221A7T-E/OT | 01001111 | 0x4F | 79  | GG      |


## LIBRARY INSTALLATION & SETUP

Begin by installing the library either by using the Arduino IDE's Installation Wizard (Arduino Version >1.5) or simply download the library's ZIP folder from GITHUB, extract it, and copy the extraxcted folder to your Arduino 'libraries' folder.

Next, include the library at the top of the sketch as follows:

```
#include "MCP3221.h"
```

At this point you can construct a new MPC3221 instance(s) by using the following command (at the top of the sketch after the 'include' line):

```
MPC3221 device_name(device_address);
```

>Replace '__device_name__' with a name of your choice. Also, remember to replace  '__device_address__' with the specific I2C address of your device (see I2C ADDRESSES Section above).

Next, make sure to include an instruction for initializing the I2C Bus for the [Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire) Library, as follows:

>There's no need to include the [Wire](https://github.com/arduino/Arduino/tree/master/hardware/arduino/avr/libraries/Wire) Library at the top of the sketch as it's already included internally by the MCP3221 Library

```
void setup() {
    Wire.begin();
    // ...other setup code...
}
```

## LIBRARY FUNCTIONS

With the library installed & included in the sketch, and an MCP3221 instance created, the following functions are available (see the usage example sketch for a detailed implementation):

__Note About Methods' Return Values:__  
All 'get' methods return some sort of value (e.g. temp reading, hysteresis setting, etc.), while all 'set' methods return nothing. Nevertheless, ALL methods implicitly update the library's __I2C _comBuffer__ (=communication buffer) after each I2C transmission. The reason for this functional design is to maintain structural coherance between the 'get' and 'set' methods. As 'get' methods cannot return both the desired value and the I2C transmission's result simultaniously. Consequently, if the relevant value hasn't been obtained by a particular 'get' method, the user can simply check the content of the _comBuffer to see what error occured. Similarly, it is possible to check if a particular setting has been successfully applied via a 'set' method either by preforming the corresponding 'get' method - e.g. getHystC() after using setHystC() - or by checking the content of the _comBuffer (0 indicates a successful transmission, 1-6 indicate an error as listed below). 

__ping();__                                    
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Searches for the MCP3221 at the pre-defined I2C Bus address &amp; returns byte with the relevant success/error code, as follows:  
0  ...  Success (no error)  
1  ...  Buffer overflow  
2  ...  Address sent, NACK received  
3  ...  Data send, NACK received  
4  ...  Other error (lost bus arbitration, bus error, etc.)  
5  ...  Timed-out while trying to become Bus Master  
6  ...  Timed-out while waiting for data to be sent  
\>6 ... Unlisted error (potential future implementation/s)<br>
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte  

__getVref();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current setting of the 'Voltage Reference' parameter in mV (default: 4096mV). Acceptable range: 2700-5500mV.   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int  

__getRes1();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current value of Resistor 1 (in Ω) of the hardware voltage divider (if used). Set automatically to 0Ω if device is initialized with Voltage Input set to 5V (default), or to 10KΩ if device is initialized with Voltage Input set to 12V (value can be changed after device initialization).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int  

__getRes2();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current value of Resistor 2 (in Ω) of the hardware voltage divider (if used). Set automatically to 0Ω if device is initialized with Voltage Input set to 5V (default), or to 4K7Ω if device is initialized with Voltage Input set to 12V (value can be changed after device initialization).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int  

__getAlpha();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current value of the 'Alpha' parameter used by the 'EMAVG' smoothing method (default: 178).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int  

__getNumSamples();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current number of samples used by the 'Rolling-Average' smoothing method (default: 10 Samples).  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte  

__getVinput();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the voltage input settings (5V [default] / 12V) used for voltage reading calculations.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte   

__getSmoothing();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the current smoothing method (0 = NO SMOOTHING / 1 = ROLLING-AVERAGE / 2 = EMAVG [default]) used for voltage reading calculations.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte   

__getData();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the latest conversion data from the device (the data is automatically smoothed by the selected smoothing method if used). To obtain raw data from the device simply set the Smoothing Method settings to 'NO SMOOTHING'.  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int  

__getVoltage();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Gets the latest voltage reading (in mV) from the device (the reading is automatically smoothed by the selected smoothing method if used). To obtain unmodified voltage readings from the device simply set the Smoothing Method settings to 'NO SMOOTHING'.   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;unsigned int  

__getComResult();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;Returns the latest I2C Communication result code (see Success/Error codes above)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte  

__setVref();__  
Parameters:&nbsp;&nbsp;&nbsp;unsigned int  
Description:&nbsp;&nbsp;&nbsp;Sets the current value of the 'Voltage Reference' parameter (in mV). This value can be obtained by measuring the input voltage on the devices VCC pin    
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__setRes1();__  
Parameters:&nbsp;&nbsp;&nbsp;unsigned int  
Description:&nbsp;&nbsp;&nbsp;sets the current value of Resistor 1 (in Ω) of the hardware voltage divider (if used)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__setRes2();__  
Parameters:&nbsp;&nbsp;&nbsp;unsigned int  
Description:&nbsp;&nbsp;&nbsp;Sets the current value of Resistor 2 (in Ω) of the hardware voltage divider (if used)  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__setAlpha();__  
Parameters:&nbsp;&nbsp;&nbsp;unsigned int  
Description:&nbsp;&nbsp;&nbsp;Sets the current value of the 'Alpha' parameter (used by the 'EMAVG' smoothing method). Acceptable range: 0-256 (attempting to set this parameter to lower/heigher values, sets actual value to minimum/maximum respectively).   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__setNumSamples();__  
Parameters:&nbsp;&nbsp;&nbsp;byte  
Description:&nbsp;&nbsp;&nbsp;Sets the current number of samples used by the 'Rolling-Average' smoothing method. Acceptable range: 1-20 samples (attempting to set this parameter to lower/heigher values, sets actual value to minimum/maximum respectively).   
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;byte  

__setVinput();__  
Parameters:&nbsp;&nbsp;&nbsp;VOLTAGE_INPUT_5V [default] / VOLTAGE_INPUT_12V  
Description:&nbsp;&nbsp;&nbsp;Sets the voltage input parameter (5V or 12V) which is used for voltage reading calculations  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None   

__setSmoothing();__  
Parameters:&nbsp;&nbsp;&nbsp;NO_SMOOTHING / ROLLING_AVERAGE / EMAVG  
Description:&nbsp;&nbsp;&nbsp;Sets the current smoothing method used for voltage reading calculations  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None     

__reset();__  
Parameters:&nbsp;&nbsp;&nbsp;None  
Description:&nbsp;&nbsp;&nbsp;Resets the device to its default settings  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;None  

__Destructor__  
If you want to destruct an instantiated MCP3221 object, you can use the following method to do so:  

```
~MCP3221 device_name();
```
>Replace '__device_name__' with the name of your MCP3221 instance.

## Extended Functionality*

(* requires an additional '\#include' of the relevant *.h file as shown in the corresponding example sketches)  

__MCP3221ComStr();__  
Parameters:&nbsp;&nbsp;&nbsp;Name of an initialized MCP3221 instance  
Description:&nbsp;&nbsp;Returns printable string containing human-friendly information about the device's latest I2C communication result  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PString  

__MCP3221InfoStr();__  
Parameters:&nbsp;&nbsp;&nbsp;Name of an initialized MCP3221 instance  
Description:&nbsp;&nbsp;Returns printable string containing detailed information about the device's current settings  
Returns:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;PString  

## RUNNING THE EXAMPLE SKETCH

1) Start the Arduino IDE and open the relevant example sketch  
1) Hook-up the MCP3221 to the Arduino as explained in the sketch's notes   
2) Upload the sketch to the Arduino  
3) Open the Serial Communications Window (make sure the baud-rate is set to 9600 or change it in the sketch to match your Serial Port's buad-rate)  

## BUG REPORTS

Please report any issues/bugs/suggestions at the [Issues](https://github.com/nadavmatalon/MCP3221/issues) section of this Github repository.

## TODO

## VERSION HISTORY

__Ver. 1.0.0__ - First release (16.10.16)  

## LICENSE

[The MIT License (MIT)](https://opensource.org/licenses/MIT)
Copyright (c) 2016 Nadav Matalon

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


