#ifndef SPI_VFD_h
#define SPI_VFD_h

#include <inttypes.h>
#include "Print.h"

// commands
#define VFD_CLEARDISPLAY 0x01
#define VFD_RETURNHOME 0x02
#define VFD_ENTRYMODESET 0x04
#define VFD_DISPLAYCONTROL 0x08
#define VFD_CURSORSHIFT 0x10
#define VFD_FUNCTIONSET 0x30
#define VFD_SETCGRAMADDR 0x40
#define VFD_SETDDRAMADDR 0x80

// flags for display entry mode
#define VFD_ENTRYRIGHT 0x00
#define VFD_ENTRYLEFT 0x02
#define VFD_ENTRYSHIFTINCREMENT 0x01
#define VFD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define VFD_DISPLAYON 0x04
#define VFD_DISPLAYOFF 0x00
#define VFD_CURSORON 0x02
#define VFD_CURSOROFF 0x00
#define VFD_BLINKON 0x01
#define VFD_BLINKOFF 0x00

// flags for display/cursor shift
#define VFD_DISPLAYMOVE 0x08
#define VFD_CURSORMOVE 0x00
#define VFD_MOVERIGHT 0x04
#define VFD_MOVELEFT 0x00

// flags for function set
#define VFD_2LINE 0x08
#define VFD_1LINE 0x00
#define VFD_BRIGHTNESS25 0x03
#define VFD_BRIGHTNESS50 0x02
#define VFD_BRIGHTNESS75 0x01
#define VFD_BRIGHTNESS100 0x00

#define VFD_SPICOMMAND 0xF8
#define VFD_SPIDATA 0xFA

class SPI_VFD : public Print {
public:
  SPI_VFD(uint8_t data, uint8_t clock, uint8_t strobe, uint8_t brightness = VFD_BRIGHTNESS100);
    
  void init(uint8_t data, uint8_t clock, uint8_t strobe, uint8_t brightness);
  void begin(uint8_t cols, uint8_t rows, uint8_t brightness = VFD_BRIGHTNESS100);

  void clear();
  void home();

  void setBrightness(uint8_t brightness);
  uint8_t getBrightness();
  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t); 
#if ARDUINO >= 100
  virtual size_t write(uint8_t);
#else
  virtual void write(uint8_t);
#endif
  void command(uint8_t);
private:
  inline void send(uint8_t data);
  
  uint8_t _clock, _data, _strobe;  // SPI interface

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;

  uint8_t _numlines, _currline;
};

#endif
