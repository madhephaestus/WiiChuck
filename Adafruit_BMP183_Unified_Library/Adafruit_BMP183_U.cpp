/***************************************************************************
  This is a library for the BMP183 pressure sensor

  Designed specifically to work with the Adafruit BMP183 Breakout 
  ----> http://www.adafruit.com/products/1900
 
  These sensors use SPI to communicate, 4 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "SPI.h"
#include <math.h>
#include <limits.h>

#include "Adafruit_BMP183_U.h"


#define BMP183_USE_DATASHEET_VALS (0) /* Set to 1 for sanity check */


/***************************************************************************
 CONSTRUCTORS
 ***************************************************************************/
 
/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit_BMP183_Unified class
*/
/**************************************************************************/

Adafruit_BMP183_Unified::Adafruit_BMP183_Unified(int8_t SPICS,
						 int32_t sensorID ) {
  _cs = SPICS;
  _clk = _miso = _mosi = -1;
  
  _sensorID = sensorID;
}

Adafruit_BMP183_Unified::Adafruit_BMP183_Unified(int8_t SPICLK, 
						 int8_t SPIMISO, 
						 int8_t SPIMOSI, 
						 int8_t SPICS, 
						 int32_t sensorID) {
  _cs = SPICS;
  _clk = SPICLK;
  _miso = SPIMISO;
  _mosi = SPIMOSI;
  
  _sensorID = sensorID;
}


/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

uint8_t Adafruit_BMP183_Unified::SPIxfer(uint8_t x) {
  if (_clk == -1) {
    return SPI.transfer(x);
  } else {
    //Serial.println("Software SPI");
    uint8_t reply = 0;
    for (int i=7; i>=0; i--) {
      reply <<= 1;
      digitalWrite(_clk, LOW);
      digitalWrite(_mosi, x & (1<<i));
      digitalWrite(_clk, HIGH);
      if (digitalRead(_miso)) 
	reply |= 1;
    }
    return reply;
  }
}

/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over SPI
*/
/**************************************************************************/
void Adafruit_BMP183_Unified::writeCommand(byte reg, byte value)
{
  digitalWrite(_cs, LOW);
  SPIxfer(reg & 0x7F);
  SPIxfer(value);
  digitalWrite(_cs, HIGH);
}

