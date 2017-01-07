/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */

#include "WizFi250Server.h"
#include "utility/WizFi250_spi_drv.h"
#include "utility/Debug.h"

WiFiServer::WiFiServer(uint16_t port)
{
	_port = port;
}

void WiFiServer::begin()
{
	LOGDEBUG(F("Starting server"));
	bool status;

	status = wizfi250Drv.startServer(_port);

	if (status)
	{
		LOGINFO1(F("Server started on port"), _port);
	}
	else
	{
		LOGERROR(F("Server failed to start"));
	}
}

WiFiClient WiFiServer::available(byte* status)
{
	if( wizfi250Drv.isServerRun() == false )
	{
		if( wizfi250Drv.startServer(_port) == false )
		{
			wizfi250Drv.stopClient(0);
		}
	}

	int bytes = wizfi250Drv.availData(0);
	if(bytes > 0 && wizfi250Drv.isDataMode())
	{
		LOGINFO(F("New client"));
		WiFiClient client(0);
		return client;
	}

	return WiFiClient(255);
}

uint8_t WiFiServer::status()
{
	return 0;
}

size_t WiFiServer::write(uint8_t b)
{
	return write(&b, 1);
}

size_t WiFiServer::write(const uint8_t *buffer, size_t size)
{
	size_t n = 0;

	for (int sock = 0; sock < MAX_SOCK_NUM; sock++)
	{
		if(WizFi250Class::_state[sock] != 0)
		{
			WiFiClient client(sock);
			n += client.write(buffer, size);
		}
	}

	return n;
}

void WiFiServer::stop()
{
	wizfi250Drv.stopClient(0);
}


