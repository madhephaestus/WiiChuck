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
  
  char ip_str[16] = {0};
  char mac_str[18] = {0};
  uint8_t mac_addr[6] = {0};
  uint32_t ip_addr = 0;
  
  if(!esp.getIPAddress(&ip_addr)){
    Serial.println("Failed to get IP Address as uint32_t");
  }
  else{
    Serial.print("IP[hex]: 0x");
    Serial.println(ip_addr, HEX);
    Serial.print("IP[dots]: ");
    Serial.print((uint8_t)(ip_addr >> 24));
    Serial.print('.');
    Serial.print((uint8_t)(ip_addr >> 16));
    Serial.print('.');
    Serial.print((uint8_t)(ip_addr >> 8));
    Serial.print('.');
    Serial.print((uint8_t)(ip_addr));  
    Serial.println();
  }

  if(!esp.getIPAddress((char *) &(ip_str[0]))){
    Serial.println("Failed to get IP Address as String");
  }
  else{
    Serial.print("IP[str]: ");
    Serial.println((char *) ip_str);
  }
  
  if(!esp.getMacAddress(mac_addr)){
    Serial.println("Failed to get MAC address as uint8_t *");
  }
  else{
    Serial.print("MAC[uint8 *]: ");
    for(uint8_t ii = 0; ii < 6; ii++){

      if(mac_addr[ii] < 0x10){
        Serial.print("0");
      }     
      Serial.print(mac_addr[ii], HEX);     
      
      if(ii != 5){
        Serial.print(":");
      }
    }
    Serial.println();
  }

  if(!esp.getMacAddress((char *) &(mac_str[0]))){
    Serial.print("Failed to get MAC addres as String");
  }
  else{
    Serial.print("MAC[str]: ");
    Serial.println((char *) mac_str);
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
