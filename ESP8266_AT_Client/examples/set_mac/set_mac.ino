#include <ESP8266_AT_Client.h>

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
  
  uint8_t mac[6] = {0x18,0x3f,0x7a,0x11,0xF3,0x29};
  if(!esp.setMacAddress(mac)){
    Serial.println("Unable to set MAC address!");
  }
  else{
    char mac_str[18] = {0};
    if(!esp.getMacAddress(&(mac_str[0]))){
      Serial.println("Unable to get MAC address!");
    }
    else{
      Serial.print("MAC Address set to: ");
      Serial.println(mac_str);
    }
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
