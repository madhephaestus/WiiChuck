/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#include "Debug.h"
#include "WizFi250_spi_drv.h"

#include <avr/pgmspace.h>


char WizFi250SpiDrv::fwVersion[]    = {0};
char WizFi250SpiDrv::_ssid[]		= {0};
uint8_t WizFi250SpiDrv::_bssid[]	= {0};
uint8_t WizFi250SpiDrv::_mac[]      = {0};
uint8_t WizFi250SpiDrv::_localIp[]  = {0};
uint8_t WizFi250SpiDrv::_remoteIP[] = {0};

WizFiRingBuffer WizFi250SpiDrv::ringBuf(CMD_BUFFER_SIZE);

WizFi250SpiDrv::WizFi250SpiDrv()
{
	m_WizFi250_DataReady	= 3;
    m_WizFi250_CS			= 4;
    m_WizFi250_Reset_Pin	= 2;
	
	m_spi_init_flag	= 0;
	m_use_config = 0;
    m_wifi_mode = 0;	// Default STA(Station Mode)
    m_is_datamode = 0;
    m_is_server_run = 0;
    _localPort = 5000;	// Default localPort
    _Current_ESC_State = 0;
    _remotePort = 0;
    _recvedLen = 0;
}

void WizFi250SpiDrv::wifiDriverInit()
{

	if( m_spi_init_flag == 0 ){
		pinMode(m_WizFi250_DataReady, INPUT);
		pinMode(m_WizFi250_CS, OUTPUT);
		pinMode(m_WizFi250_Reset_Pin, OUTPUT);
#if defined(ARDUINO_ARCH_SAMD)
		initSS();
#endif
		SPI.begin();
		SPI.setClockDivider(SPI_CLOCK_DIV4);	// Max Clock Frequency
		m_spi_init_flag = 1;
	}

	hwReset();

	wizfi250Drv.sendCmd(F("AT\r"));
	wizfi250Drv.sendCmd(F("AT+MECHO=0\r"));
}

void WizFi250SpiDrv::hwReset()
{
	digitalWrite(m_WizFi250_Reset_Pin, LOW);
	delay(500);
	digitalWrite(m_WizFi250_Reset_Pin, HIGH);

	delay(1000);
}

uint8_t WizFi250SpiDrv::wizspi_byte(uint8_t byte, uint8_t issue_cs, uint16_t delay1, uint16_t delay2)
{
	uint8_t spi_data;

	if ( issue_cs != 0 )	digitalWrite(m_WizFi250_CS, LOW);
	if ( delay1 > 0 )		delayMicroseconds(delay1 * 10);
	spi_data = SPI.transfer(byte);
	if ( delay2 > 0 )		delayMicroseconds(delay2 * 10);
	if ( issue_cs != 0 )	digitalWrite(m_WizFi250_CS, HIGH);

	return spi_data;
}

void WizFi250SpiDrv::process_esc_code	(uint8_t *spi_recv, uint8_t *valid_data)
{
	static uint8_t esc_mode = 0;
	static uint8_t esc2_mode = 0;
	uint8_t spi2_recv = 0;

	(*valid_data) = 0;

	if( (*spi_recv) == SPI_NULL )		{ return; }
	if( (*spi_recv) == SPI_ESC )		{ esc_mode = 1; return; }

	if( esc_mode == 1 )
	{
		esc_mode = 0;
		switch((*spi_recv))
		{
			case SPI_F0:					// Press 'F0' to upper
				(*valid_data) = 1;
				(*spi_recv) = SPI_NULL;
				break;
			case SPI_F1:					// Press 'F1' to upper
				(*valid_data) = 1;
				(*spi_recv) = SPI_ESC;
				break;
			case SPI_SYNC:					// Handle Sync Signal
				(*spi_recv) = wizspi_byte((uint8_t)SPI_ESC, 1, 1, 1);
				if ( (*spi_recv) != SPI_NULL )
				{
					LOGDEBUG0(F("DBG>>>>Need SPI_NULL"));
				}
				(*spi_recv) = wizspi_byte((uint8_t)SPI_SYNC, 1, 1, 1);
				break;
			case SPI_XON:					// Handle Oon Signal
				break;
			case SPI_XOFF:					// Handle Xoff Signal
				{
					uint8_t found_xon = 0;
					while(1)
					{
						delay(10);
						spi2_recv = wizspi_byte((uint8_t)SPI_NULL, 1, 1, 1);
						if ( spi2_recv == SPI_NULL )	{ continue; }
						if ( spi2_recv == SPI_ESC )		{ esc2_mode = 1;	return;	}

						if ( esc2_mode == 1 )
						{
							esc2_mode = 0;
							switch(spi2_recv)
							{
								case SPI_XON:	// Handle Xon Signal
									found_xon = 1;
									break;
								default:
									break;
							}
						}

						if ( found_xon == 1 )		break;
					}
				}
				break;
			case SPI_ERR:						// Handle Error Signal
				break;
			case SPI_ESC:						// Lost ESC Data
				esc_mode = 1;					// Just Ignore previous ESC
				break;
			default:
				break;
		}
	}
	else
	{
		(*valid_data) = 1;
	}
}

