/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>


// Change _ESPLOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#define _WizFiLOGLEVEL_ 3

#if defined(ARDUINO_ARCH_SAMD) 

#define LOGERROR(x)    if(_WizFiLOGLEVEL_>0) { SerialUSB.print("[WizFi250] "); SerialUSB.println(x); }
#define LOGERROR1(x,y) if(_WizFiLOGLEVEL_>2) { SerialUSB.print("[WizFi250] "); SerialUSB.print(x); SerialUSB.print(" "); SerialUSB.println(y); }
#define LOGWARN(x)     if(_WizFiLOGLEVEL_>1) { SerialUSB.print("[WizFi250] "); SerialUSB.println(x); }
#define LOGWARN1(x,y)  if(_WizFiLOGLEVEL_>2) { SerialUSB.print("[WizFi250] "); SerialUSB.print(x); SerialUSB.print(" "); SerialUSB.println(y); }
#define LOGINFO(x)     if(_WizFiLOGLEVEL_>2) { SerialUSB.print("[WizFi250] "); SerialUSB.println(x); }
#define LOGINFO1(x,y)  if(_WizFiLOGLEVEL_>2) { SerialUSB.print("[WizFi250] "); SerialUSB.print(x); SerialUSB.print(" "); SerialUSB.println(y); }

#define LOGDEBUG(x)      if(_WizFiLOGLEVEL_>3) { SerialUSB.println(x); }
#define LOGDEBUG0(x)     if(_WizFiLOGLEVEL_>3) { SerialUSB.print(x); }
#define LOGDEBUG1(x,y)   if(_WizFiLOGLEVEL_>3) { SerialUSB.print(x); SerialUSB.print(" "); SerialUSB.println(y); }
#define LOGDEBUG2(x,y,z) if(_WizFiLOGLEVEL_>3) { SerialUSB.print(x); SerialUSB.print(" "); SerialUSB.print(y); SerialUSB.print(" "); SerialUSB.println(z); }
#define LOGDEBUG_TYPE(x,y)      if(_WizFiLOGLEVEL_>3) { SerialUSB.print(x,y); }
#define LOGDEBUG_LN_TYPE(x,y)   if(_WizFiLOGLEVEL_>3) { SerialUSB.println(x,y); }
#else
	
#define LOGERROR(x)    if(_WizFiLOGLEVEL_>0) { Serial.print(F("[WizFi250] ")); Serial.println(x); }
#define LOGERROR1(x,y) if(_WizFiLOGLEVEL_>2) { Serial.print(F("[WizFi250] ")); Serial.print(x); Serial.print(" "); Serial.println(y); }
#define LOGWARN(x)     if(_WizFiLOGLEVEL_>1) { Serial.print(F("[WizFi250] ")); Serial.println(x); }
#define LOGWARN1(x,y)  if(_WizFiLOGLEVEL_>2) { Serial.print(F("[WizFi250] ")); Serial.print(x); Serial.print(" "); Serial.println(y); }
#define LOGINFO(x)     if(_WizFiLOGLEVEL_>2) { Serial.print(F("[WizFi250] ")); Serial.println(x); }
#define LOGINFO1(x,y)  if(_WizFiLOGLEVEL_>2) { Serial.print(F("[WizFi250] ")); Serial.print(x); Serial.print(" "); Serial.println(y); }

#define LOGDEBUG(x)      if(_WizFiLOGLEVEL_>3) { Serial.println(x); }
#define LOGDEBUG0(x)     if(_WizFiLOGLEVEL_>3) { Serial.print(x); }
#define LOGDEBUG1(x,y)   if(_WizFiLOGLEVEL_>3) { Serial.print(x); Serial.print(F(" ")); Serial.println(y); }
#define LOGDEBUG2(x,y,z) if(_WizFiLOGLEVEL_>3) { Serial.print(x); Serial.print(F(" ")); Serial.print(y); Serial.print(F(" ")); Serial.println(z); }
#define LOGDEBUG_TYPE(x,y)      if(_WizFiLOGLEVEL_>3) { Serial.print(x,y); }
#define LOGDEBUG_LN_TYPE(x,y)   if(_WizFiLOGLEVEL_>3) { Serial.println(x,y); }

#endif // defined(ARDUINO_ARCH_SAMD) 

#endif // __DEBUG_H__
