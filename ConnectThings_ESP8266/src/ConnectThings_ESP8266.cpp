/**
 * @file ConnectThings_ESP8266.cpp
 * @brief The implementation of class ESP8266. 
 * @author Francisco Calaça<chicocx@gmail.com> 
 * @date 2016.12
 * 
 * @par Copyright:
 * Copyright (c) 2016 Francisco Calaça. \n\n
 * Based on ITEAD Intelligent Systems Co., Ltd. version: ESP8266.cpp\n
 * ITEADLIB_Arduino_WeeESP8266\n\n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version. \n\n
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
#include "ConnectThings_ESP8266.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

Wifi::Wifi(SoftwareSerial &_esp8266): esp8266(&_esp8266)
{
    esp8266->begin(9600);
    rx_empty();

}



String Wifi::recvString(String target, uint32_t timeout=10000)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(esp8266->available() > 0) {
            a = esp8266->read();
      if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target) != -1) {
            break;
        }   
    }
    return data;
}

String Wifi::recvString(String target1, String target2, uint32_t timeout=10000)
{
    String data;
    char a;
    unsigned long start = millis();
    bool isTimeout = true;
    while (millis() - start < timeout) {
        while(esp8266->available() > 0) {
            a = esp8266->read();
            if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            isTimeout = false;
            break;
        } else if (data.indexOf(target2) != -1) {
            isTimeout = false;
            break;
        }
    }
    if(isTimeout){
      Serial.println("**TIMEOUT OCCURRED. Timeout " + String(timeout) + " was insufficient");
    }
    return data;
}

String Wifi::recvString(String target1, String target2, String target3, uint32_t timeout=10000)
{
    String data;
    char a;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        while(esp8266->available() > 0) {
            a = esp8266->read();
            if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            Serial.println("saiu " + target1);
            break;
        } else if (data.indexOf(target2) != -1) {
            Serial.println("saiu " + target2);
            break;
        } else if (data.indexOf(target3) != -1) {
            Serial.println("saiu " + target3);
            break;
        }
    }
    return data;
}


bool Wifi::recvFind(String target, uint32_t timeout)
{
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        return true;
    }
    return false;
}

bool Wifi::recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout = 10000)
{
    String data_tmp;
    data_tmp = recvString(target, timeout);
    if (data_tmp.indexOf(target) != -1) {
        int32_t index1 = data_tmp.indexOf(begin);
        int32_t index2 = data_tmp.indexOf(end);
        if (index1 != -1 && index2 != -1) {
            index1 += begin.length();
            data = data_tmp.substring(index1, index2);
            return true;
        }
    }
    data = "";
    return false;
}


void Wifi::rx_empty(void) 
{
    while(esp8266->available() > 0) {
        esp8266->read();
    }
}

bool Wifi::AT(void)
{
    rx_empty();
    esp8266->println("AT");
    return recvFind("OK", 2000);
}

bool Wifi::AT_CWJAP(String ssid, String pwd){
    String data;
    rx_empty();
    esp8266->print("AT+CWJAP=\"");
    esp8266->print(ssid);
    esp8266->print("\",\"");
    esp8266->print(pwd);
    esp8266->println("\"");
    
    data = recvString("OK", "FAIL", 1800000);
    return data.indexOf("OK") != -1;

}

bool Wifi::AT_CIFSR(String &list)
{
    rx_empty();
    esp8266->println("AT+CIFSR");
    return recvFindAndFilter("OK", "\r\r\n", "\r\n\r\nOK", list);
}

bool Wifi::AT_CIPMUX(uint8_t mode){
    String data;
    rx_empty();
    esp8266->print("AT+CIPMUX=");
    esp8266->println(String(mode));
    
    data = recvString("OK", "Link is builded");
    if (data.indexOf("OK") != -1) {
        return true;
    }
    return false;
}

bool Wifi::AT_CIPSTART(String type, String host, int port)
{
    String data;
    rx_empty();
    esp8266->print("AT+CIPSTART=\"");
    esp8266->print(type);
    esp8266->print("\",\"");
    esp8266->print(host);
    esp8266->print("\",");
    esp8266->println(port);
    
    data = recvString("OK", "ERROR", "ALREADY CONNECT", 10000);
    Serial.println("***");
    Serial.println(data);
    Serial.println("***");
    if (data.indexOf("OK") != -1 || data.indexOf("ALREADY CONNECT") != -1) {
        return true;
    }
    return false;
}

bool Wifi::AT_CIPCLOSE(void){
    rx_empty();
    esp8266->println("AT+CIPCLOSE");
    return recvFind("OK", 10000);
}

bool Wifi::AT_CIPSEND(const uint8_t *buffer, uint32_t len){
    rx_empty();
    esp8266->print("AT+CIPSEND=");
    esp8266->println(len);
    if (recvFind(">", 10000)) {
        rx_empty();
        for (uint32_t i = 0; i < len; i++) {
            esp8266->write(buffer[i]);
        }
        return recvFind("SEND OK", 10000);
    }
    return false;
}

bool Wifi::connect(String _ssid, String _pwd){
  ssid = _ssid;
  pwd = _pwd;

  if(AT()){
    Serial.println("AT OK");
    while(!AT_CWJAP(ssid, pwd)){
      Serial.println("Error trying to join in access point " + ssid  +" try again...");
      delay(2000);
    }
    Serial.println("Connected in accesspoint: " + ssid);
    AT_CIFSR(ipMac);
    Serial.println("IP/MAC: " + ipMac);
    AT_CIPMUX(0);
    status = 1;
  }else{
    Serial.println("Error on connect with Esp8266");
  }
}

void Wifi::keepConnected(){
   switch(status)
   {
      case 0:
      if(connect(ssid, pwd)){
         status = 1;
      }else{
         Serial.println("Not connected");
      }
      break;
   }
}

bool Wifi::closeTcpSocket(){
   if(AT_CIPCLOSE()){
      status = 1;
      Serial.println("Connection closed successfully");
      return true;
   }else{
      status = 1;
      Serial.println("Connection closed with error");
      return false;
   }
}

bool Wifi::openTcpSocket(String host, int port){
      if(AT_CIPSTART("TCP", host, port)){
         status = 2;
      }
}

bool Wifi::sendTcpData(char *hello){
     if(AT_CIPSEND((const uint8_t*)hello, strlen(hello))){
        Serial.println("Sended tcp data");
     }else{
        Serial.println("error in send tcp data");
        Serial.println("Not connected. Use openTcpSocket(host, port) before");
     }
}

bool Wifi::httpGET(String host, int port, String query){
   openTcpSocket(host, port);
   char *data;
   String httpQuery = "GET "+query+" HTTP/1.1\r\nHost: "+host+":"+String(port)+"\r\nConnection: close\r\n\r\n";
   Serial.println(httpQuery);
   httpQuery.toCharArray(data, httpQuery.length());
   sendTcpData(data);
   closeTcpSocket();
}

bool Wifi::isConnected(void){
   return status == 1;
}