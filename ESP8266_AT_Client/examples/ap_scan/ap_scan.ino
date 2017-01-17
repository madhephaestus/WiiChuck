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
  
  ap_scan_result_t results[20] = {0};
  uint8_t num_results_found = 0;
  if(!esp.scanAccessPoints((ap_scan_result_t *) &(results[0]), 20, &num_results_found)){
    Serial.println("Failed to perform AP Scan");
  }
  else{
    Serial.print("Found ");
    Serial.print(num_results_found);
    Serial.println(" Access Points");
    Serial.println("#,Security,SSID,RSSI,MAC");
    for(uint8_t ii = 0; ii < num_results_found; ii++){
      char mac[18] = {0};
      esp.macArrayToString(&(results[ii].mac[0]), (char *) mac);
      Serial.print(ii+1);
      Serial.print(",");      
      Serial.print(results[ii].security);
      Serial.print(",");
      Serial.print(results[ii].ssid);
      Serial.print(",");
      Serial.print(results[ii].rssi);
      Serial.print(",");
      Serial.print(mac);
      Serial.println();
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
