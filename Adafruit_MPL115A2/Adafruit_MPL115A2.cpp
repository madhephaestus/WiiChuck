/**************************************************************************/
/*!
    @file     Adafruit_MPL115A2.cpp
    @author   K.Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    Driver for the MPL115A2 barometric pressure sensor

    This is a library for the Adafruit MPL115A2 breakout
    ----> https://www.adafruit.com/products/992

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0 - First release
    v1.1 - Rick Sellens added casts to make bit shifts work below 22.6C
         - get both P and T with a single call to getPT
*/
/**************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "Adafruit_MPL115A2.h"

static uint8_t i2cread(void) {
  uint8_t x;
  #if ARDUINO >= 100
  x = Wire.read();
  #else
  x = Wire.receive();
  #endif
  //Serial.print("0x"); Serial.println(x, HEX);
  return x;
}


static void i2cwrite(uint8_t x) {
  #if ARDUINO >= 100
  Wire.write((uint8_t)x);
  #else
  Wire.send(x);
  #endif
}

/**************************************************************************/
/*!
    @brief  Gets the factory-set coefficients for this particular sensor
*/
/**************************************************************************/
void Adafruit_MPL115A2::readCoefficients() {
  int16_t a0coeff;
  int16_t b1coeff;
  int16_t b2coeff;
  int16_t c12coeff;

  Wire.beginTransmission(MPL115A2_ADDRESS);
  i2cwrite((uint8_t)MPL115A2_REGISTER_A0_COEFF_MSB);
  Wire.endTransmission();

  Wire.requestFrom(MPL115A2_ADDRESS, 8);
  a0coeff = (( (uint16_t) i2cread() << 8) | i2cread());
  b1coeff = (( (uint16_t) i2cread() << 8) | i2cread());
  b2coeff = (( (uint16_t) i2cread() << 8) | i2cread());
  c12coeff = (( (uint16_t) (i2cread() << 8) | i2cread())) >> 2;

  /*  
  Serial.print("A0 = "); Serial.println(a0coeff, HEX);
  Serial.print("B1 = "); Serial.println(b1coeff, HEX);
  Serial.print("B2 = "); Serial.println(b2coeff, HEX);
  Serial.print("C12 = "); Serial.println(c12coeff, HEX);
  */

  _mpl115a2_a0 = (float)a0coeff / 8;
  _mpl115a2_b1 = (float)b1coeff / 8192;
  _mpl115a2_b2 = (float)b2coeff / 16384;
  _mpl115a2_c12 = (float)c12coeff;
  _mpl115a2_c12 /= 4194304.0;

  /*
  Serial.print("a0 = "); Serial.println(_mpl115a2_a0);
  Serial.print("b1 = "); Serial.println(_mpl115a2_b1);
  Serial.print("b2 = "); Serial.println(_mpl115a2_b2);
  Serial.print("c12 = "); Serial.println(_mpl115a2_c12);
  */
}

/**************************************************************************/
/*!
    @brief  Instantiates a new MPL115A2 class
*/
/**************************************************************************/
Adafruit_MPL115A2::Adafruit_MPL115A2() {
  _mpl115a2_a0 = 0.0F;
  _mpl115a2_b1 = 0.0F;
  _mpl115a2_b2 = 0.0F;
  _mpl115a2_c12 = 0.0F;
}

/**************************************************************************/
/*!
    @brief  Setups the HW (reads coefficients values, etc.)
*/
/**************************************************************************/
void Adafruit_MPL115A2::begin() {
  Wire.begin();
  // Read factory coefficient values (this only needs to be done once)
  readCoefficients();
}

/**************************************************************************/
/*!
    @brief  Gets the floating-point pressure level in kPa
*/
/**************************************************************************/
float Adafruit_MPL115A2::getPressure() {
  float     pressureComp,centigrade;

  getPT(&pressureComp, &centigrade);
  return pressureComp;
}


/**************************************************************************/
/*!
    @brief  Gets the floating-point temperature in Centigrade
*/
/**************************************************************************/
float Adafruit_MPL115A2::getTemperature() {
  float     pressureComp, centigrade;

  getPT(&pressureComp, &centigrade);
  return centigrade;
}

/**************************************************************************/
/*!
    @brief  Gets both at once and saves a little time
*/
/**************************************************************************/
void Adafruit_MPL115A2::getPT(float *P, float *T) {
  uint16_t 	pressure, temp;
  float     pressureComp;

  // Get raw pressure and temperature settings
  Wire.beginTransmission(MPL115A2_ADDRESS);
  i2cwrite((uint8_t)MPL115A2_REGISTER_STARTCONVERSION);
  i2cwrite((uint8_t)0x00);
  Wire.endTransmission();

  // Wait a bit for the conversion to complete (3ms max)
  delay(5);

  Wire.beginTransmission(MPL115A2_ADDRESS);
  i2cwrite((uint8_t)MPL115A2_REGISTER_PRESSURE_MSB);  // Register
  Wire.endTransmission();

  Wire.requestFrom(MPL115A2_ADDRESS, 4);
  pressure = (( (uint16_t) i2cread() << 8) | i2cread()) >> 6;
  temp = (( (uint16_t) i2cread() << 8) | i2cread()) >> 6;

  // See datasheet p.6 for evaluation sequence
  pressureComp = _mpl115a2_a0 + (_mpl115a2_b1 + _mpl115a2_c12 * temp ) * pressure + _mpl115a2_b2 * temp;

  // Return pressure and temperature as floating point values
  *P = ((65.0F / 1023.0F) * pressureComp) + 50.0F;        // kPa
  *T = ((float) temp - 498.0F) / -5.35F +25.0F;           // C
  
}



