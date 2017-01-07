// Copyright Nat Weerawan 2015-2016
// MIT License

#include <Arduino.h>
#include <WiFiConnector.h>
#include <ESP8266WebServer.h>
#include "FS.h"

#ifndef WIFI_SSID
  #define WIFI_SSID       "Nat"
  #define WIFI_PASSPHRASE "1234567890"
#endif

WiFiConnector wifi(WIFI_SSID, WIFI_PASSPHRASE);
ESP8266WebServer server(80);

void fail(const char* msg) {
  Serial.println(msg);
  while (true) {
    yield();
  }
}

void init_hardware()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("BEGIN");

  if (!SPIFFS.begin()) {
    fail("SPIFFS init failed");
  }

  Dir root = SPIFFS.openDir("/");
  Serial.println("READING ROOT..");
  while (root.next()) {
    String fileName = root.fileName();
    File f = root.openFile("r");
    Serial.printf("%s: %d\r\n", fileName.c_str(), f.size());
  }
}

void init_wifi() {
  wifi.init();

  wifi.on_connecting([&](const void* message)
  {
    Serial.print("Connecting to ");
    Serial.println(wifi.get("ssid") + ", " + wifi.get("password"));
    delay(200);
  });

  wifi.on_connected([&](const void* message)
  {
    // Print the IP address
    Serial.print("WIFI CONNECTED => ");
    Serial.println(WiFi.localIP());

    server.on ( "/test", []() {
      char buff[100];
      String ms = String(millis());
      sprintf(buff, "{\"millis\": %s }", ms.c_str());
      server.send ( 200, "text/plain", buff );
    });

    server.on ( "/atmosphere", []() {
      char buff[100];
      String ms = String(millis());
      sprintf(buff, "{\"millis\": %s, \"temp\": %s, \"humid\": %s}",
      ms.c_str(), "99.9", "55.5");
      server.send ( 200, "text/plain", buff );
    });

    server.on ( "/hello", []() {
      server.send ( 200, "text/plain", String("Hi, there"));
    });

    server.serveStatic("/", SPIFFS, "/");
    server.begin();

    Serial.println("SERVER Started.");
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
  server.handleClient();
}
