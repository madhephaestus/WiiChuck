// LEDmatrix7219_Intensity_Demo_64Wide
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo on setting the display intensity/brightness
//
// This demo is designed for a 64x8 matrix (8 pcs 8x8 matrices)
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
  myMatrix.begin(8);
  myMatrix.disableSleep();
  myMatrix.setIntensity(0);
  myMatrix.setFont(TextFont);
  myMatrix.print("\\\\\\\\\\\\\\\\\\\\", 2);
}

void loop()
{
  for (int i = 0; i < 16; i++)
  {
    myMatrix.setIntensity(i);
    delay(100);
  }
  for (int i = 15; i >= 0; i--)
  {
    myMatrix.setIntensity(i);
    delay(100);
  }
}
