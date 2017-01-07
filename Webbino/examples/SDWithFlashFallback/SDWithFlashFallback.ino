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

/* SS pin for the SD card reader. Pin 4 is used for the reader included
 * on most WIZ5100-based Ethernet shields
 */
#define SD_SS 4

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

const Page page01 PROGMEM = {index_htm_name, index_htm, NULL};

const Page* const pages[] PROGMEM = {
	&page01,
	NULL
};


/******************************************************************************
 * DEFINITION OF TAGS                                                         *
 ******************************************************************************/

#define REP_BUFFER_LEN 32
char replaceBuffer[REP_BUFFER_LEN];
PString subBuffer (replaceBuffer, REP_BUFFER_LEN);

PString& evaluate_ip (void *data __attribute__ ((unused))) {
   subBuffer.print (netint.getIP ());

  return subBuffer;
}

PString& evaluate_netmask (void *data __attribute__ ((unused))) {
  subBuffer.print (netint.getNetmask ());

  return subBuffer;
}

PString& evaluate_gw (void *data __attribute__ ((unused))) {
  subBuffer.print (netint.getGateway ());

  return subBuffer;
}

PString& evaluate_mac_addr (void *data __attribute__ ((unused))) {
	const byte *buf = netint.getMAC ();

	for (byte i = 0; i < 6; i++) {
		if (buf[i] < 16)
			subBuffer.print ('0');
		subBuffer.print (buf[i], HEX);

		if (i < 5)
			subBuffer.print (':');
	}

	return subBuffer;
}

PString& evaluate_ip_src (void *data __attribute__ ((unused))) {
	if (netint.usingDHCP ())
		subBuffer.print (F("DHCP"));
	else
		subBuffer.print (F("MANUAL"));

	return subBuffer;
}

PString& evaluate_webbino_version (void *data __attribute__ ((unused))) {
	subBuffer.print (WEBBINO_VERSION);

	return subBuffer;
}

PString& evaluate_uptime (void *data __attribute__ ((unused))) {
	unsigned long uptime = millis () / 1000;
	byte d, h, m, s;

	d = uptime / 86400;
	uptime %= 86400;
	h = uptime / 3600;
	uptime %= 3600;
	m = uptime / 60;
	uptime %= 60;
	s = uptime;

  if (d > 0) {
    subBuffer.print (d);
    subBuffer.print (d == 1 ? F(" day, ") : F(" days, "));
  }

  if (h < 10)
    subBuffer.print ('0');
  subBuffer.print (h);
  subBuffer.print (':');
  if (m < 10)
    subBuffer.print ('0');
  subBuffer.print (m);
  subBuffer.print (':');
  if (s < 10)
    subBuffer.print ('0');
  subBuffer.print (s);

  return subBuffer;
}

PString& evaluate_free_ram (void *data __attribute__ ((unused))) {
	// This only works on AVRs
#if defined (ARDUINO_ARCH_AVR)
	extern int __heap_start, *__brkval;
	int v;

	subBuffer.print ((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
#else
	subBuffer.print (F("N/A"));
#endif

	return subBuffer;
}


EasyReplacementTag (tagMacAddr, NET_MAC, evaluate_mac_addr);
EasyReplacementTag (tagIPAddress, NET_IP, evaluate_ip);
EasyReplacementTag (tagNetmask, NET_MASK, evaluate_netmask);
EasyReplacementTag (tagGateway, NET_GW, evaluate_gw);
EasyReplacementTag (tagNetConfSrc, NET_CONF_SRC, evaluate_ip_src);
EasyReplacementTag (tagWebbinoVer, WEBBINO_VER, evaluate_webbino_version);
EasyReplacementTag (tagUptime, UPTIME, evaluate_uptime);
EasyReplacementTag (tagFreeRAM, FREERAM, evaluate_free_ram);

EasyReplacementTagArray tags[] PROGMEM = {
	&tagMacAddr,
	&tagIPAddress,
	&tagNetmask,
	&tagGateway,
	&tagNetConfSrc,
	&tagWebbinoVer,
	&tagUptime,
	&tagFreeRAM,
	NULL
};


/******************************************************************************
 * MAIN STUFF                                                                 *
 ******************************************************************************/

// Avoid some bug reports :)
#if !defined (WEBBINO_ENABLE_SD) && !defined (WEBBINO_ENABLE_SDFAT)
#error Please enable WEBBINO_ENABLE_SD or WEBBINO_ENABLE_SDFAT in webbino_config.h
#endif

void setup () {
	Serial.begin (9600);
	Serial.println (F("Using Webbino " WEBBINO_VERSION));

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

		Serial.print (F("Initializing SD card..."));
		if (!webserver.begin (netint, NULL, tags, SD_SS)) {
			Serial.println (F(" failed"));
			while (42)
				;
		}
		Serial.println (F(" done"));
	}
}

void loop () {
	webserver.loop ();
}
