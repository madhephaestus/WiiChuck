/*
ProTrinketKeyboard prank example

For Pro Trinket (ATmega328P based Trinket) by Adafruit Industries
Please use library TrinketKeyboard for the ATtiny85 based Trinket

Version 1.0  2015-01-01 Initial Version by Mike Barela
*/

#include <ProTrinketKeyboard.h>  // include the Adafruit library

void setup()
{
  TrinketKeyboard.begin();  // initialize USB keyboard code
}

void loop()
{
  unsigned long secs_to_wait = random(6, 60); // wait 6 to 60 seconds between keys
  unsigned long time_stamp = millis();
  while (millis() < (time_stamp + (secs_to_wait * 1000))) // wait the random amount of time
  {
    TrinketKeyboard.poll();
    // the poll function must be called at least once every 10 ms
    // or cause a keystroke
    // if it is not, then the computer may think that the device
    // has stopped working, and give errors
  }
  TrinketKeyboard.typeChar((char)random(33, 122)); // type out a random character (valid ASCII)
}


