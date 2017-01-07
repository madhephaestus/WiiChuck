/*
 AD5241.h
 AD5241 driver for Arduino
 
 Copyright 2015-2016 Max Chan <max@maxchan.info>
 */

#ifndef _AD5241_H_
#define _AD5241_H_

#include <Arduino.h>
#include <Wire.h>

#ifndef DAC_SHUTDOWN
#define DAC_SHUTDOWN UINT16_MAX
#endif

#ifndef DAC_RESET
#define DAC_RESET UINT16_MAX - 1
#endif

/// Driver for Analog Devices AD5241/2 digi-pot.
class AD5241
{
private:
	TwoWire &_wire;
    uint8_t _address;
	uint8_t _digitalOutputs;
    
public:
    /*!
     Initialize the TLC5615 driver.
     
     @param ss GPIO pin used for the chip select for the DAC.
     */
	AD5241(TwoWire &wire = Wire, uint8_t address = 0x2c);
	AD5241(uint8_t address);
    
    /*!
     Initialize the SPI bus and the DAC chip.
     */
    void begin(void);
    
	/*!
	 Write a value to the DAC chip.

	 @param value The DAC value. It is capped to 10 bits and excessive bits are
	 ignored.
	 */
	void analogWrite(uint16_t value);

    /*!
     Write a value to the DAC chip.
     
	 @param gate  The ID of the DAC.
     @param value The DAC value. It is capped to 10 bits and excessive bits are
     ignored.
     */
    void analogWrite(uint8_t gate, uint16_t value);

	void digitalWrite(uint8_t pin, uint8_t value);
};

#endif // !defined(_AD5241_H_)
