/**************************************************************************/
/*!
    @file     Adafruit_FRAM_I2C.cpp
    @author   KTOWN (Adafruit Industries)
    @license  BSD (see license.txt)

    Driver for the Adafruit I2C FRAM breakout.

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/
//#include <avr/pgmspace.h>
//#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#include "Adafruit_FRAM_I2C.h"

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
Adafruit_FRAM_I2C::Adafruit_FRAM_I2C(void) 
{
  _framInitialised = false;
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    Initializes I2C and configures the chip (call this function before
    doing anything else)
*/
/**************************************************************************/
boolean Adafruit_FRAM_I2C::begin(uint8_t addr) 
{
  i2c_addr = addr;
  Wire.begin();
  
  /* Make sure we're actually connected */
  uint16_t manufID, prodID;
  getDeviceID(&manufID, &prodID);
  if (manufID != 0x00A)
  {
    Serial.print("Unexpected Manufacturer ID: 0x");
    Serial.println(manufID, HEX);
    return false;
  }
  if (prodID != 0x510)
  {
    Serial.print("Unexpected Product ID: 0x");
    Serial.println(prodID, HEX);
    return false;
  }

  /* Everything seems to be properly initialised and connected */
  _framInitialised = true;

  return true;
}

/**************************************************************************/
/*!
    @brief  Writes a byte at the specific FRAM address
    
    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] framAddr
                The 16-bit address to write to in FRAM memory
    @params[in] i2cAddr
                The 8-bit value to write at framAddr
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::write8 (uint16_t framAddr, uint8_t value)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(framAddr >> 8);
  Wire.write(framAddr & 0xFF);
  Wire.write(value);
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value from the specified FRAM address

    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] framAddr
                The 16-bit address to read from in FRAM memory

    @returns    The 8-bit value retrieved at framAddr
*/
/**************************************************************************/
uint8_t Adafruit_FRAM_I2C::read8 (uint16_t framAddr)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(framAddr >> 8);
  Wire.write(framAddr & 0xFF);
  Wire.endTransmission();

  Wire.requestFrom(i2c_addr, (uint8_t)1);
  
  return Wire.read();
}

/**************************************************************************/
/*!
    @brief  Reads the Manufacturer ID and the Product ID frm the IC

    @params[out]  manufacturerID
                  The 12-bit manufacturer ID (Fujitsu = 0x00A)
    @params[out]  productID
                  The memory density (bytes 11..8) and proprietary
                  Product ID fields (bytes 7..0). Should be 0x510 for
                  the MB85RC256V.
*/
/**************************************************************************/
void Adafruit_FRAM_I2C::getDeviceID(uint16_t *manufacturerID, uint16_t *productID)
{
  uint8_t a[3] = { 0, 0, 0 };
  uint8_t results;
  
  Wire.beginTransmission(MB85RC_SLAVE_ID >> 1);
  Wire.write(i2c_addr << 1);
  results = Wire.endTransmission(false);

  Wire.requestFrom(MB85RC_SLAVE_ID >> 1, 3);
  a[0] = Wire.read();
  a[1] = Wire.read();
  a[2] = Wire.read();

  /* Shift values to separate manuf and prod IDs */
  /* See p.10 of http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC256V-DS501-00017-3v0-E.pdf */
  *manufacturerID = (a[0] << 4) + (a[1]  >> 4);
  *productID = ((a[1] & 0x0F) << 8) + a[2];
}
