// Copyright Nat Weerawan 2015-2016
// MIT License

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiConnector.h>

#ifndef WIFI_SSID
  #define WIFI_SSID       "Nat"
  #define WIFI_PASSPHRASE "1234567890"
#endif

WiFiConnector wifi("Nat", "123456789");

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

void setup()
{
  init_hardware();
  init_wifi();
  wifi.connect();
}

void loop()
{
  wifi.loop();
}
