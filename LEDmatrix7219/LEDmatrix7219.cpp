/*
  LEDmatrix7219.cpp - Arduino/chipKit library support for LED matrices driven by MAX7219
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

#include "LEDmatrix7219.h"
#if defined(__AVR__)
	#include <avr/pgmspace.h>
	#include "hardware/avr/HW_AVR.h"
#elif defined(__PIC32MX__)
	#pragma message("Compiling for PIC32 Architecture...")
	#include "hardware/pic32/HW_PIC32.h"
#elif defined(__arm__)
	#pragma message("Compiling for ARM Architecture...")
	#include "hardware/arm/HW_ARM.h"
#endif

LEDmatrix7219::LEDmatrix7219(uint8_t data, uint8_t clock, uint8_t load)
{ 
	P_DATA	= portOutputRegister(digitalPinToPort(data));
	B_DATA	= digitalPinToBitMask(data);
	P_CLOCK	= portOutputRegister(digitalPinToPort(clock));
	B_CLOCK	= digitalPinToBitMask(clock);
	P_LOAD	= portOutputRegister(digitalPinToPort(load));
	B_LOAD	= digitalPinToBitMask(load);
	pinMode(data,  OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(load,  OUTPUT);
}

void LEDmatrix7219::__send_word(uint8_t opcode, uint8_t data)
{   
	for (uint8_t c = 0; c < 8; c++)
	{
		if (opcode & 0x80)
			__sbi(P_DATA, B_DATA);
		else
			__cbi(P_DATA, B_DATA);
		opcode = opcode<<1;
		pulseClock;
	}
	for (uint8_t c = 0; c < 8; c++)
	{
		if (data & 0x80)
			__sbi(P_DATA, B_DATA);
		else
			__cbi(P_DATA, B_DATA);
		data = data<<1;
		pulseClock;
	}
}

void LEDmatrix7219::__send_buffer()
{
	for (uint8_t col = 0; col < 8; col++)
	{
		__cbi(P_LOAD, B_LOAD);
		for (uint8_t disp = 0; disp < numDisplays; disp++)
			__send_word(MAX7219_DIGIT0 + col, scrbuf[((numDisplays - 1 - disp) * 8) + col]);
		__sbi(P_LOAD, B_LOAD);
	}
}

void LEDmatrix7219::__send_row(uint8_t row)
{
	uint8_t mat  = row / 8;
	uint8_t mrow = row % 8;
	for (uint8_t i = 0; i < (numDisplays * 2); i++)
		sendbuf[i]=0;

	sendbuf[((numDisplays - mat) * 2) - 2] = MAX7219_DIGIT0 + mrow;
	sendbuf[((numDisplays - mat) * 2) - 1] = scrbuf[row];

	__cbi(P_LOAD, B_LOAD);
	for (uint8_t i = 0; i < (numDisplays * 2); i+=2)
		__send_word(sendbuf[i], sendbuf[i+1]);
	__sbi(P_LOAD, B_LOAD);
}

void LEDmatrix7219::begin(uint8_t num)
{
	numDisplays = num;
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_SCANLIMIT, 7);
	__sbi(P_LOAD, B_LOAD);
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_DECODEMODE, 0);
	__sbi(P_LOAD, B_LOAD);
	setIntensity(8);
	for (uint8_t c = 0; c < sizeof(scrbuf); c++)
		scrbuf[c] = 0;
	clear();
	cfont.font = 0;
	disableTest();
}

void LEDmatrix7219::enableSleep()
{
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_SHUTDOWN, 0);
	__sbi(P_LOAD, B_LOAD);
}

void LEDmatrix7219::disableSleep()
{
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_SHUTDOWN, 1);
	__sbi(P_LOAD, B_LOAD);
}

void LEDmatrix7219::enableTest()
{
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_DISPLAYTEST, 1);
	__sbi(P_LOAD, B_LOAD);
}

void LEDmatrix7219::disableTest()
{
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_DISPLAYTEST, 0);
	__sbi(P_LOAD, B_LOAD);
}

void LEDmatrix7219::clear()
{
	for (uint8_t c = 0; c < sizeof(scrbuf); c++)
		scrbuf[c] = 0;
	__send_buffer();
}

void LEDmatrix7219::setIntensity(uint8_t level)
{
    __cbi(P_LOAD, B_LOAD);
	for (int c=0; c<numDisplays; c++)
		__send_word(MAX7219_INTENSITY, level);
	__sbi(P_LOAD, B_LOAD);
}

void LEDmatrix7219::setPixel(uint8_t x, uint8_t y)
{
	if ((x < (numDisplays * 8)) and (y < 8))
	{
		scrbuf[x] |= (1 << y);
		__send_row(x);
	}
}

void LEDmatrix7219::clrPixel(uint8_t x, uint8_t y)
{
	if ((x < (numDisplays * 8)) and (y < 8))
	{
		scrbuf[x] &= ~(1 << y);
		__send_row(x);
	}
}

void LEDmatrix7219::invPixel(uint8_t x, uint8_t y)
{
	if ((x < (numDisplays * 8)) and (y < 8))
	{
		if (scrbuf[x] & (1 << y))
			scrbuf[x] &= ~(1 << y);
		else
			scrbuf[x] |= (1 << y);
		__send_row(x);
	}
}

void LEDmatrix7219::print(char *st, int x, int y)
{
	unsigned char ch;
	int stl, row, xp;

	stl = strlen(st);

	for (int cnt=0; cnt<stl; cnt++)
		_print_char(*st++, x + (cnt*(cfont.x_size)), y);

	__send_buffer();
}

void LEDmatrix7219::print(String st, int x, int y)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	print(buf, x, y);
}

void LEDmatrix7219::print(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

void LEDmatrix7219::_print_char(unsigned char c, int x, int y)
{
	int font_idx = ((c - cfont.offset)*(cfont.x_size))+4;

	for(int cnt=0; cnt<cfont.x_size; cnt++)
		if (((x + cnt) < (numDisplays * 8)) and ((x + cnt) >= 0))
			scrbuf[x + cnt] = (fontbyte(font_idx + cnt)) << y;
}

void LEDmatrix7219::setFont(uint8_t* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

void LEDmatrix7219::scroll(uint8_t dir, boolean wrap)
{
	uint8_t temp;

	switch (dir)
	{
	case SCROLL_LEFT:
		temp = scrbuf[0];
		for (uint8_t row = 0; row < ((numDisplays * 8) - 1); row++)
			scrbuf[row] = scrbuf[row + 1];
		if (wrap)
			scrbuf[(numDisplays * 8) - 1] = temp;
		else
			scrbuf[(numDisplays * 8) - 1] = 0;
		break;
	case SCROLL_RIGHT:
		temp = scrbuf[(numDisplays * 8) - 1];
		for (uint8_t row = ((numDisplays * 8) - 1); row > 0; row--)
			scrbuf[row] = scrbuf[row - 1];
		if (wrap)
			scrbuf[0] = temp;
		else
			scrbuf[0] = 0;
		break;
	case SCROLL_UP:
		for (uint8_t row = 0; row < (numDisplays * 8); row++)
		{
			if (wrap)
			{
				temp = scrbuf[row] & 1;
				scrbuf[row] = (scrbuf[row]>>1) + (temp << 7);
			}
			else
				scrbuf[row] = scrbuf[row]>>1;
		}
		break;
	case SCROLL_DOWN:
		for (uint8_t row = 0; row < (numDisplays * 8); row++)
		{
			if (wrap)
			{
				temp = scrbuf[row] & 128;
				scrbuf[row] = (scrbuf[row]<<1) + (temp >> 7);
			}
			else
				scrbuf[row] = scrbuf[row]<<1;
		}
		break;
	}

	__send_buffer();
}

boolean LEDmatrix7219::marquee(char *st, uint16_t speed)
{
	mbuf = (char *)malloc(strlen(st));
	if (mbuf == NULL) return false;

	for (int i = 0; i < strlen(st); i++)
		mbuf[i] = st[i];

	if (speed < 10)
		speed = 10;

	clear();
	_int_counter = 0;
	_int_speed = speed;
	_int_msize = strlen(st);
	_int_ndisp = numDisplays;
	_int_char = 0;
	_int_pos = 0;
	_int_font = cfont;

	__start_interrupt();
	return true;
}

boolean LEDmatrix7219::marquee(String st, uint16_t speed)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	marquee(buf, speed);
}

boolean LEDmatrix7219::stopMarquee()
{
	if (mbuf == NULL) return false;

	__stop_interrupt();
	free(mbuf);
	clear();
	return true;
}

void LEDmatrix7219::setMarqueeSpeed(uint16_t speed)
{
	if (speed < 10)
		speed = 10;

	_int_counter = 0;
	_int_speed = speed;
}