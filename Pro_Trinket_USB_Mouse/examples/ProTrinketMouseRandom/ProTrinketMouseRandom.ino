/*
ProTrinketMouseRandom example
For Pro Trinket (ATmega328 based Trinket) by Adafruit Industries

Please use library TrinketKeyboard for the ATtiny85 based Trinket

Version 1.0  2015-01-03 Initial Version derived from TrinketMouseExample  Mike Barela

This example simply starts a mouse and randomly moves it around, while clicking random buttons
*/

#include <ProTrinketMouse.h>

void setup()
{
  TrinketMouse.begin();
}

void loop()
{
  TrinketMouse.move(random(), random(), random(), random() & 0x07);
}
