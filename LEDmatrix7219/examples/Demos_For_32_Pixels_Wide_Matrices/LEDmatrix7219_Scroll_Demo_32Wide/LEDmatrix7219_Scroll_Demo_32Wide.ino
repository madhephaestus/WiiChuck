// LEDmatrix7219_Scroll_Demo_32Wide
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demonstration on how to use scroll()
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
  myMatrix.print("Ab*12", 1);
  delay(1000);

  for (int i = 0; i < 32; i++)
  {
    myMatrix.scroll(SCROLL_LEFT, true);
    delay(100);
  }
  delay(500);

  for (int i = 0; i < 32; i++)
  {
    myMatrix.scroll(SCROLL_RIGHT, true);
    delay(100);
  }
  delay(500);

  for (int i = 0; i < 8; i++)
  {
    myMatrix.scroll(SCROLL_UP, true);
    delay(100);
  }
  delay(500);

  for (int i = 0; i < 8; i++)
  {
    myMatrix.scroll(SCROLL_DOWN, true);
    delay(100);
  }
  delay(500);

  for (int i = 0; i < 8; i++)
  {
    myMatrix.scroll(SCROLL_DOWN);
    delay(100);
  }
  delay(1000);
}
