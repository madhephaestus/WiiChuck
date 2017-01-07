/*
  SPI_VFD Library - Hello World
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
 
 This sketch prints "Hello World!" to the VFD
 and shows the time.
 
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
  // Print a message to the VFD.
  vfd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  vfd.setCursor(0, 1);
  // print the number of seconds since reset:
  vfd.print(millis()/1000);
}