void WizFi250SpiDrv::spi_senddata(uint8_t *send_data, uint32_t send_length)
{
	uint32_t i = 0;
	uint8_t	 spi_recv_byte;
	uint8_t	 spi_valid_data = 0;

	if ( send_length > CMD_BUFFER_SIZE )
	{
		LOGDEBUG0(F("DBG>>>>Error : send-data-size if too big"));
		return;
	}

	for ( i=0; i<send_length; i++ )
	{
		spi_valid_data = 0;
		spi_recv_byte = wizspi_byte((uint8_t)send_data[i], 1, 1, 1);

		process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
		if ( spi_valid_data == 1 )
		{
			ringBuf.write(spi_recv_byte);
			LOGDEBUG0((char)spi_recv_byte);
		}
	}

	while(1)
	{
		if ( digitalRead(m_WizFi250_DataReady) == HIGH )
		{
			spi_valid_data=0;
			spi_recv_byte = wizspi_byte((uint8_t)SPI_NULL, 1, 1, 1);
			process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
			if ( spi_valid_data == 1 )
			{
				ringBuf.write(spi_recv_byte);
				LOGDEBUG0((char)spi_recv_byte);
			}
		}
		else
			break;
	}
}


int WizFi250SpiDrv::getResponse(char* outStr, int outStrLen, int lineNum)
{
	int i,ret=0;

	for(i=0;i<lineNum;i++)
	{
		memset(outStr,0,outStrLen);
		ret = ringBuf.getLine(outStr,outStrLen,'\r',1);
	}

	return ret;
}

int WizFi250SpiDrv::sendCmd(const __FlashStringHelper* cmd, int timeout, ...)
{
	char cmdBuf[CMD_BUFFER_SIZE];

	va_list args;
	va_start (args, timeout);
	
#ifdef __AVR__
	vsnprintf_P (cmdBuf, CMD_BUFFER_SIZE, (char*)cmd, args);
#else
	vsnprintf (cmdBuf, CMD_BUFFER_SIZE, (char*)cmd, args);
#endif

	va_end (args);

	wizfiEmptyBuf(true);
	ringBuf.reset();

	LOGDEBUG(F("----------------------------------------------"));
	LOGDEBUG1(F(">>"), cmdBuf);

	spi_senddata((uint8_t*)cmdBuf, strlen(cmdBuf));
	int idx = readUntil(timeout);

	LOGDEBUG1(F("---------------------------------------------- >"), idx);
	LOGDEBUG();

	return idx;
}

int WizFi250SpiDrv::SendCmdWithTag(const __FlashStringHelper* cmd, const char* tag, const char* tag2, int timeout, ...)
{
	char cmdBuf[CMD_BUFFER_SIZE];

	va_list args;
	va_start (args, timeout);
	
#ifdef __AVR__
	vsnprintf_P (cmdBuf, CMD_BUFFER_SIZE, (char*)cmd, args);
#else
	vsnprintf (cmdBuf, CMD_BUFFER_SIZE, (char*)cmd, args);
#endif

	va_end (args);

	wizfiEmptyBuf(true);
	ringBuf.reset();

	LOGDEBUG(F("----------------------------------------------"));
	LOGDEBUG1(F(">>"), cmdBuf);

	spi_senddata((uint8_t*)cmdBuf, strlen(cmdBuf));
	int idx = readUntil(timeout, tag, tag2);

	LOGDEBUG1(F("---------------------------------------------- >"), idx);
	LOGDEBUG();

	return idx;
}

