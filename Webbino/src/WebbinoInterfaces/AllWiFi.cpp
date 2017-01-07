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

#include "AllWiFi.h"

#if defined (WEBBINO_USE_WIFI) || defined (WEBBINO_USE_WIFI101) || \
	  defined (WEBBINO_USE_ESP8266) || defined (WEBBINO_USE_ESP8266_STANDALONE)

#include <webbino_debug.h>

void WebClientWifi::init (InternalClient& c, char* req) {
	internalClient = c;
	request.parse (req);
	avail = 0;
}

size_t WebClientWifi::write (uint8_t c) {
	buf[avail++] = c;

	if (avail >= CLIENT_BUFSIZE) {
		flushBuffer ();
	}

	return 1;
}

void WebClientWifi::flushBuffer () {
	if (avail > 0) {
		//~ DPRINT (F("Flushing "));
		//~ DPRINT (avail);
		//~ DPRINTLN (F(" bytes to client"));

		// The cast is needed on ESP8266 standalone, byt shouldn't hurt anywhere
		internalClient.write ((const uint8_t *) buf, avail);
		avail = 0;
	}
}

void WebClientWifi::sendReply () {
	flushBuffer ();
	internalClient.stop ();
	DPRINTLN (F("Client disconnected"));
}


/****************************************************************************/

byte NetworkInterfaceWiFi::retBuffer[6];

// FIXME
NetworkInterfaceWiFi::NetworkInterfaceWiFi (): server (80) {
}

#if defined (WEBBINO_USE_WIFI) || defined (WEBBINO_USE_WIFI101) || \
	  defined (WEBBINO_USE_ESP8266_STANDALONE)
boolean NetworkInterfaceWiFi::begin (const char *_ssid, const char *_password) {
#elif defined (WEBBINO_USE_ESP8266)
boolean NetworkInterfaceWiFi::begin (Stream& _serial, const char *_ssid, const char *_password) {
	WiFi.init (&_serial);
#endif

	// Check for the presence of the WiFi interface
	if (WiFi.status () == WL_NO_SHIELD) {
		DPRINTLN (F("WiFi interface not found"));
		return false;
	}

	DPRINT (F("FW Version: "));
	DPRINTLN (WiFi.firmwareVersion ());

	// Attempt to connect to WiFi network
	// FIXME: Maybe only allow a finite number of attempts
	DPRINT (F("Connecting to AP: "));
	DPRINTLN (_ssid);
	WiFi.begin (const_cast<char *> (_ssid), _password);
	while (WiFi.status () != WL_CONNECTED) {
		delay (500);
	};
	DPRINT (F("Joined AP, local IP address: "));
	DPRINTLN (WiFi.localIP ());

	server.begin ();

	return true;

}

WebClient* NetworkInterfaceWiFi::processPacket () {
	WebClient *ret = NULL;

	InternalClient client = server.available ();
	if (client) {
		DPRINTLN (F("New client"));

		// An http request ends with a blank line
		boolean currentLineIsBlank = true;
		ethernetBufferSize = 0;
		boolean copy = true;
		while (client.connected ()) {
			if (client.available ()) {
				char c = client.read ();
				if (copy) {
					if (ethernetBufferSize < sizeof (ethernetBuffer)) {
						ethernetBuffer[ethernetBufferSize++] = c;
					} else {
						DPRINTLN (F("Ethernet buffer overflow"));
						break;
					}
				}

				// If you've gotten to the end of the line (received a newline
				// character) and the line is blank, the http request has ended
				if (c == '\n' && currentLineIsBlank) {
					webClient.init (client, (char *) ethernetBuffer);
					ret = &webClient;
					break;
				}

				if (c == '\n') {
					// See if we got the URL line
					if (strncmp_P ((char *) ethernetBuffer, PSTR ("GET "), 4) == 0) {
						// Yes, ignore the rest
						ethernetBuffer[ethernetBufferSize - 1] = '\0';
						copy = false;
					} else {
						// No, start over
						DPRINT (F("Discarding header line: \""));
						DPRINT (reinterpret_cast<char *> (ethernetBuffer));
						DPRINTLN (F("\""));

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

		// If we are not returning a client, close the connection
		if (!ret) {
			client.stop ();
			DPRINTLN (F("Client disconnected"));
		}
	}

	return ret;
}

boolean NetworkInterfaceWiFi::usingDHCP () {
	// FIXME
	return true;
}

byte *NetworkInterfaceWiFi::getMAC () {
	return WiFi.macAddress (retBuffer);
}

IPAddress NetworkInterfaceWiFi::getIP () {
	return WiFi.localIP ();
}

IPAddress NetworkInterfaceWiFi::getNetmask () {
	return WiFi.subnetMask ();
}

IPAddress NetworkInterfaceWiFi::getGateway () {
	return WiFi.gatewayIP ();
}

#endif
