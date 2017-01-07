#ifndef arduinoserialtotcpbridgeclient_H
#define arduinoserialtotcpbridgeclient_H

#include "Arduino.h"
#include "Client.h"

// Protocol Packet Headers
#define PROTOCOL_CONNECT 0
#define PROTOCOL_CONNACK 1
#define PROTOCOL_DISCONNECT 2
#define PROTOCOL_PUBLISH 3
#define PROTOCOL_ACK 4

// Protocol Link State
#define STATE_DISCONNECTED 0
#define STATE_CONNECTED 1

// Protocol Packet RX State
#define RX_PACKET_IDLE 0
#define RX_PACKET_GOTLENGTH 1
#define RX_PACKET_GOTCOMMAND 2

class ArduinoSerialToTCPBridgeClient : public Client {

public:
	ArduinoSerialToTCPBridgeClient();

	virtual int connect(IPAddress ip, uint16_t port);
	virtual int connect(const char *host, uint16_t port);
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buf, size_t size);
	virtual int available();
	virtual int read();
	virtual int read(uint8_t *buf, size_t size);
	virtual int peek();
	virtual void flush();
	virtual void stop();
	virtual uint8_t connected();
	virtual operator bool();

private:
	void rxCallback(uint8_t c);
	boolean writePacket(uint8_t command, uint8_t* payload, uint8_t pLength);
	uint8_t state;
	boolean expectedRxSeqFlag;
	boolean expectedAckSeq;
	boolean ackOutstanding;
	uint32_t lastInAct;
	uint8_t workBuffer[256];
	uint8_t rxBuffer[256];
	uint8_t readBufpH, readBufpT;
	boolean readBufisFull;
	uint8_t readBuf[256];

	friend void rxISR0(uint8_t c);
};

#endif
