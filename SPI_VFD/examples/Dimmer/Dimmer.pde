/*
  SPI_VFD Library - Dimmer
 
 Demonstrates the use a 20x2 VFD display.  The SPI_VFD
 library works with all VFD displays that are compatible with the 
 NEC PD16314 driver and has the SPI pins brought out
 
 This sketch prints "Hello World!" to the VFD
 and adjusts the brightness of the VFD
 
  The circuit:
 * VFD Data   (pin 3) to digital pin 2
 * VFD Clock  (pin 5) to digital pin 3
 * VFD Strobe (pin 4) to digital pin 4
 * VFD VCC    (pin 2) to 5V
 * VFD Ground (pin 1) to Ground
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 Dimmer example added 17 Aug 2011
 by J.P. McGlinn
 
 This example code is in the public domain.
 */

// include the library code:
#include <SPI_VFD.h>

// initialize the library with the numbers of the interface pins
SPI_VFD vfd(2, 3, 4, VFD_BRIGHTNESS25);

// alternate form of constuctor sets brightness to 100% (default)
// SPI_VFD vfd(2, 3, 4);

// global to store brightness for the sketch
uint8_t brightness = 0;

void setup() {
  // set up the VFD's number of columns and rows: 
  // unnecessary to call begin, this method is called by the constructor with 2x20.
  // vfd.begin(20, 2);
  //
  // Alternate form of begin to set brightness
  // vfd.begin(20, 2, VFD_BRIGHTNESS50);
  
  // Print a message to the VFD.
  vfd.print("hello, world!");
  vfd.setCursor(0, 1);
  vfd.print("Brightness: 0x");
  vfd.print(vfd.getBrightness(), HEX);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  vfd.setCursor(14, 0);
  // print the number of seconds since reset:
  vfd.print(millis()/1000);

  delay(1000);
  
  //set the brightness to a value between 0 and 3
  vfd.setBrightness(brightness=brightness++%4);
  
  //show the value for fun
  vfd.setCursor(14, 1);
  vfd.print(vfd.getBrightness(), HEX);
}
