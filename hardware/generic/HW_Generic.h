void WiiChuck::begin()
{


	_use_hw = false;
	initBytes();
	//Serial.println("Init sent, reading");

	delay(100);
	_burstRead();
	//Serial.println("re-reading");
	delay(100);
	_burstRead();
	_joy_x_center = _dataarray[0];
	_joy_y_center = _dataarray[1];
	_joy_x_max=_joy_x_center;
	_joy_x_min=_joy_x_center;
	_joy_y_max=_joy_y_center;
	_joy_y_min=_joy_y_center;
	Serial.println("Initialization Done");

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
	//Serial.println("Sending start");
		_sendStart(I2C_ADDR_W);
		_waitForAck();
		//Serial.println("First byte");
		_writeByte(reg);
		_waitForAck();
		//Serial.println("Seconde byte");
		_writeByte(value);
		//Serial.println("waiting");
		_waitForAck();
		//Serial.println("stopping");
		_sendStop();
		//Serial.println("done");

}