/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t Adafruit_BMP183_Unified::read8(byte reg)
{
  uint8_t value;

  digitalWrite(_cs, LOW);
  SPIxfer(0x80 | reg);
  value = SPIxfer(0x00);
  digitalWrite(_cs, HIGH);

  return value;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
*/
/**************************************************************************/
uint16_t Adafruit_BMP183_Unified::read16(byte reg)
{
  uint16_t value;

  digitalWrite(_cs, LOW);
  SPIxfer(0x80 | reg);
  value = SPIxfer(0x00);
  value <<= 8;
  value |= SPIxfer(0x00);
  digitalWrite(_cs, HIGH);

  return value;
}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t Adafruit_BMP183_Unified::readS16(byte reg)
{
  return (int16_t)read16(reg);
}

/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void Adafruit_BMP183_Unified::readCoefficients(void)
{
  #if BMP183_USE_DATASHEET_VALS
    _bmp183_coeffs.ac1 = 408;
    _bmp183_coeffs.ac2 = -72;
    _bmp183_coeffs.ac3 = -14383;
    _bmp183_coeffs.ac4 = 32741;
    _bmp183_coeffs.ac5 = 32757;
    _bmp183_coeffs.ac6 = 23153;
    _bmp183_coeffs.b1  = 6190;
    _bmp183_coeffs.b2  = 4;
    _bmp183_coeffs.mb  = -32768;
    _bmp183_coeffs.mc  = -8711;
    _bmp183_coeffs.md  = 2868;
    _bmp183Mode        = 0;
  #else
    _bmp183_coeffs.ac1 = readS16(BMP183_REGISTER_CAL_AC1);
    _bmp183_coeffs.ac2 = readS16(BMP183_REGISTER_CAL_AC2);
    _bmp183_coeffs.ac3 = readS16(BMP183_REGISTER_CAL_AC3);
    _bmp183_coeffs.ac4 = read16(BMP183_REGISTER_CAL_AC4);
    _bmp183_coeffs.ac5 = read16(BMP183_REGISTER_CAL_AC5);
    _bmp183_coeffs.ac6 = read16(BMP183_REGISTER_CAL_AC6);
    _bmp183_coeffs.b1 = readS16(BMP183_REGISTER_CAL_B1);
    _bmp183_coeffs.b2 = readS16(BMP183_REGISTER_CAL_B2);
    _bmp183_coeffs.mb = readS16(BMP183_REGISTER_CAL_MB);
    _bmp183_coeffs.mc = readS16(BMP183_REGISTER_CAL_MC);
    _bmp183_coeffs.md = readS16(BMP183_REGISTER_CAL_MD);
  #endif
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
int16_t Adafruit_BMP183_Unified::readRawTemperature()
{
  #if BMP183_USE_DATASHEET_VALS
    return 27898;
  #else
    uint16_t t;
    writeCommand(BMP183_REGISTER_CONTROL, BMP183_REGISTER_READTEMPCMD);
    delay(5);
    return  read16(BMP183_REGISTER_TEMPDATA);
  #endif
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
int32_t Adafruit_BMP183_Unified::readRawPressure()
{
  #if BMP183_USE_DATASHEET_VALS
    *pressure = 23843;
  #else
    uint8_t  p8;
    uint16_t p16;
    int32_t  p32;

    writeCommand(BMP183_REGISTER_CONTROL, BMP183_REGISTER_READPRESSURECMD + (_bmp183Mode << 6));
    switch(_bmp183Mode)
    {
      case BMP183_MODE_ULTRALOWPOWER:
        delay(5);
        break;
      case BMP183_MODE_STANDARD:
        delay(8);
        break;
      case BMP183_MODE_HIGHRES:
        delay(14);
        break;
      case BMP183_MODE_ULTRAHIGHRES:
      default:
        delay(26);
        break;
    }

    p16 = read16(BMP183_REGISTER_PRESSUREDATA);
    p32 = (uint32_t)p16 << 8;
    p8 = read8(BMP183_REGISTER_PRESSUREDATA+2);
    p32 += p8;
    p32 >>= (8 - _bmp183Mode);
    
    return p32;
#endif
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
 
/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
bool Adafruit_BMP183_Unified::begin(bmp183_mode_t mode)
{
 
  // Enable SPI
  if (_clk == -1) {
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
  } else {
    pinMode(_clk, OUTPUT);
    digitalWrite(_clk, HIGH);
    pinMode(_mosi, OUTPUT);
    digitalWrite(_mosi, HIGH);
    pinMode(_miso, INPUT);
  }
  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);

  /* Mode boundary check */
  if ((mode > BMP183_MODE_ULTRAHIGHRES) || (mode < 0))
  {
    mode = BMP183_MODE_ULTRAHIGHRES;
  }

  /* Make sure we have the right device */
  uint8_t id;
  id = read8(BMP183_REGISTER_CHIPID);
  if(id != 0x55)
  {
    return false;
  }

  /* Set the mode indicator */
  _bmp183Mode = mode;

  /* Coefficients need to be read once */
  readCoefficients();
    
  getPressure();

  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the compensated pressure level in kPa
*/
/**************************************************************************/
float Adafruit_BMP183_Unified::getPressure(void)
{
  int32_t  ut = 0, up = 0, compp = 0;
  int32_t  x1, x2, b5, b6, x3, b3, p;
  uint32_t b4, b7;

  /* Get the raw pressure and temperature values */
  ut = readRawTemperature();
  up = readRawPressure();

  /* Temperature compensation */
  x1 = (ut - (int32_t)(_bmp183_coeffs.ac6))*((int32_t)(_bmp183_coeffs.ac5))/pow(2,15);
  x2 = ((int32_t)(_bmp183_coeffs.mc*pow(2,11)))/(x1+(int32_t)(_bmp183_coeffs.md));
  b5 = x1 + x2;

  /* Pressure compensation */
  b6 = b5 - 4000;
  x1 = (_bmp183_coeffs.b2 * ((b6 * b6) >> 12)) >> 11;
  x2 = (_bmp183_coeffs.ac2 * b6) >> 11;
  x3 = x1 + x2;
  b3 = (((((int32_t) _bmp183_coeffs.ac1) * 4 + x3) << _bmp183Mode) + 2) >> 2;
  x1 = (_bmp183_coeffs.ac3 * b6) >> 13;
  x2 = (_bmp183_coeffs.b1 * ((b6 * b6) >> 12)) >> 16;
  x3 = ((x1 + x2) + 2) >> 2;
  b4 = (_bmp183_coeffs.ac4 * (uint32_t) (x3 + 32768)) >> 15;
  b7 = ((uint32_t) (up - b3) * (50000 >> _bmp183Mode));

  if (b7 < 0x80000000)
  {
    p = (b7 << 1) / b4;
  }
  else
  {
    p = (b7 / b4) << 1;
  }

  x1 = (p >> 8) * (p >> 8);
  x1 = (x1 * 3038) >> 16;
  x2 = (-7357 * p) >> 16;
  compp = p + ((x1 + x2 + 3791) >> 4);

  /* Assign compensated pressure value */
  return compp;
}

/**************************************************************************/
/*!
    @brief  Reads the temperatures in degrees Celsius
*/
/**************************************************************************/
float Adafruit_BMP183_Unified::getTemperature(void)
{
  int32_t UT, X1, X2, B5;     // following ds convention
  float t;

  UT = readRawTemperature();

  #if BMP183_USE_DATASHEET_VALS
    // use datasheet numbers!
    UT = 27898;
    _bmp183_coeffs.ac6 = 23153;
    _bmp183_coeffs.ac5 = 32757;
    _bmp183_coeffs.mc = -8711;
    _bmp183_coeffs.md = 2868;
  #endif

  // step 1
  X1 = (UT - (int32_t)_bmp183_coeffs.ac6) * ((int32_t)_bmp183_coeffs.ac5) / pow(2,15);
  X2 = ((int32_t)_bmp183_coeffs.mc * pow(2,11)) / (X1+(int32_t)_bmp183_coeffs.md);
  B5 = X1 + X2;
  t = (B5+8)/pow(2,4);
  t /= 10;

  return t;
}

/**************************************************************************/
/*!
    Calculates the altitude (in meters) from the specified atmospheric
    pressure (in hPa), sea-level pressure (in hPa), and temperature (in �C)

    @param  seaLevel      Sea-level pressure in hPa
    @param  atmospheric   Atmospheric pressure in hPa
    @param  temp          Temperature in degrees Celsius
*/
/**************************************************************************/
float Adafruit_BMP183_Unified::pressureToAltitude(float seaLevel, float atmospheric, float temp)
{
  /* Hyposometric formula:                      */
  /*                                            */
  /*     ((P0/P)^(1/5.257) - 1) * (T + 273.15)  */
  /* h = -------------------------------------  */
  /*                   0.0065                   */
  /*                                            */
  /* where: h   = height (in meters)            */
  /*        P0  = sea-level pressure (in hPa)   */
  /*        P   = atmospheric pressure (in hPa) */
  /*        T   = temperature (in �C)           */

  return (((float)pow((seaLevel/atmospheric), 0.190223F) - 1.0F)
         * (temp + 273.15F)) / 0.0065F;
}

float Adafruit_BMP183_Unified::seaLevelForAltitude(float altitude, float atmospheric, float temp)
{
  /* Hyposometric formula:                      */
  /*                                            */
  /* P0=((((h*0.0065)/(temp + 273.15F))+1)^(^/0.190223F))*P */
  /*                                            */
  /* where: h   = height (in meters)            */
  /*        P0  = sea-level pressure (in hPa)   */
  /*        P   = atmospheric pressure (in hPa) */
  /*        T   = temperature (in �C)           */
  
  return (float)pow((((altitude*0.0065)/(temp + 273.15F))+1), (1.0/0.190223F))*atmospheric;
}



/**************************************************************************/
/*!
    @brief  Provides the sensor_t data for this sensor
*/
/**************************************************************************/
void Adafruit_BMP183_Unified::getSensor(sensor_t *sensor)
{
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "BMP183", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_PRESSURE;
  sensor->min_delay   = 0;
  sensor->max_value   = 300.0F;               // 300..1100 hPa
  sensor->min_value   = 1100.0F;
  sensor->resolution  = 0.01F;                // Datasheet states 0.01 hPa resolution
}

/**************************************************************************/
/*!
    @brief  Reads the sensor and returns the data as a sensors_event_t
*/
/**************************************************************************/
bool Adafruit_BMP183_Unified::getEvent(sensors_event_t *event)
{
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_PRESSURE;
  event->timestamp = 0;
  event->pressure = getPressure() / 100.0F;
  
  return true;
}
