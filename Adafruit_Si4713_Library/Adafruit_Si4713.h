/*************************************************** 
  This is a library for the Si4713 FM Radio Transmitter with RDS

  Designed specifically to work with the Si4713 breakout in the
  adafruit shop
  ----> https://www.adafruit.com/products/1958

  These transmitters use I2C to communicate, plus reset pin. 
  3 pins are required to interface
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
#include <Wire.h>

//#define SI471X_CMD_DEBUG 

#define SI4710_ADDR0 0x11  // if SEN is low
#define SI4710_ADDR1 0x63  // if SEN is high, default!
#define SI4710_STATUS_CTS 0x80

/* COMMANDS */
#define SI4710_CMD_POWER_UP     0x01
#define SI4710_CMD_GET_REV      0x10
#define SI4710_CMD_POWER_DOWN   0x11
#define SI4710_CMD_SET_PROPERTY 0x12
#define SI4710_CMD_GET_PROPERTY 0x13
#define SI4710_CMD_GET_INT_STATUS 0x14
#define SI4710_CMD_PATCH_ARGS 0x15
#define SI4710_CMD_PATCH_DATA 0x16
#define SI4710_CMD_TX_TUNE_FREQ 0x30
#define SI4710_CMD_TX_TUNE_POWER 0x31
#define SI4710_CMD_TX_TUNE_MEASURE 0x32
#define SI4710_CMD_TX_TUNE_STATUS 0x33
#define SI4710_CMD_TX_ASQ_STATUS 0x34
#define SI4710_CMD_TX_RDS_BUFF 0x35
#define SI4710_CMD_TX_RDS_PS 0x36
#define SI4710_CMD_TX_AGC_OVERRIDE 0x48
#define SI4710_CMD_GPO_CTL 0x80
#define SI4710_CMD_GPO_SET 0x81

/* Parameters */

#define SI4713_PROP_GPO_IEN 0x0001
#define SI4713_PROP_DIGITAL_INPUT_FORMAT 0x0101
#define SI4713_PROP_DIGITAL_INPUT_SAMPLE_RATE 0x0103
#define SI4713_PROP_REFCLK_FREQ 0x0201
#define SI4713_PROP_REFCLK_PRESCALE 0x0202
#define SI4713_PROP_TX_COMPONENT_ENABLE 0x2100
#define SI4713_PROP_TX_AUDIO_DEVIATION 0x2101
#define SI4713_PROP_TX_PILOT_DEVIATION 0x2102
#define SI4713_PROP_TX_RDS_DEVIATION 0x2103
#define SI4713_PROP_TX_LINE_LEVEL_INPUT_LEVEL 0x2104
#define SI4713_PROP_TX_LINE_INPUT_MUTE 0x2105
#define SI4713_PROP_TX_PREEMPHASIS 0x2106
#define SI4713_PROP_TX_PILOT_FREQUENCY 0x2107
#define SI4713_PROP_TX_ACOMP_ENABLE 0x2200
#define SI4713_PROP_TX_ACOMP_THRESHOLD 0x2201
#define SI4713_PROP_TX_ATTACK_TIME 0x2202
#define SI4713_PROP_TX_RELEASE_TIME 0x2203
#define SI4713_PROP_TX_ACOMP_GAIN 0x2204
#define SI4713_PROP_TX_LIMITER_RELEASE_TIME 0x2205
#define SI4713_PROP_TX_ASQ_INTERRUPT_SOURCE 0x2300
#define SI4713_PROP_TX_ASQ_LEVEL_LOW 0x2301
#define SI4713_PROP_TX_ASQ_DURATION_LOW 0x2302
#define SI4713_PROP_TX_AQS_LEVEL_HIGH 0x2303
#define SI4713_PROP_TX_AQS_DURATION_HIGH 0x2304

#define SI4713_PROP_TX_RDS_INTERRUPT_SOURCE 0x2C00
#define SI4713_PROP_TX_RDS_PI 0x2C01
#define SI4713_PROP_TX_RDS_PS_MIX 0x2C02
#define SI4713_PROP_TX_RDS_PS_MISC 0x2C03
#define SI4713_PROP_TX_RDS_PS_REPEAT_COUNT 0x2C04
#define SI4713_PROP_TX_RDS_MESSAGE_COUNT 0x2C05
#define SI4713_PROP_TX_RDS_PS_AF 0x2C06
#define SI4713_PROP_TX_RDS_FIFO_SIZE 0x2C07


/* REGISTERS */


class Adafruit_Si4713  {
 public:
  Adafruit_Si4713(int8_t rstpin = -1);
  boolean begin(uint8_t addr = SI4710_ADDR1);
  void reset();
  
  void powerUp(void);
  void configure(void);
  uint8_t getRev(void);

  void tuneFM(uint16_t freqKHz);
  uint8_t getStatus(void);
  void readTuneStatus(void);
  void readTuneMeasure(uint16_t freq);
  void setTXpower(uint8_t pwr, uint8_t antcap = 0);
  void readASQ(void);
  void setProperty(uint16_t p, uint16_t v);

  // RDS stuff
  void beginRDS(uint16_t programID = 0xADAF);
  void setRDSstation(char *s);
  void setRDSbuffer(char *s);

  uint16_t currFreq;
  uint8_t currdBuV, currAntCap, currNoiseLevel, currASQ;
  int8_t currInLevel;


  void setGPIO(uint8_t x);
  void setGPIOctrl(uint8_t x);

 private:

  void sendCommand(uint8_t len);

  int8_t _rst;
  uint8_t _i2ccommand[10];  // holds the command buffer
  uint8_t _i2caddr;
};

