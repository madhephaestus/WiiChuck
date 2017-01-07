/**************************************************************************/
/*!
    @file     Adafruit_MPL3115A2.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    This is a library for the Adafruit MPL3115A2 breakout board
    ----> https://www.adafruit.com/products/1893

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#else
#include <Wire.h>
#endif

/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
    #define MPL3115A2_ADDRESS                       (0x60)    // 1100000
/*=========================================================================*/

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    #define MPL3115A2_REGISTER_STATUS               (0x00)
    #define MPL3115A2_REGISTER_STATUS_TDR 0x02
    #define MPL3115A2_REGISTER_STATUS_PDR 0x04
    #define MPL3115A2_REGISTER_STATUS_PTDR 0x08

    #define MPL3115A2_REGISTER_PRESSURE_MSB         (0x01)
    #define MPL3115A2_REGISTER_PRESSURE_CSB         (0x02)
    #define MPL3115A2_REGISTER_PRESSURE_LSB         (0x03)

    #define MPL3115A2_REGISTER_TEMP_MSB             (0x04)
    #define MPL3115A2_REGISTER_TEMP_LSB             (0x05)

    #define MPL3115A2_REGISTER_DR_STATUS            (0x06)

    #define MPL3115A2_OUT_P_DELTA_MSB               (0x07)
    #define MPL3115A2_OUT_P_DELTA_CSB               (0x08)
    #define MPL3115A2_OUT_P_DELTA_LSB               (0x09)

    #define MPL3115A2_OUT_T_DELTA_MSB               (0x0A)
    #define MPL3115A2_OUT_T_DELTA_LSB               (0x0B)

    #define MPL3115A2_WHOAMI                        (0x0C)

#define MPL3115A2_PT_DATA_CFG 0x13
#define MPL3115A2_PT_DATA_CFG_TDEFE 0x01
#define MPL3115A2_PT_DATA_CFG_PDEFE 0x02
#define MPL3115A2_PT_DATA_CFG_DREM 0x04

#define MPL3115A2_CTRL_REG1                     (0x26)
#define MPL3115A2_CTRL_REG1_SBYB 0x01
#define MPL3115A2_CTRL_REG1_OST 0x02
#define MPL3115A2_CTRL_REG1_RST 0x04
#define MPL3115A2_CTRL_REG1_OS1 0x00
#define MPL3115A2_CTRL_REG1_OS2 0x08
#define MPL3115A2_CTRL_REG1_OS4 0x10
#define MPL3115A2_CTRL_REG1_OS8 0x18
#define MPL3115A2_CTRL_REG1_OS16 0x20
#define MPL3115A2_CTRL_REG1_OS32 0x28
#define MPL3115A2_CTRL_REG1_OS64 0x30
#define MPL3115A2_CTRL_REG1_OS128 0x38
#define MPL3115A2_CTRL_REG1_RAW 0x40
#define MPL3115A2_CTRL_REG1_ALT 0x80
#define MPL3115A2_CTRL_REG1_BAR 0x00
#define MPL3115A2_CTRL_REG2                     (0x27)
#define MPL3115A2_CTRL_REG3                     (0x28)
#define MPL3115A2_CTRL_REG4                     (0x29)
#define MPL3115A2_CTRL_REG5                     (0x2A)

    #define MPL3115A2_REGISTER_STARTCONVERSION      (0x12)
/*=========================================================================*/

class Adafruit_MPL3115A2{
 public:
  Adafruit_MPL3115A2();
  boolean begin(void);
  float getPressure(void);
  float getAltitude(void);
  float getTemperature(void);

  void write8(uint8_t a, uint8_t d);

 private:
  uint8_t read8(uint8_t a);
  uint8_t mode;

};
