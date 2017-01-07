/*
  SPI_VFD Library - Hello World
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
  
 This sketch demonstrates how to use leftToRight() and rightToLeft()
 to move the cursor.
 

  The circuit:
 * VFD Clock to digital pin 2
 * VFD Data to digital pin 3
 * VFD Chip select to digital pin 4
 * VFD VCC (power) to 5V
 * VFD Ground (power) to Ground
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 */

// include the library code:
#include <SPI_VFD.h>

// initialize the library with the numbers of the interface pins
SPI_VFD vfd(2, 3, 4);

int thisChar = 'a';

void setup() {
  // set up the LCD's number of columns and rows: 
  vfd.begin(20, 2);
  // turn on the cursor:
  vfd.cursor();
  Serial.begin(9600);
}

void loop() {
  // reverse directions at 'm':
  if (thisChar == 'm') {
    // go right for the next letter
    vfd.rightToLeft(); 
  }
  // reverse again at 's':
  if (thisChar == 's') {
    // go left for the next letter
    vfd.leftToRight(); 
  }
  // reset at 'z':
  if (thisChar > 'z') {
    // go to (0,0):
    vfd.home(); 
    // start again at 0
    thisChar = 'a';
  }
  // print the character
#if ARDUINO >= 100
  vfd.write(thisChar);
#else
  vfd.print(thisChar, BYTE);
#endif
  // wait a second:
  delay(1000);
  // increment the letter:
  thisChar++;
}








