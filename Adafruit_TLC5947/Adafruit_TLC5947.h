/*************************************************** 
  This is a library for our Adafruit 24-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1429

  These drivers uses SPI to communicate, 3 pins are required to  
  interface: Data, Clock and Latch. The boards are chainable

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _ADAFRUIT_TLC5947_H
#define _ADAFRUIT_TLC5947_H

#include <Arduino.h>


class Adafruit_TLC5947 {
 public:
  Adafruit_TLC5947(uint16_t n, uint8_t c, uint8_t d, uint8_t l);

  boolean begin(void);

  void setPWM(uint16_t chan, uint16_t pwm);
  void setLED(uint16_t lednum, uint16_t r, uint16_t g, uint16_t b);
  void write(void);


 private:
  uint16_t *pwmbuffer;

  uint16_t numdrivers;
  uint8_t _clk, _dat, _lat;

};


#endif
