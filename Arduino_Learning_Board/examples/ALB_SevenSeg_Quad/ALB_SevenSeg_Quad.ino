/*
* Arduino Learning Board Project - 4 Digit Sevent Segment Display
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

/*
Display Pinout:

    D1   A   F   D2   D3   B
---------------------------------
|                               |
| /-a-\   /-a-\   /-a-\   /-a-\ |    
| f   b   f   b   f   b   f   b |
| |-g-|   |-g-|   |-g-|   |-g-| |
| e   c   e   c   e   c   e   c |  
| \-d-/.  \-d-/.  \-d-/.  \-d-/.|
|                               |
---------------------------------
     E   D   DP   C   G   D4

*/

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_SevenSegment to include the 7-Segment Display functions
// of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)

#define USE_ALB_SevenSegment

// NOW include the main ArduinoLearningBoard library (quotes now, <Arduino...> when it's "live")
// Based on the defines above, the appropriate code will be added to the project
#include "ArduinoLearningBoard.h"

SevSeg sevseg; //Instantiate a seven segment controller object

void setup() {
  byte numDigits = 4; // Only 1 digit for this demo
  byte digitPins[] = { 2, 5, 6, 13 }; // Pins 2,5,6,13 connect to digit pins D1, D2, D3, D4 (common cathode) of the display
  byte segmentPins[] = {3, 7, 11, 9, 8, 4, 12, 10}; // Connect to segments a,b,c,d,e,f,g,decimal

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
}

void loop() {
  static unsigned long timer = millis();
  static int value = 0;
  
  if (millis() >= timer) {
    value++;
    timer += 100; 
    if (value == 10000) { // Reset to 0 after counting past 9999
      value = 0;
    }
    
    // (value to display, # of decimal places)
    sevseg.setNumber(value, 1);
  }

  sevseg.refreshDisplay(); // Must run repeatedly
}