int WizFi250SpiDrv::readUntil(int timeout, const char* tag, const char* tag2)
{
	uint8_t	 spi_recv_byte, is_found1=0, is_found2=0;
	uint8_t	 spi_valid_data = 0;
	unsigned long start = millis();
	int ret = -1;

	while ( (millis() - start < (unsigned long)timeout) and ret < 0 )
	{
		if ( digitalRead(m_WizFi250_DataReady) == HIGH )
		{
			spi_recv_byte = wizspi_byte((uint8_t)SPI_NULL, 1, 1, 1);
			process_esc_code(&spi_recv_byte, &spi_valid_data);
			if ( spi_valid_data == 1 )
			{
				ringBuf.write(spi_recv_byte);
				LOGDEBUG0((char)spi_recv_byte);
			}
		}
		else
		{
			if( is_found1 == 0 && ringBuf.FindStr(tag))
			{
				is_found1 = 1;
			}
			if( is_found2 == 0 && ringBuf.FindStr(tag2))
			{
				is_found2 = 1;
			}
		}

		if ( is_found1 && is_found2 )
		{
			ret = 0;
			break;
		}
	}
	if (millis() - start >= (unsigned long)timeout)
	{
		LOGWARN(F(">>> TIMEOUT >>>"));
	}

	return ret;
}

void WizFi250SpiDrv::wizfiEmptyBuf(bool warn)
{
	int i=0;
	uint8_t	 spi_recv_byte;
	uint8_t	 spi_valid_data = 0;

	while(1)
	{
		if ( digitalRead(m_WizFi250_DataReady) == HIGH )
		{
			spi_recv_byte = wizspi_byte((uint8_t)SPI_NULL, 1, 1, 1);
			process_esc_code(&spi_recv_byte, &spi_valid_data);
			if ( spi_valid_data == 1 )
			{
				if(warn == true)
					LOGDEBUG_TYPE((char)spi_recv_byte,HEX);
					LOGDEBUG0(" ");
				i++;
			}
		}
		else
			break;
	}
	if (i>0 && warn == true)
	{
		LOGDEBUG(F(""));
		LOGDEBUG1(F("Dirty characters in the serial buffer! >"), i);

	}
}

bool WizFi250SpiDrv::wifiConnect(char* ssid, const char *passphrase)
{
	LOGDEBUG(F("> wifiConnect"));

	if( sendCmd(F("AT+WSET=0,%s\r"),1000, ssid) == -1 )	return false;
	if( sendCmd(F("AT+WSEC=0,,%s\r"),1000, passphrase) == -1 )	return false;
	if( !m_use_config ){
		if( sendCmd(F("AT+WNET=1\r"), 1000) == -1 )	return false;
	}
	if( sendCmd(F("AT+WJOIN\r"),30000 ) == -1 )	return false;

	LOGINFO1(F("Connected to"), ssid);
	return true;
}

bool WizFi250SpiDrv::wifiStartAP(char* ssid, const char *pwd, uint8_t channel, uint8_t enc )
{
	LOGDEBUG(F("> wifiStartAP"));
	char ch_enc[5]={0,};
	if		( enc == 0 ) strcpy(ch_enc,"OPEN");
	else if	( enc == 1 ) strcpy(ch_enc,"WEP");
	else if ( enc == 2 ) strcpy(ch_enc,"WPA");
	else if ( enc == 3 || enc == 4 ) strcpy(ch_enc,"WPA2");

	if( sendCmd(F("AT+WSET=1,%s\r"),1000, ssid) == -1 )	return false;
	if( sendCmd(F("AT+WSEC=1,%s,%s\r"),1000, ch_enc,pwd) == -1 )	return false;
	if( sendCmd(F("AT+WJOIN\r"),10000 ) == -1 )	return false;

	LOGINFO1(F("Access point started"), ssid);
	return true;
}

uint8_t WizFi250SpiDrv::disconnect()
{
	LOGDEBUG(F("> disconnect"));

	while( sendCmd(F("AT+WLEAVE\r") ) == -1 )
	{
		delay(2000);
		wizfiEmptyBuf(true);
	}

	return WL_DISCONNECTED;
}

void WizFi250SpiDrv::config(IPAddress ip, IPAddress subnet, IPAddress gw)
{
	LOGDEBUG(F("> config(IP,Subnet,Gateway)"));
	char buf_ip[16];
	char buf_sub[16];
	char buf_gw[16];

	sprintf_P(buf_ip, PSTR("%d.%d.%d.%d"),ip[0], ip[1], ip[2], ip[3] );
	sprintf_P(buf_sub, PSTR("%d.%d.%d.%d"), subnet[0], subnet[1], subnet[2], subnet[3] );
	sprintf_P(buf_gw, PSTR("%d.%d.%d.%d"), gw[0], gw[1], gw[2], gw[3] );
	
	if( sendCmd(F("AT+WNET=0,%s,%s,%s\r"), 1000, buf_ip, buf_sub, buf_gw) != -1)
		m_use_config = 1;
}

