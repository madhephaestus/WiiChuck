/*
 VoltageReference.cpp - VoltageReference library
 Copyright (c) 2014 Roberto Lo Giacco.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _VOLTAGE_REFERENCE_
#define _VOLTAGE_REFERENCE_

#include "Arduino.h"

#define ANALOG_MAX_VALUE 0x3FF
#define DEFAULT_REFERENCE_CALIBRATION 1126400L
#define INVALID_REFERENCE_CALIBRATION 0xFFFFFFL

#define getHiByte(calibration) (calibration >> 16)
#define getMidByte(calibration) (calibration >> 8)
#define getLowByte(calibration) (calibration & 0xFF)
#define mergeBytes(hi, mid, low) ((((long)hi) << 16) | ((mid << 8) | ((low & 0xFF) & 0xFFFF)))

class VoltageReference {
	private:
		uint32_t calibration;

		/**
		 * Reads the internal voltage reference against Vcc
		 */
		uint16_t readInternalRef();
	public:

		/**
		 * Initializes the library by setting the calibration value for the 1.1V reference
		 * either to the provided value or to its default
		 */
		void begin(uint32_t reference = DEFAULT_REFERENCE_CALIBRATION);

		/**
		 * Initializes the library by setting the calibration value for the 1.1V reference
		 * from a three bytes representation
		 */
		void begin(uint8_t hi, uint8_t mid, uint8_t low);

		/**
		 * Reads the input voltage value applying any necessary calibration
		 */
		uint16_t readVcc();

		/**
		 * Returns the calibration value to be used for the specified input voltage
		 */
		uint32_t calibrate(uint16_t milliVolt);

		/**
		 * The calibrated internal reference voltage
		 */
		const uint16_t internalValue();

};
#endif // _VOLTAGE_REFERENCE_
