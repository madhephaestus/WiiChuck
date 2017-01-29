void WiiChuck::begin()
{


	_use_hw = false;
	pinMode(_scl_pin, OUTPUT);
	// improved startup procedure from http://playground.arduino.cc/Main/WiiChuckClass
	_writeRegister(0xF0, 0x55);
	_writeRegister(0xFB, 0x00);
	//Serial.println("Init sent, reading");

	delay(100);
	_burstRead();
	//Serial.println("re-reading");
	delay(100);
	_burstRead();
	_joy_x_center = _dataarray[0];
	_joy_y_center = _dataarray[1];
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
		Serial.println("Seconde byte");
		_writeByte(value);
		_waitForAck();
		//Serial.println("stopping");
		_sendStop();
		//Serial.println("done");

}