void WizFi250SpiDrv::config()
{
	LOGDEBUG(F("> config(DHCP)"));
	sendCmd(F("AT+WNET=1\r"), 1000);
}

uint8_t* WizFi250SpiDrv::getMacAddress()
{
	LOGDEBUG(F("> getMacAddress"));

	memset(_mac, 0, WL_MAC_ADDR_LENGTH);
	char buf[20];
	if( sendCmd(F("AT+MMAC=?\r")) != -1 )
	{
		char* token;
		if( getResponse(buf, sizeof(buf), 1) > 0)
		{
			token = strtok(buf, ":");
			_mac[0] = (byte)strtol(token, NULL, 16);
			token = strtok(NULL, ":");
			_mac[1] = (byte)strtol(token, NULL, 16);
			token = strtok(NULL, ":");
			_mac[2] = (byte)strtol(token, NULL, 16);
			token = strtok(NULL, ":");
			_mac[3] = (byte)strtol(token, NULL, 16);
			token = strtok(NULL, ":");
			_mac[4] = (byte)strtol(token, NULL, 16);
			token = strtok(NULL, ":");
			_mac[5] = (byte)strtol(token, NULL, 16);
		}
	}

	return _mac;
}

void WizFi250SpiDrv::getIpAddress(IPAddress& ip)
{
	char buff[CMD_BUFFER_SIZE];
	char *token;
	char *p_ip;

	sendCmd(F("AT+WSTATUS\r"));
	getResponse(buff, sizeof(buff), 2);

	if( strstr(buff, "STA") || strstr(buff,"AP") )
	{
		// Parsing IP-Addr ( IF/SSID/IP-Addr/Gateway/MAC/TxPower(dBm)/RSSI(-dBm) )
		token = strtok(buff, "/");	token = strtok(NULL, "/");
		token = strtok(NULL, "/");

		p_ip = strtok(token,".");	_localIp[0] = atoi(p_ip);
		p_ip = strtok(NULL,".");	_localIp[1] = atoi(p_ip);
		p_ip = strtok(NULL,".");	_localIp[2] = atoi(p_ip);
		p_ip = strtok(NULL,".");	_localIp[3] = atoi(p_ip);
	}
	else
	{
		sendCmd(F("AT+WNET=?\r"));
		if( getResponse(buff, sizeof(buff), 1) > 0)
		{
			token = strtok(buff,",");	token = strtok(NULL,",");

			p_ip = strtok(token,".");	_localIp[0] = atoi(p_ip);
			p_ip = strtok(NULL,".");	_localIp[1] = atoi(p_ip);
			p_ip = strtok(NULL,".");	_localIp[2] = atoi(p_ip);
			p_ip = strtok(NULL,".");	_localIp[3] = atoi(p_ip);
		}
	}

	ip = _localIp;
}

void WizFi250SpiDrv::getNetmask(IPAddress& mask)
{
	char buff[CMD_BUFFER_SIZE];
	char *token;
	char *p_ip;

	sendCmd(F("AT+WNET=?\r"));
	if( getResponse(buff, sizeof(buff), 1) > 0)
	{
		token = strtok(buff,",");	token = strtok(NULL,",");
		token = strtok(NULL,",");

		p_ip = strtok(token,".");	mask[0] = atoi(p_ip);
		p_ip = strtok(NULL,".");	mask[1] = atoi(p_ip);
		p_ip = strtok(NULL,".");	mask[2] = atoi(p_ip);
		p_ip = strtok(NULL,".");	mask[3] = atoi(p_ip);
	}
	else
	{
		mask[0] = 0;	mask[1] = 0;
		mask[2] = 0;	mask[3] = 0;
	}
}

