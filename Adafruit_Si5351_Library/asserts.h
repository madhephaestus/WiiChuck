/**************************************************************************/
/*!
    @file     asserts.h
    @author   K. Townsend (Adafruit Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2014, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _ASSERTS_H_
#define _ASSERTS_H_

#include "errors.h"

/**************************************************************************/
/*!
    @brief Checks the condition, and if the assert fails the supplied
           returnValue will be returned in the calling function.

    @code
    // Make sure 'addr' is within range
    ASSERT(addr <= MAX_ADDR, ERROR_ADDRESSOUTOFRANGE);
    @endcode
*/
/**************************************************************************/
#define ASSERT(condition, returnValue) \
        do{\
          if (!(condition)) {\
            return (returnValue);\
          }\
        }while(0)

/**************************************************************************/
/*!
    @brief  Checks the supplied \ref err_t value (sts), and if it is
            not equal to \ref ERROR_NONE the sts value will be returned.

    @details
    This macro is useful to check if a function returned an error without
    bloating your own code with endless "if (error) {...}".

    @code
    // If anything other than ERROR_NONE is returned by tsl2561Enable()
    // this macro will log the error and exit the function returning the
    // error_t value.
    ASSERT_STATUS(tsl2561Enable());
    @endcode
*/
/**************************************************************************/
#define ASSERT_STATUS(sts) \
        do{\
          err_t status = (sts);\
          if (ERROR_NONE != status) {\
            return status;\
          }\
        } while(0)

#endif
