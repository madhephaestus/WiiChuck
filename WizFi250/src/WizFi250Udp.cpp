/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */

#include "WizFi250.h"
#include "WizFi250Udp.h"

#include "utility/WizFi250_spi_drv.h"
#include "utility/debug.h"

/* Constructor */
WiFiUDP::WiFiUDP() : _sock(NO_SOCKET_AVAIL) {}

/* Start WiFiUDP socket, listening at local port PORT */
uint8_t WiFiUDP::begin(uint16_t port)
{
	_sock = 0;
	wizfi250Drv.startServer(port, UDP_MODE);
	return 1;
}

/* return number of bytes available in the current packet,
   will return zero if parsePacket hasn't been called yet */
int WiFiUDP::available()
{
	if (_sock != 255)
	{
		int bytes = wizfi250Drv.availDataUdp(_sock);
		if (bytes>0)
		{
			return bytes;
		}
	}

	return 0;
}

/* Release any resources being used by this WiFiUDP instance */
void WiFiUDP::stop()
{
	wizfi250Drv.stopUDP(0);
}

int WiFiUDP::beginPacket(const char *host, uint16_t port)
{
	_sock = 0;
	if( wizfi250Drv.startClient(host, port, _sock, UDP_MODE) == true)
	{
		_remotePort = port;
		//strcpy(_remoteHost, host);
		WizFi250Class::_state[_sock] = _sock;

		return 1;
	}
	return 0;
}

int WiFiUDP::beginPacket(IPAddress ip, uint16_t port)
{
	char s[18];
	sprintf_P(s, PSTR("%d.%d.%d.%d"), ip[0], ip[1], ip[2], ip[3]);
	return beginPacket(s, port);
}

int WiFiUDP::endPacket()
{
	return 1; 
}

size_t WiFiUDP::write(uint8_t byte)
{
	return write(&byte, 1);
}

size_t WiFiUDP::write(const uint8_t *buffer, size_t size)
{
	int r = wizfi250Drv.SendDataUDP(_sock, buffer, size);

	return r;
}

int WiFiUDP::parsePacket()
{
	return available();
}

int WiFiUDP::read()
{
	uint8_t b;
	wizfi250Drv.getData(_sock,&b);

	if( !available() )
	{
		WizFi250Class::_state[_sock] = NA_STATE;
		_sock = 255;
		return -1;
	}

	return b;
}

int WiFiUDP::read(uint8_t* buf, size_t size)
{
	return wizfi250Drv.getDataBuf(_sock, buf, size, UDP_MODE);
}

int WiFiUDP::peek()
{
	uint8_t b;
	if (!available())
	return -1;

	return b;
}

void WiFiUDP::flush()
{

}


IPAddress  WiFiUDP::remoteIP()
{
	IPAddress ret;
	wizfi250Drv.getRemoteIpAddress(ret);
	return ret;
}

uint16_t  WiFiUDP::remotePort()
{
	return wizfi250Drv.getRemotePort();
}

////////////////////////////////////////////////////////////////////////////////
// Private Methods
////////////////////////////////////////////////////////////////////////////////

uint8_t WiFiUDP::getFirstSocket()
{
	return 0;
}