void WizFi250SpiDrv::getGateway(IPAddress& gw)
{
	char buff[CMD_BUFFER_SIZE];
	char *token;
	char *p_ip;

	sendCmd(F("AT+WSTATUS\r"));
	getResponse(buff, sizeof(buff), 2);

	if( strstr(buff, "STA") || strstr(buff,"AP") )
	{
		// Parsing Gateway ( IF/SSID/IP-Addr/Gateway/MAC/TxPower(dBm)/RSSI(-dBm) )
		token = strtok(buff, "/");	token = strtok(NULL, "/");
		token = strtok(NULL, "/");	token = strtok(NULL, "/");

		p_ip = strtok(token,".");	gw[0] = atoi(p_ip);
		p_ip = strtok(NULL,".");	gw[1] = atoi(p_ip);
		p_ip = strtok(NULL,".");	gw[2] = atoi(p_ip);
		p_ip = strtok(NULL,".");	gw[3] = atoi(p_ip);
	}
	else
	{
		sendCmd(F("AT+WNET=?\r"));
		if( getResponse(buff, sizeof(buff), 1) > 0)
		{
			token = strtok(buff,",");	token = strtok(NULL,",");
			token = strtok(NULL,",");	token = strtok(NULL,",");

			p_ip = strtok(token,".");	gw[0] = atoi(p_ip);
			p_ip = strtok(NULL,".");	gw[1] = atoi(p_ip);
			p_ip = strtok(NULL,".");	gw[2] = atoi(p_ip);
			p_ip = strtok(NULL,".");	gw[3] = atoi(p_ip);
		}
		else
		{
			gw[0] = 0;	gw[1] = 0;
			gw[2] = 0;	gw[3] = 0;
		}
	}
}

char* WizFi250SpiDrv::getCurrentSSID()
{
	LOGDEBUG(F("> getCurrentSSID"));
	char buff[CMD_BUFFER_SIZE];
	char *token;

	memset(_ssid, 0, WL_SSID_MAX_LENGTH);

	sendCmd(F("AT+WSET=?\r"));
	if( getResponse(buff, sizeof(buff), 1) > 0)
	{
		token = strtok(buff,",");	token = strtok(NULL,",");
		memcpy(_ssid, token, WL_SSID_MAX_LENGTH);
	}

	return _ssid;
}

uint8_t* WizFi250SpiDrv::getCurrentBSSID()
{
	LOGDEBUG(F("> getCurrentBSSID"));
	char buff[CMD_BUFFER_SIZE];
	char *token;
	char *p_bssid;

	memset(_bssid, 0, WL_MAC_ADDR_LENGTH);

	sendCmd(F("AT+WSET=?\r"));
	if( getResponse(buff, sizeof(buff), 1) > 0)
	{
		token = strtok(buff,",");	token = strtok(NULL,",");
		token = strtok(NULL,",");

		p_bssid = strtok(token, ":");
		_bssid[5] = (byte)strtol(p_bssid, NULL, 16);
		token = strtok(NULL, ":");
		_bssid[4] = (byte)strtol(p_bssid, NULL, 16);
		token = strtok(NULL, ":");
		_bssid[3] = (byte)strtol(p_bssid, NULL, 16);
		token = strtok(NULL, ":");
		_bssid[2] = (byte)strtol(p_bssid, NULL, 16);
		token = strtok(NULL, ":");
		_bssid[1] = (byte)strtol(p_bssid, NULL, 16);
		token = strtok(NULL, ":");
		_bssid[0] = (byte)strtol(p_bssid, NULL, 16);
	}

	return _bssid;
}

int32_t WizFi250SpiDrv::getCurrentRSSI()
{
	LOGDEBUG(F("> getCurrentRSSI"));
	char buff[CMD_BUFFER_SIZE];
	char *token;
	int ret=0;

	sendCmd(F("AT+WSTATUS\r"));
	getResponse(buff, sizeof(buff), 2);

	if( strstr(buff, "STA") || strstr(buff,"AP") )
	{
		// Parsing Gateway ( IF/SSID/IP-Addr/Gateway/MAC/TxPower(dBm)/RSSI(-dBm) )
		token = strtok(buff, "/");	token = strtok(NULL, "/");
		token = strtok(NULL, "/");	token = strtok(NULL, "/");
		token = strtok(NULL, "/");	token = strtok(NULL, "/");
		token = strtok(NULL, "/");

		ret = -atoi(token);
	}

	return ret;
}

uint8_t WizFi250SpiDrv::getScanNetworks()
{
	//sendCmd(F("AT+WSCAN\r"),30000);
	return 0;
}

char* WizFi250SpiDrv::firmwareVersion()
{
	sendCmd(F("AT+MINFO\r"));
	getResponse(fwVersion, sizeof(fwVersion), 2);

	return fwVersion;
}

