#include <ESP8266_AT_Client.h>

#define NETWORK_SSID     "YOUR NETWORK NAME"
#define NETWORK_PASSWORD "YOUR NETWORK PASSWORD"

int esp8266_enable_pin = 23; // Arduino digital the pin that is used to reset/enable the ESP8266 module
Stream * at_command_interface = &Serial1;  // Serial1 is the 'stream' the AT command interface is on
ESP8266_AT_Client esp(esp8266_enable_pin, at_command_interface); // instantiate the client object

#define ESP8266_INPUT_BUFFER_SIZE (1500)
uint8_t input_buffer[ESP8266_INPUT_BUFFER_SIZE] = {0};     // sketch must instantiate a buffer to hold incoming data
                                      // 1500 bytes is way overkill for MQTT, but if you have it, may as well
                                      // make space for a whole TCP packet

void setup(void){
  Serial.begin(115200);               // debug console
  Serial1.begin(115200);              // AT command interface

  //ESP8266_AT_Client::debugStream = &Serial;
  
  esp.setInputBuffer(input_buffer, ESP8266_INPUT_BUFFER_SIZE); // connect the input buffer up
  esp.reset();                                                 // reset the module
  
  Serial.print("Set Mode to Station...");
  esp.setNetworkMode(1);
  Serial.println("OK");   
  
  Serial.print("Connecting to Network...");  
  esp.connectToNetwork(NETWORK_SSID, NETWORK_PASSWORD, 60000, NULL); // connect to the network
  Serial.println("OK");   
  
  uint8_t ip[4] = {192,168,1,47};
  uint8_t gateway[4] = {192,168,1,1};
  uint8_t subnetmask[4] = {255,255,255,0};
  uint8_t dnsserver[4] = {192,168,1,1};
  
  uint32_t ip_u32 = esp.IpArrayToIpUint32((uint8_t *) ip); 
  uint32_t gateway_u32 = esp.IpArrayToIpUint32((uint8_t *) gateway); 
  uint32_t subnetmask_u32 = esp.IpArrayToIpUint32((uint8_t *) subnetmask); 
  uint32_t dnsserver_u32 = esp.IpArrayToIpUint32((uint8_t *) dnsserver); 
  
  if(!esp.setStaticIPAddress(ip_u32, subnetmask_u32, gateway_u32, dnsserver_u32)){
    Serial.println("Failed to set Static IP");
  }
  else{
    Serial.println("Successfully set Static IP");
  }

}

void loop(void){
  if(Serial1.available()){
    Serial.write(Serial1.read());
  }

  if(Serial.available()){
    Serial1.write(Serial.read());
  }
}
