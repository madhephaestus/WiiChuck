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

#ifndef _WEBSERVERDIGIFI_H_
#define _WEBSERVERDIGIFI_H_

#include <webbino_config.h>

#ifdef WEBBINO_USE_DIGIFI

#include <DigiFi.h>
#include <WebbinoCore/WebClient.h>
#include <WebbinoCore/WebServer.h>


/* This class implements an interface with the wifi interface embedded on the
 * DigiX board by Digistump (http://digistump.com/products/50).
 *
 * It uses the USR-WIFI232-G wifi chip, which has a number of oddities. The one
 * that strikes us most is the IMPOSSIBILITY to close a TCP connection when in
 * server mode, so we can't tell tell the client this way that data is over.
 * So we must use a different way. The HTTP protocol either allows us to send
 * the content length in advance, using the Content-Length header, or to send
 * chunks of data using what is known as "Chunked transfer encoding". This class
 * currently implements the former, i.e.: it caches the whole data in memory,
 * calculates the content length and adds the relevant header.
 *
 * Note that this wifi chip must be configured through its own web interface,
 * which is available on port 80. This means that port 80 cannot be used for
 * your own server. All network settings (IP, mask, DHCP, SSID, password, etc.)
 * must be configured there. Default user/pass is admin/admin. See
 * https://digistump.com/wiki/digix/tutorials/wifi for help.
 *
 * Data sheet of wifi chip is at
 * http://digistump.com/wiki/_media/digix/tutorials/usr-wifi232-g_en.pdf.
 */
class WebClientDigiFi: public WebClient {
private:
	// Size of the buffer that holds the content. Increase for bigger pages.
	static const unsigned int BUFFER_SIZE = 2048;

	DigiFi& wifi;

	byte buf[BUFFER_SIZE];

	unsigned int bufUsed;

	unsigned int headerLen;

public:
	WebClientDigiFi (DigiFi& wifi);

	void initReply (char* req);

	size_t write (uint8_t c) override;

	void sendReply () override;
};


class NetworkInterfaceDigiFi: public NetworkInterface {
private:
	static const unsigned int REQUEST_TIMEOUT = 15;		// Seconds

	DigiFi wifi;

	byte macAddress[6];
	byte ethernetBuffer[MAX_URL_LEN + 16];			// MAX_URL_LEN + X is enough, since we only store the "GET <url> HTTP/1.x" request line
	unsigned int ethernetBufferSize;

	WebClientDigiFi webClient;

public:
	NetworkInterfaceDigiFi ();

	boolean begin ();

	WebClient* processPacket () override;

	boolean usingDHCP () override;

	byte *getMAC () override;

	IPAddress getIP () override;

	IPAddress getNetmask () override;

	IPAddress getGateway () override;
};

#endif

#endif
