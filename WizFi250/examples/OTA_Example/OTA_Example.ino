#include <Arduino.h>

#include <SPI.h>
#include "WizFi250.h"

void setup() {
    // initialize serial for debugging
    Serial.begin(115200);
    // initialize serial for ESP module
    WiFi.init();

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");

      Serial.println(F("WizFi250 firmware is something wrong."));
    Serial.println(F("WizFi250 needs <Factory Recovery>"));

      // don't continue
      while (true);
    }

    Serial.println("WiFi shield present");
}

void loop() {
  bool status;
  String ota_ssid;

  Serial.println(F("Start OTA"));
  ota_ssid = "WIZFI_OTA_";
  ota_ssid += WiFi.macAddress();


  status = WiFi.runOTA();
  if(status != true)
  {
    Serial.println(F("WizFi250 firmware is wrong."));
    Serial.println(F("WizFi250 needs <Factory Recovery>"));
    while(1);
  }
  else
  {
    Serial.println(F("OTA is running"));
    Serial.print(F("Please search SSID and connect to "));
    Serial.print(ota_ssid);
    Serial.println(F(" in your PC"));
    Serial.print(F("And then open the web browser and go to 'wizfi250.wiznet.com'"));
    Serial.println(F(""));
    Serial.println(F(""));
    Serial.println(F("For more information about OTA method, refer to this URL."));
    Serial.println(F("http://wizwiki.net/wiki/doku.php?id=products:wizfi250:wizfi250pg:start#upload_newest_firmware_in_ota_mode"));

    while(1);
  }
}

