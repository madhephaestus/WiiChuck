/* Version V1.0.2
  PS2KeyMap.h - PS2KeyAdvanced library
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

  Takes output key codes from PS2KeyAdvanced and translates them to ASCII/UTF-8
  if valid as these characters, or returns PS2KeyAdvanced codings for function keys
  as these do NOT have any standard ASCII/UTF-8 encodings.

  Use defines from here and PS2KeyAdvanced.h to make reference to key codes

  This library REQUIRES PS2KeyAdvanced as the codes used to remap to ASCII/UTF-8
  are specific to that library to match ALL keys on a keyboard

  To reduce flash memory footprint comment out any of the following in
  PSKeyMap.h and recompile

    // Uncomment any of following defines to include those mappings
    // or comment to exclude
    //#define GERMAN
    //#define FRENCH
    //#define SPECIAL

  The functions in this library takes the unsigned int values produced from
  PS2KeyAdvanced and translate them as follows into an unsigned int value

        0       invalid/error
        1-1F    ASCII Control Codes (BS, TAB, ESC, ENTER, SPACE)
        20-7F   ASCII Printable characters and DEL
        80-9F   UTF-8 Control codes (none in base library)
        A0-FF   UTF-8 1 byte printable characters (See PS2KeyMap.h)

  All other codes for F1 to F24 (yes there are 24 function key codes) arrow keys page up
  page down etc are returned as the same codes as PS2KeyAdvanced uses.

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

#ifndef PS2KeyMap_h
#define PS2KeyMap_h

#ifndef PS2KeyAdvanced_h
#error PS2KeyAdvanced library missing
#endif

// Uncomment any of following defines to include those mappings
// or comment to exclude
#define GERMAN
#define FRENCH
//#define SPECIAL

/* UTF-8 single byte LATIN encodings
   128 to 159 (0x80 to 0x9F) are control characters application generated
   160 to 255 (0xA0 to 0XFF) are used depending on keymap tables
   These defines are used to insert them into correct tables */

