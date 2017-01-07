/*
 * Four Letter Word Generator
 * (C) 2012-13 Akafugu Corporation
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 */

//
// Swear word blacklist for Four Letter Word database
// All words are encoded using ROT13 so that the cencored words are not
// directly visible in the file
//
// Must be alphabetized (according to the decrypted value!), since the
// list is searched by binary search
//

#include <avr/pgmspace.h>

const char word_0[]  PROGMEM = "NAHF";
const char word_1[]  PROGMEM = "NEFR";
const char word_2[]  PROGMEM = "PNJX";
const char word_3[]  PROGMEM = "PYVG";
const char word_4[]  PROGMEM = "PBPX";
const char word_5[]  PROGMEM = "PBBA";
const char word_6[]  PROGMEM = "PENC";
const char word_7[]  PROGMEM = "PHAG";
const char word_8[]  PROGMEM = "QNTB";
const char word_9[]  PROGMEM = "QNZA";
const char word_10[] PROGMEM = "QVPX";
const char word_11[] PROGMEM = "QLXR";
const char word_12[] PROGMEM = "SNTF";
const char word_13[] PROGMEM = "SNEG";
const char word_14[] PROGMEM = "SHPX";
const char word_15[] PROGMEM = "TBBX";
const char word_16[] PROGMEM = "URYY";
const char word_17[] PROGMEM = "WRJF";
const char word_18[] PROGMEM = "WVFZ";
const char word_19[] PROGMEM = "WVMZ";
const char word_20[] PROGMEM = "WVMM";
const char word_21[] PROGMEM = "XVXR";
const char word_22[] PROGMEM = "ZHSS";
const char word_23[] PROGMEM = "ANMV";
const char word_24[] PROGMEM = "CNXV";
const char word_25[] PROGMEM = "CVFF";
const char word_26[] PROGMEM = "CBBA";
const char word_27[] PROGMEM = "CBBC";
const char word_28[] PROGMEM = "CBEA";
const char word_29[] PROGMEM = "ENCR";
const char word_30[] PROGMEM = "FUVG";
const char word_31[] PROGMEM = "FZHG";
const char word_32[] PROGMEM = "FCVP";
const char word_33[] PROGMEM = "FYHG";
const char word_34[] PROGMEM = "GVGF";
const char word_35[] PROGMEM = "GHEQ";
const char word_36[] PROGMEM = "GJNG";
const char word_37[] PROGMEM = "JNAX";

#define BLACKLIST_SIZE 37

PGM_P const flw_blacklist[] PROGMEM =
{   
	word_0, word_1, word_2, word_3, word_4, word_5, word_6, word_7, word_8, word_9, 
	word_10, word_11, word_12, word_13, word_14, word_15, word_16, word_17, word_18, word_19,
	word_20, word_21, word_22, word_23, word_24, word_25, word_26, word_27, word_28, word_29, 
	word_30, word_31, word_32, word_33, word_34, word_35, word_36, word_37
};

