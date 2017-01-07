/*
This is the part of the ProTrinketMouse code that is usually written in C
Placing it here does not mix it with the object oriented C++ code

Copyright (c) 2015 Adafruit Industries
All rights reserved.

ProTrinketMouse is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

ProTrinketMouse is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with ProTrinketMouse. If not, see
<http://www.gnu.org/licenses/>.
*/

#include "cmdline_defs.h"
#include "ProTrinketMouseC.h"
#include "usbconfig.h"
#include "usbdrv/usbdrv.h"
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t report_buffer[4];
char usb_hasCommed = 0;
uint8_t idle_rate = 500 / 4;  // see HID1_11.pdf sect 7.2.4
uint8_t protocol_version = 0; // see HID1_11.pdf sect 7.2.6

void usbBegin()
{
    cli();

    // fake a disconnect to force the computer to re-enumerate
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
			usbSetInterrupt((uint8_t*)report_buffer, 4); // send
			break;

			// see http://vusb.wikidot.com/driver-api
		}
	}
}

// USB HID report descriptor for boot protocol keyboard
// see HID1_11.pdf appendix B section 1
// USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH is defined in usbconfig (it's supposed to be 50)
const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
	0x05, 0x01,       // USAGE_PAGE (Generic Desktop)
	0x09, 0x02,       // USAGE (Mouse)
	0xa1, 0x01,       // COLLECTION (Application)
	0x09, 0x01,       //   USAGE (Pointer)
	0xA1, 0x00,       //   COLLECTION (Physical)
	0x05, 0x09,       //     USAGE_PAGE (Button)
	0x19, 0x01,       //     USAGE_MINIMUM
	0x29, 0x03,       //     USAGE_MAXIMUM
	0x15, 0x00,       //     LOGICAL_MINIMUM (0)
	0x25, 0x01,       //     LOGICAL_MAXIMUM (1)
	0x95, 0x03,       //     REPORT_COUNT (3)
	0x75, 0x01,       //     REPORT_SIZE (1)
	0x81, 0x02,       //     INPUT (Data,Var,Abs)
	0x95, 0x01,       //     REPORT_COUNT (1)
	0x75, 0x05,       //     REPORT_SIZE (5)
	0x81, 0x03,       //     INPUT (Const,Var,Abs)
	0x05, 0x01,       //     USAGE_PAGE (Generic Desktop)
	0x09, 0x30,       //     USAGE (X)
	0x09, 0x31,       //     USAGE (Y)
	0x09, 0x38,       //     USAGE (Wheel)
	0x15, 0x81,       //     LOGICAL_MINIMUM (-127)
	0x25, 0x7F,       //     LOGICAL_MAXIMUM (127)
	0x75, 0x08,       //     REPORT_SIZE (8)
	0x95, 0x03,       //     REPORT_COUNT (3)
	0x81, 0x06,       //     INPUT (Data,Var,Rel)
	0xC0,             //   END_COLLECTION
	0xC0,             // END COLLECTION
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
			return 3;
		case USBRQ_HID_SET_REPORT:
			return 0; // send nothing, mouses don't do this
		default: // do not understand data, ignore
			return 0; // send nothing
	}
}

