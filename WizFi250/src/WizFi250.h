/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */

#ifndef _WIZFI250_H_
#define _WIZFI250_H_

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>
#include <inttypes.h>

#include "WizFi250Client.h"
#include "WizFi250Server.h"
#include "WizFi250Udp.h"

#include "utility/WizFiRingBuffer.h"
#include "utility/WizFi250_spi_drv.h"
#include "utility/Debug.h"


typedef enum WiFiMode
{
    WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3
} WiFiMode_t;


class WizFi250Class
{
public:
	static int16_t _state[MAX_SOCK_NUM];
	static uint16_t _server_port[MAX_SOCK_NUM];


	WizFi250Class();

	void	init	();
	char* 	firmwareVersion();
	uint8_t status	();

	int begin	(char* ssid, const char *passphrase);
	int beginAP	(char* ssid, uint8_t channel, const char* pwd, uint8_t enc);
	int beginAP	(char* ssid);
	int beginAP	(char* ssid, uint8_t channel);
	void config	(IPAddress ip, IPAddress subnet, IPAddress gw);
	void config	();
	int	disconnect	();

	String		macAddress	(void);
	uint8_t*	macAddress	(uint8_t* mac);
	IPAddress	localIP		();
	IPAddress	subnetMask	();
	IPAddress	gatewayIP	();
	char* 		SSID		();
	uint8_t*	BSSID		(uint8_t* bssid);
	int32_t		RSSI		();
	int8_t		scanNetworks();

	bool mode(WiFiMode_t m);

	bool runOTA();

private:

	friend class WizFi250SpiDrv;
};

extern WizFi250Class WiFi;

#endif /* _WIZFI250_H_ */
