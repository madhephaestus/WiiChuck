inline void _waitForIdleBus() { while (I2C1CON & 0x1f) {} }

void WiiChuck::begin()
{
	if ((_sda_pin == SDA) and (_scl_pin == SCL))
	{
		uint32_t	tpgd;

		_use_hw = true;
		pinMode(SDA, OUTPUT);
		digitalWrite(SDA, HIGH);
		IFS0CLR = 0xE0000000;									// Clear Interrupt Flag
		IEC0CLR = 0xE0000000;									// Disable Interrupt
		I2C1CONCLR = (1 << _I2CCON_ON);							// Disable I2C interface
		tpgd = ((F_CPU / 8) * 104) / 125000000;
		I2C1BRG = (F_CPU / (2 * TWI_FREQ) - tpgd) - 2;			// Set I2C Speed
		I2C1ADD = I2C_ADDR;										// Set I2C device address
		I2C1CONSET = (1 << _I2CCON_ON) | (1 << _I2CCON_STREN);	// Enable I2C Interface
	}
	else
	{
		_use_hw = false;
		pinMode(_scl_pin, OUTPUT);
	}
	initBytes();
	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
	_joy_x_center = _dataarray[0];
	_joy_y_center = _dataarray[1];
}

void WiiChuck::_burstRead()
{
	if (_use_hw)
	{
		_waitForIdleBus();									// Wait for I2C bus to be Idle before starting
		I2C1CONSET = (1 << _I2CCON_SEN);					// Send start condition
		if (I2C1STAT & (1 << _I2CSTAT_BCL)) { return; }		// Check if there is a bus collision
		while (I2C1CON & (1 << _I2CCON_SEN)) {}				// Wait for start condition to finish
		I2C1TRN = (I2C_ADDR<<1);							// Send device Write address
		while (I2C1STAT & (1 << _I2CSTAT_IWCOL))			// Check if there is a Write collision
		{
			I2C1STATCLR = (1 << _I2CSTAT_IWCOL);			// Clear Write collision flag
			I2C1TRN = (I2C_ADDR<<1);						// Retry send device Write address
		}
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = 0;										// Send the register address
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		_waitForIdleBus();									// Wait for I2C bus to be Idle before starting
		I2C1CONSET = (1 << _I2CCON_RSEN);					// Send start condition
		if (I2C1STAT & (1 << _I2CSTAT_BCL)) { return; }		// Check if there is a bus collision
		while (I2C1CON & (1 << _I2CCON_RSEN)) {}			// Wait for start condition to finish
		I2C1TRN = (I2C_ADDR<<1) | 1;						// Send device Read address
		while (I2C1STAT & (1 << _I2CSTAT_IWCOL))			// Check if there is a Write collision
		{
			I2C1STATCLR = (1 << _I2CSTAT_IWCOL);			// Clear Write collision flag
			I2C1TRN = (I2C_ADDR<<1) | 1;					// Retry send device Read address
		}
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		byte dummy = I2C1RCV;								// Clear _I2CSTAT_RBF (Receive Buffer Full)
		for (int i=0; i<6; i++)
		{
			_waitForIdleBus();								// Wait for I2C bus to be Idle before continuing
			I2C1CONSET = (1 << _I2CCON_RCEN);				// Set RCEN to start receive
			while (I2C1CON & (1 << _I2CCON_RCEN)) {}		// Wait for Receive operation to finish
			while (!(I2C1STAT & (1 << _I2CSTAT_RBF))) {}	// Wait for Receive Buffer Full
			_dataarray[i] = (I2C1RCV ^ 0x17) + 0x17;		// Read data
			if (i == 5)
				I2C1CONSET = (1 << _I2CCON_ACKDT);			// Prepare to send NACK
			else
				I2C1CONCLR = (1 << _I2CCON_ACKDT);			// Prepare to send ACK
			I2C1CONSET = (1 << _I2CCON_ACKEN);				// Send ACK/NACK
			while (I2C1CON & (1 << _I2CCON_ACKEN)) {}		// Wait for ACK/NACK send to finish
		}
		I2C1CONSET = (1 << _I2CCON_PEN);					// Send stop condition
		while (I2C1CON & (1 << _I2CCON_PEN)) {}				// Wait for stop condition to finish
	}
	else
	{
		delay(3);
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		delayMicroseconds(5);
		_writeByte(0);
		_waitForAck();
		delayMicroseconds(10);
		_sendStop();

		delay(3);
		_sendStart(I2C_ADDR_R);
		_waitForAck();

		delayMicroseconds(5);
		for (int i=0; i<6; i++)
		{
			delayMicroseconds(20);
			_dataarray[i] = (_readByte() ^ 0x17) + 0x17;
			if (i<5)
				_sendAck();
			else
				_sendNack();
		}
		delayMicroseconds(5);
		_sendStop();
	}
}

void WiiChuck::_writeRegister(uint8_t reg, uint8_t value)
{
	if (_use_hw)
	{
		_waitForIdleBus();									// Wait for I2C bus to be Idle before starting
		I2C1CONSET = (1 << _I2CCON_SEN);					// Send start condition
		if (I2C1STAT & (1 << _I2CSTAT_BCL)) { return; }		// Check if there is a bus collision
		while (I2C1CON & (1 << _I2CCON_SEN)) {}				// Wait for start condition to finish
		I2C1TRN = (I2C_ADDR<<1);							// Send device Write address
		while (I2C1STAT & (1 << _I2CSTAT_IWCOL))			// Check if there is a Write collision
		{
			I2C1STATCLR = (1 << _I2CSTAT_IWCOL);			// Clear Write collision flag
			I2C1TRN = (I2C_ADDR<<1);						// Retry send device Write address
		}
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = reg;										// Send the 1st data byte
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1TRN = value;									// Send the 2nd data byte
		while (I2C1STAT & (1 << _I2CSTAT_TRSTAT)) {}		// Wait for transmit to finish
		while (I2C1STAT & (1 << _I2CSTAT_ACKSTAT)) {}		// Wait for ACK
		I2C1CONSET = (1 << _I2CCON_PEN);					// Send stop condition
		while (I2C1CON & (1 << _I2CCON_PEN)) {}				// Wait for stop condition to finish
	}
	else
	{
		delay(3);
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		delayMicroseconds(5);
		_writeByte(reg);
		_waitForAck();
		delayMicroseconds(5);
		_writeByte(value);
		_waitForAck();
		delayMicroseconds(5);
		_sendStop();
	}
}
