#include <ESP8266_AT_Client.h>
#include <Time.h>

#define NETWORK_SSID     "YOUR NETWORK NAME"
#define NETWORK_PASSWORD "YOUR NETWORK PASSWORD"
char npt_server[32] = "pool.ntp.org";  
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

  getNetworkTime();

}

void loop(void){
  if(Serial1.available()){
    Serial.write(Serial1.read());
  }

  if(Serial.available()){
    Serial1.write(Serial.read());
  }
}


uint32_t getNetworkTime(void){
  const unsigned long connectTimeout  = 15L * 1000L; // Max time to wait for server connection
  const unsigned long responseTimeout = 15L * 1000L; // Max time to wait for data from server  
  uint8_t       buf[48];
  unsigned long ip, startTime, t = 0L;
  
  if(esp.getHostByName(npt_server, &ip)) {
    static const char PROGMEM
      timeReqA[] = { 227,  0,  6, 236 },
      timeReqB[] = {  49, 78, 49,  52 };
    
    Serial.print(F("Info: Getting NTP Time..."));
    startTime = millis();
    do {
      esp.connectUDP(ip, 123);
    } while((!esp.connected()) &&
            ((millis() - startTime) < connectTimeout));
      
    if(esp.connected()) {      
      // Assemble and issue request packet
      memset(buf, 0, sizeof(buf));
      memcpy_P( buf    , timeReqA, sizeof(timeReqA));
      memcpy_P(&buf[12], timeReqB, sizeof(timeReqB));
      esp.write(buf, sizeof(buf));
      memset(buf, 0, sizeof(buf));
      startTime = millis();
      
      while((esp.available() < 44) &&
            ((millis() - startTime) < responseTimeout));
            
      if(esp.available() >= 44) {
        esp.read(buf, sizeof(buf));
        t = (((unsigned long)buf[40] << 24) |
             ((unsigned long)buf[41] << 16) |
             ((unsigned long)buf[42] <<  8) |
              (unsigned long)buf[43]) - 2208988800UL;      
      }
      esp.stop();
    }
    
  }

  if(t){
    tmElements_t tm;        
    breakTime(t, tm);    
    
    memset(buf, 0, 48);
    snprintf((char *) buf, 47, 
      "%d/%d/%d",
      tm.Month,
      tm.Day,
      1970 + tm.Year);
              
    Serial.print((char *) buf);
    Serial.print(" ");
    memset(buf, 0, 48);
    snprintf((char *) buf, 47, 
      "%02d:%02d:%02d",
      tm.Hour,
      tm.Minute,
      tm.Second);
   
    Serial.println((char *) buf);
    
  }
  else{
    Serial.print(F("Failed."));
  }
  
  return t;
}