uint8_t WizFi250SpiDrv::getConnectionStatus ()
{
	char buff[CMD_BUFFER_SIZE];

	if( m_is_datamode == true )
	{
		return WL_CONNECTED;
	}

	if( sendCmd(F("AT+WSTATUS\r")) == -1 )
		return WL_NO_SHIELD;

	getResponse(buff, sizeof(buff), 2);
	LOGDEBUG(buff);

	if( strstr(buff, "Down") )
		return WL_DISCONNECTED;
	if( strstr(buff, "STA") || strstr(buff, "AP") )
		return WL_CONNECTED;

	return WL_IDLE_STATUS;
}


////////////////////////////////////////////////////////////
// TCP/IP
////////////////////////////////////////////////////////////
void WizFi250SpiDrv::stopClient(uint8_t sock)
{
	if( m_is_datamode == true && ringBuf.available())
		return;

	if( m_is_datamode == true )
	{
		sendCmd(F("+++"),2000);
		if( sendCmd(F("AT+SMGMT=ALL\r")) == -1)
		{
			sendCmd(F("AT\r"));
			sendCmd(F("AT+SMGMT=ALL\r"));
		}

		m_is_datamode = false;
		m_is_server_run = false;
	}
	else
	{
		if( sendCmd(F("AT\r")) == -1)
		{
			sendCmd(F("+++"),2000);
			sendCmd(F("AT+SMGMT=ALL\r"));
			m_is_datamode = false;
			m_is_server_run = false;
		}
		else
		{
			sendCmd(F("AT+SMGMT=ALL\r"));
		}
	}
}

void WizFi250SpiDrv::stopUDP(uint8_t sock)
{
	m_is_datamode = false;
	m_is_server_run = false;

	sendCmd(F("AT+SMGMT=ALL\r"));
}

uint16_t WizFi250SpiDrv::availData(uint8_t connId)
{
	uint8_t	 spi_recv_byte=0;
	uint8_t	 spi_valid_data = 0;

	if( ringBuf.available() <= CMD_BUFFER_SIZE - 50)
	{
		if ( digitalRead(m_WizFi250_DataReady) == HIGH )
		{
			spi_valid_data=0;
			spi_recv_byte = wizspi_byte((uint8_t)SPI_NULL, 1, 1, 1);
			process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
			if ( spi_valid_data == 1 )
			{
				ringBuf.write(spi_recv_byte);
			}
		}
	}

	//if( ringBuf.FindStr("[DISCONNECT 0]") )
	if( ringBuf.endsWith("[DISCONNECT 0]\r\n"))
	{
		m_is_datamode = false;
		m_is_server_run = false;
	}

	//if( ringBuf.FindStr("[CONNECT "))
	if( ringBuf.endsWith("[CONNECT 0]\r\n"))
	{
		m_is_datamode = true;
	}

	return ringBuf.available();
}


static uint8_t tempIP[18]={0,};
static uint8_t tempIP_idx=0;


void WizFi250SpiDrv::parsingUDPData(uint8_t recv_data)
{
	switch(_Current_ESC_State)
	{
	case ESC_IDLE:
		_recvedLen = 0;
		tempIP_idx = 0;
		if(recv_data == '{')
		{
			_Current_ESC_State = ESC_CID;
		}
		break;
	case ESC_CID:
		if( (recv_data >= '0') && (recv_data <= '9') )
		{

		}
		else if( recv_data == ',')
		{
			_Current_ESC_State = ESC_PEERIP;
		}
		else
		{
			_Current_ESC_State = ESC_IDLE;
		}
		break;
	case ESC_PEERIP:
		if( ((recv_data >= '0') && (recv_data <= '9')) || (recv_data == '.'))
		{
			tempIP[tempIP_idx++] = recv_data;
		}
		else if(recv_data == ',')
		{
			char *token;
			token = strtok((char*)tempIP,".");	_remoteIP[0] = atoi(token);
			token = strtok(NULL,".");	_remoteIP[1] = atoi(token);
			token = strtok(NULL,".");	_remoteIP[2] = atoi(token);
			token = strtok(NULL,".");	_remoteIP[3] = atoi(token);
			_Current_ESC_State = ESC_PEERPORT;
			tempIP_idx = 0;
			_remotePort = 0;
		}
		else
		{
			_Current_ESC_State = ESC_IDLE;
		}
		break;
	case ESC_PEERPORT:
		if((recv_data >= '0') && (recv_data <= '9'))
		{
			_remotePort *= 10;
			_remotePort += (uint16_t)(recv_data - '0');
		}
		else if(recv_data == ',')
		{
			_Current_ESC_State = ESC_LENGTH;
		}
		else _Current_ESC_State = ESC_IDLE;
		break;
	case ESC_LENGTH:
		if((recv_data >= '0') && (recv_data <= '9'))
		{
			_recvedLen *= 10;
			_recvedLen += (uint16_t)(recv_data - '0');
		}
		else if(recv_data == '}')
		{
			_Current_ESC_State = ESC_RECV_DATA;
		}
		else _Current_ESC_State = ESC_IDLE;
		break;
	case ESC_RECV_DATA:
		ringBuf.write(recv_data);
		_recvedLen--;

		if(_recvedLen < 0)
		{
			_recvedLen = 0;
			_Current_ESC_State = ESC_IDLE;
		}
		break;
	}

}

