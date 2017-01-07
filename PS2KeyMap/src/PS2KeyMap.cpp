/* Version V1.0.2
  PS2KeyMap.cpp - PS2KeyMap library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
  Created September 2014
  Updated January 2016 - Paul Carpenter - add tested on Due and tidy ups for V1.5 Library Management

  IMPORTANT WARNING

    If using a DUE or similar board with 3V3 I/O you MUST put a level translator
    like a Texas Instruments TXS0102 or FET circuit as the signals are
    Bi-directional (signals transmitted from both ends on same wire).

    Failure to do so may damage your Arduino Due or similar board.

  Test History
    September 2014 Uno and Mega 2560 September 2014 using Arduino V1.6.0
    January 2016   Uno, Mega 2560 and Due using Arduino 1.6.7 and Due Board
                    Manager V1.6.6

  This is for a LATIN style keyboard. Currently Supports
    US - Default
    UK - By selecting with string "UK" or "GB"
    DE - German if define GERMAN exists
    FR - French if define FRENCH exists
    --   Special if you have created your own mapping see PS2KeyMap.h

  US and UK mappings are base layouts always compiled. All mappings are done
  based on US mapping with CHANGE information to reduce storage space.

  To add other mappings

  Works with PS2KeyAdvanced library ONLY as this is an extension of that
  library.

  REQUIRES PS2KeyAdvanced library BEFORE THIS ONE

  To reduce flash memory footprint comment out any of the following in
  PSKeyMap.h and recompile

    // Uncomment any of following defines to include those mappings
    // or comment to exclude
    //#define GERMAN
    //#define FRENCH
    //#define SPECIAL

  Library converts key codes from PS2KeyAdvanced to enable full ASCCII/UTF-8
  codes depending on language of keyboard and all the modifier keys or any
  combination of them e.g.
        SHIFT
        CTRL
        ALT
        ALT GR (Right ALT)
        GUI  (Windows key)

  Remap functions convert to USASCII and US keyboard first, then if an
  additional map is selected, scans that table for the few keys that are
  different. Making additional keyboard mappings easier, as well as
  enabling special key combinations to produce special codes. Also you
  can ignore key combinations.

  For example you could specify that CTRL + ENTER key produces the linefeed
  code, by adding one entry in a mapping table.

  Allows tables for any keyboard layout and special cases to be added
  Returns either uint8_t or uint16_t depending on which function is used

  When returning a uint8_t (byte) the code ranges are
        0       invalid/error
        1-1F    ASCII Control Codes (BS, TAB, ESC, ENTER, SPACE)
        20-7F   ASCII Printable characters and DEL
        80-9F   UTF-8 Control codes (none in base library)
        A0-FF   UTF-8 1 byte printable characters (See PS2KeyMap.h)

  When return an uint16_t containing
        Make/Break status
        Caps status
        Shift, CTRL, ALT, ALT GR, GUI keys
        Flag for function key not a displayable/printable character
        8 bit key code

    Top Byte is 8 bits denoting as follows with defines for bit code
        (see PS2KeyAdvanced.h for definitions)

        Define name bit     description
        PS2_BREAK   15      1 = Break key code
                   (MSB)    0 = Make Key code
        PS2_SHIFT   14      1 = Shift key pressed as well (either side)
                            0 = NO shift key
        PS2_CTRL    13      1 = Ctrl key pressed as well (either side)
                            0 = NO Ctrl key
        PS2_CAPS    12      1 = Caps Lock ON
                            0 = Caps lock OFF
        PS2_ALT     11      1 = Left Alt key pressed as well
                            0 = NO Left Alt key
        PS2_ALT_GR  10      1 = Right Alt (Alt GR) key pressed as well
                            0 = NO Right Alt key
        PS2_GUI      9      1 = GUI key pressed as well (either)
                            0 = NO GUI key
        PS2_FUNCTION 8      1 = FUNCTION key non-printable character (plus space, tab, enter)
                            0 = standard character key

  Code Ranges (bottom byte of uint16_t)
     When bit 8 = 0
        0       invalid/error
        1-1F    ASCII Control Codes (BS, TAB, ESC, ENTER, SPACE)
        20-7F   ASCII Printable characters and DEL
        80-9F   UTF-8 Control codes (none in base library)
        A0-FF   UTF-8 1 byte printable characters (See PS2KeyMap.h)

      When bit 8 = 1 Function keys see PS2KeyAdvanced for coding details)
        0       invalid/error
        1-19    Functions PGUP PGDN etc..
        1A-60   NOT RETURNED
        61-9F   Function keys and other special keys (plus F2 and F1)
                    61-78 F1 to F24
                    79-8A Multimedia
                    8B-8D ACPI power
                    90-9F Special multilingual
        A0-FF   Keyboard communications commands (note F2 and F1 are special
                codes for special multi-lingual keyboards)

  Error Codes
     Most functions return 0 or 0xFFFF as error, other codes to note and
     handle appropriately for bottom byte -
        0xAA   keyboard has reset and passed power up tests
               will happen if keyboard plugged in after code start
        0xFC   Keyboard General error or power up fail

  Functions contained are
    selectMap()  Pass in 2 digit ISO country code to select map if loaded
                 returns 0 for done or 1 for not found.

    remapKey()  Pass in unsigned int returned from PS2KeyAdvanced
                Returns 0 for error
                        unsigned int as described above
    remapKeyByte() Returns uint8_t version of remapKey ONLY for standard ASCII/UTF-8 codes
                   Invalid codes returned as 0

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

/* To Add a key mapping for one of the below add the line to YOUR CODE
   BEFORE including PS2KeyMap.h see examples for this library

    #define FRENCH
    #define GERMAN

   Add following line when you add you own map see #ifdef SPECIAL sections
   Selected by country code of "--" when created and enabled

     #define SPECIAL
*/

