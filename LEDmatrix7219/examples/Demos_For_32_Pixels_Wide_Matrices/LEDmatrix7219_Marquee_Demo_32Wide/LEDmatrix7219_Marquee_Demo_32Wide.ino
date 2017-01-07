// LEDmatrix7219_Marquee_Demo_32Wide
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demonstration on how to use the marquee methods
//
// This demo is designed for a 32x8 matrix (4 pcs 8x8 matrices)
//
// It is assumed that the first MAX7219 is connected to
// the following pins using a levelshifter to get the
// correct voltage to the module.
//      Data  - Pin 5
//      Clock - Pin 6
//      Load  - Pin 7
//

#include <LEDmatrix7219.h>

LEDmatrix7219 myMatrix(5, 6, 7);

extern uint8_t TextFont[];

void setup()
{
  myMatrix.begin();
  myMatrix.disableSleep();
  myMatrix.setFont(TextFont);
}

void loop()
{
  // Start the marquee
  myMatrix.marquee("LEDmatrix7219 Library Demo", 100);
  delay(1000);

  // Change the marquee speed
  myMatrix.setMarqueeSpeed(50);  
  delay(1000);

  // Change the marquee speed
  myMatrix.setMarqueeSpeed(30);  
  delay(3000);

  // Change the marquee speed
  myMatrix.setMarqueeSpeed(50);  
  delay(1000);

  // Change the marquee speed
  myMatrix.setMarqueeSpeed(100);  
  delay(4000);

  // Stop the marquee and display a static text
  myMatrix.stopMarquee();
  myMatrix.print("Pause", 1);
  delay(5000);
}
