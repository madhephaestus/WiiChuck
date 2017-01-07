/* 
  MCP3221 LIBRARY - BASIC DEVICE TESTING EXAMPLE
  ----------------------------------------------
  
  INTRODUCTION
  ------------
  This sketch offers a quick & simple code for testing that the MCP3221 is hooked-up and operating correctly.

  The sketch begins by searching for the MCP3221 on the I2C Bus and then moves on to get a voltage reading from the device.

  INPORTANT: The current library depends on the Arduino IDE's native 'Wire' library for I2C communication between the Arduino (Master) and the MCP3221 (Slave).

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
  PIN 3 (AIN) - Connect to Arduino's 3.3V Output
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

const byte DEV_ADDR = 0x4D;                       // I2C address of the MCP3221 (Change as needed)

MCP3221 mcp3221(DEV_ADDR);

void setup() {
    Serial.begin(9600);
    Wire.begin();
    while(!Serial);
    printDivider();
    Serial.print(F("\nMCP3221 DEVICE TESTING\n"));
    printDivider();
    Serial.print(F("\nINITIATING SERIAL COMMUNICATION\n"));  
    Serial.print(F("\nSerial Port is "));
    Serial.print(Serial ? "Open\n" : "Could not be opened\n"); 
    printDivider();
    Serial.print(F("\nINITIALIZING TESTS\n"));
    testPingDevice();
    testGetReading();
}

void loop() {}

void testPingDevice() {
    Serial.print(F("\nSearching for device...Device "));
    Serial.print(mcp3221.ping() ? (F("Not Found\n")) : (F("Found!\n")));
    printDivider(); 
}

void testGetReading() {
    unsigned int reading = mcp3221.getVoltage();
    Serial.print(F("\n\nVoltage:  "));
    Serial.print(reading);
    Serial.print(F("mV\n\n"));
}

void printDivider() {
    Serial.print(F("\n-----------------------------------------\n"));
}
