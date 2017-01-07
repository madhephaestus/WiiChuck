/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#ifndef WIZFI250_SPI_DRV_H_
#define WIZFI250_SPI_DRV_H_

#include <Stream.h>
#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>
#include <IPAddress.h>

#include "WizFiRingBuffer.h"
#include "WizFi250_definitions.h"
#include "general_parse.h"


#if defined(ARDUINO_ARCH_SAMD) 
  inline static void initSS()    { PORT->Group[g_APinDescription[4].ulPort].PINCFG[g_APinDescription[4].ulPin].reg&=~(uint8_t)(PORT_PINCFG_INEN) ;
								   PORT->Group[g_APinDescription[4].ulPort].DIRSET.reg = (uint32_t)(1<<g_APinDescription[4].ulPin) ;
								   PORT->Group[g_APinDescription[4].ulPort].PINCFG[g_APinDescription[4].ulPin].reg=(uint8_t)(PORT_PINCFG_PULLEN) ;
                                   PORT->Group[g_APinDescription[4].ulPort].OUTSET.reg = (1ul << g_APinDescription[4].ulPin) ; };
  inline static void setSS()     { PORT->Group[g_APinDescription[4].ulPort].OUTCLR.reg = (1ul << g_APinDescription[4].ulPin) ; };
  inline static void resetSS()   { PORT->Group[g_APinDescription[4].ulPort].OUTSET.reg = (1ul << g_APinDescription[4].ulPin) ; };
#endif // ARDUINO_ARCH_AVR


// WizFi250 SPI CONTROL CODE
#define SPI_NULL	(uint8_t) 0xF0
#define SPI_ESC		(uint8_t) 0xF1
#define SPI_F0		(uint8_t) 0x00
#define SPI_F1		(uint8_t) 0x01
#define SPI_SYNC	(uint8_t) 0x02
#define SPI_XON		(uint8_t) 0x03
#define SPI_XOFF	(uint8_t) 0x04
#define SPI_ERR		(uint8_t) 0x05

// maximum size of AT command
#define CMD_BUFFER_SIZE 170

// Default state value for Wifi state field
#define NA_STATE -1
#define NO_SOCKET_AVAIL 255

typedef enum eProtMode {TCP_MODE, UDP_MODE, SSL_MODE} tProtMode;

typedef enum ESC_State {ESC_IDLE, ESC_CID, ESC_PEERIP, ESC_PEERPORT, ESC_LENGTH, ESC_RECV_DATA} tESC_State;

typedef enum {
	WL_NO_SHIELD = 255,
	WL_IDLE_STATUS = 0,
	//WL_NO_SSID_AVAIL,
	//WL_SCAN_COMPLETED,
	WL_CONNECTED,
	WL_CONNECT_FAILED,
	//WL_CONNECTION_LOST,
	WL_DISCONNECTED
} wl_status_t;

/* Encryption modes */
enum wl_enc_type {
	ENC_TYPE_NONE = 0,
	ENC_TYPE_WEP = 1,
	ENC_TYPE_WPA_PSK = 2,
	ENC_TYPE_WPA2_PSK = 3,
	ENC_TYPE_WPA_WPA2_PSK = 4
};

enum wl_tcp_state {
	CLOSED      = 0,
	LISTEN      = 1,
	SYN_SENT    = 2,
	SYN_RCVD    = 3,
	ESTABLISHED = 4,
	FIN_WAIT_1  = 5,
	FIN_WAIT_2  = 6,
	CLOSE_WAIT  = 7,
	CLOSING     = 8,
	LAST_ACK    = 9,
	TIME_WAIT   = 10
};


class WizFi250SpiDrv
{
public:
	enum { SPI_SUCCESS = 0, SPI_RX_BUFFER_MAX = 1, SPI_TIMEOUT = 2, SPI_RECV_DATA = 3 };

public:
	WizFi250SpiDrv	();

