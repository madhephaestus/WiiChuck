// #ifndef WIFI_CONNECTOR_H
// #define WIFI_CONNECTOR_H
/*

Copyright Nat Weerawan 2015-2016
MIT License

The MIT License (MIT)

Copyright (c) Nat Weerawan <nat.wrw@gmail.com> (cmmakerclub.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "WiFiConnector.h"

// static WiFiConnector* __wifi_connector = NULL;
static WiFiConnector *_wifi = NULL;

WiFiConnector::WiFiConnector(String ssid, String password) {
  _ssid = ssid;
  _passphase = password;

  _config = new Config;
  _config->instance = this;
  _config->ssid = ssid;
  _config->passphase = password;

};

WiFiConnector::WiFiConnector(String ssid, String password, uint8_t pin) {
  WiFiConnector(ssid, password);
};

void WiFiConnector::setSsid(String ssid){
  _ssid = ssid;
}

void WiFiConnector::setPasspharse(String pass){
  _passphase = pass;
}

String WiFiConnector::get(String key) {
  if (key == "ssid") {
    return WiFi.SSID();
  }
  // backward compatibility
  else if (key == "psk" || key == "password" || key == "passpharse") {
    return WiFi.psk();
  }
  else {
    return "WRONG argument.";
  }
}

String WiFiConnector::psk() {
  return WiFi.psk();
}
String WiFiConnector::SSID() {
  return WiFi.SSID();
}

void WiFiConnector::init() {
  _wifi = WiFiConnector::instance();
  _wifi->setSsid(_ssid);
  _wifi->setPasspharse(_passphase);
  _initialised = true;

  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  static WiFiConnector *_this = this;

  WiFi.onEvent([](WiFiEvent_t event) {
      DEBUG_PRINT("[WiFi-event] event: ");
      DEBUG_PRINTLN(event);
      switch (event) {
          case WIFI_EVENT_STAMODE_CONNECTED:
            DEBUG_PRINTLN("DEBUG: WIFI_EVENT_STAMODE_CONNECTED");
            _wifi->_connected = true;
            if (_this->_user_on_connected) {
              // DEBUG_PRINTLN("DEBUG: STAMOD_CONNECTED.");
              // _this->_user_on_connected((void*)"CONNECTED");
              // _this->_connected = true;
            }
            else {

            }
            break;
          case WIFI_EVENT_STAMODE_DISCONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_DISCONNECTED\r\n", millis());
            DEBUG_PRINTLN("DEBUG: WIFI_EVENT_STAMODE_DISCONNECTED");
            _wifi->_connected = false;
            _wifi->_got_ip = false;

            _this->_connected = false;
            _this->_got_ip = false;
            if (_this->_user_on_disconnected) {
              _this->_user_on_disconnected((void*)"..DISCONNECTED");
              DEBUG_PRINTLN("DEBUG: DISCONNECTED");
            }
            break;
          case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_AUTHMODE_CHANGE\r\n", millis());
            break;
          case WIFI_EVENT_STAMODE_GOT_IP:
            if (_this->_user_on_connected) {
              _this->_user_on_connected((void*)"CONNECTED");
              DEBUG_PRINTLN("DEBUG: CONNECTED");
              _this->_got_ip = true;
              _this->_connected = true;

              _wifi->_got_ip = true;
              _wifi->_connected = true;

            }
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_GOT_IP: ", millis());
            // Serial.println(WiFi.localIP());
            break;
          case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
            // Serial.printf("%lu => WIFI_EVENT_STAMODE_DHCP_TIMEOUT: ", millis());
            break;
          case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_SOFTAPMODE_STACONNECTED: \r\n", millis());
            break;
          case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
            // Serial.printf("%lu => WIFI_EVENT_SOFTAPMODE_STADISCONNECTED: \r\n", millis());
            break;
          case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
            // Serial.printf("%lu => WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED: \r\n", millis());
            break;
          case WIFI_EVENT_MAX:
            // Serial.printf("%lu => WIFI_EVENT_MAX: \r\n", millis());
            break;
          }
    });
}

void WiFiConnector::loop() {
  if (_initialised == false) {
    if (millis() % 1000 == 0) {
      DEBUG_PRINTLN("DEBUG: not initialized.");
    }
    return;
  }

  if (_wifi->_connected == false) {
    if (_user_on_connecting) {
      _user_on_connecting((void*) "CONNECTING...");
      DEBUG_PRINTLN("DEBUG: CONNECTING..");
    }
  }
}

void WiFiConnector::connect() {
  WiFi.begin(_config->ssid.c_str(), _config->passphase.c_str());
}

bool WiFiConnector::connected() {
  // return _wifi->_got_ip;
  return this->_got_ip;
}

void WiFiConnector::disconnect(bool wifioff) {
  WiFi.disconnect(wifioff);
}

void WiFiConnector::on_disconnected(wifi_callback_t callback)
{
    _user_on_disconnected = callback;
}

void WiFiConnector::on_connected(wifi_callback_t callback)
{
  DEBUG_PRINTLN("SET CALLBACK on_connected");
  _user_on_connected = callback;
}

void WiFiConnector::on_connecting(wifi_callback_t callback)
{
    _user_on_connecting = callback;
}

void WiFiConnector::on_smartconfig_waiting(wifi_callback_t callback) {
  _user_on_smartconfig_waiting = callback;
}

void WiFiConnector::on_smartconfig_done(wifi_callback_t callback)
{
  _user_on_smartconfig_done = callback;
}

void WiFiConnector::on_smartconfig_processing(wifi_callback_t callback)
{
  DEBUG_PRINTLN("SMARTCONFIG DONE.");
  _user_on_smartconfig_processing = callback;
}

// #endif /* WIFI_CONNECTOR_H */
