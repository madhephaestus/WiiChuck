// Using Timer Counter 1, Channel 0

void LEDmatrix7219::__start_interrupt()
{
	cli();																				// Disable interrupts
	PMC->PMC_WPMR = PMC_WPMR_WPKEY_VALUE;												// Disable PMC register write protection
	PMC->PMC_PCER0 = 1 << ID_TC3;														// Enable peripheral clock for TC1 Channel 0 (= ID_TC3)

	TcChannel* pTcCh = TC1->TC_CHANNEL + 0;												// Timer Counter 1, Channel 0
	pTcCh->TC_CCR = TC_CCR_CLKDIS;														// Disable TC clock
	pTcCh->TC_IDR = 0xFFFFFFFF;															// Disable interrupts
	pTcCh->TC_SR;																		// Clear status register
	pTcCh->TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4;		// Set mode

	uint32_t rc = VARIANT_MCK/128/1000;													// Set frequency to 1kHz
	TC1->TC_CHANNEL[0].TC_RA = rc/2;													// Set RA to 50% of rc
	TC1->TC_CHANNEL[0].TC_RC = rc;														// Set RC to rc 
	pTcCh->TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;										// Start counter
	TC1->TC_CHANNEL[0].TC_IER=TC_IER_CPCS;
	TC1->TC_CHANNEL[0].TC_IDR=~TC_IER_CPCS;
	NVIC->ISER[((uint32_t)(TC3_IRQn) >> 5)] = (1 << ((uint32_t)(TC3_IRQn) & 0x1F));		// Enable Timer Counter 1, Channel 0 (TC3_IRQn)
	sei();																				// Enable interrupts
}

void LEDmatrix7219::__stop_interrupt()
{
	cli();										// Disable interrupts
	TC1->TC_CHANNEL->TC_CCR = TC_CCR_CLKDIS;	// Stop timer compare interrupt
	sei();										// Enable interrupts
}

void TC3_Handler()
{
	TC1->TC_CHANNEL->TC_SR;  // Read the status register to clear it
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