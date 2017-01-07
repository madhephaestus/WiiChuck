/*************************************************** 
  This is a library for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef ADAFRUIT_MAX31856_H
#define ADAFRUIT_MAX31856_H

#define MAX31856_CR0_REG           0x00
#define MAX31856_CR0_AUTOCONVERT   0x80
#define MAX31856_CR0_1SHOT         0x40
#define MAX31856_CR0_OCFAULT1      0x20
#define MAX31856_CR0_OCFAULT0      0x10
#define MAX31856_CR0_CJ            0x08
#define MAX31856_CR0_FAULT         0x04
#define MAX31856_CR0_FAULTCLR      0x02

#define MAX31856_CR1_REG           0x01
#define MAX31856_MASK_REG          0x02
#define MAX31856_CJHF_REG          0x03
#define MAX31856_CJLF_REG          0x04
#define MAX31856_LTHFTH_REG        0x05
#define MAX31856_LTHFTL_REG        0x06
#define MAX31856_LTLFTH_REG        0x07
#define MAX31856_LTLFTL_REG        0x08
#define MAX31856_CJTO_REG          0x09
#define MAX31856_CJTH_REG          0x0A
#define MAX31856_CJTL_REG          0x0B
#define MAX31856_LTCBH_REG         0x0C
#define MAX31856_LTCBM_REG         0x0D
#define MAX31856_LTCBL_REG         0x0E
#define MAX31856_SR_REG            0x0F

#define MAX31856_FAULT_CJRANGE     0x80
#define MAX31856_FAULT_TCRANGE     0x40
#define MAX31856_FAULT_CJHIGH      0x20
#define MAX31856_FAULT_CJLOW       0x10
#define MAX31856_FAULT_TCHIGH      0x08
#define MAX31856_FAULT_TCLOW       0x04
#define MAX31856_FAULT_OVUV        0x02
#define MAX31856_FAULT_OPEN        0x01

typedef enum
{
  MAX31856_TCTYPE_B  = 0b0000,
  MAX31856_TCTYPE_E  = 0b0001,
  MAX31856_TCTYPE_J  = 0b0010,
  MAX31856_TCTYPE_K  = 0b0011,
  MAX31856_TCTYPE_N  = 0b0100,
  MAX31856_TCTYPE_R  = 0b0101,
  MAX31856_TCTYPE_S  = 0b0110,
  MAX31856_TCTYPE_T  = 0b0111,
  MAX31856_VMODE_G8  = 0b1000,
  MAX31856_VMODE_G32 = 0b1100,
} max31856_thermocoupletype_t;

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Adafruit_MAX31856 {
 public:
  Adafruit_MAX31856(int8_t spi_cs, int8_t spi_mosi, int8_t spi_miso, int8_t spi_clk);
  Adafruit_MAX31856(int8_t spi_cs);

  boolean begin(void);

  void setThermocoupleType(max31856_thermocoupletype_t type);
  max31856_thermocoupletype_t getThermocoupleType(void);

  uint8_t readFault(void);
  void oneShotTemperature(void);

  float readCJTemperature(void);
  float readThermocoupleTemperature(void);

  void setTempFaultThreshholds(float flow, float fhigh);
  void setColdJunctionFaultThreshholds(int8_t low, int8_t high);

 private:
  int8_t _sclk, _miso, _mosi, _cs;

  void readRegisterN(uint8_t addr, uint8_t buffer[], uint8_t n);

  uint8_t  readRegister8(uint8_t addr);
  uint16_t readRegister16(uint8_t addr);
  uint32_t readRegister24(uint8_t addr);

  void     writeRegister8(uint8_t addr, uint8_t reg);
  uint8_t spixfer(uint8_t addr);
};

#endif
