/*
    MCP3221 LIBRARY - COMPLETE USAGE EXAMPLE
    ----------------------------------------

    INTRODUCTION
    ------------

    This sketch offers a complete usage illustration, as well as a rubust testing mechanism, for the MCP3221.

    The sketch exposes all available configuration settings and data reading encapsulated within the MCP3221.

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

const byte DEV_ADDR = 0x4D;                                           // I2C address of the MCP3221 (Change if needed)

MCP3221 mcp3221(DEV_ADDR);                                            // constructs a new MCP3221 object with the relevant I2C address

void setup() {
    Serial.begin(9600);                                               // initiallizes the Serial Communications Port (at 9600bd)
    Wire.begin();                                                     // initiallizes the I2C Communications bus
    while(!Serial);                                                   // waits for Serial Port to initialize
    printDivider();
    runTests();
}

void loop() {}

void runTests() {
    Serial.print(F("\nMCP3221 LIBRARY TESTING\n"));
    testPingDevice();
    printDivider();
    Serial.print(F("\nGETTING CONFIGURATION\n"));
    testGetConfigData();
    printDivider();
    Serial.print(F("\nSETTING CONFIGURATION\n"));
    testSetConfigData();
    printDivider();
    Serial.print(F("\nGETTING RAW DATA\n"));
    testGetData();
    printDivider();
    Serial.print(F("\nGETTING VOLTAGE READINGN\n"));
    testGetVoltage();
    printDivider();
    Serial.print(F("\nTESTING DEVICE RESET\n"));
    testReset();
    printDivider();
}

void testPingDevice() {
    Serial.print(F("\nSearching for device...Device "));
    Serial.print(mcp3221.ping() ? "Not Found\n" : "Found!\n");
    quickDelay();
}

void testGetConfigData() {
    testGetVoltageReference();
    testGetSmoothingMethod();
    testGetVoltageInput();
    testGetResistorOne();
    testGetResistorTwo();
    testGetAlpha();
    testGetNumSamples();
}

void testGetVoltageReference() {
    Serial.print(F("\nVOLTAGE REFERENCE:\t"));
    Serial.print(mcp3221.getVref());
    Serial.print(F("mV\n"));
}

void testGetSmoothingMethod() {
    Serial.print(F("\nSMOOTHING METHOD:\t"));
    switch (mcp3221.getSmoothing()) {
        case (0): Serial.print(F("NO SMOOTHING\n")); break;
        case (1): Serial.print(F("ROLLING-AVERAGE\n")); break;
        case (2): Serial.print(F("EMAVG\n")); break;
    }
}

void testGetVoltageInput() {
    Serial.print(F("\nVOLTAGE INPUT:\t\t"));
    Serial.print(mcp3221.getVinput() ? (F("12V\n")) : (F("5V\n")));
 }

void testGetResistorOne() {
    Serial.print(F("\nVD RESISTOR 1:\t\t"));
    Serial.print(mcp3221.getRes1());
    Serial.print(F("R\n"));
}

void testGetResistorTwo() {
    Serial.print(F("\nVD RESISTOR 2:\t\t"));
    Serial.print(mcp3221.getRes2());
    Serial.print(F("R\n"));
}

void testGetAlpha() {
    Serial.print(F("\nALPHA:\t\t\t"));
    Serial.print(mcp3221.getAlpha());
    Serial.print(F("\n"));
}

void testGetNumSamples() {
    Serial.print(F("\nSAMPLES BUFFER:\t\t"));
    Serial.print(mcp3221.getNumSamples());
    Serial.print(F(" SAMPLES\n"));
}

void testSetConfigData() {
    testSetVoltageReference();
    testSetSmoothingMethod();
    testSetVoltageInput();
    testSetResistorOne();
    testSetResistorTwo();
    testSetAlpha();
    testSetNumSamples();
}

void testSetVoltageReference() {
    unsigned int voltageReferemces[4] = { 4650, 5700, 1170, DEFAULT_VREF };
    for (byte i=0; i<4; i++) {
        Serial.print(F("\nSetting Voltage Reference to "));
        Serial.print(voltageReferemces[i]);
        Serial.print(F("mV"));
        mcp3221.setVref(voltageReferemces[i]);
        Serial.print(F(" ... DONE\n"));
        testGetVoltageReference();
        quickDelay();
    }
}

void testSetSmoothingMethod() {
    smoothing_t smoothingMethods[3] = { NO_SMOOTHING, ROLLING_AVG, EMAVG };
    for (byte i=0; i<3; i++) {
        Serial.print(F("\nSetting Smoothing Method to "));
        switch (i) {
            case (0): Serial.print(F("NO SMOOTHING")); break;
            case (1): Serial.print(F("ROLLING-AVERAGE")); break;
            case (2): Serial.print(F("EMAVG")); break;
        }  
        mcp3221.setSmoothing(smoothingMethods[i]);
        Serial.print(F(" ... DONE\n"));
        testGetSmoothingMethod();
        quickDelay();
    }
}

void testSetVoltageInput() {
    voltage_input_t voltageInputParams[2] = { VOLTAGE_INPUT_12V, VOLTAGE_INPUT_5V };
    for (byte i=0; i<2; i++) {
        Serial.print(F("\nSetting Voltage Input to "));
        Serial.print(i ? (F("5V")) : (F("12V")));
        mcp3221.setVinput(voltageInputParams[i]);
        Serial.print(F(" ... DONE\n"));
        testGetVoltageInput();
        quickDelay();
    }
}

void testSetResistorOne() {
    unsigned int resistorOneParams[3] = { 6800, 10000, 0 };
    for (byte i=0; i<3; i++) {
        Serial.print(F("\nSetting Resistor One to "));
        Serial.print(resistorOneParams[i]);
        Serial.print(F("R"));
        mcp3221.setRes1(resistorOneParams[i]);
        Serial.print(F(" ... DONE\n"));
        testGetResistorOne();
        quickDelay();
    }
}

void testSetResistorTwo() {
    unsigned int resistorTwoParams[3] = { 2800, 4700, 0 };
    for (byte i=0; i<3; i++) {
        Serial.print(F("\nSetting Resistor Two to "));
        Serial.print(resistorTwoParams[i]);
        Serial.print(F("R"));
        mcp3221.setRes2(resistorTwoParams[i]);
        Serial.print(F(" ... DONE\n"));
        testGetResistorTwo();
        quickDelay();
    }
}
void testSetAlpha() {
    unsigned int alphaParams[4] = { 105, 0, 258, DEFAULT_ALPHA };
    for (byte i=0; i<4; i++) {
        Serial.print(F("\nSetting Alpha to "));
        Serial.print(alphaParams[i]);
        mcp3221.setAlpha(alphaParams[i]);
        Serial.print(F(" ... DONE\n"));
        testGetAlpha();
        quickDelay();
    }
}

void testSetNumSamples() {
    byte numSamplesParams[4] = { 18, 0, 21, DEFAULT_NUM_SAMPLES };
    for (byte i=0; i<4; i++) {
        Serial.print(F("\nSetting Number of Samples to "));
        Serial.print(numSamplesParams[i]);
        Serial.print(F(" Samples"));
        mcp3221.setNumSamples(numSamplesParams[i]);
        Serial.print(F("...DONE\n"));
        testGetNumSamples();
        quickDelay();
    }
}

void testGetData() {
    mcp3221.setSmoothing(NO_SMOOTHING);
    for (byte i=0; i<10; i++) {
        Serial.print(F("\nRaw Data:\t\t"));
        Serial.print(mcp3221.getData());
        Serial.print(F("\n\n"));
        longDelay();
    }
}

void testGetVoltage() {
    for (byte i=0; i<10; i++) {
        Serial.print(F("\nVoltage:\t\t"));
        Serial.print(mcp3221.getVoltage());
        Serial.print(F("mV\n\n"));
        longDelay();
    }
    mcp3221.setSmoothing(EMAVG);
}

void testReset() {
    Serial.print(F("\nCurrent Settings:\n"));
    testGetConfigData();
    Serial.print(F("\nCreating New Settings..."));
    mcp3221.setVref(5112);
    mcp3221.setSmoothing(ROLLING_AVG);
    mcp3221.setRes1(10251);
    mcp3221.setRes2(4705);
    mcp3221.setVinput(VOLTAGE_INPUT_12V);
    mcp3221.setAlpha(134);
    mcp3221.setNumSamples(16);
    Serial.print(F("DONE\n\nCurrent Settings:\n"));
    testGetConfigData();
    Serial.print(F("\nResetting Device to Default Settings..."));
    mcp3221.reset();
    Serial.print(F("DONE\n\nCurrent Settings:\n"));
    testGetConfigData();
}

void printDivider() {
    Serial.print(F("\n--------------------------------\n"));
}

void quickDelay() {
    delay(50);
}

void longDelay() {
    delay(750);
}