/****************************************************************
  IMPORTANT NOTE EURO currency Symbol is NOT supported in UTF-8 single
  byte codings, as EURO symbol came after UTF-8 single byte codings
****************************************************************/
#define PS2_NO_BREAK_SPACE              160 // nbsp
#define PS2_INVERTED_EXCLAMATION        161 // ¡
#define PS2_CENT_SIGN			        162 // ¢
#define PS2_POUND_SIGN			        163 // £
#define PS2_CURRENCY_SIGN		        164 // ¤
#define PS2_YEN_SIGN			        165 // ¥
#define PS2_BROKEN_BAR                  166 // ¦
#define PS2_SECTION_SIGN		        167 // §
#define PS2_DIAERESIS			        168 // ¨
#define PS2_COPYRIGHT_SIGN		        169 // ©
#define PS2_FEMININE_ORDINAL	        170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE	    171 // «
#define PS2_NOT_SIGN			        172 // ¬
#define PS2_HYPHEN			            173
#define PS2_REGISTERED_SIGN		        174 // ®
#define PS2_MACRON			            175 // ¯
#define PS2_DEGREE_SIGN			        176 // °
#define PS2_PLUS_MINUS_SIGN		        177 // ±
#define PS2_SUPERSCRIPT_TWO		        178 // ²
#define PS2_SUPERSCRIPT_THREE	        179 // ³
#define PS2_ACUTE_ACCENT		        180 // ´
#define PS2_MICRO_SIGN			        181 // µ
#define PS2_PILCROW_SIGN		        182 // ¶
#define PS2_MIDDLE_DOT			        183 // ·
#define PS2_CEDILLA			            184 // ¸
#define PS2_SUPERSCRIPT_ONE		        185 // ¹
#define PS2_MASCULINE_ORDINAL	        186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE	187 // »
#define PS2_FRACTION_ONE_QUARTER	    188 // ¼
#define PS2_FRACTION_ONE_HALF	        189 // ½
#define PS2_FRACTION_THREE_QUARTERS	    190 // ¾
#define PS2_INVERTED_QUESTION MARK	    191 // ¿
#define PS2_A_GRAVE			            192 // À
#define PS2_A_ACUTE			            193 // Á
#define PS2_A_CIRCUMFLEX		        194 // Â
#define PS2_A_TILDE			            195 // Ã
#define PS2_A_DIAERESIS			        196 // Ä
#define PS2_A_RING_ABOVE		        197 // Å
#define PS2_AE				            198 // Æ
#define PS2_C_CEDILLA			        199 // Ç
#define PS2_E_GRAVE			            200 // È
#define PS2_E_ACUTE			            201 // É
#define PS2_E_CIRCUMFLEX		        202 // Ê
#define PS2_E_DIAERESIS			        203 // Ë
#define PS2_I_GRAVE			            204 // Ì
#define PS2_I_ACUTE			            205 // Í
#define PS2_I_CIRCUMFLEX		        206 // Î
#define PS2_I_DIAERESIS			        207 // Ï
#define PS2_ETH				            208 // Ð
#define PS2_N_TILDE			            209 // Ñ
#define PS2_O_GRAVE			            210 // Ò
#define PS2_O_ACUTE			            211 // Ó
#define PS2_O_CIRCUMFLEX		        212 // Ô
#define PS2_O_TILDE			            213 // Õ
#define PS2_O_DIAERESIS			        214 // Ö
#define PS2_MULTIPLICATION		        215 // ×
#define PS2_O_STROKE			        216 // Ø
#define PS2_U_GRAVE			            217 // Ù
#define PS2_U_ACUTE			            218 // Ú
#define PS2_U_CIRCUMFLEX		        219 // Û
#define PS2_U_DIAERESIS			        220 // Ü
#define PS2_Y_ACUTE			            221 // Ý
#define PS2_THORN			            222 // Þ
#define PS2_SHARP_S			            223 // ß
#define PS2_a_GRAVE			            224 // à
#define PS2_a_ACUTE			            225 // á
#define PS2_a_CIRCUMFLEX		        226 // â
#define PS2_a_TILDE			            227 // ã
#define PS2_a_DIAERESIS			        228 // ä
#define PS2_a_RING_ABOVE		        229 // å
#define PS2_ae				            230 // æ
#define PS2_c_CEDILLA			        231 // ç
#define PS2_e_GRAVE			            232 // è
#define PS2_e_ACUTE			            233 // é
#define PS2_e_CIRCUMFLEX		        234 // ê
#define PS2_e_DIAERESIS			        235 // ë
#define PS2_i_GRAVE			            236 // ì
#define PS2_i_ACUTE			            237 // í
#define PS2_i_CIRCUMFLEX		        238 // î
#define PS2_i_DIAERESIS			        239 // ï
#define PS2_eth				            240 // ð
#define PS2_n_TILDE			            241 // ñ
#define PS2_o_GRAVE			            242 // ò
#define PS2_o_ACUTE			            243 // ó
#define PS2_o_CIRCUMFLEX		        244 // ô
#define PS2_o_TILDE			            245 // õ
#define PS2_o_DIAERESIS			        246 // ö
#define PS2_DIVISION			        247 // ÷
#define PS2_o_STROKE			        248 // ø
#define PS2_u_GRAVE			            249 // ù
#define PS2_u_ACUTE			            250 // ú
#define PS2_u_CIRCUMFLEX		        251 // û
#define PS2_u_DIAERESIS			        252 // ü
#define PS2_y_ACUTE			            253 // ý
#define PS2_thorn			            254 // þ
#define PS2_y_DIAERESIS			        255 // ÿ


/* Public class definition */
class PS2KeyMap {
  public:
  	/**
  	 * This constructor just sets the default key mapping to US. */
    PS2KeyMap();

    /**
     Pass in 2 character string for the ISO 2 letter country code in use
        For UK "UK" or "GB" are valid
        "US" is built-in default
     Returns 1 for done or 0 for not found.
     */
    uint8_t selectMap( char * );

    /**
     Return selected map as a string pointer (2 chars and terminator)    */
    const char *getMap( void );

    /**
     Pass in unsigned int returned from PS2KeyAdvanced
                Returns 0 for error
                        uint16_t for valid code
     */
    uint16_t remapKey( uint16_t );

    /**
      Returns uint8_t version of remapKey ONLY for standard ASCII/UTF-8 codes
      Invalid codes returned as 0
     */
    uint8_t remapKeyByte( uint16_t );
};
#endif
