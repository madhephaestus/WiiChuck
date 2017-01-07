#include <Arduino.h>
#include <SPI.h>
#include <IPAddress.h>
#include "WizFi250.h"

char ssid[] = "DIR-815_Wiznet";    // your network SSID (name)
char pass[] = "12345678";          // your network password
int status = WL_IDLE_STATUS;       // the Wifi radio's status

//Ubidots information
#define TOKEN    "2cITDFGbcP1WYIszeMJPjK6PLDBrAq"
#define VARID_LED_Red "579064ca76254255dcca5e62"

//Server information
char server[] = "thing.ubidots.com";
#define  REMOTE_PORT    80

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

//Hardware Pin status
#define Pin_LED_Red     49

// Initialize the Ethernet client object
WiFiClient client;

// use a ring buffer to increase speed and reduce memory allocation
WizFiRingBuffer buf();

bool isLEDOn = false;

void printWifiStatus();
void httpRequest();

void setup()
{
  Serial.begin(115200);
  Serial.println("\r\nSerial Init");

  pinMode(Pin_LED_Red, OUTPUT);   //added
  digitalWrite(Pin_LED_Red, LOW);

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
}

void loop()
{
  char c;
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    c = client.read();
    if( c != -1)
    {
      buf.push(c);
      // printing the stream to the serial monitor will slow down
      Serial.write((char)c);
    }

    if (buf.endsWith("\"value\": 1.0,"))
    {
      isLEDOn = false;
    }
    else if(buf.endsWith("\"value\": 0.0,"))
    {
      isLEDOn = true;
    }
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    if( isLEDOn )
    {
      Serial.println("Turn On LED");
      digitalWrite(Pin_LED_Red, LOW);
    }
    else
    {
      Serial.println("Turn Off LED");
      digitalWrite(Pin_LED_Red, HIGH);
    }

    httpRequest();
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

// this method makes a HTTP connection to the server
void httpRequest()
{
  Serial.println();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, REMOTE_PORT)) {
    Serial.println("Connecting...");

    // send the HTTP PUT request
    client.print(F("GET /api/v1.6/variables/"));
    client.print(VARID_LED_Red);
    client.print(F(" HTTP/1.1\r\n"));
    client.print(F("Host: things.ubidots.com\r\n"));
    client.print(F("X-Auth-Token: "));
    client.print(TOKEN);
    client.print(F("\r\n"));
    client.print(F("Content-Type: application/json\r\n"));
    client.print(F("Connection: close\r\n"));
    client.print(F("\r\n\r\n"));

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}



