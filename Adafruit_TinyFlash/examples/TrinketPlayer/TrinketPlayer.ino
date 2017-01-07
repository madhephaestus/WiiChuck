// Audio playback sketch for Adafruit Trinket.  Requires 3.3V
// Trinket board and Winbond W25Q80BV serial flash loaded with
// audio data.  PWM output on pin 4; add ~25 KHz low-pass filter
// prior to amplification.  Uses ATtiny-specific registers;
// WILL NOT RUN ON OTHER ARDUINOS.

#include <Adafruit_TinyFlash.h>

#if(F_CPU == 16000000L)
#error "Compile for 8 MHz Trinket"
#endif

Adafruit_TinyFlash flash;
uint16_t           sample_rate, delay_count;
uint32_t           samples;
volatile uint32_t  index = 0L;

void setup() {
  uint8_t  data[6];
  uint32_t bytes;

  if(!(bytes = flash.begin())) {     // Flash init error?
    for(;; PORTB ^= 2, delay(250));  // Blink 2x/sec
  }

  // First six bytes contain sample rate, number of samples
  flash.beginRead(0);
  for(uint8_t i=0; i<6; i++) data[i] = flash.readNextByte();
  sample_rate = ((uint16_t)data[0] <<  8)
              |  (uint16_t)data[1];
  samples     = ((uint32_t)data[2] << 24)
              | ((uint32_t)data[3] << 16)
              | ((uint32_t)data[4] <<  8)
              |  (uint32_t)data[5];
  // Audio begins at next byte, so DON'T endRead() here

  PLLCSR |= _BV(PLLE);               // Enable 64 MHz PLL
  delayMicroseconds(100);            // Stabilize
  while(!(PLLCSR & _BV(PLOCK)));     // Wait for it...
  PLLCSR |= _BV(PCKE);               // Timer1 source = PLL

  // Set up Timer/Counter1 for PWM output
  TIMSK  = 0;                        // Timer interrupts OFF
  TCCR1  = _BV(CS10);                // 1:1 prescale
  GTCCR  = _BV(PWM1B) | _BV(COM1B1); // PWM B, clear on match
  OCR1C  = 255;                      // Full 8-bit PWM cycle
  OCR1B  = 127;                      // 50% duty at start

  pinMode(4, OUTPUT);                // Enable PWM output pin

  // Set up Timer/Counter0 for sample-playing interrupt.
  // TIMER0_OVF_vect is already in use by the Arduino runtime,
  // so TIMER0_COMPA_vect is used.  This code alters the timer
  // interval, making delay(), micros(), etc. useless (the
  // overflow interrupt is therefore disabled).

  // Timer resolution is limited to either 0.125 or 1.0 uS,
  // so it's rare that the playback rate will precisely match
  // the data, but the difference is usually imperceptible.
  TCCR0A = _BV(WGM01) | _BV(WGM00);  // Mode 7 (fast PWM)
  if(sample_rate >= 31250) {
    TCCR0B = _BV(WGM02) | _BV(CS00); // 1:1 prescale
    OCR0A  = ((F_CPU + (sample_rate / 2)) / sample_rate) - 1;
  } else {                           // Good down to about 3900 Hz
    TCCR0B = _BV(WGM02) | _BV(CS01); // 1:8 prescale
    OCR0A  = (((F_CPU / 8L) + (sample_rate / 2)) / sample_rate) - 1;
  }
  TIMSK = _BV(OCIE0A); // Enable compare match, disable overflow
}

void loop() { }

ISR(TIMER0_COMPA_vect) {
  OCR1B = flash.readNextByte();      // Read flash, write PWM reg.
  if(++index >= samples) {           // End of audio data?
    index = 0;                       // We must repeat!
    flash.endRead();
    flash.beginRead(6);              // Skip 6 byte header
  }
}

