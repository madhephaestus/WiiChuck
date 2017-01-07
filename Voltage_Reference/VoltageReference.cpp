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

#include "Arduino.h"
#include "VoltageReference.h"

void VoltageReference::begin(uint32_t calibration) {
	VoltageReference::calibration = calibration;
}

void VoltageReference::begin(uint8_t hi, uint8_t mid, uint8_t low) {
	VoltageReference::calibration = mergeBytes(hi, mid, low);
	if (calibration == INVALID_REFERENCE_CALIBRATION)
		VoltageReference::calibration = DEFAULT_REFERENCE_CALIBRATION;
}

uint16_t VoltageReference::readInternalRef() {
	// Read 1.1V reference against Vcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284P__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif

	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA, ADSC)); // measuring
	uint8_t low = ADCL; // must read ADCL first - it then locks ADCH
	uint8_t high = ADCH; // unlocks both
	return (high << 8) | low; // raw bandgap value;
}

uint16_t VoltageReference::readVcc() {
	return calibration / readInternalRef();
}

const uint16_t VoltageReference::internalValue() {
	return (calibration + (ANALOG_MAX_VALUE / 2)) / ANALOG_MAX_VALUE;
}


uint32_t VoltageReference::calibrate(uint16_t milliVolts) {
	return ((uint32_t) milliVolts * readInternalRef());
}
