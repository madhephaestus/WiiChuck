#ifndef _UNTZTRUMENT_H_
#define _UNTZTRUMENT_H_

#include <Arduino.h>
#include <Adafruit_Trellis.h>

// UNTZTRUMENT/TRELLIS STUFF -----------------------------------------------

class Adafruit_UNTZtrument : public Adafruit_TrellisSet {
 public:
  Adafruit_UNTZtrument(
    Adafruit_Trellis *matrix0,
    Adafruit_Trellis *matrix1=0,
    Adafruit_Trellis *matrix2=0,
    Adafruit_Trellis *matrix3=0,
    Adafruit_Trellis *matrix4=0,
    Adafruit_Trellis *matrix5=0,
    Adafruit_Trellis *matrix6=0,
    Adafruit_Trellis *matrix7=0);

  uint8_t xy2i(uint8_t x, uint8_t y);
  void    i2xy(uint8_t i, uint8_t *x, uint8_t *y);

 private:
  uint8_t size;
};

// ENCODER STUFF -----------------------------------------------------------

// Encoder values & bounds are 16-bit signed; balance between RAM and
// resolution.  Could easily change to 8 or 32 bits if an application
// needs it, but unsigned types will require a change to the default
// min & max values in the constructor (see comment in .cpp).
typedef int16_t enc_t;

class enc {

 public:
  enc(uint8_t a, uint8_t b, boolean p = true);

  void  setBounds(enc_t lo, enc_t hi, boolean wrap = false),
        setValue(enc_t v);
  enc_t getValue(void);

  // Static member funcs are shared across all objects, not per-instance.
  // e.g. enc::poll() reads the lot, no need to call for each item.
  static void begin(void),
              poll(void);

 private:
  uint8_t            pinA, pinB;
  boolean            pullup;
#if defined(ARDUINO_ARCH_SAMD)
  volatile uint32_t *pinRegA, *pinRegB;  // PIN registers for A & B
  uint32_t           pinMaskA, pinMaskB; // Bitmask for A & B
#else
  volatile uint8_t  *pinRegA, *pinRegB;   // PIN registers for A & B
  uint8_t            pinMaskA, pinMaskB;  // Bitmask for A & B
#endif
  enc_t              value,               // Current value
                     min, max;            // Range bounds, if set
  uint8_t            flags,               // Limit (clip) vs wraparound
                     state;               // Saved encoder pin state
  int8_t             x2;                  // Last motion X2 (-2,0,+2)
  enc               *next;                // Next in linked list
};

#endif // _UNTZTRUMENT_H_
