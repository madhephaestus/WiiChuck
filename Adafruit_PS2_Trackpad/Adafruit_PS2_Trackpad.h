
#ifndef _ADAFRUIT_PS2_H
#define _ADAFRUIT_PS2_H

#define ADAFRUIT_PS2_RESET 0xFF
#define ADAFRUIT_PS2_RESEND 0xFE
#define ADAFRUIT_PS2_SETDEFAULTS 0xF6
#define ADAFRUIT_PS2_DISABLE 0xF5
#define ADAFRUIT_PS2_ENABLE 0xF4
#define ADAFRUIT_PS2_SETRATE 0xF3
#define ADAFRUIT_PS2_GETDEVICEID 0xF2
#define ADAFRUIT_PS2_SETPOLL 0xF0
#define ADAFRUIT_PS2_SETWRAPMODE 0xEE
#define ADAFRUIT_PS2_RESETWRAPMODE 0xEC
#define ADAFRUIT_PS2_READDATA 0xEB
#define ADAFRUIT_PS2_SETSTREAMMODE 0xEA
#define ADAFRUIT_PS2_STATUSREQUEST 0xE9
#define ADAFRUIT_PS2_ERROR 0xFC
#define ADAFRUIT_PS2_ACK 0xFA

#define ADAFRUIT_PS2_SETRES 0xE8   // sends one argument
#define ADAFRUIT_PS2_SETSCALE11 0xE6 // no arguments
#define ADAFRUIT_PS2_SETSCALE21 0xE7 // no arguments
#define ADAFRUIT_PS2_GETINFO 0xE9  // receives 3 arg. bytes

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Adafruit_PS2 {
 public:
  Adafruit_PS2(uint8_t c, uint8_t d);
  void constructor(uint8_t c, uint8_t d);
  Adafruit_PS2();

  boolean command(uint8_t cmd);
  void write(uint8_t x);
  uint8_t read(void);
  void idle();
  void inhibit();

 private:
  void high(uint8_t p);
  void low(uint8_t p);


  uint8_t _clk, _data;
};

class Adafruit_PS2_Mouse : public Adafruit_PS2 {
 public:
  Adafruit_PS2_Mouse(uint8_t c, uint8_t d);
  Adafruit_PS2_Mouse();

  boolean begin(void);
  boolean reset(void);
  boolean readData(void);
  uint8_t readID(void);

  int16_t x, y, z, status;
  boolean left, right, middle;

 private:
};


class Adafruit_PS2_Trackpad : public Adafruit_PS2_Mouse {
 public:
  Adafruit_PS2_Trackpad(uint8_t c, uint8_t d);

  boolean begin(void);
  uint32_t E6Report(void);
  uint32_t E7Report(void);
  boolean absoluteMode(void);
  boolean tapMode(boolean enable);
  uint32_t getStatus(void);
  boolean readData(void);

  boolean finger, gesture;
 private:


};
#endif
