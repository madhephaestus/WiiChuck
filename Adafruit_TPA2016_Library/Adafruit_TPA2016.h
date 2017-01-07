/*************************************************** 
  This is a library for the Adafruit TPA2016D2 Class D Amplifier Breakout

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1712

  This amplifier uses I2C to communicate, 2 pins are required to interface

  Check out the links above for our tutorials and wiring diagrams 

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _ADAFRUIT_TPA2016_H
#define _ADAFRUIT_TPA2016_H

#include <Arduino.h>
#include <Wire.h>

#define TPA2016_SETUP 0x1
#define TPA2016_SETUP_R_EN 0x80
#define TPA2016_SETUP_L_EN 0x40
#define TPA2016_SETUP_SWS 0x20
#define TPA2016_SETUP_R_FAULT 0x10
#define TPA2016_SETUP_L_FAULT 0x08
#define TPA2016_SETUP_THERMAL 0x04
#define TPA2016_SETUP_NOISEGATE 0x01


#define TPA2016_ATK 0x2
#define TPA2016_REL 0x3
#define TPA2016_HOLD 0x4
#define TPA2016_GAIN 0x5
#define TPA2016_AGCLIMIT 0x6
#define TPA2016_AGC 0x7
#define TPA2016_AGC_OFF 0x00
#define TPA2016_AGC_2 0x01
#define TPA2016_AGC_4 0x02
#define TPA2016_AGC_8 0x03

#define TPA2016_I2CADDR 0x58


class Adafruit_TPA2016 {
 public:
  Adafruit_TPA2016(void);

  boolean begin(void);

  void enableChannel(boolean r, boolean l);

  // Register #5
  void setGain(int8_t g);
  int8_t getGain();

  // Register #3
  void setReleaseControl(uint8_t release);
  // Register #2
  void setAttackControl(uint8_t attack);
  // Register #4
  void setHoldControl(uint8_t hold);

  // Register #6
  void setLimitLevelOn(void);
  void setLimitLevelOff(void);
  void setLimitLevel(uint8_t limit);

  // Register #7
  void setAGCCompression(uint8_t x);
  void setAGCMaxGain(uint8_t x);

 private:

  uint8_t read8(uint8_t a);
  void write8(uint8_t a, uint8_t d);

};


#endif
