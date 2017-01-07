/******************************************************************************
 VKey_demo_main.ino
 VKey Voltage Keypad decoding demo
 By Byron Jacquot @ @ SparkFun Electronics
 February 4, 2014
 https://github.com/sparkfun/VKey_Voltage_Keypad
  
 This demonstrates interfacing the SparkFun VKey voltage keypad using 
 the VKey Arduino library.
 
 The VKey has an array of 12 pushbuttons, each producing a unique analog 
 voltage when  pushed.  A microcontroller can read the voltage, and determine 
 which key has been pressed.  The VKey library keeps track of the analog pin 
 connection, key status, and voltage calibration details.
 
 The VKey was connected to the Arduino as follows:
 VKey GND  -> Arduino GND
 VKey Vout -> Arduino analog input A1
 VKey V+   -> Arduino VCC 
 
 To use the library, instantiate a VKey object and periodically check for input 
 using the checkKeys() function.

 Resources:
 no additional library requirements

 Development environment specifics:
 Developed on Arduino IDE 1.0.5
 Tested on a 5V Redboard and 3.3V Pro Mini

 This code is beerware; if you see me (or any other SparkFun employee) at the
 local, and you've found our code helpful, please buy us a round!

 Distributed as-is; no warranty is given.
******************************************************************************/

#include <SparkFunVKeyVoltageKeypad.h>

// Global declaration of the VKey class
// Initialized with analog pin number and supply voltage
VKey keypad(A1, VKey::FIVE );

void setup()
{
  // Initialize serial port for text output
  Serial.begin(9600);
  Serial.println("Welcome to VKey example");
  
  // No VKey specific initialization required.
}

void loop() 
{
  VKey::eKeynum k;  // Variable to receive the key indication

    /* CheckKeys will always return the current key in parameter k.
       The boolean return value indicates whether that value is different than
       the previous value.
     */
  if(keypad.checkKeys(k))
  {
    // Only print when value has changed
    Serial.print("Got key: ");
    Serial.println(k);
  }

  // The responsiveness of the keypad depends on how frequently it is checked. 
  // 50 milliseconds seems to be a reasonable poll interval.
  delay(50);
}

