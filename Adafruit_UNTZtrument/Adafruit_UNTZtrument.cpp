/*------------------------------------------------------------------------
  Arduino library for Adafruit UNTZtrument - a Trellis button controller.

  The Adafruit_UNTZtrument class is just a slight wrapper around a
  TrellisSet (see the Adafruit_Trellis library for details), adding
  X/Y-to-button-index and button-index-to-X/Y functions.

  The enc class provides basic encoder support.  It's polling-based
  (rather than interrupt-based) so it can work with any pins and any
  number of encoders, but the poll() function must be called frequently
  (a ~1 ms timer interrupt can optionally be used).  Supports upper/lower
  limits with clipping or wraparound.  Does not (currently) provide
  acceleration, detent division or shaft button debouncing...just basics,
  can use alternative libraries if special functionality is required.

  Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!
  ------------------------------------------------------------------------*/

#include "Adafruit_UNTZtrument.h"

// UNTZTRUMENT/TRELLIS STUFF -----------------------------------------------

Adafruit_UNTZtrument::Adafruit_UNTZtrument(
  Adafruit_Trellis *matrix0, Adafruit_Trellis *matrix1,
  Adafruit_Trellis *matrix2, Adafruit_Trellis *matrix3,
  Adafruit_Trellis *matrix4, Adafruit_Trellis *matrix5,
  Adafruit_Trellis *matrix6, Adafruit_Trellis *matrix7) :
  Adafruit_TrellisSet(matrix0, matrix1, matrix2, matrix3,
                      matrix4, matrix5, matrix6, matrix7) {
	size = matrix4 ? 128 : 64;
}

// I've got a thing for tables, despite size.  Fast constant-time lookup.
static const uint8_t PROGMEM
  i2xy64[] = { // Remap 8x8 TrellisSet button index to column/row
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31,
    0x02, 0x12, 0x22, 0x32, 0x03, 0x13, 0x23, 0x33,
    0x40, 0x50, 0x60, 0x70, 0x41, 0x51, 0x61, 0x71,
    0x42, 0x52, 0x62, 0x72, 0x43, 0x53, 0x63, 0x73,
    0x04, 0x14, 0x24, 0x34, 0x05, 0x15, 0x25, 0x35,
    0x06, 0x16, 0x26, 0x36, 0x07, 0x17, 0x27, 0x37,
    0x44, 0x54, 0x64, 0x74, 0x45, 0x55, 0x65, 0x75,
    0x46, 0x56, 0x66, 0x76, 0x47, 0x57, 0x67, 0x77 },
  i2xy128[] = { // Remap 16x8 TrellisSet button index to column/row
    0x00, 0x10, 0x20, 0x30, 0x01, 0x11, 0x21, 0x31,
    0x02, 0x12, 0x22, 0x32, 0x03, 0x13, 0x23, 0x33,
    0x40, 0x50, 0x60, 0x70, 0x41, 0x51, 0x61, 0x71,
    0x42, 0x52, 0x62, 0x72, 0x43, 0x53, 0x63, 0x73,
    0x80, 0x90, 0xA0, 0xB0, 0x81, 0x91, 0xA1, 0xB1,
    0x82, 0x92, 0xA2, 0xB2, 0x83, 0x93, 0xA3, 0xB3,
    0xC0, 0xD0, 0xE0, 0xF0, 0xC1, 0xD1, 0xE1, 0xF1,
    0xC2, 0xD2, 0xE2, 0xF2, 0xC3, 0xD3, 0xE3, 0xF3,
    0x04, 0x14, 0x24, 0x34, 0x05, 0x15, 0x25, 0x35,
    0x06, 0x16, 0x26, 0x36, 0x07, 0x17, 0x27, 0x37,
    0x44, 0x54, 0x64, 0x74, 0x45, 0x55, 0x65, 0x75,
    0x46, 0x56, 0x66, 0x76, 0x47, 0x57, 0x67, 0x77,
    0x84, 0x94, 0xA4, 0xB4, 0x85, 0x95, 0xA5, 0xB5,
    0x86, 0x96, 0xA6, 0xB6, 0x87, 0x97, 0xA7, 0xB7,
    0xC4, 0xD4, 0xE4, 0xF4, 0xC5, 0xD5, 0xE5, 0xF5,
    0xC6, 0xD6, 0xE6, 0xF6, 0xC7, 0xD7, 0xE7, 0xF7 },
  xy2i64[8][8] = { // Remap [row][col] to Trellis button/LED index
    {  0,  1,  2,  3, 16, 17, 18, 19 },
    {  4,  5,  6,  7, 20, 21, 22, 23 },
    {  8,  9, 10, 11, 24, 25, 26, 27 },
    { 12, 13, 14, 15, 28, 29, 30, 31 },
    { 32, 33, 34, 35, 48, 49, 50, 51 },
    { 36, 37, 38, 39, 52, 53, 54, 55 },
    { 40, 41, 42, 43, 56, 57, 58, 59 },
    { 44, 45, 46, 47, 60, 61, 62, 63 } },
  xy2i128[8][16] = {
    {  0,  1,  2,  3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51 },
    {  4,  5,  6,  7, 20, 21, 22, 23, 36, 37, 38, 39, 52, 53, 54, 55 },
    {  8,  9, 10, 11, 24, 25, 26, 27, 40, 41, 42, 43, 56, 57, 58, 59 },
    { 12, 13, 14, 15, 28, 29, 30, 31, 44, 45, 46, 47, 60, 61, 62, 63 },
    { 64, 65, 66, 67, 80, 81, 82, 83, 96, 97, 98, 99,112,113,114,115 },
    { 68, 69, 70, 71, 84, 85, 86, 87,100,101,102,103,116,117,118,119 },
    { 72, 73, 74, 75, 88, 89, 90, 91,104,105,106,107,120,121,122,123 },
    { 76, 77, 78, 79, 92, 93, 94, 95,108,109,110,111,124,125,126,127 } };

