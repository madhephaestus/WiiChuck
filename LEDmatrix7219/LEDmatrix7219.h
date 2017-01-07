/*
  LEDmatrix7219.h - Arduino/chipKit library support for LED matrices driven by MAX7219
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  I needed a library to drive some 8x8 LED matrices using MAX7219 drivers. This library 
  is the result.

  It should be noted that the MAX7219 is designed to drive Common Cathode LED displays 
  but my 8x8 matrices were all Common Anode. The library has not been tested with 
  Common Cathode matrices.

  For Common Anode matrices the columns should be connected to the DIGx outputs from 
  the MAX7219, and the matrix rows should be connected to the SEGx outputs. SEG DP 
  is the bottom row.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef LEDmatrix7219_h
#define LEDmatrix7219_h

#define SCROLL_LEFT		1
#define SCROLL_RIGHT	2
#define	SCROLL_UP		3
#define SCROLL_DOWN		4

// MAX7219 Commandset
// ------------------
#define MAX7219_NOOP		0
#define MAX7219_DIGIT0		1
#define MAX7219_DIGIT1		2
#define MAX7219_DIGIT2		3
#define MAX7219_DIGIT3		4
#define MAX7219_DIGIT4		5
#define MAX7219_DIGIT5		6
#define MAX7219_DIGIT6		7
#define MAX7219_DIGIT7		8
#define MAX7219_DECODEMODE	9
#define MAX7219_INTENSITY	10
#define MAX7219_SCANLIMIT	11
#define MAX7219_SHUTDOWN	12
#define MAX7219_DISPLAYTEST	15

#if defined(__AVR__)
	#include "Arduino.h"
	#include "hardware/avr/HW_AVR_defines.h"
#elif defined(__PIC32MX__)
	#include "WProgram.h"
	#include "hardware/pic32/HW_PIC32_defines.h"
#elif defined(__arm__)
	#include "Arduino.h"
	#include "hardware/arm/HW_ARM_defines.h"
#endif

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

static	uint16_t		_int_counter;
static	uint16_t		_int_speed;
static	uint16_t		_int_msize;
static	uint16_t		_int_char;
static	uint8_t			_int_pos;
static	uint8_t			_int_ndisp;
static	char			*mbuf;
static	regtype			*P_DATA, *P_CLOCK, *P_LOAD;
static	regsize			B_DATA, B_CLOCK, B_LOAD;
static	uint8_t			scrbuf[64];
static	_current_font	_int_font;

class LEDmatrix7219
{
	public:
		LEDmatrix7219(uint8_t data, uint8_t clock, uint8_t load);
		void	begin(uint8_t num = 4);
		void	enableSleep();
		void	disableSleep();
		void	enableTest();
		void	disableTest();
		void	clear();
		void	setIntensity(uint8_t level);
		void	setPixel(uint8_t x, uint8_t y);
		void	clrPixel(uint8_t x, uint8_t y);
		void	invPixel(uint8_t x, uint8_t y);
		void	setFont(uint8_t* font);
		void	print(char *st, int x, int y = 0);
		void	print(String st, int x, int y = 0);
		void	print(long num, int x, int y = 0, int length = 0, char filler = ' ');
		void	scroll(uint8_t dir, boolean wrap = false);
		boolean	marquee(char *st, uint16_t speed);
		boolean	marquee(String st, uint16_t speed);
		boolean	stopMarquee();
		void	setMarqueeSpeed(uint16_t speed);

	protected:
		_current_font	cfont;
		uint8_t			sendbuf[16];
		uint8_t			numDisplays;

		void	_print_char(unsigned char c, int x, int y);

		void	__send_word(uint8_t opcode, uint8_t data);
		void	__send_buffer();
		void	__send_row(uint8_t row);

		void	__start_interrupt();
		void	__stop_interrupt();

	private:
};

#endif