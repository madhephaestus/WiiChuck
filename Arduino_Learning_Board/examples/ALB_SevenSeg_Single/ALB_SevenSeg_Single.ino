/*
* Arduino Learning Board Project - Single Digit Sevent Segment Display
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

/*
Display Pinout:

g f CC a b
------------
|          |
| /--a--\  |
| f     b  |
| |--g--|  |
| e     c  |
| \--d--/ .|
|          |
------------
e d CC c DP

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
  byte numDigits = 1; // Only 1 digit for this demo
  byte digitPins[] = { 5 }; // Pin 5 connects to common cathode of the display
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // Connect to segments a,b,c,d,e,f,g,decimal

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
}

void loop() {
  static unsigned long timer = millis();
  static int value = 0;
  
  if (millis() >= timer) {
    value++;
    timer += 500; 
    if (value == 10) { // Reset to 0 after counting past 9
      value = 0;
    }
    
    // (value to display, # of decimal places)
    sevseg.setNumber(value, 0); // 0 for decimal places shows decimal for single digits
    							// 1 for decimal places turns off decimal point for single digits
  }

  sevseg.refreshDisplay(); // Must run repeatedly
}