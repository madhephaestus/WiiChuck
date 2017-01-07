/*
  WiiChuck.cpp - Arduino/chipKit library support for the WiiChuck
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library has been made to easily interface and use the WiiChuck with
  an Arduino or chipKit.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#ifndef WiiChuck_h
#define WiiChuck_h

#if defined(__AVR__)
	#include "Arduino.h"
	#include "hardware/avr/HW_AVR_defines.h"
#elif defined(__PIC32MX__)
	#include "WProgram.h"
	#include "hardware/pic32/HW_PIC32_defines.h"
#elif defined(__arm__)
	#include "Arduino.h"
	#include "hardware/arm/HW_ARM_defines.h"
#endif

#define I2C_ADDR		0x52
#define I2C_ADDR_R		((I2C_ADDR << 1) + 1)
#define I2C_ADDR_W		(I2C_ADDR << 1)

class WiiChuck
{
	public:
		WiiChuck(uint8_t data_pin, uint8_t sclk_pin);
		void	begin();
		void	readData();

		int		getJoyX();
		int		getJoyY();

		int		getRollAngle();
		int		getPitchAngle();
		int		getAccelX();
		int		getAccelY();
		int		getAccelZ();

		boolean	checkButtonC();
		boolean	checkButtonZ();

	private:
		uint8_t _scl_pin;
		uint8_t _sda_pin;
		uint8_t _dataarray[6];
		uint8_t	_joy_x_center, _joy_y_center;
		boolean	_use_hw;

		void	_sendStart(byte addr);
		void	_sendStop();
		void	_sendAck();
		void	_sendNack();
		void	_waitForAck();
		uint8_t	_readByte();
		void	_writeByte(uint8_t value);
		void	_burstRead();
		void 	_writeRegister(uint8_t reg, uint8_t value);
#if defined(__arm__)
		Twi		*twi;
#endif
};
#endif
