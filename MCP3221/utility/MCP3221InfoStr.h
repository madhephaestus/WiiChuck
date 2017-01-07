/*==============================================================================================================*
 
    @file     MCP3221InfoStr.h
    @author   Nadav Matalon
    @license  MIT (c) 2016 Nadav Matalon

    MCP3221 Driver (12-BIT Single Channel ADC with I2C Interface)

    Ver. 1.0.0 - First release (16.10.16)

 *===============================================================================================================*
    LICENSE
 *===============================================================================================================*
 
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
 
 *==============================================================================================================*/

#if 1
__asm volatile ("nop");
#endif

#ifndef MCP3221InfoStr_h
#define MCP3221InfoStr_h

#include <avr/pgmspace.h>
#include "utility/MCP3221ComStr.h"

namespace Mcp3221 {

    const int  INFO_BUFFER_SIZE = 60;
    const byte NUM_OF_INFO_STR  = 12;

    const char infoStr0[]  PROGMEM = "\nMCP3221 DEVICE INFORMATION";
    const char infoStr1[]  PROGMEM = "\n--------------------------";
    const char infoStr2[]  PROGMEM = "\nI2C ADDRESS:\t   %d (%#X)";
    const char infoStr3[]  PROGMEM = "\nI2C COM STATUS:\t   %sCONNECTED";
    const char infoStr4[]  PROGMEM = "\nVOLTAGE REFERENCE: %dmV";
    const char infoStr5[]  PROGMEM = "\nSMOOTHING METHOD:  %s";
    const char infoStr6[]  PROGMEM = "\nVOLTAGE INPUT:\t   %dV";
    const char infoStr7[]  PROGMEM = "\nVD RESISTOR 1:\t   %dR";
    const char infoStr8[]  PROGMEM = "\nVD RESISTOR 2:\t   %dR";
    const char infoStr9[]  PROGMEM = "\nALPHA:\t\t   %d";
    const char infoStr10[] PROGMEM = "\nSAMPLES BUFFER:\t   %d SAMPLES\n";
    const char errStr[]    PROGMEM = "\nI2C ERROR:\t ";

    const char * const infoStrs[NUM_OF_INFO_STR] PROGMEM = {
        infoStr0,
        infoStr1,
        infoStr2,
        infoStr3,
        infoStr4,
        infoStr5,
        infoStr6,
        infoStr7,
        infoStr8,
        infoStr9,
        infoStr10,
        errStr
    };

/*==============================================================================================================*
    GENERATE DEVICE INFORMATION STRING (PRINTABLE FORMAT)
 *==============================================================================================================*/

    MCP3221_PString MCP3221InfoStr(const MCP3221& devParams) {
        char * ptr;
        char strBuffer[360];    // 338
        char devInfoBuffer[INFO_BUFFER_SIZE];
        MCP3221_PString resultStr(strBuffer, sizeof(strBuffer));
        MCP3221 mcp3221(devParams._devAddr);
        byte comErrCode = mcp3221.ping();
        unsigned int res1 = mcp3221.getRes1();
        unsigned int res2 = mcp3221.getRes2();
        
        for (byte i=0; i<4; i++) {
            ptr = (char *) pgm_read_word(&infoStrs[i]);
            if (i < 2)   snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr);
            if (i == 2)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devParams._devAddr, devParams._devAddr);
            if (i == 3)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (comErrCode ? "NOT " : ""));
            resultStr += devInfoBuffer;
        }
        if (!comErrCode) {
            for (byte i=4; i<(NUM_OF_INFO_STR - 1); i++) {
                ptr = (char *) pgm_read_word(&infoStrs[i]);
                if (i == 4)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devParams._vRef);
                if (i == 5)  switch (devParams._smoothing) {
                                case (NO_SMOOTHING): snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, "NO SMOOTHING"); break;
                                case (ROLLING_AVG):  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, "ROLLING-AVAREGE"); break;
                                case (EMAVG):        snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, "EMAVG"); break;
                            }
                if (i == 6)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, (devParams._voltageInput ? 12 : 5));
                if (i == 7)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devParams._res1);
                if (i == 8)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devParams._res2);
                if (i == 9)  snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devParams._alpha);
                if (i == 10) snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, ptr, devParams._numSamples);
                if (i != 11) resultStr += devInfoBuffer;
            }
        } else {
            snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, (char *) pgm_read_word(&infoStrs[11]));
            resultStr += devInfoBuffer;
            snprintf_P(devInfoBuffer, INFO_BUFFER_SIZE, (char *) pgm_read_word(&comCodes[comErrCode]));
            resultStr += devInfoBuffer;
            resultStr += "\n";
        }
        return resultStr;
    }
}

using namespace Mcp3221;

#endif
