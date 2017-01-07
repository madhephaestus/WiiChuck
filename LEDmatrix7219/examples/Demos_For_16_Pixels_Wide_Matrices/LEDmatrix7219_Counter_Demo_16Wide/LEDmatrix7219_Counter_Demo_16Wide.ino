// LEDmatrix7219_Counter_Demo_16Wide
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo on displaying numbers
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

extern uint8_t NumFont[];

void setup()
{
  myMatrix.begin(2);
  myMatrix.disableSleep();
  myMatrix.setFont(NumFont);
}

void loop()
{
  for (long i = 0; i < 100; i++)
  {
    myMatrix.print(i, 2, 0, 2, '0');   // If you want leading spaces instead of leading zeros you can replace '0' with ';'.
    delay(100);
  }
}