int WizFi250SpiDrv::availDataUdp(uint8_t connId)
{
	uint8_t	 spi_recv_byte=0;
	uint8_t	 spi_valid_data = 0;

	//if ( digitalRead(m_WizFi250_DataReady) == HIGH )
	do
	{
		spi_recv_byte = wizspi_byte((uint8_t)SPI_NULL, 1, 1, 1);
		process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
		if( spi_valid_data == 1)
		{
			parsingUDPData(spi_recv_byte);
			if( ringBuf.isFull() )	return ringBuf.available();
		}

	}while(digitalRead(m_WizFi250_DataReady) == HIGH);

	return ringBuf.available();
}

bool WizFi250SpiDrv::getData(uint8_t connId, uint8_t *data)
{
//	if( !m_is_datamode )
//		return false;
	long _startMillis = millis();
	do
	{
		if( ringBuf.available() )
		{
			*data = (uint8_t)ringBuf.read();
			return true;
		}
	}while((millis() - _startMillis) < 2000);

    // timed out, reset the buffer
	LOGERROR(F("TIMEOUT"));
	*data = 0;

	return false;
}

int WizFi250SpiDrv::getDataBuf(uint8_t connId, uint8_t *buf, uint16_t bufSize, uint8_t protMode)
{
	uint16_t i,idx=0;
	int recv_data=0;


	if( !m_is_datamode )
		return -1;

	if(protMode == TCP_MODE)
	{
		for(i=0; i<bufSize; i++)
		{
			if( availData(connId) )
			{
				recv_data = ringBuf.read();
				if( recv_data != -1 )
				{
					buf[idx] = (uint8_t)recv_data;
					idx++;
				}
			}
			else
				return idx;
		}
	}
	else if(protMode == UDP_MODE)
	{
		for(i=0; i<bufSize; i++)
		{
			if( availDataUdp(connId) )
			{
				recv_data = ringBuf.read();
				if( recv_data != -1 )
				{
					buf[idx] = (uint8_t)recv_data;
					idx++;
				}
			}
			else
				return idx;
		}
	}

	return idx;
}

bool WizFi250SpiDrv::startServer(uint16_t port, uint8_t protMode)
{
	int ret=0;
	LOGDEBUG1(F("> startServer"), port);

	if(protMode == TCP_MODE)
	{
		int ret = sendCmd(F("AT+SCON=O,TSN,,,%d,1\r"),1000,port);
		if(ret == 0)
		{
			m_is_server_run = true;
			wizfiEmptyBuf(true);
			ringBuf.reset();
			setLocalPort(port);
		}
	}
	else
	{
		sendCmd(F("AT+SMGMT=ALL\r"));
		ret = SendCmdWithTag(F("AT+SCON=O,USN,,,%d,0\r"),(char*)"[OK]",(char*)"[CONNECT",10000,port);
		if(ret == 0)
		{
			m_is_datamode = true;
			ringBuf.reset();
			setLocalPort(port);
		}
		else
		{
			m_is_datamode = false;
		}
	}

	return ret == 0;
}

