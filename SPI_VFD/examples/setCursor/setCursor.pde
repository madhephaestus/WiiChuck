/*
  SPI_VFD Library - Hello World
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
 
 This sketch prints to all the positions of the LCD using the
 setCursor() method:
 

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

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 20;

// initialize the library with the numbers of the interface pins
SPI_VFD vfd(2, 3, 4);

void setup() {
  // set up the LCD's number of columns and rows: 
  vfd.begin(numCols,numRows);
}

void loop() {
  // loop from ASCII 'a' to ASCII 'z':
  for (int thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) {
    // loop over the columns:
    for (int thisCol = 0; thisCol < numRows; thisCol++) {
      // loop over the rows:
      for (int thisRow = 0; thisRow < numCols; thisRow++) {
        // set the cursor position:
        vfd.setCursor(thisRow,thisCol);
        // print the letter:
#if ARDUINO >= 100
        vfd.write(thisLetter);
#else
        vfd.print(thisLetter, BYTE);
#endif
        delay(200);
      }
    }
  }
}


