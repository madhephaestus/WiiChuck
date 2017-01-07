/*
This is the part of the ProTrinketKeyboard code that is usually written in C
Placing it here does not mix it with the object oriented C++ code

Copyright (c) 2015 Adafruit Industries
All rights reserved.

ProTrinketKeyboard is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

ProTrinketKeyboard is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with ProTrinketKeyboard. If not, see
<http://www.gnu.org/licenses/>.
*/

#include "cmdline_defs.h" 
#include "ProTrinketKeyboardC.h"
#include "usbconfig.h"
#include "usbdrv/usbdrv.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t report_buffer[8];
char usb_hasCommed = 0;
uint8_t idle_rate = 500 / 4;  // see HID1_11.pdf sect 7.2.4
uint8_t protocol_version = 0; // see HID1_11.pdf sect 7.2.6
uint8_t led_state = 0; // caps/num/scroll lock LEDs

void usbBegin()
{
    cli();

	// fake a disconnect to force the computer to re-enumerate
     // Change from Trinket: Pro Trinket uses pins on PORTD
    PORTD &= ~(_BV(USB_CFG_DMINUS_BIT) | _BV(USB_CFG_DPLUS_BIT)); 

    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();

    // start the USB driver
    usbInit();
    sei();
}

void usbPollWrapper()
{
	usbPoll();
}

void usbReportSend()
{
	// perform usb background tasks until the report can be sent, then send it
	while (1)
	{
		usbPoll(); // this needs to be called at least once every 10 ms
		if (usbInterruptIsReady())
		{
			usbSetInterrupt((uint8_t*)report_buffer, 8); // send
			break;

			// see http://vusb.wikidot.com/driver-api
		}
	}
}

// USB HID report descriptor for boot protocol keyboard
// see HID1_11.pdf appendix B section 1
// USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH is defined in usbconfig (it's supposed to be 63)
const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
	0x05, 0x01,  // USAGE_PAGE (Generic Desktop)
	0x09, 0x06,  // USAGE (Keyboard)
	0xA1, 0x01,  // COLLECTION (Application)
	0x05, 0x07,  //   USAGE_PAGE (Keyboard)(Key Codes)
	0x19, 0xE0,  //   USAGE_MINIMUM (Keyboard LeftControl)(224)
	0x29, 0xE7,  //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
	0x15, 0x00,  //   LOGICAL_MINIMUM (0)
	0x25, 0x01,  //   LOGICAL_MAXIMUM (1)
	0x75, 0x01,  //   REPORT_SIZE (1)
	0x95, 0x08,  //   REPORT_COUNT (8)
	0x81, 0x02,  //   INPUT (Data,Var,Abs) ; Modifier byte
	0x95, 0x01,  //   REPORT_COUNT (1)
	0x75, 0x08,  //   REPORT_SIZE (8)
	0x81, 0x03,  //   INPUT (Cnst,Var,Abs) ; Reserved byte
	0x95, 0x05,  //   REPORT_COUNT (5)
	0x75, 0x01,  //   REPORT_SIZE (1)
	0x05, 0x08,  //   USAGE_PAGE (LEDs)
	0x19, 0x01,  //   USAGE_MINIMUM (Num Lock)
	0x29, 0x05,  //   USAGE_MAXIMUM (Kana)
	0x91, 0x02,  //   OUTPUT (Data,Var,Abs) ; LED report
	0x95, 0x01,  //   REPORT_COUNT (1)
	0x75, 0x03,  //   REPORT_SIZE (3)
	0x91, 0x03,  //   OUTPUT (Cnst,Var,Abs) ; LED report padding
	0x95, 0x06,  //   REPORT_COUNT (6)
	0x75, 0x08,  //   REPORT_SIZE (8)
	0x15, 0x00,  //   LOGICAL_MINIMUM (0)
	0x25, 0x65,  //   LOGICAL_MAXIMUM (101)
	0x05, 0x07,  //   USAGE_PAGE (Keyboard)(Key Codes)
	0x19, 0x00,  //   USAGE_MINIMUM (Reserved (no event indicated))(0)
	0x29, 0x65,  //   USAGE_MAXIMUM (Keyboard Application)(101)
	0x81, 0x00,  //   INPUT (Data,Ary,Abs)
	0xC0         // END_COLLECTION
};

// see http://vusb.wikidot.com/driver-api
// constants are found in usbdrv.h
usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	usb_hasCommed = 1;

	// see HID1_11.pdf sect 7.2 and http://vusb.wikidot.com/driver-api
	usbRequest_t *rq = (void *)data;

	if ((rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_CLASS)
		return 0; // ignore request if it's not a class specific request

	// see HID1_11.pdf sect 7.2
	switch (rq->bRequest)
	{
		case USBRQ_HID_GET_IDLE:
			usbMsgPtr = &idle_rate; // send data starting from this byte
			return 1; // send 1 byte
		case USBRQ_HID_SET_IDLE:
			idle_rate = rq->wValue.bytes[1]; // read in idle rate
			return 0; // send nothing
		case USBRQ_HID_GET_PROTOCOL:
			usbMsgPtr = &protocol_version; // send data starting from this byte
			return 1; // send 1 byte
		case USBRQ_HID_SET_PROTOCOL:
			protocol_version = rq->wValue.bytes[1];
			return 0; // send nothing
		case USBRQ_HID_GET_REPORT:
			usbMsgPtr = (uint8_t*)report_buffer; // send the report data
			return 8;
		case USBRQ_HID_SET_REPORT:
			if (rq->wLength.word == 1) // check data is available
			{
				// 1 byte, we don't check report type (it can only be output or feature)
				// we never implemented "feature" reports so it can't be feature
				// so assume "output" reports
				// this means set LED status
				// since it's the only one in the descriptor
				return USB_NO_MSG; // send nothing but call usbFunctionWrite
			}
			else // no data or do not understand data, ignore
			{
				return 0; // send nothing
			}
		default: // do not understand data, ignore
			return 0; // send nothing
	}
}

// see http://vusb.wikidot.com/driver-api
usbMsgLen_t usbFunctionWrite(uint8_t * data, uchar len)
{
	led_state = data[0];
	return 1; // 1 byte read
}