	void 		wifiDriverInit		();
	void		hwReset				();


	uint8_t 	wizspi_byte			(uint8_t byte, uint8_t issue_cs, uint16_t delay1, uint16_t delay2);
	void		process_esc_code	(uint8_t *spi_recv, uint8_t *valid_data);

	void 		spi_senddata		(uint8_t *send_data, uint32_t send_length);

	int 		getResponse			(char* outStr, int outStrLen, int lineNum);
	int			sendCmd				(const __FlashStringHelper* cmd, int timeout=1000, ...);
	int			SendCmdWithTag		(const __FlashStringHelper* cmd, const char* tag="[OK]", const char* tag2="", int timeout=10000, ...);
	int 		readUntil			(int timeout, const char* tag="[OK]", const char* tag2="");
	void		wizfiEmptyBuf		(bool warn=true);


	// Driver
	bool 		wifiConnect			(char* ssid, const char *passphrase);
	bool		wifiStartAP			(char* ssid, const char *pwd, uint8_t channel, uint8_t enc = 0 );
	uint8_t		disconnect			();
	void 		config				(IPAddress ip, IPAddress subnet, IPAddress gw);
	void 		config				();
	uint8_t* 	getMacAddress		();
	void		getIpAddress		(IPAddress& ip);
	void		getNetmask			(IPAddress& mask);
	void		getGateway			(IPAddress& gw);
	char*		getCurrentSSID		();
	uint8_t*	getCurrentBSSID		();
	int32_t		getCurrentRSSI		();
	uint8_t		getScanNetworks		();

	char*		firmwareVersion		();
	uint8_t		getConnectionStatus ();


	void		stopClient	(uint8_t sock);
	void 		stopUDP		(uint8_t sock);

	// TCP/IP
	uint16_t	availData			(uint8_t connId);
	void		parsingUDPData		(uint8_t recv_data);
	int			availDataUdp		(uint8_t connId);
	bool		getData				(uint8_t connId, uint8_t *data);
	int			getDataBuf			(uint8_t connId, uint8_t *buf, uint16_t bufSize, uint8_t protMode=TCP_MODE);
	bool		startServer			(uint16_t port, uint8_t protMode=TCP_MODE);
	bool		startClient			(const char* host, uint16_t port, uint8_t sock, uint8_t protMode);
	int			sendData			(uint8_t sock, const uint8_t *data, uint16_t len);
	int			SendDataUDP			(uint8_t sock, const uint8_t *data, uint16_t len);
	int 		timedRead			();
	bool		isDataMode			();
	bool		isServerRun			();
	void		setLocalPort		(uint16_t port);
	void		getRemoteIpAddress	(IPAddress& ip);
	uint16_t	getRemotePort		();


private:
	static WizFiRingBuffer ringBuf;

	// firmware version string
	static char 	fwVersion[WL_FW_VER_LENGTH];
	static char		_ssid[WL_SSID_MAX_LENGTH];
	static uint8_t  _bssid[WL_MAC_ADDR_LENGTH];
	static uint8_t	_mac[WL_MAC_ADDR_LENGTH];
	static uint8_t  _localIp[WL_IPV4_LENGTH];
	static uint8_t  _remoteIP[WL_IPV4_LENGTH];

	uint16_t _localPort;
	uint16_t _remotePort;
	int _recvedLen;
	uint16_t _Current_ESC_State;


private:
    uint8_t		m_wifi_mode;
    bool		m_is_datamode;
    bool		m_is_server_run;
    uint8_t		m_WizFi250_DataReady;
    uint8_t		m_WizFi250_CS;
    uint8_t		m_WizFi250_Reset_Pin;
	bool		m_spi_init_flag;
	bool		m_use_config;



    friend class WizFi250Class;
	friend class WizFi250Server;
	friend class WizFi250Client;
};

extern WizFi250SpiDrv wizfi250Drv;

#endif /* WIZFI250_SPI_DRV_H_ */
