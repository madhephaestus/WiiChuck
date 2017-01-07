#include "Adafruit_FloraPixel.h"
#include <util/delay.h>

/*****************************************************************************/
// Example to control Adafruit Flora Sewable RGB LED Modules in a strand or strip
// Adapted from Pololu LED strip code with timing modifications 
// Adapted for our pixels which have different timing
// See https://github.com/pololu/pololu-led-strip-arduino for original code

/*****************************************************************************/

Adafruit_FloraPixel::Adafruit_FloraPixel(uint16_t n) {
  numLEDs = n;

  pixels = (RGBPixel *)malloc(numLEDs);
  for (uint16_t i=0; i< numLEDs; i++) {
    setPixelColor(i, 0, 0, 0);
  }
}

void Adafruit_FloraPixel::begin(void) {
  // start low!
  DATAPORT &= ~_BV(DATAPIN);
  // Output
  DATADIR |= _BV(DATAPIN);
}

uint16_t Adafruit_FloraPixel::numPixels(void) {
  return numLEDs;
}

void Adafruit_FloraPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if (n > numLEDs) return;
  
  pixels[n].red = r;
  pixels[n].green = g;
  pixels[n].blue = b;
}

void Adafruit_FloraPixel::setPixelColor(uint16_t n, RGBPixel p) {
  if (n > numLEDs) return;

  pixels[n].red = p.red;
  pixels[n].green = p.green;
  pixels[n].blue = p.blue;
}

#if (F_CPU == 8000000UL)
#define FIRSTNOPS "nop\n"
#define MIDDLENOPS "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
#define LASTNOPS "nop\n"
#elif (F_CPU == 10000000UL)
#define FIRSTNOPS "nop\n" "nop\n"
#define MIDDLENOPS  "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
#define LASTNOPS "nop\n"
#elif (F_CPU == 16000000UL)
#define FIRSTNOPS "nop\n" "nop\n" "nop\n" "nop\n" "nop\n"
#define MIDDLENOPS "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" "nop\n" 
#define LASTNOPS "nop\n" "nop\n" "nop\n" "nop\n"  "nop\n"
#else
#error "CPU SPEED NOT SUPPORTED"
#endif

void __attribute__((noinline)) Adafruit_FloraPixel::show(void) {
  RGBPixel *strip = pixels;
  uint16_t count = numLEDs;

  cli(); // Disable interrupts temporarily because we don't want our pulse timing to be messed up.
  while (count--)
  {
    // Send a color to the LED strip.
    // The assembly below also increments the 'strip' pointer,
    // it will be pointing to the next color at the end of this loop.
    asm volatile(
        "rcall send_led_strip_byte%=\n" // Send red component.
        "rcall send_led_strip_byte%=\n" // Send green component.
        "rcall send_led_strip_byte%=\n" // Send blue component.
        "rjmp led_strip_asm_end%=\n" // Jump past the assembly subroutines.

        // send_led_strip_byte subroutine: Sends a byte to the LED strip.
        "send_led_strip_byte%=:\n"
        "ld __tmp_reg__, %a0+\n" // Read the next color brightness byte and advance the pointer

	// Unroll the loop!
// bit #0
        "rol __tmp_reg__\n" // Rotate left through carry.
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
	LASTNOPS
// bit #1
        "sbi %2, %3\n" // Drive the line high.
        FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
	LASTNOPS
// bit #2
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
	LASTNOPS
// bit #3
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
	LASTNOPS
// bit #4
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
	LASTNOPS
// bit #5
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
        LASTNOPS
// bit #6
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
        "rol __tmp_reg__\n" // Rotate left through carry.
        LASTNOPS
// bit #7
        "sbi %2, %3\n" // Drive the line high.
	FIRSTNOPS
        "brcs .+2\n" "cbi %2, %3\n" // If the bit to send is 0, drive the line low now (500ns)
	MIDDLENOPS
        "brcc .+2\n" "cbi %2, %3\n" // If the bit to send is 1, drive the line low now.
// done!
        "ret\n"
        
        "led_strip_asm_end%=: "
        : "=b" (strip)
        : "0" (strip), // %a0 points to the next color to display
          "I" (_SFR_IO_ADDR(DATAPORT)), // %2 is the port register (e.g. PORTC)
          "I" (DATAPIN) // %3 is the pin number (0-8)
	);

  }
  _delay_us(100); // Hold the line low for 100+ microseconds to send the reset signal.
  sei(); // Re-enable interrupts now that we are done.
}
