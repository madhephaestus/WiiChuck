#include "DHT.h"
#include <SPI.h>
#include "WizFi250.h"

char ssid[] = "SSID";    //  your network SSID (name)
char pass[] = "PASSWORD";   // your network password

String apiKey = "YOUR API KEY";
const char* server = "api.thingspeak.com";

//#define DHTPIN 7 
//DHT dht(DHTPIN, DHT11,15);

int status = WL_IDLE_STATUS;
WiFiClient  client;

void printWifiStatus();


void setup() {
  Serial.begin(115200);

  WiFi.init();
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(t);
    postStr +="&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius Humidity: ");
    Serial.print(h);
    Serial.println("% send to Thingspeak");
  }
  client.stop();

  Serial.println("Waiting…");
  // thingspeak needs minimum 15 sec delay between updates
  delay(20000);
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

