/*
  SPI_VFD Library - Hello World
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
  
 This sketch displays text sent over the serial port 
 (e.g. from the Serial Monitor) on an attached VFD.
 
 
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

void setup(){
    // set up the VFD's number of columns and rows: 
  vfd.begin(20, 2);
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    vfd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      vfd.write(Serial.read());
    }
  }
}
