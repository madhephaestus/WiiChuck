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

#ifndef _WEBSERVER8266_H_
#define _WEBSERVER8266_H_

#include <webbino_config.h>

#if defined (WEBBINO_USE_WIFI) || defined (WEBBINO_USE_WIFI101) || \
	  defined (WEBBINO_USE_ESP8266) || defined (WEBBINO_USE_ESP8266_STANDALONE)

#if defined (WEBBINO_USE_WIFI)
#include <WiFi.h>

typedef WiFiClient InternalClient;
typedef WiFiServer InternalServer;

#elif defined (WEBBINO_USE_WIFI101)
#include <WiFi101.h>

typedef WiFiClient InternalClient;
typedef WiFiServer InternalServer;

#elif defined (WEBBINO_USE_ESP8266)
#include <WiFiEsp.h>

typedef WiFiEspClient InternalClient;
typedef WiFiEspServer InternalServer;

#elif defined (WEBBINO_USE_ESP8266_STANDALONE)

#include <ESP8266WiFi.h>

typedef WiFiClient InternalClient;
typedef WiFiServer InternalServer;

#endif

#include "WebbinoCore/WebClient.h"
#include "WebbinoCore/WebServer.h"


#define CLIENT_BUFSIZE 256

class WebClientWifi: public WebClient {
private:
	InternalClient internalClient;
	byte buf[CLIENT_BUFSIZE];
	int avail;

	void flushBuffer ();

public:
	void init (InternalClient& c, char* req);

	size_t write (uint8_t c) override;

	void sendReply () override;
};


class NetworkInterfaceWiFi: public NetworkInterface {
private:
	static byte retBuffer[6];

	InternalServer server;
	byte ethernetBuffer[MAX_URL_LEN + 16];			// MAX_URL_LEN + X is enough, since we only store the "GET <url> HTTP/1.x" request line
	unsigned int ethernetBufferSize;

	WebClientWifi webClient;

public:
	NetworkInterfaceWiFi ();

#if defined (WEBBINO_USE_WIFI) || defined (WEBBINO_USE_WIFI101) || \
      defined (WEBBINO_USE_ESP8266_STANDALONE)
	boolean begin (const char *_ssid, const char *_password);
#elif defined (WEBBINO_USE_ESP8266)
	boolean begin (Stream& _serial, const char *_ssid, const char *_password);
#endif

	WebClient* processPacket () override;

	boolean usingDHCP () override;

	byte *getMAC () override;

	IPAddress getIP () override;

	IPAddress getNetmask () override;

	IPAddress getGateway () override;
};

#endif

#endif
