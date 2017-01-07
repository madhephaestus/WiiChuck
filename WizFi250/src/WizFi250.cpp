/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include "WizFi250.h"

#include <avr/pgmspace.h>

int16_t 	WizFi250Class::_state[MAX_SOCK_NUM] = { NA_STATE };
uint16_t 	WizFi250Class::_server_port[MAX_SOCK_NUM] = {0};


WizFi250Class::WizFi250Class()
{
}

void WizFi250Class::init()
{
	LOGINFO(F("Initializing WizFi250"));
	wizfi250Drv.wifiDriverInit();
}

char* WizFi250Class::firmwareVersion()
{
	return wizfi250Drv.firmwareVersion();
}

int WizFi250Class::begin(char* ssid, const char *passphrase)
{
	if( wizfi250Drv.wifiConnect(ssid, passphrase) )
		return WL_CONNECTED;

	return WL_CONNECT_FAILED;
}

int WizFi250Class::beginAP(char* ssid, uint8_t channel, const char* pwd, uint8_t enc)
{
	if( wizfi250Drv.wifiStartAP(ssid, pwd, channel, enc) )
		return WL_CONNECTED;

	return WL_CONNECT_FAILED;
}

int WizFi250Class::beginAP(char* ssid)
{
	return beginAP(ssid, 10, "", 0);
}

int WizFi250Class::beginAP(char* ssid, uint8_t channel)
{
	return beginAP(ssid, channel, "", 0);
}

void WizFi250Class::config(IPAddress ip, IPAddress subnet, IPAddress gw)
{
	wizfi250Drv.config(ip,subnet,gw);
}

void WizFi250Class::config()
{
	wizfi250Drv.config();
}

int WizFi250Class::disconnect()
{
	return wizfi250Drv.disconnect();
}

String WizFi250Class::macAddress(void)
{
	uint8_t mac[6];
	char macStr[18] = {0};

	macAddress(mac);

	sprintf_P(macStr, PSTR("%02X:%02X:%02X:%02X:%02X:%02X"),mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return String(macStr);
}

uint8_t* WizFi250Class::macAddress(uint8_t* mac)
{
	uint8_t* _mac = wizfi250Drv.getMacAddress();
	memcpy(mac, _mac, WL_MAC_ADDR_LENGTH);
	return mac;
}

IPAddress WizFi250Class::localIP()
{
	IPAddress ret;
	wizfi250Drv.getIpAddress(ret);
	return ret;
}

IPAddress WizFi250Class::subnetMask()
{
	IPAddress mask;
	wizfi250Drv.getNetmask(mask);
	return mask;
}

IPAddress WizFi250Class::gatewayIP()
{
	IPAddress gw;
	wizfi250Drv.getGateway(gw);
	return gw;
}

char* WizFi250Class::SSID()
{
	return wizfi250Drv.getCurrentSSID();
}

uint8_t* WizFi250Class::BSSID(uint8_t* bssid)
{
	return wizfi250Drv.getCurrentBSSID();
}

int32_t WizFi250Class::RSSI()
{
	return wizfi250Drv.getCurrentRSSI();
}

int8_t WizFi250Class::scanNetworks()
{
	return wizfi250Drv.getScanNetworks();
}

uint8_t WizFi250Class::status()
{
	return wizfi250Drv.getConnectionStatus();
}

bool WizFi250Class::mode(WiFiMode_t m)
{
	return true;
}

bool WizFi250Class::runOTA()
{
	if( wizfi250Drv.sendCmd(F("AT+FOTA\r")) == -1 ) return false;
	return true;
}


WizFi250Class WiFi;

