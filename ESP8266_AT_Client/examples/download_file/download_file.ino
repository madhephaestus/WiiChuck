#include <ESP8266_AT_Client.h>
#include <util/crc16.h>

#define NETWORK_SSID     "YOUR NETWORK NAME"
#define NETWORK_PASSWORD "YOUR NETWORK PASSWORD"

#define DOWNLOAD_FILE_LOOP      (true)
#define IDLE_TIMEOUT_MS  10000     // Amount of time to wait (in milliseconds) with no data 
                                   // received before closing the connection.  If you know the server
                                   // you're accessing is quick to respond, you can reduce this value.

int esp8266_enable_pin = 23; // Arduino digital the pin that is used to reset/enable the ESP8266 module
Stream * at_command_interface = &Serial1;  // Serial1 is the 'stream' the AT command interface is on

ESP8266_AT_Client esp(esp8266_enable_pin, at_command_interface); // instantiate the client object

#define ESP8266_INPUT_BUFFER_SIZE (1500)
uint8_t input_buffer[ESP8266_INPUT_BUFFER_SIZE] = {0};     // sketch must instantiate a buffer to hold incoming data
                                      // 1500 bytes is way overkill for MQTT, but if you have it, may as well
                                      // make space for a whole TCP packet

void downloadFile(char * hostname, uint16_t port, char * filename, void (*responseBodyProcessor)(uint8_t *, uint32_t));
void processResponseData(uint8_t * data, uint32_t data_length);

char scratch[1024] = {0};

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
  
  do {
    downloadFile("update.wickeddevice.com", 80, "archive/aqev2_no2_co-2.0.5.hex", processResponseData);
  } 
  while(DOWNLOAD_FILE_LOOP);
}

void loop(void){
  if(Serial1.available()){
    Serial.write(Serial1.read());
  }

  if(Serial.available()){
    Serial1.write(Serial.read());
  }
}

uint16_t download_body_crc16_checksum = 0;
uint32_t download_body_bytes_received = 0;
boolean download_past_header = false; 
uint32_t download_content_length = 0;

void downloadHandleHeader(char * key, char * value){
  Serial.print("\"");
  Serial.print(key);
  Serial.print("\" => \"");
  Serial.print(value);
  Serial.println("\"");
  
  if(strcmp(key, "Content-Length") == 0){
    download_content_length = strtoul(value, NULL, 10);   
    Serial.print("Content-Length = ");
    Serial.println(download_content_length);
  }
}

uint32_t downloadProcessHeader(uint8_t * data, uint32_t data_length){
  uint32_t start_index = 0;         
  static uint8_t header_guard_index = 0; 
  static boolean past_first_line = false;
  static char key[64] = {0};
  static char value[64] = {0};
  static uint8_t key_or_value = 0;
  static uint8_t keyval_index = 0;
  
  if(!download_past_header){
    for(uint32_t ii = 0; ii < data_length; ii++){                 
      switch(header_guard_index){
      case 0:
        if(data[ii] == '\r') header_guard_index++;
        else if(data[ii] == ':'){
          key_or_value = 1;
          keyval_index = 0;
        }
        else if(past_first_line){
          if(keyval_index < 63){
            if(!((keyval_index == 0) && (data[ii] == ' '))){ // strip leading spaces
              if(key_or_value == 0) key[keyval_index++] = data[ii];
              else value[keyval_index++] = data[ii];
            }
          }
          else{
            // warning the key string doesn't fit in 64 characters
          }
        }
        break;
      case 1:
        if(data[ii] == '\n'){
          header_guard_index++;
          
          if(past_first_line){
            downloadHandleHeader((char *) key, (char *) value);
          }
          
          past_first_line = true;
          key_or_value = 0;
          keyval_index = 0;
          memset(key, 0, 64);
          memset(value, 0, 64);          
        }
        else header_guard_index = 0;        
        break;
      case 2:
        if(data[ii] == '\r') header_guard_index++;
        else{
          key[keyval_index++] = data[ii];
          header_guard_index = 0;         
        }
        break;
      case 3:
        if(data[ii] == '\n') header_guard_index++;
        else header_guard_index = 0;         
        break;
      case 4:        
        download_past_header = true;
        start_index = ii;
        header_guard_index = 0;
        break;
      }      
    }
  }  

  return start_index;
}

void downloadFile(char * hostname, uint16_t port, char * filename, void (*responseBodyProcessor)(uint8_t *, uint32_t)){      
  unsigned long total_bytes_read = 0;
  uint8_t mybuffer[64] = {0};
  
  // re-initialize the globals
  download_body_crc16_checksum = 0;
  download_body_bytes_received = 0;   
  download_past_header = false;  
  download_content_length = 0;
  
  /* Try connecting to the website.
     Note: HTTP/1.1 protocol is used to keep the server from closing the connection before all data is read.
  */ 
  esp.connect(hostname, port);
  if (esp.connected()) {   
    memset(scratch, 0, 1024);
    snprintf(scratch, 1023, "GET /%s HTTP/1.1\r\nHost: %s\r\n\r\n\r\n", filename, hostname);        
    esp.print(scratch);
    //Serial.print(scratch);    
  } else {
    Serial.println(F("Error: Server Connection failed"));    
    return;
  }

  Serial.println(F("Info: -------------------------------------"));
  
  /* Read data until either the connection is closed, or the idle timeout is reached. */ 
  unsigned long lastRead = millis(); 
  unsigned long num_bytes_read = 0;  
  unsigned long start_time = millis();
  uint32_t loop_counter = 0;
  
  while (esp.connected(false) && (millis() - lastRead < IDLE_TIMEOUT_MS)) {        
    while (esp.available()) {
      //char c = esp.read();
      num_bytes_read = esp.read(mybuffer, 64);     
      total_bytes_read += num_bytes_read;
      
      loop_counter++;
      if((loop_counter % 4096) == 0){
        Serial.print(".");
      }

      if(responseBodyProcessor != 0){
        responseBodyProcessor(mybuffer, num_bytes_read); // signal end of stream
      }        
        
      lastRead = millis();
    }
  }
  
  esp.stop();
  
  Serial.println();  
  Serial.println("Debug: Download Complete");
  Serial.print("Total Bytes: ");
  Serial.println(total_bytes_read);
  Serial.print("File Size: ");
  Serial.println(download_body_bytes_received);
  Serial.print("Checksum: ");
  Serial.println(download_body_crc16_checksum);  
  Serial.print("Duration: ");
  Serial.println(millis() - start_time);   
  
  
}

void processResponseData(uint8_t * data, uint32_t data_length){
  uint32_t start_index = downloadProcessHeader(data, data_length);
  
  if(download_past_header){
    download_body_bytes_received += data_length - start_index;
    for(uint32_t ii = start_index; ii < data_length; ii++){     
      download_body_crc16_checksum = _crc16_update(download_body_crc16_checksum, data[ii]);
    }
  }    
}