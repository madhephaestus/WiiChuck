#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <Arduino.h>
#include <functional>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
//#define WIFI_CONNECTOR_DEBUG_MODE
using namespace std;

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

#ifdef WIFI_CONNECTOR_DEBUG_MODE
    #define DEBUG_PRINTER Serial
    #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
    #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
    #define DEBUG_PRINT(...) { }
    #define DEBUG_PRINTLN(...) { }
#endif

// TYPES
// typedef enum {
//     WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
//     WL_IDLE_STATUS      = 0,
//     WL_NO_SSID_AVAIL    = 1,
//     WL_SCAN_COMPLETED   = 2,
//     WL_CONNECTED        = 3,
//     WL_CONNECT_FAILED   = 4,
//     WL_CONNECTION_LOST  = 5,
//     WL_DISCONNECTED     = 6
// } wl_status_t;


typedef std::function<void(const void*)> wifi_callback_t;


class WiFiConnector;

typedef struct {
  String ssid;
  String passphase;
  WiFiConnector *instance;
} Config;

class WiFiConnector {
  private:
    String _ssid;
    String _passphase;
    WiFiConnector* s_instance;
    bool _initialised = false;
    bool _connected = false;
    bool _got_ip = false;
    Config *_config;

    wifi_callback_t _user_on_disconnected = NULL;
    wifi_callback_t _user_on_connected  = NULL;
    wifi_callback_t _user_on_connecting = NULL;

    wifi_callback_t _user_on_smartconfig_waiting;
    wifi_callback_t _user_on_smartconfig_done;
    wifi_callback_t _user_on_smartconfig_processing;

  protected:
    WiFiConnector() { };
public:
    uint32_t counter = 0;
    WiFiConnector(String, String);
    WiFiConnector(String, String, uint8_t);

    ~WiFiConnector() {
      delete s_instance;
      delete _config;
    };

    String get(String);
    String SSID();
    String psk();

    void setSsid(String);
    void setPasspharse(String);

    static WiFiConnector* instance()
    {
      static WiFiConnector *s_instance = NULL;
      Serial.printf("addr: %x\r\n", s_instance);
      if (!s_instance) {
        s_instance = new WiFiConnector;
      }
      Serial.printf("addr: %x\r\n", s_instance);
      return s_instance;
    }

    void init();
    void loop();
    void connect();
    bool connected();
    void disconnect(bool wifioff = false);

    void on_disconnected(wifi_callback_t callback = NULL);
    void on_connected(wifi_callback_t callback = NULL);
    void on_connecting(wifi_callback_t callback = NULL);

    void on_smartconfig_waiting(wifi_callback_t callback = NULL);
    void on_smartconfig_done(wifi_callback_t callback = NULL);
    void on_smartconfig_processing(wifi_callback_t callback = NULL);
};

// WiFiConnector* WiFiConnector::s_instance = NULL;

#endif /* WIFI_CONNECTOR_H */
