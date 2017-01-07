#include <SPI.h>
#include "WizFi250.h"


char ssid[] = "DIR-815_Wiznet";         // your network SSID (name)
char pass[] = "12345678";          // your network password
int status = WL_IDLE_STATUS;      // the Wifi radio's status

char server[] = "arduino.cc";

// Initialize the Ethernet client object
WiFiClient client;

void printWifiStatus();

void setup()
{
  Serial.begin(115200);

  WiFi.init();

    // check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue
      while (true);
    }

    // attempt to connect to WiFi network
    while ( status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network
      status = WiFi.begin(ssid, pass);
    }

    // you're connected now, so print out the data
    Serial.println("You're connected to the network");

    printWifiStatus();

    Serial.println();
    Serial.println("Starting connection to server...");
    // if you get a connection, report back via serial
    if (client.connect(server, 80)) {
      Serial.println("Connected to server");
      // Make a HTTP request
      client.println("GET /asciilogo.txt HTTP/1.1");
      client.println("Host: arduino.cc");
      client.println("Connection: close");
      client.println();
    }
}



void loop()
{
  char test_buff[200]={0,};

  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) {
  if( client.read((uint8_t*)test_buff,sizeof(test_buff)-1) >= 0)
      Serial.print(test_buff);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();

    // do nothing forevermore
    while (true);
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


