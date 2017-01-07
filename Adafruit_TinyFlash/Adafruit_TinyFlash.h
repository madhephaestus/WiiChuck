#ifndef _TINYFLASH_H_
#define _TINYFLASH_H_

#include <Arduino.h>

class Adafruit_TinyFlash {
 public:
#ifdef __AVR_ATtiny85__
  Adafruit_TinyFlash(uint8_t cs = 3);
#else
  Adafruit_TinyFlash(uint8_t cs = 10);
#endif
  uint32_t          begin(void);
  boolean           beginRead(uint32_t addr),
                    writePage(uint32_t addr, uint8_t *data),
                    eraseChip(void),
                    eraseSector(uint32_t addr);
  uint8_t           readNextByte(void);
  void              endRead(void);
 private:
  boolean           waitForReady(uint32_t timeout = 100L),
                    writeEnable(void);
  void              writeDisable(void),
                    cmd(uint8_t c);
#ifndef __AVR_ATtiny85__
  volatile uint8_t *cs_port;
#endif
  uint8_t           cs_mask;
};

#endif // _TINYFLASH_H_