uint8_t Adafruit_UNTZtrument::xy2i(uint8_t x, uint8_t y) {
	if(y > 7) return 255;
	if(size == 64) {
		if(x > 7) return 255;
		return pgm_read_byte(&xy2i64[y][x]);
	} else {
		if(x > 15) return 255;
		return pgm_read_byte(&xy2i128[y][x]);
	}
}

void Adafruit_UNTZtrument::i2xy(uint8_t i, uint8_t *x, uint8_t *y) {
	if(i > size) {
		*x = *y = 255;
		return;
	}
	uint8_t xy = pgm_read_byte((size == 64) ? &i2xy64[i] : &i2xy128[i]);
	*x = xy >> 4;
	*y = xy & 15;
}


// ENCODER STUFF -----------------------------------------------------------

#define ENCODER_LIMIT 0
#define ENCODER_WRAP  1

static enc *list = NULL; // Encoder linked list

// Constructor: pass two pin numbers and optional pullup enable flag
// (use latter for open-drain encoders such as PEC11 - tie C pin to GND).
enc::enc(uint8_t a, uint8_t b, boolean p) {
	pinA     = a;
	pinB     = b;
	pullup   = p;
        pinRegA  = portInputRegister(digitalPinToPort(pinA));
        pinRegB  = portInputRegister(digitalPinToPort(pinB));
	pinMaskA = digitalPinToBitMask(pinA);
	pinMaskB = digitalPinToBitMask(pinB);
	next     = list; // Insert at head
	list     = this; // of linked list
	min      = 0x80000000 >> ((4 - sizeof(enc_t)) * 8); // e.g. -32768
	max      = 0x7FFFFFFF >> ((4 - sizeof(enc_t)) * 8); // e.g. +32767
	// ^ For unsigned types, use 0 and (0xFFFFFFFF >> (stuff))
	value    = 0;
	flags    = 0;
	x2       = 0;
	state    = 0;
	if(*pinRegA & pinMaskA) state |= 2; // Read initial
	if(*pinRegB & pinMaskB) state |= 1; // pin state
}

// 2016-11-29 PhilB: added begin() function because SAMD doesn't
// appreciate pinMode() calls in constructor -- must explicitly call
// enc::begin() in setup() now.
void enc::begin(void) {
	uint8_t mode;
	for(enc *e = list; e; e = e->next) {
		mode = e->pullup ? INPUT_PULLUP : INPUT;
		pinMode(e->pinA, mode);
		pinMode(e->pinB, mode);
	}
}

// Limit encoder range between two values (inclusive).
// Note: if changing bounds AND value, set bounds first!
// If wrap==true, encoder value rolls over, else stops at limit.
void enc::setBounds(enc_t lo, enc_t hi, boolean wrap) {
	if(lo > hi) {
		enc_t temp = lo;
		lo = hi;
		hi = temp;
	}
	min = lo;
	max = hi;
	if(wrap) {
		flags |=  ENCODER_WRAP;
		if(value < lo)      value = hi - ((lo-(value+1)) % (hi-lo+1));
		else if(value > hi) value = lo + ((value-(hi+1)) % (hi-lo+1));
	} else {
		flags &= ~ENCODER_WRAP;
		if(value < lo)      value = lo;
		else if(value > hi) value = hi;
	}
}

// Change stored encoder value, applying any previously-set range limit.
void enc::setValue(enc_t v) {
	if(v < min) {
		value = (flags & ENCODER_WRAP) ?
		  (max - ((min-(v+1)) % (max-min+1))) : min;
	} else if(v > max) {
		value = (flags & ENCODER_WRAP) ?
		  (min + ((v-(max+1)) % (max-min+1))) : max;
	} else {
		value = v;
	}
}

// Read stored encoder value.
enc_t enc::getValue(void) {
	return value;
}

// Update status of all encoders.  This is a static member function and
// only needs to be invoked once for all encoders, not for each one.
void enc::poll(void) {
	for(enc *e = list; e; e = e->next) {
		e->state <<= 2;                              // Prior state
		if(*e->pinRegA & e->pinMaskA) e->state |= 2; // New state
		if(*e->pinRegB & e->pinMaskB) e->state |= 1;
		switch(e->state & B1111) {
		   case B0000: case B0101: case B1010: case B1111: // No change
			e->x2 = 0;
			break;
		   case B0001: case B0111: case B1000: case B1110: // Left
			e->value--; e->x2 = -2;
			if(e->value < e->min) {
				e->value = (e->flags & ENCODER_WRAP) ?
				  e->max : e->min;
			}
			break;
		   case B0010: case B0100: case B1011: case B1101: // Right
			e->value++; e->x2 = 2;
			if(e->value > e->max) {
				e->value = (e->flags & ENCODER_WRAP) ?
				  e->min : e->max;
			}
			break;
		   case B0011: case B0110: case B1001: case B1100: // Ambiguous
			e->value += e->x2; // Use 2X prior motion
			if(e->value < e->min) {
				e->value = (e->flags & ENCODER_WRAP) ?
				  e->max - ((e->min-(e->value+1)) %
				  (e->max-e->min+1)) : e->min;
			} else if(e->value > e->max) {
				e->value = (e->flags & ENCODER_WRAP) ?
				  e->min + ((e->value-(e->max+1)) %
				  (e->max-e->min+1)) : e->max;
			}
			break;
		}
	}
}
