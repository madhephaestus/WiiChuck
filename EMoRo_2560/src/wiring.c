/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id$
*/

#include "wiring_private.h"

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
#if defined(TCNT0)
	t = TCNT0;
#elif defined(TCNT0L)
	t = TCNT0L;
#else
	#error TIMER 0 not defined
#endif

  
#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

void delay(unsigned long ms)
{
	uint16_t start = (uint16_t)micros();

	while (ms > 0) {
		if (((uint16_t)micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
#if F_CPU >= 20000000L
	// for the 20 MHz clock on rare Arduino boards

	// for a one-microsecond delay, simply wait 2 cycle and return. The overhead
	// of the function call yields a delay of exactly a one microsecond.
	__asm__ __volatile__ (
		"nop" "\n\t"
		"nop"); //just waiting 2 cycle
	if (--us == 0)
		return;

	// the following loop takes a 1/5 of a microsecond (4 cycles)
	// per iteration, so execute it five times for each microsecond of
	// delay requested.
	us = (us<<2) + us; // x5 us

	// account for the time taken in the preceeding commands.
	us -= 2;

#elif F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (--us == 0)
		return;

	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 2;
#else
	// for the 8 MHz internal clock on the ATmega168

	// for a one- or two-microsecond delay, simply return.  the overhead of
	// the function calls takes more than two microseconds.  can't just
	// subtract two, since us is unsigned; we'd overflow.
	if (--us == 0)
		return;
	if (--us == 0)
		return;

	// the following loop takes half of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1;
    
	// partially compensate for the time taken by the preceeding commands.
	// we can't subtract any more than this or we'd overflow w/ small delays.
	us--;
#endif

	// busy wait
	__asm__ __volatile__ (
		"1: sbiw %0,1" "\n\t" // 2 cycles
		"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
	);
}

void init()
{
	// this needs to be called before setup() or some functions won't work there
	sei();

/*--------------------------------------------- TIMER 0 -----------------------------------------------*/
	// set timer 0 Fast PWM mode, top 0xFF
#if defined(TCCR0A) && defined(WGM01)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);
#endif  
	
	// set timer 0 prescale factor to 64
#if defined(TCCR0B) && defined(CS01) && defined(CS00)
	sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
#else
	#error Timer 0 prescale factor 64 not set correctly
#endif

	// enable timer 0 overflow interrupt
#if defined(TIMSK0) && defined(TOIE0)
	sbi(TIMSK0, TOIE0);
#else
	#error	Timer 0 overflow interrupt not set correctly
#endif

/*--------------------------------------------- TIMER 1 -----------------------------------------------*/
#if defined(TCCR1A) && defined(TCCR1B) && defined(CS11) && defined(CS10) && defined(WGM10)
	TCCR1A = 0; TCCR1B = 0; TCCR1C = 0; TIMSK1 = 0;
	sbi(TCCR1B, CS11);		// set timer 1 prescale factor to 64
	sbi(TCCR1B, CS10);
	sbi(TCCR1A, WGM10);		// put timer 1 in 8-bit phase correct pwm mode
#else
	#warning Timer 1 not finished (may not be present on this CPU)
#endif
	
/*--------------------------------------------- TIMER 2 -----------------------------------------------*/	
#if defined(TCCR2A) && defined(TCCR2B) && defined(CS22) && defined(WGM20)
	TCCR2A = 0; TCCR2B = 0; TIMSK2 = 0;
	sbi(TCCR2B, CS22);		// set timer 2 prescale factor to 64
	sbi(TCCR2A, WGM20);		// configure timer 2 for phase correct pwm (8-bit)
#else
	#warning Timer 2 not finished (may not be present on this CPU)
#endif

/*--------------------------------------------- TIMER 3 -----------------------------------------------*/
#if defined(TCCR3A) && defined(TCCR3B) && defined(CS31) && defined(CS30) && defined(WGM30)
	TCCR3A = 0; TCCR3B = 0; TCCR3C = 0; TIMSK3 = 0;
	sbi(TCCR3B, CS31);		// set timer 3 prescale factor to 64
	sbi(TCCR3B, CS30);
	sbi(TCCR3A, WGM30);		// put timer 3 in 8-bit phase correct pwm mode
#else
	#warning Timer 3 not finished (may not be present on this CPU)
#endif

/*--------------------------------------------- TIMER 4 -----------------------------------------------*/
#if defined(TCCR4A) && defined(TCCR4B) && defined(CS41) && defined(CS40) && defined(WGM40)
	TCCR4A = 0; TCCR4B = 0; TCCR4C = 0; TIMSK4 = 0;
	sbi(TCCR4B, CS41);		// set timer 4 prescale factor to 64
	sbi(TCCR4B, CS40);
	sbi(TCCR4A, WGM40);		// put timer 4 in 8-bit phase correct pwm mode
#else
	#warning Timer 4 not finished (may not be present on this CPU)
#endif

/*--------------------------------------------- TIMER 5 -----------------------------------------------*/
#if defined(TCCR5A) && defined(TCCR5B) && defined(CS51) && defined(CS50) && defined(WGM50)
	TCCR5A = 0; TCCR5B = 0; TCCR5C = 0; TIMSK5 = 0;
	sbi(TCCR5B, CS51);		// set timer 5 prescale factor to 64
	sbi(TCCR5B, CS50);
	sbi(TCCR5A, WGM50);		// put timer 5 in 8-bit phase correct pwm mode
#else
	#warning Timer 5 not finished (may not be present on this CPU)
#endif

/*--------------------------------------------- ADC Clock --------------------------------------------*/
#if defined(ADCSRA)
	sbi(ADCSRA, ADPS2);		// set a2d prescale factor to 16 (16 MHz / 16 = 1 MHz)
	sbi(ADCSRA, ADEN);		// enable a2d conversions
#endif

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(UCSR0B)
	UCSR0B = 0;
#endif
}
