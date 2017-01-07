/***************************************************************************
 *   This file is part of Webbino.                                         *
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

#include <Webbino.h>

// Instantiate the WebServer
WebServer webserver;

// Instantiate the network interface defined in the Webbino headers
#if defined (WEBBINO_USE_ENC28J60)
	#include <WebbinoInterfaces/ENC28J60.h>
	NetworkInterfaceENC28J60 netint;
#elif defined (WEBBINO_USE_WIZ5100) || defined (WEBBINO_USE_WIZ5500)
	#include <WebbinoInterfaces/WIZ5x00.h>
	NetworkInterfaceWIZ5x00 netint;
#elif defined (WEBBINO_USE_ESP8266)
	#include <WebbinoInterfaces/AllWiFi.h>

	#include <SoftwareSerial.h>
	SoftwareSerial swSerial (7, 8);

	// Wi-Fi parameters
	#define WIFI_SSID        "ssid"
	#define WIFI_PASSWORD    "password"

	NetworkInterfaceWiFi netint;
#elif defined (WEBBINO_USE_WIFI) || defined (WEBBINO_USE_WIFI101) || \
	  defined (WEBBINO_USE_ESP8266_STANDALONE)
	#include <WebbinoInterfaces/AllWiFi.h>

	// Wi-Fi parameters
	#define WIFI_SSID        "ssid"
	#define WIFI_PASSWORD    "password"

	NetworkInterfaceWiFi netint;
#elif defined (WEBBINO_USE_DIGIFI)
	#include <WebServer_DigiFi.h>
	NetworkInterfaceDigiFi netint;
#endif

/******************************************************************************
 * DEFINITION OF PAGES                                                        *
 ******************************************************************************/

#include "html.h"

const Page indexPage PROGMEM = {index_html_name, index_html, NULL};

const Page * const pages[] PROGMEM = {
	&indexPage,
 	NULL
};


/******************************************************************************
 * MAIN STUFF                                                                 *
 ******************************************************************************/

void setup () {
	Serial.begin (9600);
	Serial.println (F("Webbino " WEBBINO_VERSION));

	Serial.println (F("Trying to get an IP address through DHCP"));
#if defined (WEBBINO_USE_ENC28J60) || defined (WEBBINO_USE_WIZ5100) || defined (WEBBINO_USE_WIZ5500)
	byte mac[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
	bool ok = netint.begin (mac);
#elif defined (WEBBINO_USE_ESP8266)
	swSerial.begin (9600);
	bool ok = netint.begin (swSerial, WIFI_SSID, WIFI_PASSWORD);
#elif defined (WEBBINO_USE_WIFI) || defined (WEBBINO_USE_WIFI101) || \
	  defined (WEBBINO_USE_ESP8266_STANDALONE)
	bool ok = netint.begin (WIFI_SSID, WIFI_PASSWORD);
#elif defined (WEBBINO_USE_DIGIFI)
	bool ok = netint.begin ();
#endif

	if (!ok) {
		Serial.println (F("Failed to get configuration from DHCP"));
		while (42)
			;
	} else {
		Serial.println (F("DHCP configuration done:"));
		Serial.print (F("- IP: "));
		Serial.println (netint.getIP ());
		Serial.print (F("- Netmask: "));
		Serial.println (netint.getNetmask ());
		Serial.print (F("- Default Gateway: "));
		Serial.println (netint.getGateway ());

		webserver.begin (netint, pages);
	}
}

void loop () {
	webserver.loop ();
}
