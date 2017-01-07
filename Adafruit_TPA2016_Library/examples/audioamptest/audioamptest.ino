/*************************************************** 
  This is an example for our Adafruit TPA2016 Class D Amplifier Breakout

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1712

  This amplifier uses I2C to communicate, 2 pins are required to  
  interface

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Wire.h>
#include "Adafruit_TPA2016.h"

Adafruit_TPA2016 audioamp = Adafruit_TPA2016();

void setup() {
 Serial.begin(9600);
 
 
 Serial.println("TPA2016 Audio Test"); 
 audioamp.begin();
 
 // Dump register map, for debugging purposes.
 /*
 for (uint8_t i=1; i<8; i++) {
   Serial.print("Register #"); Serial.print(i); 
   Serial.print(": 0x");
   Serial.println(audioamp.read8(i), HEX);
 }
 */
 
  // Turn off AGC for the gain test
  audioamp.setAGCCompression(TPA2016_AGC_OFF);
  // we also have to turn off the release to really turn off AGC
  audioamp.setReleaseControl(0);
  
  // We can update the gain, from -28dB up to 30dB
  for (int8_t i=-28; i<=30; i++) {
    Serial.print("Gain = "); Serial.println(i); 
    audioamp.setGain(i);
    delay(500);
  }
  
  // Each channel can be individually controlled
  Serial.println("Left off");
  audioamp.enableChannel(true, false);
  delay(1000);
  Serial.println("Left On, Right off");
  audioamp.enableChannel(false, true);
  delay(1000);
  Serial.println("Left On, Right On");
  audioamp.enableChannel(true, true);
  delay(1000);

  // OK now we'll turn the AGC back on and mess with the settings :)
  
  // AGC can be TPA2016_AGC_OFF (no AGC) or
  //  TPA2016_AGC_2 (1:2 compression)
  //  TPA2016_AGC_4 (1:4 compression)
  //  TPA2016_AGC_8 (1:8 compression)
  Serial.println("Setting AGC Compression");
  audioamp.setAGCCompression(TPA2016_AGC_2);

  // See Datasheet page 22 for value -> dBV conversion table
  Serial.println("Setting Limit Level");
  audioamp.setLimitLevelOn();
  // or turn off with setLimitLevelOff()
  audioamp.setLimitLevel(25);  // range from 0 (-6.5dBv) to 31 (9dBV)
  
  // See Datasheet page 23 for value -> ms conversion table
  Serial.println("Setting AGC Attack");
  audioamp.setAttackControl(5);
  
  // See Datasheet page 24 for value -> ms conversion table
  Serial.println("Setting AGC Hold");
  audioamp.setHoldControl(0);
  
  // See Datasheet page 24 for value -> ms conversion table
  Serial.println("Setting AGC Release");
  audioamp.setReleaseControl(11);
}


void loop() {

}