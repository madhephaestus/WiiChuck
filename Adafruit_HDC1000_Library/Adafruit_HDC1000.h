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

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#define HDC1000_I2CADDR       0x40
#define HDC1000_TEMP          0x00
#define HDC1000_HUMID         0x01
#define HDC1000_CONFIG        0x02
#define HDC1000_CONFIG_RST    (1 << 15)
#define HDC1000_CONFIG_HEAT   (1 << 13)
#define HDC1000_CONFIG_MODE   (1 << 12)
#define HDC1000_CONFIG_BATT   (1 << 11)
#define HDC1000_CONFIG_TRES_14  0
#define HDC1000_CONFIG_TRES_11  (1 << 10)
#define HDC1000_CONFIG_HRES_14  0
#define HDC1000_CONFIG_HRES_11  (1 << 8)
#define HDC1000_CONFIG_HRES_8   (1 << 9)

#define HDC1000_SERIAL1       0xFB
#define HDC1000_SERIAL2       0xFC
#define HDC1000_SERIAL3       0xFD
#define HDC1000_MANUFID       0xFE
#define HDC1000_DEVICEID      0xFF



class Adafruit_HDC1000 {
 public:
  Adafruit_HDC1000();
  boolean begin(uint8_t a = HDC1000_I2CADDR);
  float readTemperature(void);
  float readHumidity(void);
  void reset(void);

  uint16_t read16(uint8_t a, uint8_t d=0);
  uint32_t read32(uint8_t a, uint8_t d=0);
  
 private:
  boolean readData(void);
  float humidity, temp;
  uint8_t _i2caddr;
};

