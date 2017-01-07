/*************************************************** 
  This is a library for the HDC1000 Humidity & Temp Sensor

  Designed specifically to work with the HDC1000 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_HDC1000.h"
#if defined(__AVR__)
  #include <util/delay.h>
#endif

Adafruit_HDC1000::Adafruit_HDC1000() {
}


boolean Adafruit_HDC1000::begin(uint8_t addr) {
  _i2caddr = addr;

  Wire.begin();
  
  reset();
  if (read16(HDC1000_MANUFID) != 0x5449) return false;
  if (read16(HDC1000_DEVICEID) != 0x1000) return false;
  return true;
}



void Adafruit_HDC1000::reset(void) {
  // reset, and select 14 bit temp & humidity
  uint16_t config = HDC1000_CONFIG_RST | HDC1000_CONFIG_MODE | HDC1000_CONFIG_TRES_14 | HDC1000_CONFIG_HRES_14;

  Wire.beginTransmission(_i2caddr);
  Wire.write(config>>8);
  Wire.write(config&0xFF);
  Wire.endTransmission();
  delay(15);
}


float Adafruit_HDC1000::readTemperature(void) {
  
  float temp = (read32(HDC1000_TEMP, 20) >> 16);
  temp /= 65536;
  temp *= 165;
  temp -= 40;

  return temp;
}
  

float Adafruit_HDC1000::readHumidity(void) {

  float hum = (read32(HDC1000_TEMP, 20) & 0xFFFF);

  hum /= 65536;
  hum *= 100;

  return hum;
}



/*********************************************************************/

uint16_t Adafruit_HDC1000::read16(uint8_t a, uint8_t d) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(a);
  Wire.endTransmission();
  delay(d);
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)2);
  uint16_t r = Wire.read();
  r <<= 8;
  r |= Wire.read();
  //Serial.println(r, HEX);
  return r;
}

uint32_t Adafruit_HDC1000::read32(uint8_t a, uint8_t d) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(a);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom((uint8_t)_i2caddr, (uint8_t)4);
  uint32_t r = Wire.read();
  r <<= 8;
  r |= Wire.read();
  r <<= 8;
  r |= Wire.read();
  r <<= 8;
  r |= Wire.read();
  //Serial.println(r, HEX);
  return r;
}
