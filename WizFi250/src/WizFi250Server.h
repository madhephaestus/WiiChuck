/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */

#ifndef WIZFI250_WIZFI250SERVER_H_
#define WIZFI250_WIZFI250SERVER_H_

#include "Print.h"
#include "WizFi250.h"
//#include "WizFi250Client.h"

//class WiFiClient;

class WiFiServer : public Print
{

public:
	WiFiServer(uint16_t port);


	/*
	* Gets a client that is connected to the server and has data available for reading.
	* The connection persists when the returned client object goes out of scope; you can close it by calling client.stop().
	* Returns a Client object; if no Client has data available for reading, this object will evaluate to false in an if-statement.
	*/
	WiFiClient available(uint8_t* status = NULL);

	/*
	* Start the TCP server
	*/
	virtual void begin();

	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buf, size_t size);

	uint8_t status();

	using Print::write;

	void stop();


private:
	uint16_t _port;

};

#endif
