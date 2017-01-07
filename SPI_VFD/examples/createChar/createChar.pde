/*
  SPI_VFD Library - Custom Character
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
 
 This sketch prints "I <3 Arduino" to the VFD

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

byte newChar[8] = {
	B00000,
	B01010,
	B11111,
	B11111,
	B11111,
	B01110,
	B00100,
	B00000
};



void setup() {
  // create a new character
  vfd.createChar(0, newChar);
  
  // set up the VFD's number of columns and rows: 
  vfd.begin(20, 2);
  // Print a message to the VFD.
  vfd.print(" 20x2 char. SPI VFD");
   vfd.setCursor(0, 1);
  vfd.print(" Adafruit ");
  vfd.write(0);
  vfd.print(" Arduino");
}

void loop() {

}

