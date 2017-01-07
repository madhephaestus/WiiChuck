/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#include <inttypes.h>
#include "WizFi250.h"
#include "WizFi250Client.h"
#include "utility/WizFi250_spi_drv.h"
#include "utility/Debug.h"

WiFiClient::WiFiClient() : _sock(255)
{
}

WiFiClient::WiFiClient(uint8_t sock) : _sock(sock)
{
}

int WiFiClient::connectSSL(const char* host, uint16_t port)
{
	return connect(host, port, SSL_MODE);
}

int WiFiClient::connectSSL(IPAddress ip, uint16_t port)
{
	char s[16];
	sprintf_P(s,PSTR("%d.%d.%d.%d"), ip[0],ip[1],ip[2],ip[3]);
	return connect(s, port, SSL_MODE);
}

int WiFiClient::connect(const char* host, uint16_t port)
{
	return connect(host, port, TCP_MODE);
}

int WiFiClient::connect(IPAddress ip, uint16_t port)
{
	char s[16];
	sprintf_P(s,PSTR("%d.%d.%d.%d"), ip[0],ip[1],ip[2],ip[3]);
	return connect(s, port, TCP_MODE);
}


int WiFiClient::connect(const char* host, uint16_t port, uint8_t protMode)
{
	LOGINFO1(F("Connecting to"), host);

	//_sock = getFirstSocket();
	_sock = 0;

	if (_sock != NO_SOCKET_AVAIL)
	{
		if (!wizfi250Drv.startClient(host, port, _sock, protMode))
			return 0;
		WizFi250Class::_state[_sock] = _sock;
	}
	else
	{
		Serial.println(F("No socket available"));
		return 0;
	}
	return 1;
}

size_t WiFiClient::write(uint8_t b)
{
	return write(&b, 1);
}

size_t WiFiClient::write(const uint8_t *buf, size_t size)
{
	int ret;
	if (_sock >= MAX_SOCK_NUM or size==0)
	{
		setWriteError();
		return 0;
	}

	ret = wizfi250Drv.sendData(_sock, buf, size);
	if(ret == 0)
	{
		setWriteError();
		LOGERROR1(F("Failed to write to socket"), _sock);
		delay(4000);
		stop();
		return 0;
	}

	return ret;
}

int WiFiClient::available()
{
	if (_sock != 255)
	{
		int bytes = wizfi250Drv.availData(_sock);
		if (bytes>0)
		{
			return bytes;
		}
	}

	return 0;
}

int WiFiClient::read()
{
	uint8_t b;
	wizfi250Drv.getData(_sock,&b);

	if( !available() && wizfi250Drv.isDataMode() == false)
	{
		WizFi250Class::_state[_sock] = NA_STATE;
		_sock = 255;
		return -1;
	}

	return b;
}

int WiFiClient::read(uint8_t* buf, size_t size)
{
	if (!available() && wizfi250Drv.isDataMode() == false)
		return -1;
	return wizfi250Drv.getDataBuf(_sock, buf, size);
}

int WiFiClient::peek()
{
	return read();
}

void WiFiClient::flush()
{
	while (available())
		read();
}

void WiFiClient::stop()
{
	wizfi250Drv.stopClient(_sock);
}

uint8_t WiFiClient::connected()
{
	return (status() == ESTABLISHED);
}

WiFiClient::operator bool()
{
	return _sock != 255;
}


uint8_t WiFiClient::status()
{
	if (_sock == 255)
	{
		return CLOSED;
	}

	if ( wizfi250Drv.isDataMode() == true )
	{
		return ESTABLISHED;
	}

	WizFi250Class::_state[_sock] = NA_STATE;
	_sock = 255;

	return CLOSED;
}
