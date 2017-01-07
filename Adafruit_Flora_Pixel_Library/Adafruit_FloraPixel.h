#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#define TRESET delayMicroseconds(50);

// data pin shortcuts
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328) || (__AVR_ATmega8__)
  // sadly the library must send data so fast we have to 'hardcode' the pins
  #define DATAPORT PORTD
  #define DATAPIN  4
  #define DATADIR  DDRD 
#elif defined(__AVR_ATmega32U4__)
  // sadly the library must send data so fast we have to 'hardcode' the pins
  #define DATAPORT PORTD
  #define DATAPIN  7
  #define DATADIR  DDRD
#elif defined(__AVR_ATtiny85__)
  // sadly the library must send data so fast we have to 'hardcode' the pins
  #define DATAPORT PORTB
  #define DATAPIN  1
  #define DATADIR  DDRB
#endif

#define dataPinHigh DATAPORT |= (1 << DATAPIN)
#define dataPinLow DATAPORT &= ~(1 << DATAPIN)
#define dataPinOut DATADIR |= (1 << DATAPIN)

typedef struct RGBPixel
{
  uint8_t red, green, blue;
} RGBPixel;

class Adafruit_FloraPixel{
 private:
  // the arrays of int32s that hold each LED's 24 bit color values
  RGBPixel *pixels;
  uint16_t numLEDs;
 public:
  Adafruit_FloraPixel(uint16_t n);
  void begin();
  void show();
  void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
  void setPixelColor(uint16_t n, RGBPixel p);
  uint16_t numPixels(void);
  inline void writeByte(uint8_t b);
};