/*------------------ Code starts here -------------------------*/
#include <PS2KeyAdvanced.h>
#include "PS2KeyMap.h"
#include "PS2KeyData.h"

uint8_t _selected_map;

// Private country code strings
const char *_USStr[] = { "US" };


/* Common function to scan a Look up table table and
   return a value or 0 for not found

   Checks using default mask of keycode plus ALT_GR and SHIFT
   To add in match other modifiers set mask to combination of
        PS2_CTRL        match either Control key as well
        PS2_ALT         match left ALT as well
        PS2_GUI         match either GUI key as well

   Assumes map table has 2 dimensions of type prog_uint16_t (in Flash memory)
   i.e. an array like test[ ][ 2 ]
        First entry  (test[][ 0 ]) is item to match
        Second entry (test[][ 1 ]) is item to return

   Parameters are
        data    unsigned int 16 from PS2KeyAdvanced library
        index   index of mapping table to use
*/
uint16_t scan_map( uint16_t data, uint8_t index )
{
uint16_t idx, result, size;
uint16_t *maparray;

// Convert entries count to number of ints
size = ( _KeyMaps[ index ].size ) << 1;
maparray = ( uint16_t *)_KeyMaps[ index ].map;
// Leave only required bits for checking against
result = data & ( PS2_ALT_GR + PS2_SHIFT + 0xFF );

// scan Lookup Table (array) jumping 2 integers at a time
for( idx = 0; idx < size; idx += 2 )
#if defined(ARDUINO_ARCH_AVR)
   if( result == pgm_read_word( maparray + idx ) )
     { // second integer is the replacement value
     result = pgm_read_word( maparray + idx + 1 );
#elif defined(ARDUINO_ARCH_SAM)
   if( result == *( maparray + idx ) )
     { // second integer is the replacement value
     result = *( maparray + idx + 1 );
#endif
     data &= ~0xFF;         // clear old bits retaining CTRL etc
     data |= result;        // Make new code
     break;
     }
return data;
}


/*
   Pass in 2 character string for the ISO 2 letter country code in use
   For UK "UK" or "GB" are valid
      "US" is built-in default

     Returns 1 for done
          or 0 for not found.
*/
uint8_t PS2KeyMap::selectMap( char *ISO = (char *)_USStr )
{
uint8_t outer, idx, end;

end = sizeof( _KeyMaps ) / sizeof( PS2Advmap );
for( outer = 0; outer < end; outer++ )
   {
   for( idx = 0; idx < 3; idx++ )
      if( _KeyMaps[ outer ].name[ idx ] != ISO[ idx ] )
         break;             // No match
   if( idx == 3 )           // done whole match
     break;
   }
if( outer < end )            // found entry
  {
  _selected_map = outer;     // Copy over to selected
  return 1;
  }
return 0;
}


/*
   Return selected map as a string pointer (2 chars and terminator)
*/
const char *PS2KeyMap::getMap( void )
{
return ( _KeyMaps[ _selected_map ].name );
}


/* Pass in unsigned int returned from PS2KeyAdvanced
     Returns 0 for error
         uint16_t for valid code
   Converts code to US-ASCII first (honouring SHIFT ONLY)
   then if additional map defined scan and remap those keys
*/
uint16_t PS2KeyMap::remapKey( uint16_t code )
{
uint8_t temp;

// for bottom byte checks
temp = code & 0xFF;

// Sort standard ASCII control codes
if( temp >= PS2_KEY_DELETE && temp <= PS2_KEY_SPACE )
  {
  code &= ~( PS2_FUNCTION + 0xFF );     // convert to ASCII code not function
#if defined(ARDUINO_ARCH_AVR)
  temp = pgm_read_byte( &_control_codes[ temp - PS2_KEY_DELETE ] );
#elif defined(ARDUINO_ARCH_SAM)
  temp = _control_codes[ temp - PS2_KEY_DELETE ];
#endif
  code |= temp;
  }
else
  // Remainder convert printable to US-ASCII
  if( !( code & PS2_FUNCTION ) )
    {  // Some codes are straight return values We deal with ones that need work
    // When A-Z not shifted create lower case a - z
    if( temp >= PS2_KEY_A && temp <= PS2_KEY_Z )
      {
      // Deal with Caps Lock by inverting PS2_SHIFT and remove caps lock
      if( ( code & PS2_CAPS ) )
        code ^= ( PS2_SHIFT + PS2_CAPS );
      if( !( code & PS2_SHIFT ) )
        code |= 0x20;
      // When shifted pass through code as received as the codes are A - Z
      else
        code &= ~PS2_SHIFT;
      }
    else
      // Convert KeyPad 0-9 to number codes
      if( temp >= PS2_KEY_KP0 && temp <= PS2_KEY_KP9 )
        {
        code |= 0x10;
        code &= ~( PS2_SHIFT + PS2_ALT + PS2_ALT_GR + PS2_CTRL + PS2_GUI + PS2_CAPS );
        }
      else
        {
        // Some keys ignore shift, the extra keys on numeric keypad
        if( ( temp >= PS2_KEY_KP_DOT && temp <= PS2_KEY_KP_DIV )
              || temp == PS2_KEY_KP_EQUAL  || temp == PS2_KEY_KP_COMMA )
          code &= ~PS2_SHIFT;
        // Do default US-ASCII mapping of remainders excluding (CTRL, ALT, GUI)
        code = (code & PS2_ALT_GR ) + scan_map( code & ~PS2_ALT_GR, 0 );
        }
    //Now do remap if necessary for all modifiers
    if( code > 0 && _selected_map > 0 )
      code = scan_map( code, _selected_map );
    else // For USASCII ignore extra key in case
      if( ( temp == PS2_KEY_EUROPE2 ) )
        code = 0;
    code &= ~( PS2_SHIFT + PS2_ALT_GR );      // Now clear shift status
    }
  else
    code = 0;
return code;
}


/*
   Returns uint8_t version of remapKey ONLY for standard ASCII/UTF-8 codes
   Invalid codes returned as 0
   Also any key with BREAK condition are returned as 0
*/
uint8_t PS2KeyMap::remapKeyByte( uint16_t code )
{
uint16_t data;

// Invalid codes returned as 0
if( code & ( PS2_FUNCTION + PS2_BREAK ) )
  return 0;
data = remapKey( code );
return ( uint8_t )( data & 0xFF );
}


/*  Class constructor
   At construction ensure default map selected
*/
PS2KeyMap::PS2KeyMap()
{
_selected_map = 0;
}
