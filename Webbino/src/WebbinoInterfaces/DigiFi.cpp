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

#include "DigiFi.h"

#ifdef WEBBINO_USE_DIGIFI

#include <webbino_debug.h>


WebClientDigiFi::WebClientDigiFi (DigiFi& _wifi): wifi (_wifi) {
}

void WebClientDigiFi:: initReply (char* req) {
	request.parse (req);

	bufUsed = 0;
	headerLen = 0;
}

size_t WebClientDigiFi::write (uint8_t c) {
	size_t ret;

	if (bufUsed < BUFFER_SIZE) {
		if (headerLen == 0 && bufUsed >= 3 &&
		  buf[bufUsed - 3] == '\r' && buf[bufUsed - 2] == '\n' &&
		  buf[bufUsed - 1] == '\r' && c == '\n') {

			/* Manipulate headers a bit for later, i.e. replace \r (last char
			 * added to buffer) with \0 and don't insert \n (current char)
			 */
			buf[bufUsed - 1] = '\0';
			headerLen = bufUsed;	// Remember header len
		} else {
			buf[bufUsed++] = c;
		}

		ret = 1;
	} else {
		ret = 0;
	}

	return ret;
}

void WebClientDigiFi::sendReply () {
	//~ DPRINTLN (F("HEADERS:"));
	//~ DPRINT ((char*) buf);

	//~ DPRINTLN (F("BODY:"));
	//~ DPRINTLN ((char*) buf + headerLen);

	// Send headers
	wifi.write (buf, headerLen - 1);

	// Add content length header
	wifi.print (F("Content-Length: "));
	wifi.print (bufUsed - headerLen);
	wifi.write (F("\r\n"));

	// Add connection close header
	wifi.print (F("Connection: close\r\n"));

	// Send end of headers
	wifi.print (F("\r\n"));

	// Send body
	wifi.write (buf + headerLen, bufUsed - headerLen);

	// No way to close connection with this chip :(
}

/****************************************************************************/

NetworkInterfaceDigiFi::NetworkInterfaceDigiFi (): webClient (wifi) {
}

boolean NetworkInterfaceDigiFi::begin () {
	boolean ret = false;

#ifndef WEBBINO_NDEBUG
	/* DigiX trick: since we are on serial over USB wait for a character to be
	 * entered in serial terminal
	 */
	while (!Serial.available ()) {
		Serial.println (F("Enter any key to begin"));
		delay (1000);
	}
#endif

	DPRINTLN (F("Using DigiFi library"));

	if (SERVER_PORT == 80) {
		/* Port 80 is used by the configuration web interface, so it can't be
		 * used for our server
		 */
		DPRINTLN (F("Port 80 is not supported on this chip"));
	} else {
		/* Note that speed here must match the speed set under "Other Setting"
		 * -> "Serial Port Parameters Setting" in the wigi module web
		 * interface. If no speed is specified, it defaults to 9600 bps, but
		 * note that the module defaults to 115200!
		 */
		wifi.begin ();

		// Attempt to connect to WiFi network
		DPRINTLN (F("Connecting to network"));
		while (!wifi.ready ()) {
			DPRINTLN (F("Failed, retrying in 5 seconds"));
			delay (5000);
		}

		// Sets up server and returns IP
		/* String address = */ wifi.server (SERVER_PORT);

		DPRINT (F("Joined AP, local IP address: "));
		DPRINTLN (wifi.localIP ());

		/* This is the ONLY way we can close the TCP connection after we have
		 * replied. See http://digistump.com/board/index.php/topic,1270.0.html
		 * for details.
		 *
		 * Note that this must allow time for all tags to be replaces, which
		 * might take a bit, since some tags might need to talk to the wifi
		 * module to retrieve IP settings, etc.
		 */
		wifi.setTCPTimeout (15);

		ret = true;
	}

	return ret;
}

/* The DigiFi library is total crap, so we have to go low-level. Of course it
 * would be much better to rewrite said library from scratch, but it doesn't
 * really look it is worth the effort, so let's make do with this.
 */
WebClient* NetworkInterfaceDigiFi::processPacket () {
	WebClient *ret = NULL;

	if (wifi.available ()) {
		DPRINTLN (F("New client"));

		// An http request ends with a blank line
		boolean currentLineIsBlank = true;
		ethernetBufferSize = 0;
		boolean copy = true;
		unsigned int startTime = millis ();
		while (!ret && millis () - startTime < REQUEST_TIMEOUT * 1000L) {
			if (wifi.available ()) {
				char c = wifi.read ();
				if (copy) {
					if (ethernetBufferSize < sizeof (ethernetBuffer)) {
						ethernetBuffer[ethernetBufferSize++] = c;
					} else {
						DPRINTLN (F("Ethernet buffer overflow"));
						break;
					}
				}

				// If you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended,
				if (c == '\n' && currentLineIsBlank) {
					webClient.initReply (reinterpret_cast<char *> (ethernetBuffer));
					ret = &webClient;
				}

				if (c == '\n') {
					// See if we got the URL line
					if (strncmp_P (reinterpret_cast<char *> (ethernetBuffer), PSTR ("GET "), 4) == 0) {
						// Yes, ignore the rest
						ethernetBuffer[ethernetBufferSize - 1] = '\0';
						copy = false;
					} else {
						// No, start over
						ethernetBufferSize = 0;
					}

					// you're starting a new line
					currentLineIsBlank = true;
				} else if (c != '\r') {
					// you've gotten a character on the current line
					currentLineIsBlank = false;
				}
			}
		}

		if (!ret)
			DPRINTLN (F("Request timeout"));

		// If we are not returning a client, close the connection
		//~ if (!ret) {
			//~ client.stop ();
			//~ DPRINTLN (F("Client disconnected"));
		//~ }
	}

	return ret;
}

/* FIXME All of the following methods are very slow, since they need to talk to
 * the module. Maybe we'd better cache the results in begin().
 */
boolean NetworkInterfaceDigiFi::usingDHCP () {
	wifi.startATMode ();
	String str = wifi.getSTANetwork ();
	wifi.endATMode ();

	//~ DPRINT ("getSTANetwork = ");
	//~ DPRINT (str);

	// Response is like "+ok=DHCP,10.0.0.152,255.255.255.0,10.0.0.254"
	return str.startsWith ("+ok") && str.length () > 4 && str.substring (4).startsWith ("DHCP");
}

byte *NetworkInterfaceDigiFi::getMAC () {
	wifi.startATMode ();
	String str = wifi.getSTAMac ();
	wifi.endATMode ();

	//~ DPRINT ("getSTAMac = ");
	//~ DPRINTLN (str);

	// Response is like "+ok=ACCF223657B4" (+\r\n, I guess)
	if (str.startsWith ("+ok") && str.length () >= 16) {
		for (byte b = 0; b < 6; b++) {
			String oct = str.substring (b * 2 + 4, b * 2 + 4 + 2);
			macAddress[b] = (byte) strtol (oct.c_str (), NULL, 16);
		}
	}

	return macAddress;
}

IPAddress NetworkInterfaceDigiFi::getIP () {
	return wifi.localIP ();
}

IPAddress NetworkInterfaceDigiFi::getNetmask () {
	return wifi.subnetMask ();
}

IPAddress NetworkInterfaceDigiFi::getGateway () {
	return wifi.gatewayIP ();
}

#endif
