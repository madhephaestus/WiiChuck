// (c) Adafruit Industries / Limor Fried 2010. Released under MIT license.

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// some spi defines
// Classic Arduinos
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)|| defined(__AVR_ATmega168__) 
  #define SPI_PORT PORTB
  #define SPI_DDR  DDRB
  #define SPI_PIN  PINB
  #define SPI_MOSI 3       // Arduino pin 11.
  #define SPI_SCK  5       // Arduino pin 13.
  #define DATA_PIN 11
  #define CLOCK_PIN 13
// Megas
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define SPI_PORT PORTB
  #define SPI_DDR  DDRB
  #define SPI_PIN  PINB
  #define SPI_MOSI 2       // Arduino pin 51.
  #define SPI_SCK  1       // Arduino pin 52.
  #define DATA_PIN 51
  #define CLOCK_PIN 52
#elif defined(__AVR_ATmega32U4__)
  #define SPI_PORT PORTB
  #define SPI_DDR  DDRB
  #define SPI_PIN  PINB
  #define SPI_MOSI 2       // Teensyduino pin 2
  #define SPI_SCK  1       // Teensyduino pin 1
  #define DATA_PIN 2
  #define CLOCK_PIN 1
#endif

class HL1606stripPWM {
 private:
  // How many bits of color per LED for PWM?
  // if its set to 2 bits/LED that means 6 bit color (since there are 3 LEDs)
  // if its set to 3 bits/LED that equals 9 bit color
  // if its set to 4 bits/LED that equals 12 bit color
  // Setting this to be higher means the CPU works harder, at some point it will run out of CPU
  // and look weird or flash
  uint8_t PWMbits;

// This is how much we want to divide the SPI clock by
// valid #s are: 2, 4, 8, 16, 32, 64 and 128
// SPI speed = CPU speed / divider
// So divider of 32 and CPU clock of 16MHz (standard arduino) -> 500 KHz SPI
// Smaller divider numbers means faster SPI
// ...but HL1606 cant take faster than 1.5MHz and they're kinda sucky chips which
// means that if you have one bad chip in your string that cant go faster than say 500 MHz
// then the entire strand will fail. Sometimes you're lucky and they're all good - try 16
// but don't be surprised if they fail at 1MHz
  uint8_t SPIspeedDiv;

// What 'percentage' of CPU you're willing to use for the PWM action.
  uint8_t CPUmaxpercent;

  void timerinit(void);
  void SPIinit(void);

 public:
  HL1606stripPWM(uint8_t nLEDs, uint8_t latch);
  void begin(void);
  void setLEDcolorPWM(uint8_t n, uint8_t r, uint8_t g, uint8_t b);
  uint8_t numLEDs(void);

  void setSPIdivider(uint8_t div);
  uint8_t getSPIdivider();

  void setPWMbits(uint8_t b);
  uint8_t getPWMbits();

  uint8_t getCPUmax();
  void setCPUmax(uint8_t cpumax);
};
