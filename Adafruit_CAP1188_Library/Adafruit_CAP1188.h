/*************************************************** 
  This is a library for the CAP1188 8-Channel Capacitive Sensor

  Designed specifically to work with the CAP1188 breakout from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <Wire.h>
#include <SPI.h>

// The default I2C address
#define CAP1188_I2CADDR 0x29

// Some registers we use
#define CAP1188_SENINPUTSTATUS 0x3
#define CAP1188_MTBLK 0x2A
#define CAP1188_LEDLINK 0x72
#define CAP1188_PRODID 0xFD
#define CAP1188_MANUID 0xFE
#define CAP1188_STANDBYCFG 0x41
#define CAP1188_REV 0xFF
#define CAP1188_MAIN 0x00
#define CAP1188_MAIN_INT 0x01
#define CAP1188_LEDPOL 0x73


class Adafruit_CAP1188 {
 public:
  // Software SPI
  Adafruit_CAP1188(int8_t clkpin, int8_t misopin, 
		   int8_t mosipin,int8_t cspin, 
		   int8_t resetpin);
  // Hardware SPI
  Adafruit_CAP1188(int8_t cspin, int8_t resetpin);
  // Hardware I2C
  Adafruit_CAP1188(int8_t resetpin = -1);

  boolean begin(uint8_t i2caddr = CAP1188_I2CADDR);
  uint8_t readRegister(uint8_t reg);
  void writeRegister(uint8_t reg, uint8_t value);
  uint8_t touched(void);
  void LEDpolarity(uint8_t x);

 private:
  uint8_t spixfer(uint8_t x);
  boolean _i2c;
  int8_t _i2caddr, _resetpin, _cs, _clk, _mosi, _miso;
};

