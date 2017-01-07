/* 
  MCP3221 LIBRARY - DEVICE INFORMATION STRING EXAMPLE
  ---------------------------------------------------

  INTRODUCTION
  ------------
  This sketch presents a minimal example of extending the MCP3221 Library to include an additional function for generating a printable device
  information string which may be useful, for example, during debugging sessions.

  As can be seen in the sketch below, implementation of this extended functionality only requires adding a single 'include' to the code, namely: 
  to that of the relevant *.h file (i.e. '/utility/MCP3221InfoStr.h').
  
  Note that this functional extension does come at the cost of an increased memory usage, and therefore it seemed preferable to maintain it 
  as an optional add-on rather than include it in the core MCP3221 Library itself.
  
  WIRING DIAGRAM
  --------------
                                       MCP3221
                                       -------
                                VCC --| •     |-- SCL
                                      |       |
                                GND --|       |
                                      |       |
                                AIN --|       |-- SDA
                                       -------

  PIN 1 (VCC/VREF) - Serves as both Power Supply input and Voltage Reference for the ADC. Connect to Arduino 5V output or any other
                equivalent power source (5.5V max). If using an external power source, remember to connect all GND's together
  PIN 2 (GND) - connect to Arduino GND
  PIN 3 (AIN) - This pin can be left unconnected for the purpose of this sketch
  PIN 4 (SDA) - Connect to Arduino's PIN A4 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
  PIN 5 (SCL) - Connect to Arduino's PIN A5 with a 2K2 (400MHz I2C Bus speed) or 10K (100MHz I2C Bus speed) pull-up resistor
  DECOUPING:    Minimal decoupling consists of a 0.1uF Ceramic Capacitor between the VCC & GND PINS. For improved performance,
                add a 1uF and a 10uF Ceramic Capacitors as well across these pins

  I2C ADDRESSES
  -------------
  Each MCP3221 has 1 of 8 possible I2C addresses (factory hardwired & recognized by its specific part number & top marking
  on the package itself):

       PART                  DEVICE I2C ADDRESS          PART
      NUMBER             (BIN)      (HEX)     (DEC)     MARKING
  MCP3221A0T-E/OT       01001000      0x48       72       GE
  MCP3221A1T-E/OT       01001001      0x49       73       GH
  MCP3221A2T-E/OT       01001010      0x4A       74       GB
  MCP3221A3T-E/OT       01001011      0x4B       75       GC
  MCP3221A4T-E/OT       01001100      0x4C       76       GD
  MCP3221A5T-E/OT       01001101      0x4D       77       GA
  MCP3221A6T-E/OT       01001110      0x4E       78       GF
  MCP3221A7T-E/OT       01001111      0x4F       79       GG

  BUG REPORTS
  -----------
  Please report any bugs/issues/suggestions at the GITHUB Repository of this library at: https://github.com/nadavmatalon/MCP3221

  LICENSE
  -------

  The MIT License (MIT)
  Copyright (c) 2016 Nadav Matalon
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without
  limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
  conditions:
  
  The above copyright notice and this permission notice shall be included in all copies or substantial
  portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "MCP3221.h"
#include "utility/MCP3221InfoStr.h"

const byte DEV_ADDR = 0x4D;                       // I2C address of the MCP3221 (Change as needed)

MCP3221 mcp3221(DEV_ADDR);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    while(!Serial);
    Serial.print(F("\n\nCURRENT SETTINGS:\n"));
    Serial.print(MCP3221InfoStr(mcp3221));
    Serial.print(F("\nCHANGING TO NEW SETTINGS..."));
    mcp3221.setVref(5112);
    mcp3221.setSmoothing(ROLLING_AVG);
    mcp3221.setRes1(10251);
    mcp3221.setRes2(4705);
    mcp3221.setVinput(VOLTAGE_INPUT_12V);
    mcp3221.setAlpha(134);
    mcp3221.setNumSamples(16);
    Serial.print(F("DONE\n"));
    Serial.print(MCP3221InfoStr(mcp3221));
    Serial.print(F("\nRESETTING DEVICE..."));
    mcp3221.reset();
    Serial.print(F("DONE\n"));
    Serial.print(MCP3221InfoStr(mcp3221));
    Serial.print(F("\n\n"));
}

void loop() {}