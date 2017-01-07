/*
  SPI_VFD Library - scrollDisplayLeft() and scrollDisplayRight()
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
 
 This sketch prints "Hello World!" to the VFD and uses the
 scrollDisplayLeft() and scrollDisplayRight() methods to scroll
 the text.
 
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


void setup() {
  // set up the VFD's number of columns and rows: 
  vfd.begin(16, 2);
  // Print a message to the VFD.
  vfd.print("hello, world!");
  delay(1000);
}

void loop() {
  // scroll 13 positions (string length) to the left 
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    // scroll one position left:
    vfd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 33; positionCounter++) {
    // scroll one position right:
    vfd.scrollDisplayRight(); 
    // wait a bit:
    delay(150);
  }
  
    // scroll 20 positions (display length) to the left
    // to move it back to center:
  for (int positionCounter = 0; positionCounter < 20; positionCounter++) {
    // scroll one position left:
    vfd.scrollDisplayLeft(); 
    // wait a bit:
    delay(150);
  }
  
  // delay at the end of the full loop:
  delay(1000);

}

