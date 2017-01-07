/***************************************************************************
 *   This file is part of Webbino                                          *
 *                                                                         *
 *   Copyright (C) 2012-2016 by SukkoPera                                  *
 *                                                                         *
 *   Webbino is free software: you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Webbino is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Webbino. If not, see <http://www.gnu.org/licenses/>.       *
 ***************************************************************************/

#include "ENC28J60.h"

#ifdef WEBBINO_USE_ENC28J60

#include <webbino_debug.h>

// Ethernet packet buffer
byte Ethernet::buffer[NetworkInterfaceENC28J60::ETHERNET_BUFSIZE];


void WebClientENC28J60::initReply () {
	bfill = ether.tcpOffset ();
}

void WebClientENC28J60::sendReply () {
	ether.httpServerReply (bfill.position ());
}

size_t WebClientENC28J60::write (uint8_t c) {
	//uint8_t tmp[2] = {c, '\0'};
	//bfill.emit_p (PSTR ("$S"), tmp);
	bfill.emit_raw ((const char *) &c, 1);

	return 1;
}

/****************************************************************************/


boolean NetworkInterfaceENC28J60::begin (byte *mac, byte csPin) {
	boolean ret;

	DPRINTLN (F("Using EtherCard library"));

	if ((ret = ether.begin (sizeof (Ethernet::buffer), mac, csPin))) {
		ret = ether.dhcpSetup ();
		dhcp = true;
	}

	return ret;
}

boolean NetworkInterfaceENC28J60::begin (byte *mac, IPAddress ip, IPAddress dns, IPAddress gw, IPAddress mask, byte csPin) {
	boolean ret;

	DPRINTLN (F("Using EtherCard library"));

	if ((ret = ether.begin (sizeof (Ethernet::buffer), mac, csPin))) {
		ret = ether.staticSetup (&ip[0], &gw[0], &dns[0], &mask[0]);
		dhcp = false;
	}

	return ret;
}

WebClient *NetworkInterfaceENC28J60::processPacket () {
	WebClient* ret = NULL;

	word len = ether.packetReceive ();
	word pos = ether.packetLoop (len);

	if (pos) {
		// Got a packet
		client.request.parse ((char *) Ethernet::buffer + pos);
		//DPRINTLN ((char *) Ethernet::buffer + pos);
		ret = &client;
	}

	return ret;
}

boolean NetworkInterfaceENC28J60::usingDHCP () {
	return dhcp;
}

byte *NetworkInterfaceENC28J60::getMAC () {
	return EtherCard::mymac;
}

// These need to be tested!
IPAddress NetworkInterfaceENC28J60::getIP () {
	IPAddress ip = EtherCard::myip;
	return ip;
}

IPAddress NetworkInterfaceENC28J60::getNetmask () {
	IPAddress ip = EtherCard::netmask;
	return ip;
}

IPAddress NetworkInterfaceENC28J60::getGateway () {
	IPAddress ip = EtherCard::gwip;
	return ip;
}

#endif