bool WizFi250SpiDrv::startClient(const char* host, uint16_t port, uint8_t sock, uint8_t protMode)
{
	LOGDEBUG2(F("> startClient"), host, port);
	int status=0;
	bool is_ip,ret=false;
	char host_ip[16]={0,};
	char resp_ok[5] = "[OK]";
	char resp_con[9] = "[CONNECT";
	IPAddress ip;

	is_ip = WXParse_Ip((uint8_t*)host,ip);
	if( is_ip == true )
	{
		sprintf_P(host_ip,PSTR("%d.%d.%d.%d"),ip[0],ip[1],ip[2],ip[3]);
	}
	else
	{
		if( sendCmd(F("AT+FDNS=%s,%d\r"),3000,host,3000) == -1)
		{
			m_is_datamode = false;
			return false;
		}

		getResponse(host_ip, sizeof(host_ip), 1);
	}

	if(protMode == TCP_MODE)
	{
		sendCmd(F("AT+SMGMT=ALL\r"));
		status = SendCmdWithTag(F("AT+SCON=O,TCN,%s,%d,,1\r"),resp_ok,resp_con,10000,host_ip,port);
		ringBuf.reset();
	}
	else if(protMode == SSL_MODE)
	{
		sendCmd(F("AT+SMGMT=ALL\r"));
		status = SendCmdWithTag(F("AT+SCON=O,TCS,%s,%d,,1\r"),resp_ok,resp_con,10000,host_ip,port);
		ringBuf.reset();
	}
	else if(protMode == UDP_MODE)
	{
		sendCmd(F("AT+SMGMT=ALL\r"));
		status = SendCmdWithTag(F("AT+SCON=O,UCN,%s,%d,%d,0\r"),resp_ok,resp_con,10000,host_ip,port,_localPort);
		ringBuf.reset();
	}

	if( status == 0 )
	{
		m_is_datamode = true;
		ret = true;
	}
	else
	{
		m_is_datamode = false;
		ret = false;
	}

	return ret;
}

int WizFi250SpiDrv::sendData(uint8_t sock, const uint8_t *data, uint16_t len)
{
	LOGDEBUG(F(">Send Data"));
	int i;
	uint8_t spi_valid_data=0, spi_recv_byte=0;

	if( len > CMD_BUFFER_SIZE )
	{
		LOGDEBUG0(F("DBG>>>>Error : send-data-size if too big"));
		return 0;
	}

	for( i=0; i<len; i++ )
	{
//		if( ringBuf.available() == CMD_BUFFER_SIZE )	break;
//
		//LOGDEBUG_TYPE((char)data[i],HEX);
		if( isprint((char)data[i]) || data[i] == 0x0d || data[i] == 0x0a )
		{
			LOGDEBUG0((char)data[i]);
		}
		else
		{
			LOGDEBUG0(F("{0x"));
			LOGDEBUG_TYPE(data[i], HEX);
			LOGDEBUG0(F("}"));
		}

		spi_valid_data = 0;
		spi_recv_byte = wizspi_byte((uint8_t)data[i], 1, 1, 1);
		process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
		if( spi_valid_data == 1 )
		{
			ringBuf.write(spi_recv_byte);
			//LOGDEBUG0((char)spi_recv_byte);
		}
	}
	LOGDEBUG(F(""));

	return i;
}

int WizFi250SpiDrv::SendDataUDP(uint8_t sock, const uint8_t *data, uint16_t len)
{
	LOGDEBUG(F(">Send Data(UDP)"));
	uint8_t spi_valid_data=0, spi_recv_byte=0;
	int i = 0;

	if( len > CMD_BUFFER_SIZE )
	{
		LOGDEBUG0(F("DBG>>>>Error : send-data-size if too big"));
		return 0;
	}

	char cmdBuf[100]={0};
	sprintf_P(cmdBuf, PSTR("AT+SSEND=%d,,,%d\r"), sock, len);

	for(int i=0; i<strlen(cmdBuf); i++)
	{
		spi_valid_data = 0;
		spi_recv_byte = wizspi_byte((uint8_t)cmdBuf[i], 1, 1, 1);
		process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
		if( spi_valid_data == 1 )
		{
			parsingUDPData(spi_recv_byte);
		}
	}

	for(i=0; i<(int)len; i++)
	{
		spi_valid_data = 0;
		spi_recv_byte = wizspi_byte((uint8_t)data[i], 1, 1, 1);
		process_esc_code((uint8_t*)&spi_recv_byte, &spi_valid_data);
		if( spi_valid_data == 1 )
		{
			parsingUDPData(spi_recv_byte);
		}
	}
	LOGDEBUG(F(""));

	return i;
}


bool WizFi250SpiDrv::isDataMode()
{
	return m_is_datamode;
}

bool WizFi250SpiDrv::isServerRun()
{
	return m_is_server_run;
}

void WizFi250SpiDrv::setLocalPort(uint16_t port)
{
	_localPort = port;
}

void WizFi250SpiDrv::getRemoteIpAddress(IPAddress& ip)
{
	ip = _remoteIP;
}

uint16_t WizFi250SpiDrv::getRemotePort()
{
	return _remotePort;
}



WizFi250SpiDrv wizfi250Drv;

