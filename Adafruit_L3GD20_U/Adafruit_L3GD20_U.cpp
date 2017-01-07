/***************************************************
  This is a library for the L3GD20 GYROSCOPE

  Designed specifically to work with the Adafruit L3GD20 Breakout
  ----> https://www.adafruit.com/products/1032

  These sensors use I2C or SPI to communicate, 2 pins (I2C)
  or 4 pins (SPI) are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Kevin "KTOWN" Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <limits.h>

#include "Adafruit_L3GD20_U.h"

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
void Adafruit_L3GD20_Unified::write8(byte reg, byte value)
{
  Wire.beginTransmission(L3GD20_ADDRESS);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)value);
  #else
    Wire.send(reg);
    Wire.send(value);
  #endif
  Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
byte Adafruit_L3GD20_Unified::read8(byte reg)
{
  byte value;

  Wire.beginTransmission((byte)L3GD20_ADDRESS);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();
  Wire.requestFrom((byte)L3GD20_ADDRESS, (byte)1);
  while (!Wire.available()); // Wait for data to arrive.
  #if ARDUINO >= 100
    value = Wire.read();
  #else
    value = Wire.receive();
  #endif
  Wire.endTransmission();

  return value;
}

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit_L3GD20_Unified class
*/
/**************************************************************************/
Adafruit_L3GD20_Unified::Adafruit_L3GD20_Unified(int32_t sensorID) {
  _sensorID = sensorID;
  _autoRangeEnabled = false;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
bool Adafruit_L3GD20_Unified::begin(gyroRange_t rng)
{
  /* Enable I2C */
  Wire.begin();

  /* Set the range the an appropriate value */
  _range = rng;

  /* Clear the raw sensor data */
  raw.x = 0;
  raw.y = 0;
  raw.z = 0;

  /* Make sure we have the correct chip ID since this checks
     for correct address and that the IC is properly connected */
  uint8_t id = read8(GYRO_REGISTER_WHO_AM_I);
  //Serial.println(id, HEX);
  if ((id != L3GD20_ID) && (id != L3GD20H_ID))
  {
    return false;
  }

  /* Set CTRL_REG1 (0x20)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
   7-6  DR1/0     Output data rate                                   00
   5-4  BW1/0     Bandwidth selection                                00
     3  PD        0 = Power-down mode, 1 = normal/sleep mode          0
     2  ZEN       Z-axis enable (0 = disabled, 1 = enabled)           1
     1  YEN       Y-axis enable (0 = disabled, 1 = enabled)           1
     0  XEN       X-axis enable (0 = disabled, 1 = enabled)           1 */

  /* Reset then switch to normal mode and enable all three channels */
  write8(GYRO_REGISTER_CTRL_REG1, 0x00);
  write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG2 (0x21)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
   5-4  HPM1/0    High-pass filter mode selection                    00
   3-0  HPCF3..0  High-pass filter cutoff frequency selection      0000 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG3 (0x22)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  I1_Int1   Interrupt enable on INT1 (0=disable,1=enable)       0
     6  I1_Boot   Boot status on INT1 (0=disable,1=enable)            0
     5  H-Lactive Interrupt active config on INT1 (0=high,1=low)      0
     4  PP_OD     Push-Pull/Open-Drain (0=PP, 1=OD)                   0
     3  I2_DRDY   Data ready on DRDY/INT2 (0=disable,1=enable)        0
     2  I2_WTM    FIFO wtrmrk int on DRDY/INT2 (0=dsbl,1=enbl)        0
     1  I2_ORun   FIFO overrun int on DRDY/INT2 (0=dsbl,1=enbl)       0
     0  I2_Empty  FIFI empty int on DRDY/INT2 (0=dsbl,1=enbl)         0 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG4 (0x23)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  BDU       Block Data Update (0=continuous, 1=LSB/MSB)         0
     6  BLE       Big/Little-Endian (0=Data LSB, 1=Data MSB)          0
   5-4  FS1/0     Full scale selection                               00
                                  00 = 250 dps
                                  01 = 500 dps
                                  10 = 2000 dps
                                  11 = 2000 dps
     0  SIM       SPI Mode (0=4-wire, 1=3-wire)                       0 */

  /* Adjust resolution if requested */
  switch(_range)
  {
    case GYRO_RANGE_250DPS:
      write8(GYRO_REGISTER_CTRL_REG4, 0x00);
      break;
    case GYRO_RANGE_500DPS:
      write8(GYRO_REGISTER_CTRL_REG4, 0x10);
      break;
    case GYRO_RANGE_2000DPS:
      write8(GYRO_REGISTER_CTRL_REG4, 0x20);
      break;
  }
  /* ------------------------------------------------------------------ */

  /* Set CTRL_REG5 (0x24)
   ====================================================================
   BIT  Symbol    Description                                   Default
   ---  ------    --------------------------------------------- -------
     7  BOOT      Reboot memory content (0=normal, 1=reboot)          0
     6  FIFO_EN   FIFO enable (0=FIFO disable, 1=enable)              0
     4  HPen      High-pass filter enable (0=disable,1=enable)        0
   3-2  INT1_SEL  INT1 Selection config                              00
   1-0  OUT_SEL   Out selection config                               00 */

  /* Nothing to do ... keep default values */
  /* ------------------------------------------------------------------ */

  return true;
}

/**************************************************************************/
/*!
    @brief  Enables or disables auto-ranging
*/
/**************************************************************************/
void Adafruit_L3GD20_Unified::enableAutoRange(bool enabled)
{
  _autoRangeEnabled = enabled;
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event
*/
/**************************************************************************/
bool Adafruit_L3GD20_Unified::getEvent(sensors_event_t* event)
{
  bool readingValid = false;

  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

  /* Clear the raw data placeholder */
  raw.x = 0;
  raw.y = 0;
  raw.z = 0;

  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_GYROSCOPE;

  while(!readingValid)
  {
    event->timestamp = millis();

    /* Read 6 bytes from the sensor */
    Wire.beginTransmission((byte)L3GD20_ADDRESS);
    #if ARDUINO >= 100
      Wire.write(GYRO_REGISTER_OUT_X_L | 0x80);
    #else
      Wire.send(GYRO_REGISTER_OUT_X_L | 0x80);
    #endif
    if (Wire.endTransmission() != 0) {
        // Error. Retry.
        continue;
    }
    Wire.requestFrom((byte)L3GD20_ADDRESS, (byte)6);

    #if ARDUINO >= 100
      uint8_t xlo = Wire.read();
      uint8_t xhi = Wire.read();
      uint8_t ylo = Wire.read();
      uint8_t yhi = Wire.read();
      uint8_t zlo = Wire.read();
      uint8_t zhi = Wire.read();
    #else
      uint8_t xlo = Wire.receive();
      uint8_t xhi = Wire.receive();
      uint8_t ylo = Wire.receive();
      uint8_t yhi = Wire.receive();
      uint8_t zlo = Wire.receive();
      uint8_t zhi = Wire.receive();
    #endif

    /* Shift values to create properly formed integer (low byte first) */
    event->gyro.x = (int16_t)(xlo | (xhi << 8));
    event->gyro.y = (int16_t)(ylo | (yhi << 8));
    event->gyro.z = (int16_t)(zlo | (zhi << 8));

    /* Assign raw values in case someone needs them */
    raw.x = (int16_t)(xlo | (xhi << 8));
    raw.y = (int16_t)(ylo | (yhi << 8));
    raw.z = (int16_t)(zlo | (zhi << 8));

    /* Make sure the sensor isn't saturating if auto-ranging is enabled */
    if (!_autoRangeEnabled)
    {
      readingValid = true;
    }
    else
    {
      /* Check if the sensor is saturating or not */
      if ( (event->gyro.x >= 32760) | (event->gyro.x <= -32760) |
           (event->gyro.y >= 32760) | (event->gyro.y <= -32760) |
           (event->gyro.z >= 32760) | (event->gyro.z <= -32760) )
      {
        /* Saturating .... increase the range if we can */
        switch(_range)
        {
          case GYRO_RANGE_500DPS:
            /* Push the range up to 2000dps */
            _range = GYRO_RANGE_2000DPS;
            write8(GYRO_REGISTER_CTRL_REG1, 0x00);
            write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
            write8(GYRO_REGISTER_CTRL_REG4, 0x20);
            write8(GYRO_REGISTER_CTRL_REG5, 0x80);
            readingValid = false;
            // Serial.println("Changing range to 2000DPS");
            break;
          case GYRO_RANGE_250DPS:
            /* Push the range up to 500dps */
            _range = GYRO_RANGE_500DPS;
            write8(GYRO_REGISTER_CTRL_REG1, 0x00);
            write8(GYRO_REGISTER_CTRL_REG1, 0x0F);
            write8(GYRO_REGISTER_CTRL_REG4, 0x10);
            write8(GYRO_REGISTER_CTRL_REG5, 0x80);
            readingValid = false;
            // Serial.println("Changing range to 500DPS");
            break;
          default:
            readingValid = true;
            break;
        }
      }
      else
      {
        /* All values are withing range */
        readingValid = true;
      }
    }
  }

  /* Compensate values depending on the resolution */
  switch(_range)
  {
    case GYRO_RANGE_250DPS:
      event->gyro.x *= GYRO_SENSITIVITY_250DPS;
      event->gyro.y *= GYRO_SENSITIVITY_250DPS;
      event->gyro.z *= GYRO_SENSITIVITY_250DPS;
      break;
    case GYRO_RANGE_500DPS:
      event->gyro.x *= GYRO_SENSITIVITY_500DPS;
      event->gyro.y *= GYRO_SENSITIVITY_500DPS;
      event->gyro.z *= GYRO_SENSITIVITY_500DPS;
      break;
    case GYRO_RANGE_2000DPS:
      event->gyro.x *= GYRO_SENSITIVITY_2000DPS;
      event->gyro.y *= GYRO_SENSITIVITY_2000DPS;
      event->gyro.z *= GYRO_SENSITIVITY_2000DPS;
      break;
  }

  /* Convert values to rad/s */
  event->gyro.x *= SENSORS_DPS_TO_RADS;
  event->gyro.y *= SENSORS_DPS_TO_RADS;
  event->gyro.z *= SENSORS_DPS_TO_RADS;

  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void  Adafruit_L3GD20_Unified::getSensor(sensor_t* sensor)
{
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "L3GD20", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_GYROSCOPE;
  sensor->min_delay   = 0;
  sensor->max_value   = (float)this->_range * SENSORS_DPS_TO_RADS;
  sensor->min_value   = (this->_range * -1.0) * SENSORS_DPS_TO_RADS;
  sensor->resolution  = 0.0F; // TBD
}
