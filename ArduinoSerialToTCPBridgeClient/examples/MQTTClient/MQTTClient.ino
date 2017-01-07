/*
 * Basic example of MQTT Client running over the Serial To TCP Bridge Client.
 * The Arduino makes a connection to a Protocol Gateway instance on the PC that
 * is listening on the COM port that is connected to the Serial-over-USB of the Arduino.
 * The MQTT Client connects to a Broker running on localhost on the host PC.
 */
 
#include <ArduinoSerialToTCPBridgeClient.h>
#include <PubSubClient.h>

ArduinoSerialToTCPBridgeClient* s;  // Protocol Client running over USB Serial
PubSubClient* client;               // MQTT Client
const char* ledTopic = "led";

void setup() {
  pinMode(13, OUTPUT);
  s = new ArduinoSerialToTCPBridgeClient();
  client = new PubSubClient(*s);
  // MQTT Broker running on same PC the Arduino is connected to.
  client->setServer(IPAddress(127,0,0,1), 1883);
  client->setCallback(callback);
  if (client->connect("arduinoClient")) {
    client->publish("outTopic", "Hello world!");
    client->subscribe(ledTopic);
  }
}

void loop() {
  client->loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Only proceed if incoming message's topic matches.
  if ((strcmp(topic, ledTopic) == 0) && (length == 1)) {
    if (payload[0] == 0x31) {
      digitalWrite(13, HIGH); // Turn on led if MQTT message '1'
    } else if (payload[0] == 0x32) {
      digitalWrite(13, LOW);  // Turn off led if MQTT message '2'
    }
  }
}

