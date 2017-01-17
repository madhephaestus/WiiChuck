#include <ESP8266_AT_Client.h>
#include <PubSubClient.h> // from https://github.com/knolleary/pubsubclient

#define NETWORK_SSID     "YOUR NETWORK NAME"
#define NETWORK_PASSWORD "YOUR NETWORK PASSWORD"

int esp8266_enable_pin = 23; // Arduino digital the pin that is used to reset/enable the ESP8266 module
Stream * at_command_interface = &Serial1;  // Serial1 is the 'stream' the AT command interface is on

ESP8266_AT_Client esp(esp8266_enable_pin, at_command_interface); // instantiate the client object

// a callback to handle messages received on subscriptions
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.println("Message Received!");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  for(uint32_t ii = 0; ii < length; ii++){
    Serial.print((char) payload[ii]);
  }
  Serial.println();
  Serial.print("Length: ");
  Serial.println(length);
}

uint8_t server[4] = {192, 168, 1, 7}; // i.e. 192.168.1.7
PubSubClient mqtt_client(server, 1883, callback, esp);

#define ESP8266_INPUT_BUFFER_SIZE (1500)
uint8_t input_buffer[ESP8266_INPUT_BUFFER_SIZE] = {0};     // sketch must instantiate a buffer to hold incoming data
                                      // 1500 bytes is way overkill for MQTT, but if you have it, may as well
                                      // make space for a whole TCP packet

void setup(void){
  Serial.begin(115200);               // debug console
  Serial1.begin(115200);              // AT command interface
  
  esp.setInputBuffer(input_buffer, ESP8266_INPUT_BUFFER_SIZE); // connect the input buffer up
  esp.reset();                                                 // reset the module
  
  Serial.print("Set Mode to Station...");
  esp.setNetworkMode(1);
  Serial.println("OK");   
  
  Serial.print("Connecting to Network...");  
  esp.connectToNetwork(NETWORK_SSID, NETWORK_PASSWORD, 60000, NULL); // connect to the network
  Serial.println("OK");  
  
  if (mqtt_client.connect("arduinoClient")) {       // connect to the MQTT server
    mqtt_client.publish("outTopic","hello world");  // publish a hello world message to outTopic
  }  
}

unsigned long previousMillis = 0;
const long interval = 1000;     

void loop(void){
  unsigned long currentMillis = millis();
  
  if(!mqtt_client.loop()){
    // reconnect if necessary
    Serial.print(millis());
    Serial.print(" reconnecting...");
    esp.reset();
    esp.setNetworkMode(1);
    esp.connectToNetwork(NETWORK_SSID, NETWORK_PASSWORD, 60000, NULL);    
    if(mqtt_client.connect("arduinoClient")){  
      Serial.println("OK");  
    }
    else{
      Serial.println("Failed");       
    }
  }
  else{
    // if you're connected
    // go ahead and publish the current timestamp
    // once a second
    char temp[32] = {0};
    snprintf(temp, 31, "%lu", millis());
    if(currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;     
      mqtt_client.publish("outTopic", temp);
    }
  }
  
}
