void LEDmatrix7219::__start_interrupt()
{
	noInterrupts();											// Disable interrupts
	OpenTimer1(T1_ON | T1_PS_1_64 | T1_SOURCE_INT, 1225);	// Set up timer compare interrupt 1 (Prescaler = 64, Trigger rate = 1kHz approx.)
	ConfigIntTimer1((T1_INT_ON | T1_INT_PRIOR_3));			// Enable timer compare interrupt 1, Priority 3
	interrupts();											// Enable interrupts

}

void LEDmatrix7219::__stop_interrupt()
{
	noInterrupts();											// Disable interrupts
	OpenTimer1(T1_OFF, 0);									// Disable timer compare interrupt
	interrupts();											// Enable interrupts
}

#ifdef __cplusplus
extern "C" {
#endif
	void __ISR(_TIMER_1_VECTOR,IPL3AUTO) marquee_handler(void)
	{
		noInterrupts();		// Disable interrupts
		mT1ClearIntFlag();  // Clear interrupt flag

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
				scrbuf[(_int_ndisp * 8) - 1] = _int_font.font[font_idx + _int_pos];
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

		interrupts();	// Enable interrupts
	}
#ifdef __cplusplus
}
#endif
