// LEDmatrix7219_Pixel_Demo_16Wide
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo on how to set, clear and invert single pixels
//
// This demo is designed for a 16x8 matrix (2 pcs 8x8 matrices)
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

void setup()
{
  randomSeed(analogRead(0));
  
  myMatrix.begin();
  myMatrix.disableSleep();
  
  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 16; x++)
    {
      myMatrix.setPixel(x, y);
      delay(5);
    }

  for (int y = 0; y < 8; y++)
    for (int x = 0; x < 16; x++)
    {
      myMatrix.clrPixel(x, y);
      delay(5);
    }
}

void loop()
{
  myMatrix.invPixel(random(16), random(8));
  delay(20);
}
