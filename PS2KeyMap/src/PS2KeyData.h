/* Version V1.0.2
  PS2KeyMap.h - PS2KeyAdvanced library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
  Created September 2014
  Updated January 2016 - Paul Carpenter - add tested on Due and tidy ups for V1.5 Library Management

  PRIVATE to library data and keymapping tables

  This library REQUIRES PS2KeyAdvanced and PS2KeyMap.h as the codes used to
  remap to ASCII/UTF-8 are specific to that library to match ALL keys on a keyboard

  To create your own map to ADD to this library see the readme.txt file in
  the library directory

  See PS2KeyAvanced.h for returned definitions of Keys and accessible
  definitions

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef PS2KeyData_h
#define PS2KeyData_h

/* Standard ASCII/UTF-8 control Characters */
#define PS2_BACKSPACE   0x08
#define PS2_TAB			0x09
#define PS2_ENTER		0x0D
#define PS2_ESC			0x1B
#define PS2_DELETE		0x7F
#define PS2_SPACE		0x20


/* Standard ASCII control characters array */
/* in order of PS2_KEY_* values order is important */
#if defined(ARDUINO_ARCH_AVR)
const uint8_t PROGMEM _control_codes[] = {
#elif defined(ARDUINO_ARCH_SAM)
const uint8_t _control_codes[] = {
#endif
                PS2_DELETE, PS2_ESC, PS2_BACKSPACE,
                PS2_TAB, PS2_ENTER, PS2_SPACE
                };

// convert codes based on SHIFT and not SHIFT only for base US-ASCII
#if defined(ARDUINO_ARCH_AVR)
const uint16_t PROGMEM _US_ASCII[][ 2 ] = {
#elif defined(ARDUINO_ARCH_SAM)
const uint16_t _US_ASCII[][ 2 ] = {
#endif
                {  PS2_SHIFT + PS2_KEY_1, '!' },
                {  PS2_SHIFT + PS2_KEY_2, '@' },
                {  PS2_SHIFT + PS2_KEY_3, '#' },
                {  PS2_SHIFT + PS2_KEY_4, '$' },
                {  PS2_SHIFT + PS2_KEY_5, '%' },
                {  PS2_SHIFT + PS2_KEY_6, '^' },
                {  PS2_SHIFT + PS2_KEY_7, '&' },
                {  PS2_SHIFT + PS2_KEY_8, '*' },
                {  PS2_SHIFT + PS2_KEY_9, '(' },
                {  PS2_SHIFT + PS2_KEY_0, ')' },
                {  PS2_KEY_KP_DOT, '.' },
                {  PS2_KEY_KP_ENTER, PS2_ENTER },
                {  PS2_KEY_KP_PLUS, '+' },
                {  PS2_KEY_KP_MINUS, '-' },
                {  PS2_KEY_KP_TIMES, '*' },
                {  PS2_KEY_KP_DIV, '/' },
                {  PS2_KEY_KP_COMMA, ',' },
                {  PS2_KEY_KP_EQUAL, '=' },
                {  PS2_KEY_APOS, '\'' },
                {  PS2_SHIFT + PS2_KEY_APOS, '"' },
                {  PS2_KEY_COMMA, ',' },
                {  PS2_SHIFT + PS2_KEY_COMMA, '<' },
                {  PS2_KEY_MINUS, '-' },
                {  PS2_SHIFT + PS2_KEY_MINUS, '_' },
                {  PS2_KEY_DOT, '.' },
                {  PS2_SHIFT + PS2_KEY_DOT, '>' },
                {  PS2_KEY_DIV, '/' },
                {  PS2_SHIFT + PS2_KEY_DIV, '?' },
                {  PS2_KEY_SINGLE, '`' },
                {  PS2_SHIFT + PS2_KEY_SINGLE, '~' },
                {  PS2_KEY_SEMI, ';' },
                {  PS2_SHIFT + PS2_KEY_SEMI, ':' },
                {  PS2_KEY_BACK, '\\' },
                {  PS2_SHIFT + PS2_KEY_BACK, '|' },
                {  PS2_KEY_OPEN_SQ, '[' },
                {  PS2_SHIFT + PS2_KEY_OPEN_SQ, '{' },
                {  PS2_KEY_CLOSE_SQ, ']' },
                {  PS2_SHIFT + PS2_KEY_CLOSE_SQ, '}' },
                {  PS2_KEY_EQUAL, '=' },
                {  PS2_SHIFT + PS2_KEY_EQUAL, '+' }
                };

#if defined(ARDUINO_ARCH_AVR)
const uint16_t PROGMEM _UKmap[][ 2 ] = {
#elif defined(ARDUINO_ARCH_SAM)
const uint16_t _UKmap[][ 2 ] = {
#endif
                { PS2_SHIFT + '@', '"' },
                { PS2_SHIFT + '"', '@' },
                { PS2_ALT_GR + '`', PS2_BROKEN_BAR },
                { PS2_SHIFT + '`', PS2_NOT_SIGN },
                { PS2_SHIFT + '#', PS2_POUND_SIGN },
                { '\\', '#' },
                { PS2_SHIFT + '|', '~' },
                { PS2_KEY_EUROPE2, '\\' },
                { PS2_SHIFT + PS2_KEY_EUROPE2, '|' },
                };
#ifdef FRENCH
#if defined(ARDUINO_ARCH_AVR)
const uint16_t PROGMEM _FRmap[][ 2 ] = {
#elif defined(ARDUINO_ARCH_SAM)
const uint16_t _FRmap[][ 2 ] = {
#endif
                { '`', PS2_SUPERSCRIPT_TWO },
                { PS2_SHIFT + '~', PS2_SUPERSCRIPT_TWO },
                { 'q', 'a' },
                { 'Q', 'A' },
                { '1', '&' },
                { PS2_SHIFT + '!', '1' },
                { PS2_ALT_GR + '1', 0 },
                { PS2_ALT_GR + PS2_SHIFT + '!', 0 },
                { 'z', 'w' },
                { 'Z', 'W' },
                { 'a', 'q' },
                { 'A', 'Q' },
                { 'w', 'z' },
                { 'W', 'Z' },
                { PS2_ALT_GR + '2', '~' },
                { PS2_SHIFT + PS2_ALT_GR + '@', '~' },
                { '2', PS2_e_ACUTE },
                { PS2_SHIFT + '@', '2' },
                { PS2_ALT_GR + '3', PS2_CENT_SIGN },
                { PS2_SHIFT + PS2_ALT_GR + '#', PS2_CENT_SIGN },
                { '3', '"' },
                { PS2_SHIFT + '#', '3' },
                { PS2_ALT_GR + '4', '{' },
                { PS2_SHIFT + PS2_ALT_GR + '$', '{' },
                { '4', '\'' },
                { PS2_SHIFT + '$', '4' },
                { PS2_ALT_GR + '5', '[' },
                { PS2_SHIFT + PS2_ALT_GR + '%', '[' },
                { '5', '(' },
                { PS2_SHIFT + '%', '5' },
                { PS2_ALT_GR + '6', '|' },
                { PS2_SHIFT + PS2_ALT_GR + '^', '|' },
                { '6', '-' },
                { PS2_SHIFT + '^', '6' },
                { 'm', ',' },
                { 'M', '?' },
                { PS2_ALT_GR + '7', '`' },
                { PS2_SHIFT + PS2_ALT_GR + '&', '`' },
                { '7', PS2_e_GRAVE },
                { PS2_SHIFT + '&', '7' },
                { PS2_ALT_GR + '8', '\\' },
                { PS2_SHIFT + PS2_ALT_GR + '*', '\\' },
                { '8', '_' },
                { PS2_SHIFT + '*', '8' },
                { ',', ';' },
                { PS2_SHIFT + '<', '.' },
                { PS2_ALT_GR + '9', '^' },
                { PS2_SHIFT + PS2_ALT_GR + '(', '^' },
                { '9', PS2_c_CEDILLA },
                { PS2_SHIFT + '(', '9' },
                { PS2_ALT_GR + '0', '@' },
                { PS2_ALT_GR + ')', '@' },
                { '0', PS2_a_GRAVE },
                { PS2_SHIFT + ')', '0' },
                { '.', ':' },
                { PS2_SHIFT + '>', '/' },
                { '/', '!' },
                { PS2_SHIFT + '?', PS2_SECTION_SIGN },
                { ';', 'm' },
                { PS2_SHIFT + ':', 'M' },
                { PS2_ALT_GR + '-', ']' },
                { PS2_SHIFT + PS2_ALT_GR + '_', ']' },
                { '-', ')' },
                { PS2_SHIFT + '_', PS2_DEGREE_SIGN },
                { '\'', PS2_u_GRAVE },
                { PS2_SHIFT + '"', '%' },
                { PS2_ALT_GR + '\'', 0 },
                { PS2_SHIFT + PS2_ALT_GR + '"', 0 },
                { '[', '^' },
                { PS2_SHIFT + '{', PS2_DIAERESIS },
                { PS2_ALT_GR + '=', '}' },
                { PS2_SHIFT + PS2_ALT_GR + '+', '}' },
                { PS2_ALT_GR + ']', PS2_CURRENCY_SIGN },
                { PS2_SHIFT + PS2_ALT_GR + '}', 0 },
                { ']', '$' },
                { PS2_SHIFT + '}', PS2_POUND_SIGN },
                { '\\', '*' },
                { PS2_SHIFT + '|', PS2_MICRO_SIGN },
                { PS2_SHIFT + PS2_ALT_GR +'|', 0 },
                { PS2_ALT_GR +'\\', 0 },
                { PS2_KEY_EUROPE2 + PS2_SHIFT + PS2_ALT_GR, '|' },
                { PS2_KEY_EUROPE2 + PS2_ALT_GR, '|' },
                { PS2_SHIFT + PS2_KEY_EUROPE2, '>' },
                { PS2_KEY_EUROPE2, '<' }
                };
#endif
#ifdef GERMAN
#if defined(ARDUINO_ARCH_AVR)
const uint16_t PROGMEM _DEmap[][ 2 ] = {
#elif defined(ARDUINO_ARCH_SAM)
const uint16_t _DEmap[][ 2 ] = {
#endif
                { '`', '^' },
                { PS2_SHIFT + '~', PS2_DEGREE_SIGN },
                { PS2_ALT_GR + 'q', '@' },
                { PS2_ALT_GR + 'Q', '@' },
                { 'y', 'z' },
                { 'Y', 'Z' },
                { PS2_ALT_GR + '2', PS2_SUPERSCRIPT_TWO },
                { PS2_SHIFT + PS2_ALT_GR + '@', PS2_SUPERSCRIPT_TWO },
                { PS2_SHIFT + '@', '"' },
                { PS2_ALT_GR + 'e', PS2_CURRENCY_SIGN },
                { PS2_ALT_GR + 'E', PS2_CURRENCY_SIGN },
                { PS2_ALT_GR + '3', PS2_SUPERSCRIPT_THREE },
                { PS2_SHIFT + PS2_ALT_GR + '#', PS2_SUPERSCRIPT_THREE },
                { PS2_SHIFT + '#', PS2_SECTION_SIGN },
                { 'z', 'y' },
                { 'Z', 'Y' },
                { PS2_SHIFT + '^', '&' },
                { PS2_ALT_GR + 'm', PS2_MICRO_SIGN },
                { PS2_ALT_GR + 'M', PS2_MICRO_SIGN },
                { PS2_SHIFT + '&', '/' },
                { PS2_ALT_GR + '7', '{' },
                { PS2_SHIFT + PS2_ALT_GR + '&', '{' },
                { PS2_SHIFT + '*', '(' },
                { PS2_ALT_GR + '8', '[' },
                { PS2_SHIFT + PS2_ALT_GR + '*', '[' },
                { PS2_SHIFT + '<', ';' },
                { PS2_SHIFT + ')', '=' },
                { PS2_ALT_GR + '0', '}' },
                { PS2_SHIFT + PS2_ALT_GR + ')', '}' },
                { PS2_SHIFT + '(', ')' },
                { PS2_ALT_GR + '9', ']' },
                { PS2_SHIFT + PS2_ALT_GR + '(', ']' },
                { PS2_SHIFT + '>', ':' },
                { ';', PS2_o_DIAERESIS },
                { PS2_SHIFT + ':', PS2_O_DIAERESIS },
                { PS2_ALT_GR + '-',  '\\' },
                { PS2_SHIFT + PS2_ALT_GR + '_',  '\\' },
                { '-', PS2_SHARP_S },
                { PS2_SHIFT + '_', '?' },
                { '\'', PS2_a_DIAERESIS },
                { PS2_SHIFT + '"', PS2_A_DIAERESIS },
                { '[', PS2_u_DIAERESIS },
                { PS2_SHIFT + '{', PS2_U_DIAERESIS },
                { '=', '\'' },
                { PS2_SHIFT + '+', '`' },
                { ']' + PS2_ALT_GR, '~' },
                { PS2_SHIFT + '}' + PS2_ALT_GR, '~' },
                { ']', '+' },
                { PS2_SHIFT + '}', '*' },
                { '\\', '#' },
                { PS2_SHIFT + '|', '\'' },
                { PS2_KEY_EUROPE2 + PS2_SHIFT + PS2_ALT_GR, '|' },
                { PS2_KEY_EUROPE2 + PS2_ALT_GR, '|' },
                { PS2_SHIFT + PS2_KEY_EUROPE2, '>' },
                { PS2_KEY_EUROPE2, '<' }
                };
#endif
#ifdef SPECIAL
#if defined(ARDUINO_ARCH_AVR)
const uint16_t PROGMEM _SpecialMap[][ 2 ] = {
#elif defined(ARDUINO_ARCH_SAM)
const uint16_t _SpecialMap[][ 2 ] = {
#endif
// Insert your Special mapping DIFFERENCES from US-ASCII here
                };
#endif

// The following structures define the key maps available
typedef struct  {
                char        name[ 3 ];      // 2 Character ISO country code
                uint8_t     size;           // Number of entries in map array
                uint16_t    *map;           // Map array pointer
                } PS2Advmap;

// Actual map structure array ( 2 entries for GB and UK for same map
// Because many people dont know ISO code for UK is GB )
// Actually held in RAM but small in size
const PS2Advmap _KeyMaps[] = {
                { "US", sizeof( _US_ASCII ) / ( 2 * sizeof( uint16_t ) ), (uint16_t *)_US_ASCII },
#ifdef FRENCH
                { "FR", sizeof( _FRmap ) / ( 2 * sizeof( uint16_t ) ), (uint16_t *)_FRmap },
#endif
#ifdef GERMAN
                { "DE", sizeof( _DEmap ) / ( 2 * sizeof( uint16_t ) ), (uint16_t *)_DEmap },
#endif
#ifdef SPECIAL
                { "--", sizeof( _SpecialMap ) / ( 2 * sizeof( uint16_t ) ), (uint16_t* )_SpecialMap },
#endif
                { "UK", sizeof( _UKmap ) / ( 2 * sizeof( uint16_t ) ), (uint16_t *)_UKmap },
                { "GB", sizeof( _UKmap ) / ( 2 * sizeof( uint16_t ) ), (uint16_t *)_UKmap }
                };

#endif
