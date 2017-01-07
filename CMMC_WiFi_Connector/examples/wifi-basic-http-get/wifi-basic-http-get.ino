// Copyright Nat Weerawan 2015-2016
// MIT License

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiConnector.h>
#include <ESP8266HTTPClient.h>

#ifndef WIFI_SSID
  #define WIFI_SSID       "Nat"
  #define WIFI_PASSPHRASE "1234567890"
#endif

WiFiConnector wifi(WIFI_SSID, WIFI_PASSPHRASE);

void init_hardware()
{
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);
  Serial.flush();
  Serial.println();
  Serial.println();
  Serial.println("will be started in 500ms..");
}

void init_wifi() {
  wifi.init();
  wifi.on_connected([&](const void* message)
  {
    Serial.print("WIFI CONNECTED WITH IP: ");
    Serial.println(WiFi.localIP());
  });

  wifi.on_connecting([&](const void* message)
  {
    Serial.print("Connecting to ");
    Serial.println(wifi.get("ssid") + ", " + wifi.get("password"));
    delay(200);
  });
}


void doHttpGet() {
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
  http.begin("http://cmmc.xyz/hello.txt"); //HTTP

  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      Serial.print("[CONTENT]\n");

      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void setup()
{
  init_hardware();
  init_wifi();
  wifi.connect();
}

void loop()
{
  wifi.loop();
  if (wifi.connected()) {
    doHttpGet();
    delay(5000);
  }
}
