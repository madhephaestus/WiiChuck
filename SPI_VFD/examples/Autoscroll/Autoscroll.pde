/*
  SPI_VFD Library - Autoscroll
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
 
 This sketch demonstrates the use of the autoscroll()
 and noAutoscroll() functions to make new text scroll or not.
 
  The circuit:
 * VFD Data to digital pin 2
 * VFD Clock to digital pin 3
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

void setup() {
  // set up the VFD's number of columns and rows: 
  vfd.begin(20, 2);
}

void loop() {
  // set the cursor to (0,0):
  vfd.setCursor(0, 0);
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
   vfd.print(thisChar);
   delay(500);
  }

  // set the cursor to (20,1):
  vfd.setCursor(20,1);
  // set the display to automatically scroll:
  vfd.autoscroll();
  // print from 0 to 9:
  for (int thisChar = 0; thisChar < 10; thisChar++) {
    vfd.print(thisChar);
    delay(500);
  }
  // turn off automatic scrolling
  vfd.noAutoscroll();
  
  // clear screen for the next loop:
  vfd.clear();
}

