#ifndef CONNECTTHINGS_ESP8266_H
#define CONNECTTHINGS_ESP8266_H
 
#include <Arduino.h>
#include "SoftwareSerial.h"


class Wifi
{
public:
   Wifi(SoftwareSerial &_esp8266);
   void  test();

   bool connect(String ssid, String pwd);

   bool isConnected(void);
   void keepConnected();
   bool openTcpSocket(String host, int port);
   bool closeTcpSocket();
   bool sendTcpData(char *hello);
   bool httpGET(String host, int port, String query);
   
   String ipMac;

 private:
   String recvString(String target, uint32_t timeout);
   String recvString(String target1, String target2, uint32_t timeout);
   String recvString(String target1, String target2, String target3, uint32_t timeout);
   bool recvFind(String target, uint32_t timeout);
   bool recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout);
   void rx_empty(void);

   bool AT(void);
   bool AT_CIFSR(String &list);
   bool AT_CWJAP(String ssid, String pwd);
   bool AT_CIPMUX(uint8_t mode);
   bool AT_CIPSTART(String type, String host, int port);
   bool AT_CIPCLOSE(void);
   bool AT_CIPSEND(const uint8_t *buffer, uint32_t len);
   
   

   
   String ssid;
   String pwd;
   int status;
   SoftwareSerial *esp8266;
 
};
 
#endif
