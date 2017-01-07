/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */

#ifndef WIZFI250_WIZFI250CLIENT_H_
#define WIZFI250_WIZFI250CLIENT_H_

#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"


class WiFiClient : public Client
{
public:
	WiFiClient();
	WiFiClient(uint8_t sock);

  /*
  * Connect to the specified IP address and port. The return value indicates success or failure.
  * Returns true if the connection succeeds, false if not.
  */
  virtual int connect(IPAddress ip, uint16_t port);

  /*
  * Connect to the specified host and port. The return value indicates success or failure.
  * Returns true if the connection succeeds, false if not.
  */
  virtual int connect(const char *host, uint16_t port);

  /*
  * Connect to the specified IP address and port using SSL. The return value indicates success or failure.
  * Returns true if the connection succeeds, false if not.
  */
  int connectSSL(IPAddress ip, uint16_t port);

  /*
  * Connect to the specified host and port using SSL. The return value indicates success or failure.
  * Returns true if the connection succeeds, false if not.
  */
  int connectSSL(const char* host, uint16_t port);

  /*
  * Write a character to the server the client is connected to.
  * Returns the number of characters written.
  */
  virtual size_t write(uint8_t);

  /*
  * Write data to the server the client is connected to.
  * Returns the number of characters written.
  */
  virtual size_t write(const uint8_t *buf, size_t size);


  virtual int available();

  /*
  * Read the next byte received from the server the client is connected to (after the last call to read()).
  * Returns the next byte (or character), or -1 if none is available.
  */
  virtual int read();


  virtual int read(uint8_t *buf, size_t size);

  /*
  * Returns the next byte (character) of incoming serial data without removing it from the internal serial buffer.
  */
  virtual int peek();

  /*
  * Discard any bytes that have been written to the client but not yet read.
  */
  virtual void flush();

  /*
  * Disconnect from the server.
  */
  virtual void stop();

  /*
  * Whether or not the client is connected.
  * Note that a client is considered connected if the connection has been closed but there is still unread data.
  * Returns true if the client is connected, false if not.
  */
  virtual uint8_t connected();


  uint8_t status();

  virtual operator bool();


  // needed to correctly handle overriding
  // see http://stackoverflow.com/questions/888235/overriding-a-bases-overloaded-function-in-c
  using Print::write;
  using Print::print;
  using Print::println;


  /*
  * Returns the remote IP address.
  */
  IPAddress remoteIP();


  friend class WiFiServer;

private:
  uint8_t _sock;     // connection id

  uint8_t getFirstSocket();
  int connect(const char* host, uint16_t port, uint8_t protMode);

  size_t printFSH(const __FlashStringHelper *ifsh, bool appendCrLf);

};




#endif /* WIZFI250_WIZFI250CLIENT_H_ */
