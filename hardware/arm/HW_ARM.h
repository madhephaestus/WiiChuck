void WiiChuck::begin()
{
	_use_hw = false;
	if ((_sda_pin == SDA) and (_scl_pin == SCL))
	{
		_use_hw = true;
		twi = TWI1;
		pmc_enable_periph_clk(WIRE_INTERFACE_ID);
		PIO_Configure(g_APinDescription[PIN_WIRE_SDA].pPort, g_APinDescription[PIN_WIRE_SDA].ulPinType, g_APinDescription[PIN_WIRE_SDA].ulPin, g_APinDescription[PIN_WIRE_SDA].ulPinConfiguration);
		PIO_Configure(g_APinDescription[PIN_WIRE_SCL].pPort, g_APinDescription[PIN_WIRE_SCL].ulPinType, g_APinDescription[PIN_WIRE_SCL].ulPin, g_APinDescription[PIN_WIRE_SCL].ulPinConfiguration);
		NVIC_DisableIRQ(TWI1_IRQn);
		NVIC_ClearPendingIRQ(TWI1_IRQn);
		NVIC_SetPriority(TWI1_IRQn, 0);
		NVIC_EnableIRQ(TWI1_IRQn);

	}
	else if ((_sda_pin == SDA1) and (_scl_pin == SCL1))
	{
		_use_hw = true;
		twi = TWI0;
		pmc_enable_periph_clk(WIRE1_INTERFACE_ID);
		PIO_Configure(g_APinDescription[PIN_WIRE1_SDA].pPort, g_APinDescription[PIN_WIRE1_SDA].ulPinType, g_APinDescription[PIN_WIRE1_SDA].ulPin, g_APinDescription[PIN_WIRE1_SDA].ulPinConfiguration);
		PIO_Configure(g_APinDescription[PIN_WIRE1_SCL].pPort, g_APinDescription[PIN_WIRE1_SCL].ulPinType, g_APinDescription[PIN_WIRE1_SCL].ulPin, g_APinDescription[PIN_WIRE1_SCL].ulPinConfiguration);
		NVIC_DisableIRQ(TWI0_IRQn);
		NVIC_ClearPendingIRQ(TWI0_IRQn);
		NVIC_SetPriority(TWI0_IRQn, 0);
		NVIC_EnableIRQ(TWI0_IRQn);
	}

	if (_use_hw)
	{
		// activate internal pullups for twi.
		digitalWrite(SDA, 1);
		digitalWrite(SCL, 1);

		// Reset the TWI
		twi->TWI_CR = TWI_CR_SWRST;
		// TWI Slave Mode Disabled, TWI Master Mode Disabled.
		twi->TWI_CR = TWI_CR_SVDIS;
		twi->TWI_CR = TWI_CR_MSDIS;
		// Set TWI Speed
		twi->TWI_CWGR = (TWI_DIV << 16) | (TWI_SPEED << 8) | TWI_SPEED;
		// Set master mode
		twi->TWI_CR = TWI_CR_MSEN;
	}
	else
	{
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
		// Set slave address and number of internal address bytes.
		twi->TWI_MMR = (1 << 8) | TWI_MMR_MREAD | (I2C_ADDR << 16);
		// Set internal address bytes
		twi->TWI_IADR = 0;
		// Send START condition
		twi->TWI_CR = TWI_CR_START;

		for (int i=0; i<6; i++)
		{
			while ((twi->TWI_SR & TWI_SR_RXRDY) != TWI_SR_RXRDY) {};
			_dataarray[i] = (twi->TWI_RHR ^ 0x17) + 0x17;;
		}

		twi->TWI_CR = TWI_CR_STOP;
		while ((twi->TWI_SR & TWI_SR_RXRDY) != TWI_SR_RXRDY) {};
		uint8_t dummy = twi->TWI_RHR;
		while ((twi->TWI_SR & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {};
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
			delayMicroseconds(15);
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
		// Set slave address and number of internal address bytes.
		twi->TWI_MMR = (1 << 8) | (I2C_ADDR << 16);
		// Set internal address bytes
		twi->TWI_IADR = reg;
		// Send a single byte to start transfer
		twi->TWI_THR = value;
		while ((twi->TWI_SR & TWI_SR_TXRDY) != TWI_SR_TXRDY) {};
		// Send STOP condition
		twi->TWI_CR = TWI_CR_STOP;
		while ((twi->TWI_SR & TWI_SR_TXCOMP) != TWI_SR_TXCOMP) {};
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

