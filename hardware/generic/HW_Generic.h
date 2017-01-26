void WiiChuck::begin()
{


	_use_hw = false;
	pinMode(_scl_pin, OUTPUT);

	_writeRegister(0x40, 0x00);
	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
	_joy_x_center = _dataarray[0];
	_joy_y_center = _dataarray[1];

}

void WiiChuck::_burstRead()
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

void WiiChuck::_writeRegister(uint8_t reg, uint8_t value)
{
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		_writeByte(reg);
		_waitForAck();
		_writeByte(value);
		_waitForAck();
		_sendStop();

}

