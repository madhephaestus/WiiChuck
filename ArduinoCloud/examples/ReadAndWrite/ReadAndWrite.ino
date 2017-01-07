/*

  ArduinoCloudThing ReadAndWrite

  Example that explaines the use of read and write properties and how to
  manage a property of others devices.

  Arduino Cloud -> https://cloud.arduino.cc/cloud

  IMPORTANT: in order to establish the tls connection is necessary to add
  the arduino.cc SSL cerificate to your board, look at
  https://github.com/arduino-libraries/WiFi101-FirmwareUpdater#to-update-ssl-certificates

  created May 2016
  by Gilberto Conti and Sandeep Mistry

*/

#include <WiFi101.h>
#include <ArduinoCloud.h>

/////// Wifi Settings ///////
char ssid[] = "";
char pass[] = "";

// Arduino Cloud settings and credentials
const char userName[] = "";
const char thingName[]   = "";
const char thingId[]   = "";
const char thingPsw[]  = "";

WiFiSSLClient sslClient;

// build a new object "cloudObject"
ArduinoCloudThing cloudObject;

const int ledPin = 6;

void setup() {
  Serial.begin (9600);

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }

  // setup the "cloudObject"
  cloudObject.enableDebug(); // eneble the serial debug output
  cloudObject.begin(thingName, userName, thingId, thingPsw, sslClient);

  // define the properties
  cloudObject.addProperty("bulb", STATUS , RW);
  cloudObject.addExternalProperty("lampSwitch", "position", STATUS); // this property is owned by "lampSwitch" object
}

void loop() {
  // subscribes to RW properties and look at the connections status
  cloudObject.poll();

  // read the lamp switch "position", update "bulb" property accordingly
  if (cloudObject.readProperty("lampSwitch", "position") == "on") {
    cloudObject.writeProperty("bulb", "on");
  } else {
    cloudObject.writeProperty("bulb", "off");
  }

  // read the "bulb" property, update the LED accordingly
  if (cloudObject.readProperty("bulb") == "on") {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  delay(1000);
}
