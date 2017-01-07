/**************************************************************************/
/*!
    @file     Adafruit_MPL3115A2.cpp
    @author   K.Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    Driver for the MPL3115A2 barometric pressure sensor

    This is a library for the Adafruit MPL3115A2 breakout
    ----> https://www.adafruit.com/products/1893

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0 - First release
*/
/**************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include <TinyWireM.h>
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "Adafruit_MPL3115A2.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new MPL3115A2 class
*/
/**************************************************************************/
Adafruit_MPL3115A2::Adafruit_MPL3115A2() {

}

/**************************************************************************/
/*!
    @brief  Setups the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
boolean Adafruit_MPL3115A2::begin() {
  Wire.begin();
  uint8_t whoami = read8(MPL3115A2_WHOAMI);
  if (whoami != 0xC4) {
    return false;
  }

  write8(MPL3115A2_CTRL_REG1, 
	 MPL3115A2_CTRL_REG1_SBYB |
	 MPL3115A2_CTRL_REG1_OS128 |
	 MPL3115A2_CTRL_REG1_ALT);
  write8(MPL3115A2_PT_DATA_CFG, 
	 MPL3115A2_PT_DATA_CFG_TDEFE |
	 MPL3115A2_PT_DATA_CFG_PDEFE |
	 MPL3115A2_PT_DATA_CFG_DREM);
  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the floating-point pressure level in kPa
*/
/**************************************************************************/
float Adafruit_MPL3115A2::getPressure() {
  uint32_t pressure;

  write8(MPL3115A2_CTRL_REG1, 
	 MPL3115A2_CTRL_REG1_SBYB |
	 MPL3115A2_CTRL_REG1_OS128 |
	 MPL3115A2_CTRL_REG1_BAR);

  uint8_t sta = 0;
  while (! (sta & MPL3115A2_REGISTER_STATUS_PDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    delay(10);
  }
  Wire.beginTransmission(MPL3115A2_ADDRESS); // start transmission to device 
  Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB); 
#ifdef __AVR_ATtiny85__
  Wire.endTransmission(); // end transmission
#else
  Wire.endTransmission(false); // end transmission
#endif  
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)3);// send data n-bytes read
  pressure = Wire.read(); // receive DATA
  pressure <<= 8;
  pressure |= Wire.read(); // receive DATA
  pressure <<= 8;
  pressure |= Wire.read(); // receive DATA
  pressure >>= 4;

  float baro = pressure;
  baro /= 4.0;
  return baro;
}

float Adafruit_MPL3115A2::getAltitude() {
  int32_t alt;

  write8(MPL3115A2_CTRL_REG1, 
	 MPL3115A2_CTRL_REG1_SBYB |
	 MPL3115A2_CTRL_REG1_OS128 |
	 MPL3115A2_CTRL_REG1_ALT);

  uint8_t sta = 0;
  while (! (sta & MPL3115A2_REGISTER_STATUS_PDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    delay(10);
  }
  Wire.beginTransmission(MPL3115A2_ADDRESS); // start transmission to device 
  Wire.write(MPL3115A2_REGISTER_PRESSURE_MSB); 
#ifdef __AVR_ATtiny85__
  Wire.endTransmission(); // end transmission
#else
  Wire.endTransmission(false); // end transmission
#endif  
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)3);// send data n-bytes read
  alt = Wire.read(); // receive DATA
  alt <<= 8;
  alt |= Wire.read(); // receive DATA
  alt <<= 8;
  alt |= Wire.read(); // receive DATA
  alt >>= 4;

  if (alt & 0x800000) {
    alt |= 0xFF000000;
  }

  float altitude = alt;
  altitude /= 16.0;
  return altitude;
}

/**************************************************************************/
/*!
    @brief  Gets the floating-point temperature in Centigrade
*/
/**************************************************************************/
float Adafruit_MPL3115A2::getTemperature() {
  uint16_t t;

  uint8_t sta = 0;
  while (! (sta & MPL3115A2_REGISTER_STATUS_TDR)) {
    sta = read8(MPL3115A2_REGISTER_STATUS);
    delay(10);
  }
  Wire.beginTransmission(MPL3115A2_ADDRESS); // start transmission to device 
  Wire.write(MPL3115A2_REGISTER_TEMP_MSB); 
#ifdef __AVR_ATtiny85__
  Wire.endTransmission(); // end transmission
#else
  Wire.endTransmission(false); // end transmission
#endif  
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)2);// send data n-bytes read
  t = Wire.read(); // receive DATA
  t <<= 8;
  t |= Wire.read(); // receive DATA
  t >>= 4;

  float temp = t;
  temp /= 16.0;
  return temp;
}




/*********************************************************************/

uint8_t Adafruit_MPL3115A2::read8(uint8_t a) {
  Wire.beginTransmission(MPL3115A2_ADDRESS); // start transmission to device 
  Wire.write(a); // sends register address to read from
#ifdef __AVR_ATtiny85__
  Wire.endTransmission(); // end transmission
#else
  Wire.endTransmission(false); // end transmission
#endif  
  
  Wire.requestFrom((uint8_t)MPL3115A2_ADDRESS, (uint8_t)1);// send data n-bytes read
  return Wire.read(); // receive DATA
}

void Adafruit_MPL3115A2::write8(uint8_t a, uint8_t d) {
  Wire.beginTransmission(MPL3115A2_ADDRESS); // start transmission to device 
  Wire.write(a); // sends register address to write to
  Wire.write(d); // sends register data
#ifdef __AVR_ATtiny85__
  Wire.endTransmission(); // end transmission
#else
  Wire.endTransmission(false); // end transmission
#endif  
}
