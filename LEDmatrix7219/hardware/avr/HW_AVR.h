void LEDmatrix7219::__start_interrupt()
{
	cli();						// Disable interrupts
	TCCR2A = 0;					// Clear TCCR2A
	TCCR2B = 0;					// Clear TCCR2B
	TCNT2  = 0;					// Clear counter value
	OCR2A = 249;				// = (16*10^6) / (1000*64) - 1 (for 1kHz)
	TCCR2A |= (1 << WGM21);		// Enable CTC mode
	TCCR2B |= (1 << CS22);		// Set CS22 bit for 64 prescaler
	TIMSK2 |= (1 << OCIE2A);	// Enable timer compare interrupt
	sei();						// Enable interrupts
}

void LEDmatrix7219::__stop_interrupt()
{
	cli();						// Disable interrupts
	TIMSK2 = 0;					// Disable timer compare interrupt
	sei();						// Enable interrupts
}

ISR(TIMER2_COMPA_vect)
{
	cli();
	_int_counter++;
	if (_int_counter == _int_speed)
	{
		uint8_t opcode;
		uint8_t data;
		int font_idx = ((mbuf[_int_char] - _int_font.offset)*(_int_font.x_size))+4;

		for (uint8_t row = 0; row < ((_int_ndisp * 8) - 1); row++)
			scrbuf[row] = scrbuf[row + 1];
		if (_int_char > (_int_msize-1))
			scrbuf[(_int_ndisp * 8) - 1] = 0;
		else
			scrbuf[(_int_ndisp * 8) - 1] = pgm_read_byte(&_int_font.font[font_idx + _int_pos]);
		for (uint8_t col = 0; col < 8; col++)
		{
			__cbi(P_LOAD, B_LOAD);
			for (uint8_t disp = 0; disp < _int_ndisp; disp++)
			{
				opcode = MAX7219_DIGIT0 + col;
				for (uint8_t c = 0; c < 8; c++)
				{
					if (opcode & 0x80)
						__sbi(P_DATA, B_DATA);
					else
						__cbi(P_DATA, B_DATA);
					opcode = opcode<<1;
					pulseClock;
				}
				data = scrbuf[((_int_ndisp - 1 - disp) * 8) + col];
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
			__sbi(P_LOAD, B_LOAD);
		}
		_int_pos++;
		if (_int_pos == _int_font.x_size)
		{
			_int_char++;
			if (_int_char > (_int_msize + ((_int_ndisp * 8) / _int_font.x_size)))
				_int_char = 0;
			_int_pos = 0;
		}
		_int_counter = 0;
	}

	sei();
}