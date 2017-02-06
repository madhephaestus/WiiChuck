void WiiChuck::begin()
{
	if ((_sda_pin == SDA) and (_scl_pin == SCL))
	{
		_use_hw = true;
		// activate internal pullups for twi.
		digitalWrite(SDA, HIGH);
		digitalWrite(SCL, HIGH);

		// initialize twi prescaler and bit rate
		cbi(TWSR, TWPS0);
		cbi(TWSR, TWPS1);
		TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;

		// enable twi module, acks, and twi interrupt
		TWCR = _BV(TWEN) | _BV(TWIE);
	}
	else
	{
		_use_hw = false;
		_clockLow();
	}
	initBytes();
	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
	_joy_x_center = _dataarray[0];
	_joy_y_center = _dataarray[1];
	_joy_x_max=_joy_x_center;
	_joy_x_min=_joy_x_center;
	_joy_y_max=_joy_y_center;
	_joy_y_min=_joy_y_center;

}

void WiiChuck::_burstRead()
{
	if (_use_hw)
	{
		// Send start address
		TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);						// Send START
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = I2C_ADDR_W;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = 0;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready

		// Read data starting from start address
		TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);						// Send rep. START
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = I2C_ADDR_R;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		for (int i=0; i<6; i++)
		{
			TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);								// Send ACK and clear TWINT to proceed
			while ((TWCR & _BV(TWINT)) == 0) {};									// Wait for TWI to be ready
			_dataarray[i] = (TWDR ^ 0x17) + 0x17;
		}
		TWCR = _BV(TWEN) | _BV(TWINT);												// Send NACK and clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready

		TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);									// Send STOP
	}
	else
	{
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		_writeByte(0);
		_waitForAck();
		_sendStop();

		_sendStart(I2C_ADDR_R);
		_waitForAck();

		for (int i=0; i<6; i++)
		{
			delayMicroseconds(10);
			_dataarray[i] = (_readByte() ^ 0x17) + 0x17;
			if (i<5)
				_sendAck();
			else
				_sendNack();
		}
		_sendStop();
	}
}

void WiiChuck::_writeRegister(uint8_t reg, uint8_t value)
{
	if (_use_hw)
	{
		// Send start address
		TWCR = _BV(TWEN) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);						// Send START
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = I2C_ADDR_W;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = reg;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready
		TWDR = value;
		TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWEA);									// Clear TWINT to proceed
		while ((TWCR & _BV(TWINT)) == 0) {};										// Wait for TWI to be ready

		TWCR = _BV(TWEN)| _BV(TWINT) | _BV(TWSTO);									// Send STOP
	}
	else
	{
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		_writeByte(reg);
		_waitForAck();
		_writeByte(value);
		_waitForAck();
		_sendStop();
	}
}

