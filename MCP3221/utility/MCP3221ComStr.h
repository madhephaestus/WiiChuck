/*==============================================================================================================*

     @file     MCP3221ComStr.h
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

#ifndef MCP3221ComStr_h
#define MCP3221ComStr_h

#include <avr/pgmspace.h>

namespace Mcp3221 {

    const byte COM_BUFFER_SIZE  = 60;
    const int  NUM_OF_COM_CODES =  8;

    const char comMsg0[] PROGMEM = "Success";
    const char comMsg1[] PROGMEM = "Error Code #1: I2C Buffer overflow";
    const char comMsg2[] PROGMEM = "Error Code #2: Address sent, NACK received";
    const char comMsg3[] PROGMEM = "Error Code #3: Data send, NACK received";
    const char comMsg4[] PROGMEM = "Error Code #4: Other error (bus error, etc.)";
    const char comMsg5[] PROGMEM = "Error Code #5: Timed-out while trying to become Bus Master";
    const char comMsg6[] PROGMEM = "Error Code #6: Timed-out while waiting for data to be sent";
    const char comMsgDefault[] PROGMEM = "Error Code #%d: Unlisted error";

    const char * const comCodes[NUM_OF_COM_CODES] PROGMEM = {
            comMsg0,
            comMsg1,
            comMsg2,
            comMsg3,
            comMsg4,
            comMsg5,
            comMsg6,
            comMsgDefault
    };

/*==============================================================================================================*
    GET I2C COMMUNICATIONS RESULT MESSAGE (PRINTABLE FORMAT)
 *==============================================================================================================*/

    MCP3221_PString MCP3221ComStr(const MCP3221& devParams) {
        char devComBuffer[COM_BUFFER_SIZE];
        MCP3221_PString comStr(devComBuffer, COM_BUFFER_SIZE);
        char comCodeResult = devParams._comBuffer;
        char * ptr = (char *) pgm_read_word(&comCodes[comCodeResult]);
        snprintf_P(devComBuffer, COM_BUFFER_SIZE, ptr, comCodeResult);
        return comStr;
    }

}

using namespace Mcp3221;

#endif
