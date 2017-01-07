/*
 * AUTHOR:    Samuel M.H. <samuel.mh@gmail.com>
 * LAST REV:  17-May-2012
 * DESCRIPTION:
 *   Arduino C code headers for the SMH_7SD class (Seven Segment Display or SSD).
 *
 * LICENSE: GPL V3 <http://www.gnu.org/licenses/gpl.html>
*/


#ifndef SMH_7SD_h
#define SMH_7SD_h

#include "Arduino.h"

/*
 * A 7 SEGEMENT DISPLAY:
 *  This is the layout of a S.S.D.
 *        a
 *     +-----+
 *     |     |
 *   f |  g  | b
 *     +-----+
 *   e |     | c
 *     |     |
 *     +-----+  0
 *        d     dp
 *
 * CHARACTER REPRESENTATION:
 *  Since the display have 7 segments plus a dot, it's possible to
 *   store a complete state on a byte. Each segment state is a bit (on or off), so
 *   the resultant byte will be a byte like (a,b,c,d,e,f,dp).
 *  Following the previous, each alphanumeric character can be transcripted
 *   into a byte that will show on the display the character itself. If we store
 *   those characters and index them properly into arrays, we got the dictionaries.
 *
 * CIRCUIT:
 *   I used a 330 Ohm resistor attached to the common anode/cathode
 *    7-segment led an the other pins are connected directly to the arduino
 *    digital outputs. This is the cheap and lame way.
 *   The good way is having each segment its own resistor.
 *  
 * STROBOSCOPIC EFECT:
 *   Spinning stuff, writting in the air and so.
 *    timeOn = 1 and timeOff = 20 is a good starting point.
 *    Try with a lower resistor so the segments bright more. 100 Ohm is OK.
 *
 * KNOWN PROBLEMS:
 *   Problems (no) representig chars over 127: \, ñ, Ñ, etc
 *    due to encoding issues ASCII(arduino) VS Local (utf8, iso8859-1X)
 *  
*/

//Constants for the constructor that define the type of SSD
#define SMH_7SD_COMMON_ANODE   1 
#define SMH_7SD_COMMON_CATHODE 2



/* ========= DICTONARIES  =============  */

//Numbers 0-9
#define SMH_7SD_NUMBERS (byte[]){  \
  B11111100,                       \
  B01100000, B11011010, B11110010, \
  B01100110, B10110110, B10111110, \
  B11100000, B11111110, B11110110  \
}                                 

//Letters A-Z
#define SMH_7SD_LETTERS (byte[]){                        \
  B11101110, B00111110, B10011100, B01111010, B10011110, \
  B10001110, B10111100, B01101110, B01100000, B01111000, \
  B01011111, B00011100, B11101101, B00101010, B11111100, \
  B11001110, B11100110, B00001010, B10110110, B00011110, \
  B01111100, B00111000, B11110011, B00110011, B01110110, \
  B11011010,                                             \
}




class SMH_7SD{
  private:
    int* pins;
    short on,off;
    unsigned int timeOn, timeOff;
    
  public:
    SMH_7SD(int*,short);
    void setTimeOn (unsigned int);
    void setTimeOff(unsigned int);
    void printRaw(byte);
    void printChar(char);
    void print(char*);
};

#endif
