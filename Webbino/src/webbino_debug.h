/***************************************************************************
 *   This file is part of Webbino                                          *
 *                                                                         *
 *   Copyright (C) 2012-2016 by SukkoPera                                  *
 *                                                                         *
 *   Webbino is free software: you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Webbino is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Webbino. If not, see <http://www.gnu.org/licenses/>.       *
 ***************************************************************************/

#ifndef _WEBBINO_DEBUG_H_
#define _WEBBINO_DEBUG_H_

#include "webbino_config.h"

#ifndef WEBBINO_NDEBUG
	#define DPRINT(...) Serial.print(__VA_ARGS__)
	#define DPRINTLN(...) Serial.println(__VA_ARGS__)
#else
	#define DPRINT(...)
	#define DPRINTLN(...)
#endif

#endif

// FIXME: This should probably be moved somewhere else
#ifdef ENABLE_FLASH_STRINGS

#if defined (ARDUINO_ARCH_AVR)
#include <avr/pgmspace.h>
#endif

#define PSTR_TO_F(s) reinterpret_cast<const __FlashStringHelper *> (s)
#define F_TO_PSTR(s) reinterpret_cast<PGM_P> (s)
#define FlashString const __FlashStringHelper *

#else

#warning Flash strings disabled

#undef PSTR
#define PSTR(s) s

#undef F
#define F(s) s

#define PSTR_TO_F(s) s
#define F_TO_PSTR(s) s

#define FlashString const char *

#undef strlen_P
#define strlen_P strlen
#undef strcmp_P
#define strcmp_P strcmp
#undef strcat_P
#define strcat_P strcat
#undef strncpy_P
#define strncpy_P strncpy
#undef strncmp_P
#define strncmp_P strncmp

#endif

// Use to mark unused function parameters
#define _UNUSED __attribute__ ((unused))
