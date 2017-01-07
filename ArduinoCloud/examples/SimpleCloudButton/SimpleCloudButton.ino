/*

  ArduinoCloudThing Cloud Button

  Simple cloud button example that use WiFi101 client and display on the
  arduino cloud dashboard the value of button.

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
const char userName[]   = "";
const char thingName[] = "";
const char thingId[]   = "";
const char thingPsw[]  = "";

WiFiSSLClient sslClient;

// build a new object "cloudObject"
ArduinoCloudThing cloudObject;

const int buttonPin = 6;

void setup() {

  // configure the button pin as input
  pinMode(buttonPin, INPUT);

  Serial.begin (9600);
  delay(1000);

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
  cloudObject.addProperty("position", STATUS , R);
}

void loop() {
  // subscribes to RW properties and look at the connections status
  cloudObject.poll();
  // read the button
  if (digitalRead(buttonPin) == HIGH) {
    // button is pressed, write position as "on"
    cloudObject.writeProperty("position", "on");
  } else {
    // button is released, write position as "off"
    cloudObject.writeProperty("position", "off");
  }

  delay(1000);
}
