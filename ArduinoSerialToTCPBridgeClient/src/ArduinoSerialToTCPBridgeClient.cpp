#include "ArduinoSerialToTCPBridgeClient.h"
#include <NeoHWSerial.h>
#include <CRC32.h>

static ArduinoSerialToTCPBridgeClient* ser0;

void rxISR0(uint8_t c) {
	ser0->rxCallback(c);
}

void ArduinoSerialToTCPBridgeClient::rxCallback(uint8_t c) {
	static uint8_t packetCount = 0;
	static uint8_t rxState = RX_PACKET_IDLE;
	rxBuffer[packetCount++] = c;
	switch (rxState) {
	case RX_PACKET_IDLE:
		rxState = RX_PACKET_GOTLENGTH;
		break;
	case RX_PACKET_GOTLENGTH:
		rxState = RX_PACKET_GOTCOMMAND;
		break;
	case RX_PACKET_GOTCOMMAND:
		uint8_t packetLength = rxBuffer[0];
		if (packetCount == packetLength + 1) {
			packetCount = 0;
			// Integrity checking
			uint32_t crcRx = (uint32_t) rxBuffer[packetLength - 3] | ((uint32_t) rxBuffer[packetLength - 2] << 8)
					| ((uint32_t) rxBuffer[packetLength - 1] << 16) | ((uint32_t) rxBuffer[packetLength] << 24);
			uint32_t crcCode = CRC32::checksum(rxBuffer, packetLength - 3);
			if (crcRx == crcCode) { // validate packet
				boolean rxSeqFlag = (rxBuffer[1] & 0x80) > 0;
				switch (rxBuffer[1] & 0x7F) {
				// Connection established with destination
				case PROTOCOL_CONNACK:
					if (rxBuffer[0] == 5) {
						state = STATE_CONNECTED;
					}
					break;
				// Incoming data
				case PROTOCOL_PUBLISH:
					writePacket(PROTOCOL_ACK | (rxBuffer[1] & 0x80), NULL, 0);
					if (rxSeqFlag == expectedRxSeqFlag) {
						expectedRxSeqFlag = !expectedRxSeqFlag;
						if (rxBuffer[0] > 5) {
							for (uint8_t i = 0; i < rxBuffer[0] - 5; i++) {
								readBuf[readBufpT++] = rxBuffer[2 + i];
							}
							readBufisFull = (readBufpH == readBufpT);
						}
					}
					break;
				// Protocol Acknowledge
				case PROTOCOL_ACK:
					if (ackOutstanding) {
						if (rxSeqFlag == expectedAckSeq) {
							ackOutstanding = false;
						}
					}
					break;
				}
			}
			rxState = RX_PACKET_IDLE;
		}
		break;
	}
}

boolean ArduinoSerialToTCPBridgeClient::writePacket(uint8_t command, uint8_t* payload, uint8_t pLength) {
	workBuffer[0] = pLength + 5;
	workBuffer[1] = command;
	if (payload != NULL) {
		for (uint8_t i = 2; i < pLength + 2; i++) {
			workBuffer[i] = payload[i - 2];
		}
	}
	uint32_t crcCode = CRC32::checksum(workBuffer, pLength + 2);
	workBuffer[pLength + 2] = crcCode & 0x000000FF;
	workBuffer[pLength + 3] = (crcCode & 0x0000FF00) >> 8;
	workBuffer[pLength + 4] = (crcCode & 0x00FF0000) >> 16;
	workBuffer[pLength + 5] = (crcCode & 0xFF000000) >> 24;
	if ((int) (pLength) + 6 > NeoSerial.availableForWrite()) {
		return false;
	}
	for (int i = 0; i < pLength + 6; i++) {
		NeoSerial.write(workBuffer[i]);
	}
	return true;
}

ArduinoSerialToTCPBridgeClient::ArduinoSerialToTCPBridgeClient() {
	ackOutstanding = false;
	expectedAckSeq = false;
	expectedRxSeqFlag = false;
	readBufpH = 0;
	readBufpT = 0;
	readBufisFull = false;
	state = STATE_DISCONNECTED;
	NeoSerial.attachInterrupt(rxISR0);
	NeoSerial.begin(115200);
	ser0 = this;
}

int ArduinoSerialToTCPBridgeClient::connect(IPAddress ip, uint16_t port) {
	uint8_t destination[6] = {
		(uint8_t) ((uint32_t) ip),
		(uint8_t) ((uint32_t) ip >> 8),
		(uint8_t) ((uint32_t) ip >> 16),
		(uint8_t) ((uint32_t) ip >> 24),
		(uint8_t) port,
		(uint8_t) (port >> 8)
	};
	writePacket(PROTOCOL_CONNECT, destination, 6);
	lastInAct = millis();
	while (state != STATE_CONNECTED) {
		uint32_t now = millis();
		if (now - lastInAct >= 5000) {
			return -1;
		}
	}
	lastInAct = millis();
	return 1;
	/*
		SUCCESS 1
		TIMED_OUT -1
		INVALID_SERVER -2
		TRUNCATED -3
		INVALID_RESPONSE -4
	*/
}

int ArduinoSerialToTCPBridgeClient::connect(const char *host, uint16_t port) {

}

size_t ArduinoSerialToTCPBridgeClient::write(uint8_t) {

}

size_t ArduinoSerialToTCPBridgeClient::write(const uint8_t *buf, size_t size) {
	static bool pubSequence = false;
	uint8_t cmd = PROTOCOL_PUBLISH;
	if (pubSequence) {
		cmd |= 0x80;
	}
	pubSequence = !pubSequence;
	if (!writePacket(cmd, (uint8_t*) buf, size)) {
		return 0;
	}
	ackOutstanding = true;
	return size;
}

int ArduinoSerialToTCPBridgeClient::available() {
	if (readBufisFull) {
		return 256;
	}
	if (readBufpT >= readBufpH) {
		return (int) (readBufpT - readBufpH);
	} else {
		return 256 - (int) (readBufpH - readBufpT);
	}
}

int ArduinoSerialToTCPBridgeClient::read() {
	if (!available()) {
		return -1;
	}
	uint8_t ch = readBuf[readBufpH++];
	readBufisFull = false;
	return ch;
}

int ArduinoSerialToTCPBridgeClient::read(uint8_t *buf, size_t size) {

}

int ArduinoSerialToTCPBridgeClient::peek() {
	return readBuf[readBufpH];
}

void ArduinoSerialToTCPBridgeClient::flush() {
	NeoSerial.flush();
}

void ArduinoSerialToTCPBridgeClient::stop() {

}

uint8_t ArduinoSerialToTCPBridgeClient::connected() {
	if (state == STATE_CONNECTED) {
		return 1;
	}
	return 0;
}

ArduinoSerialToTCPBridgeClient::operator bool() {
  return 1;
}
