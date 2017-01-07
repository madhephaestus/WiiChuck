/*************************************************** 
This is a library for the MLX90614 temperature sensor SPECIFICALLY
FOR USE WITH TINYWIREM ON TRINKET/GEMMA

Requires the latest TinyWireM with repeated-start support
https://github.com/adafruit/TinyWireM

NOT FOR REGULAR ARDUINOS! Use the regular Adafruit_MLX90614 for that

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit in any redistribution
 ****************************************************/

#include "Adafruit_MiniMLX90614.h"

Adafruit_MiniMLX90614::Adafruit_MiniMLX90614(uint8_t i2caddr) {
  _addr = i2caddr;
}


boolean Adafruit_MiniMLX90614::begin(void) {
  TinyWireM.begin();

  /*
  for (uint8_t i=0; i<0x20; i++) {
    Serial.print(i); Serial.print(" = ");
    Serial.println(read16(i), HEX);
  }
  */
  return true;
}

//////////////////////////////////////////////////////


double Adafruit_MiniMLX90614::readObjectTempF(void) {
  return (readTemp(MLX90614_TOBJ1) * 9 / 5) + 32;
}


double Adafruit_MiniMLX90614::readAmbientTempF(void) {
  return (readTemp(MLX90614_TA) * 9 / 5) + 32;
}

double Adafruit_MiniMLX90614::readObjectTempC(void) {
  return readTemp(MLX90614_TOBJ1);
}


double Adafruit_MiniMLX90614::readAmbientTempC(void) {
  return readTemp(MLX90614_TA);
}

float Adafruit_MiniMLX90614::readTemp(uint8_t reg) {
  float temp;
  
  temp = read16(reg);
  temp *= .02;
  temp  -= 273.15;
  return temp;
}

/*********************************************************************/

uint16_t Adafruit_MiniMLX90614::read16(uint8_t a) {
  uint16_t ret;

  TinyWireM.beginTransmission(_addr); // start transmission to device 
  TinyWireM.write(a); // sends register address to read from
  TinyWireM.endTransmission(false); // end transmission
  
  TinyWireM.requestFrom(_addr, (uint8_t)3);// send data n-bytes read
  ret = TinyWireM.read(); // receive DATA
  ret |= TinyWireM.read() << 8; // receive DATA

  uint8_t pec = TinyWireM.read();

  return ret;
}
