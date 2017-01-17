#include <ESP8266_AT_Client.h>
#include <string.h>

//#define ESP8266_AT_CLIENT_ENABLE_DEBUG
//#define ESP8266_AT_CLIENT_DEBUG_ECHO_EVERYTHING

ESP8266_AT_Client::ESP8266_AT_Client(uint8_t enable_pin){
  this->stream = &Serial; // default assumption
  this->socket_connected = false;
  this->input_buffer = NULL;
  this->input_buffer_length = 0;  
  this->input_buffer_read_ptr = NULL;
  this->input_buffer_write_ptr = NULL;
  this->input_buffer_tail_ptr = NULL;  
  this->enable_pin = enable_pin;
  this->num_consumed_bytes_in_input_buffer = 0;
  this->num_free_bytes_in_input_buffer = 0;
  this->debugEnabled = false;
  this->debugStream = NULL;
  this->tcp_keep_alive_interval_seconds = 120;
}

ESP8266_AT_Client::ESP8266_AT_Client(uint8_t enable_pin, Stream * stream){
  this->stream = stream;
  this->socket_connected = false;
  this->input_buffer = NULL;
  this->input_buffer_length = 0;    
  this->input_buffer_read_ptr = NULL;
  this->input_buffer_write_ptr = NULL;
  this->input_buffer_tail_ptr = NULL;    
  this->enable_pin = enable_pin;
  this->num_consumed_bytes_in_input_buffer = 0;
  this->num_free_bytes_in_input_buffer = 0; 
  this->debugEnabled = false;
  this->debugStream = NULL;
  this->tcp_keep_alive_interval_seconds = 120;
}

ESP8266_AT_Client::ESP8266_AT_Client(uint8_t enable_pin, Stream * stream, uint8_t * buf, uint16_t buf_length){
  this->stream = stream; 
  this->socket_connected = false;
  this->input_buffer = buf;
  this->input_buffer_length = buf_length; 
  this->input_buffer_read_ptr = buf;
  this->input_buffer_write_ptr = buf;
  this->input_buffer_tail_ptr = &(buf[buf_length-1]);
  this->enable_pin = enable_pin;  
  this->num_consumed_bytes_in_input_buffer = 0;  
  this->num_free_bytes_in_input_buffer = 0;  
  this->debugEnabled = false;
  this->debugStream = NULL;
  this->tcp_keep_alive_interval_seconds = 120;
}

void ESP8266_AT_Client::setDebugStream(Stream * ds){
  debugStream = ds;
}

boolean ESP8266_AT_Client::reset(void){  
   // reset the buffer state pointers
   input_buffer_read_ptr = input_buffer;
   input_buffer_write_ptr = input_buffer;
   input_buffer_tail_ptr = &(input_buffer[input_buffer_length-1]);
   num_consumed_bytes_in_input_buffer = 0;
   num_free_bytes_in_input_buffer = input_buffer_length;
   
   pinMode(enable_pin, OUTPUT);  
   digitalWrite(enable_pin, LOW);    
   delay(50);
   digitalWrite(enable_pin, HIGH);    
  
   ESP8266_DEBUG("ESP8266 Hello World.");

   if(readStreamUntil("ready", 5000)){
     ESP8266_DEBUG("Received 'ready'"); 
   } 
   else{
      ESP8266_DEBUG("Panic"); 
      return false;
   }
   
  return true;
}

/** Set the stream where AT commands are sent and responses received
	@param stream     the AT stream (e.g. &Serial1)
	@return returns void
 */
void ESP8266_AT_Client::setStream(Stream * stream){
  this->stream = stream;  
}

void ESP8266_AT_Client::setInputBuffer(uint8_t * buf, uint16_t buf_length){
  input_buffer = buf;
  input_buffer_length = buf_length;
  num_consumed_bytes_in_input_buffer = 0;
  num_free_bytes_in_input_buffer = buf_length;
  input_buffer_read_ptr = buf;
  input_buffer_write_ptr = buf;
  input_buffer_tail_ptr = &(buf[buf_length-1]);     
}

int ESP8266_AT_Client::connect(IPAddress ip){
  return connect(ip, 80);
}

int ESP8266_AT_Client::connect(const char *host){
  return connect(host, 80);
}

/** Connect to server by IP address
	@param ip       the IP address to connect to
	@param port     the port to connect to
	@return returns 0 if last command is still executing, 1 success, 2 if there are no resources
 */
int ESP8266_AT_Client::connect(IPAddress ip, uint16_t port, esp8266_connect_proto_t proto){
  char host[16] = {0}; // worst case 111.111.111.111 + the null terminator = 16 characters
  snprintf(host, 15, "%d.%d.%d.%d", ip[3], ip[2], ip[1], ip[0]);
  return connect(host, port, proto);
}

int ESP8266_AT_Client::connect(uint32_t ip, uint16_t port, esp8266_connect_proto_t proto){
  char host[16] = {0}; // worst case 111.111.111.111 + the null terminator = 16 characters
  IpUint32ToString(ip, (char *) host);
  return connect(host, port, proto);  
}

/** Connect to server by hostname
	@param host			the host name to resolve to an IP address and connect to
	@param port			the port to connect to
	@return returns 0 if last command is still executing, 1 success, 2 if there are no resources
 */
int ESP8266_AT_Client::connect(const char *host, uint16_t port, esp8266_connect_proto_t proto){
  // only implementing a blocking API - never return 0
  // set up an AT command and send it
  // then return whether or not it succeeded
  int ret = 2; // initialize to error
  socket_connected = false;  
  num_characters_remaining_to_receive = 0;
  
  ESP8266_DEBUG("Connecting to ", (char *) host);
  
  flushInput();
  stream->print("AT+CIPSTART=\"");
  if(proto == ESP8266_TCP){
    stream->print("TCP");
  }
  else if(proto == ESP8266_UDP){
    stream->print("UDP");
  }
  
  stream->print("\",\"");
  stream->print(host);
  stream->print("\",");
  stream->print(port);
  if(proto == ESP8266_TCP){
    stream->print(",");
    stream->print(tcp_keep_alive_interval_seconds); // keep alive interval in units of seconds
  }
  stream->print("\r\n");  
  
  // ESP8266 responds with either "OK", "ERROR", or "ALREADY CONNECT" 
  clearTargetMatchArray();
  addStringToTargetMatchList("OK");
  addStringToTargetMatchList("ERROR");
  addStringToTargetMatchList("ALREADY CONNECT");
  uint8_t match_index = 0xFF;  
  if(readStreamUntil(&match_index)){
     if(match_index == 0){ // got "OK"     
       ESP8266_DEBUG("Connected");     
       ret = 1; // success
     }  
     else if(match_index == 2){
       ESP8266_DEBUG("Already Connected");      
       ret = 1; // success            
     }
     else{     
       ESP8266_DEBUG("Failed");
       ret = 2; // error
     }     
  }
  
  if(ret == 1){
    socket_connected = true;    
    socket_type = proto;
  } 
  
  return ret;
}

int ESP8266_AT_Client::connect(IPAddress ip, uint16_t port){
  return connect(ip, port, ESP8266_TCP);
}

int ESP8266_AT_Client::connect(const char *host, uint16_t port){
  return connect(host, port, ESP8266_TCP);
}

boolean ESP8266_AT_Client::connectUDP(uint32_t ip, uint16_t port){
  boolean ret = false;
  if(connect(ip, port, ESP8266_UDP)){
    ret = true;
  }
  return ret;
}

boolean ESP8266_AT_Client::connectUDP(const char *host, uint16_t port){
  boolean ret = false;
  if(connect(host, port, ESP8266_UDP)){
    ret = true;
  }
  return ret;  
}

boolean ESP8266_AT_Client::setMacAddress(uint8_t * mac_address){
  boolean ret = false;
  char mac_str[18] = {0};
  macArrayToString(mac_address, (char *) mac_str);
  
  if(strlen(mac_str) == 17){ // e.g. 00:04:4a:23:11:7b
    flushInput();
    stream->print("AT+CIPSTAMAC_CUR=\"");
    stream->print(mac_str);
    stream->print("\"\r\n");

    clearTargetMatchArray();
    addStringToTargetMatchList("OK");
    addStringToTargetMatchList("WIFI DISCONNECT");    
    addStringToTargetMatchList("WIFI CONNECTED");    
    addStringToTargetMatchList("WIFI GOT IP"); 
    uint8_t match_index = 0xFF;
    while(readStreamUntil(&match_index, 5000)){
      if(match_index == 0){
        ret = true;
      }
    }
  }
  
  return ret;
}

// 0 : disable sleep mode
// 1 : light-sleep mode
// 2 : modem-sleep mode 
boolean ESP8266_AT_Client::sleep(uint8_t mode){
  boolean ret = false;
  stream->print("AT+SLEEP=");
  stream->print(mode);
  stream->print("\r\n");
  
  if(readStreamUntil("OK", 100)){
    ret = true;
  }
  
  return ret;
}

// note dnsServer is currently ignored as there is no direct support for it in the AT command set, afaict
// at any rate, we are currently *emulating* DNS in this library, not actually sending explicit DNS requests to a name server
boolean ESP8266_AT_Client::setStaticIPAddress(uint32_t ipAddress, uint32_t netMask, uint32_t defaultGateway, uint32_t dnsServer){
  boolean ret = false;
  char ip_str[16] = {0};
  char netMask_str[16] = {0};
  char defaultGateway_str[16] = {0};
  char dnsServer_str[16] = {0};
  
  IpUint32ToString(ipAddress, (char *) ip_str);
  IpUint32ToString(netMask, (char *) netMask_str);
  IpUint32ToString(defaultGateway, (char *) defaultGateway_str);
  IpUint32ToString(dnsServer, (char *) dnsServer_str);
  
  stream->print("AT+CIPSTA_CUR=\"");
  stream->print((char *) ip_str);
  stream->print("\",\"");
  stream->print((char *) defaultGateway_str);
  stream->print("\",\"");
  stream->print((char *) netMask_str);
  stream->print("\"\r\n");

  if(readStreamUntil("OK", 1000)){
    ret = true;
  }
  
  return ret;
}

boolean ESP8266_AT_Client::setDHCP(void){
  boolean ret = false;
  stream->print("AT+CWDHCP_CUR=1,1\r\n");
  
  clearTargetMatchArray();
  addStringToTargetMatchList("OK");
  addStringToTargetMatchList("WIFI GOT IP");  
  uint8_t match_index = 0xff;
  while(readStreamUntil(&match_index, 2000)){
    ret = true;
  }
  
  return ret;
}

/** Write a character in request
	@param c			Character to write
	@return 1 if a character was sent 0 otherwise
 */
size_t ESP8266_AT_Client::write(uint8_t c){
  uint8_t buf[1] = {c};
  return write(buf, 1);
}

/** Write a characters buffer in request
	@param buf    a buffer of bytes to send
	@param sz     the number of bytes in the buffer
	@return sz if buf was sent 0 otherwise
 */
size_t ESP8266_AT_Client::write(const uint8_t *buf, size_t sz){
  size_t ret = 0;
  
  stream->print("AT+CIPSEND=");
  stream->print(sz);
  stream->print("\r\n");

  if(readStreamUntil(">", 1000)){  
    ret = stream->write(buf, sz); // pass it along    
    
    if(readStreamUntil("SEND OK\r\n", 1000)){
       ESP8266_DEBUG("Send succeeded");     
    }
  }
  
  return ret;
}

/** Check if there is data pending receipt
	@return 1 if exists, 0 if not exists
 */
int ESP8266_AT_Client::available(){
  // an intent to read() is always preceded by a call to available(),
  // if the caller knows what is good for them,
  // so this is where we need to perform the receive 
  // in a synchronous manner (with timeout)
  
  // some state management needs to be encapsulated here as we'll only read one
  // packet in at a time and we'll keep a 1500 byte buffer around to store
  // the buffered data, hey maybe we can even implement peek correctly
  int ret = num_consumed_bytes_in_input_buffer;  
  
  if(socket_connected){
    receive(); // doesn't really matter what the response is
    ret = num_consumed_bytes_in_input_buffer;   
  }

  return ret;
}

/** Read a character from stream
	@return character
 */
int ESP8266_AT_Client::read(){
  return readFromInputBuffer();
}

/** Read from stream and copy size specified to buffer
	@param buf			Buffer		
	@param size			Buffer size
	@return bytes read
 */
int ESP8266_AT_Client::read(uint8_t *buf, size_t size){
  int num_bytes_to_read = size;
  if(num_consumed_bytes_in_input_buffer < num_bytes_to_read){
    num_bytes_to_read = num_consumed_bytes_in_input_buffer;
  }
  
  for(int ii = 0; ii < num_bytes_to_read; ii++){
    *buf++ = readFromInputBuffer();
  }
  
  return num_bytes_to_read;
}

/** Read a character from response buffer but does not move the pointer.
	@returns *input_buffer_read_ptr
 */
int ESP8266_AT_Client::peek(){
  return *input_buffer_read_ptr;
}

/** Flush response buffer
 */
void ESP8266_AT_Client::flush(){
  stream->flush();
}

/** Stop client
 */
void ESP8266_AT_Client::stop(){
  if(socket_connected || (socket_type == ESP8266_UDP)){

    // set up an AT command and send it
    // then return whether or not it succeeded
    
    flushInput();
    stream->print("AT+CIPCLOSE");
    stream->print("\r\n");   
    
    // ESP8266 responds with either "OK", "ERROR"
    clearTargetMatchArray();
    addStringToTargetMatchList("CLOSED");
    addStringToTargetMatchList("ERROR");

    uint8_t match_index = 0xFF;        
    if(readStreamUntil(&match_index, 5000)){
       if(match_index == 0){ // got "CLOSED"
         ESP8266_DEBUG("Close Succeeded");
       }  
       else{       
         ESP8266_DEBUG("Close Failed");
       }     
    }
    
  }
    
  readStreamUntil("OK", 100);
    
  socket_connected = false;  
}

/** Check if connected to server
	@return 1 if connected
 */

uint8_t ESP8266_AT_Client::connected(){
  return connected(true);
} 

// returns true *only* if the requested SSID is found
// if multiples of the SSID are found, the one with the highest RSSI is returned
boolean ESP8266_AT_Client::scanForAccessPoint(char * ssid, ap_scan_result_t * result, uint8_t * num_results_found, uint32_t timeout_ms){
  boolean ret = false;
  int8_t max_rssi = -128;
  clearTargetMatchArray();
  addStringToTargetMatchList("+CWLAP:(");
  addStringToTargetMatchList(")\r\n");
  addStringToTargetMatchList("OK\r\n");
  
  stream->print("AT+CWLAP");
  stream->print("\r\n");    
  
  uint8_t match_index = 0xFF;
  char line[128] = {0};
  uint8_t result_number = 0;
  
  while(readStreamUntil((uint8_t *) &match_index, &(line[0]), 128, timeout_ms)){
    if(match_index == 1){ // got )      
      ap_scan_result_t res = {0};
      parseScanResult(&res, line);      
      memset((char *) line, 0, 128);
      result_number++;     
      if((strcmp(&(res.ssid[0]), ssid) == 0) && (res.rssi > max_rssi)){
        *result = res;
        max_rssi = res.rssi;
        ret = true;
      }
    }
    else if(match_index == 2){
      break;
    }
  }
  
  *num_results_found = result_number;  
  
  return ret;
}

// returns true if *any* SSIDs are found
boolean ESP8266_AT_Client::scanAccessPoints(ap_scan_result_t * results, uint8_t max_num_results, uint8_t * num_results_found, uint32_t timeout_ms){
  boolean ret = false;
    
  clearTargetMatchArray();
  addStringToTargetMatchList("+CWLAP:(");
  addStringToTargetMatchList(")\r\n");
  addStringToTargetMatchList("OK\r\n");
  
  stream->print("AT+CWLAP");
  stream->print("\r\n");    
  
  uint8_t match_index = 0xFF;
  char line[128] = {0};
  uint8_t result_number = 0;
  
  while(readStreamUntil((uint8_t *) &match_index, &(line[0]), 128, timeout_ms)){
    ret = true;
    
    if(match_index == 1){ // got )
      if(result_number < max_num_results){
        parseScanResult(&(results[result_number]), line);
        memset((char *) line, 0, 128);
      }
      result_number++;      
    }
    else if(match_index == 2){
      break;
    }
  }
  
  *num_results_found = result_number;
     
  return ret;
}

void ESP8266_AT_Client::parseScanResult(ap_scan_result_t * result, char * line){
  // tokenize the line on commas
  result->security = 0;
  memset(&(result->ssid[0]), 0, 32);
  result->rssi = -128;
  memset(&(result->mac[0]), 0, 6);
  
  char * token = strtok(line, ",");
  uint8_t token_number = 0;
  uint8_t len = 0;
  while(token != NULL){
    switch(token_number){
    case 0:
      result->security = atoi(token);
      break;
    case 1: 
      // ssid is quoted
      len = strlen(token);
      if((len <= 32) && (token[0] == '\"') && (token[len - 1] == '\"')){
        token[len - 1] = NULL;
        strncpy(&(result->ssid[0]), &(token[1]), 33);         
      }
      break;
    case 2:
      result->rssi = atoi(token);
      break;
    case 3: 
      // mac address is quoted
      len = strlen(token);
      if((len <= 32) && (token[0] == '\"') && (token[len - 1] == '\"')){      
        token[len - 1] = NULL;
        stringToMacArray(&(token[1]), &(result->mac[0])); 
        break;
      }
      break;    
    default:
      break;      
    }
    token = strtok(NULL, ",");
    token_number++;
  }  
}

boolean ESP8266_AT_Client::getRemoteIp(uint32_t * ip){
  boolean ret = false;
  
  clearTargetMatchArray();
  addStringToTargetMatchList("STATUS:2");   // got ip    
  addStringToTargetMatchList("STATUS:3");   // connected
  addStringToTargetMatchList("STATUS:4");   // disconnected 
  addStringToTargetMatchList("OK\r\n");
  
  stream->print("AT+CIPSTATUS");
  stream->print("\r\n");    
     
  uint8_t match_index = 0xFF;      
  if(readStreamUntil(&match_index, 100)){
    if((match_index == 0) || (match_index == 1) || ((match_index == 2) && (socket_type == ESP8266_UDP))){
      if(readStreamUntil("+CIPSTATUS:", 100)){
        // we'll see three quotation marks before we reach the Remote IP address
        if(readStreamUntil("\"", 100) && readStreamUntil("\"", 100) && readStreamUntil("\"", 100)){          
          char remote_ip_str[16] = {0};
          if(readStreamUntil("\"", &(remote_ip_str[0]), 16, 100)){            
            ret = stringToIpUint32((char *) remote_ip_str, ip);
            readStreamUntil("OK\r\n", 100);
          }
        }
      }
    }
  } 
  
  return ret;
}
 
uint8_t ESP8266_AT_Client::connected(boolean actively_check){
  uint8_t ret = 1; // assume we are connected
  if(!socket_connected){
    return 0;
  }
  
  if(socket_connected && actively_check){
    // set up an AT command and send it
    // then return whether or not it succeeded

    clearTargetMatchArray();
    addStringToTargetMatchList("STATUS:2"); // got ip    
    addStringToTargetMatchList("STATUS:3"); // connected
    addStringToTargetMatchList("STATUS:4"); // disconnected
    addStringToTargetMatchList("OK\r\n");  
    addStringToTargetMatchList("+IPD,");    
        
    // you have to get "STATUS:" and a numeric code
    // then you have *may* get "+CIPSTATUS:"
    // then you get "OK" 
    if(num_characters_remaining_to_receive > 0){
      receive(true); // receive is already in progress    
    }
    else{    
      stream->print("AT+CIPSTATUS");
      stream->print("\r\n");  
        
      uint8_t match_index = 0xFF;      
      if(readStreamUntil(&match_index, 100)){      
        switch(match_index){
        case 2: //disconnected
          if(socket_type == ESP8266_TCP){
            socket_connected = false;      
            ret = 0;
          }   
          break;
        case 4: // +IPD, gotta deal with it
          receive(true);
          break;
        case 0: 
        case 1:
        case 3:
        default:
           // nothing to do
          break;
        }                             
      }          
      
      if(match_index != 3){ 
        if(readStreamUntil(&match_index, 100)){
          if(match_index != 3){ //  != "OK"
             ESP8266_DEBUG("CIPSTATUS not OK"); 
             // doesn't necessarily mean we're disconnected
          }
        }
        else{
          ESP8266_DEBUG("AT+CIPSTATUS expected OK, but didn't receive it");
        }          
      }  
    }  

  }  
  
  return ret;
}

uint8_t ESP8266_AT_Client::connectedToNetwork(void){
  uint8_t ret = 0; 

  clearTargetMatchArray();  
  addStringToTargetMatchList("+CWJAP_CUR:"); // connected
  addStringToTargetMatchList("No AP"); // disconnected
  addStringToTargetMatchList("OK\r\n");    
  
  stream->print("AT+CWJAP_CUR?");
  stream->print("\r\n");  
  
  uint8_t match_index = 0xFF;  
  while(readStreamUntil(&match_index, 500)){   
    if(match_index == 0){
      ret = 1;
    }
    
    if(match_index == 2){
      break;
    }
  }
  
  return ret;  
}

boolean ESP8266_AT_Client::getIPAddress(char * ip_str, char * gateway_str, char * netmask_str){
  boolean ret = false;
  
  clearTargetMatchArray();
  addStringToTargetMatchList("+CIPSTA:ip:\"");
  addStringToTargetMatchList("+CIPSTA:gateway:\"");
  addStringToTargetMatchList("+CIPSTA:netmask:\"");    
  addStringToTargetMatchList("\""); // this is tricky, since it's a subset of other match strings, it must come after them
  addStringToTargetMatchList("OK\r\n");       
  
  flushInput();
  stream->print("AT+CIPSTA?");
  stream->print("\r\n");    
  
  uint8_t match_index = 0xFF;  
  char tmp[32] = {0};
  uint8_t which_ip = 0xFF; // 0 for ip, 1 for gateway, 2 for netmask
  while(readStreamUntil(&match_index, &(tmp[0]), 32, 100)){       
    ret = true;   
    if((which_ip < 3) && (match_index == 3) && (strlen(tmp) <= 15)){
      switch(which_ip){
      case 0: 
        strcpy(ip_str, tmp); 
        break;
      case 1: 
        strcpy(gateway_str, tmp); 
        break;
      case 2: 
        strcpy(netmask_str, tmp); 
        break;
      default: break;
      }
    }
    
    which_ip = 0xFF;
    if(match_index < 3){
      which_ip = match_index;
      memset(tmp, 0, 32);
    }
    
    if(match_index == 4){
      break;
    }    
  }
  
  return ret;
}

uint32_t ESP8266_AT_Client::IpArrayToIpUint32(uint8_t * ip){
  uint32_t ret = 0;
  ret |= ((uint32_t) ip[0]) << 24;
  ret |= ((uint32_t) ip[1]) << 16;
  ret |= ((uint32_t) ip[2]) << 8;
  ret |= ((uint32_t) ip[3]) << 0;
  return ret;
}

void ESP8266_AT_Client::IpUint32ToArray(uint32_t ip, uint8_t * ip_arr){
  ip_arr[0] = (ip >> 24) & 0xff;
  ip_arr[1] = (ip >> 16) & 0xff;
  ip_arr[2] = (ip >> 8) & 0xff;
  ip_arr[3] = (ip >> 0) & 0xff;  
}

boolean ESP8266_AT_Client::stringToIpArray(char * str, uint8_t * ip){
  uint32_t ip_u32 = 0;
  boolean ret = false;
  if(stringToIpUint32(str, &ip_u32)){
    IpUint32ToArray(ip_u32, ip);
    ret = true;
  }
  return ret;
}

void ESP8266_AT_Client::IpArrayToString(uint8_t * ip, char * tgt){
  uint32_t ip_u32 = IpArrayToIpUint32(ip);
  IpUint32ToString(ip_u32, tgt);
}

void ESP8266_AT_Client::IpUint32ToString(uint32_t ip, char * tgt){
  sprintf(tgt, "%d.%d.%d.%d",
    (uint8_t) (ip >> 24),
    (uint8_t) (ip >> 16),
    (uint8_t) (ip >> 8),
    (uint8_t) (ip >> 0));    
}

boolean ESP8266_AT_Client::stringToIpUint32(char * str, uint32_t * ip){
  char * token = strtok(str, ".");
  uint8_t num_tokens = 0;
  uint32_t ip_address = 0;
  boolean ret = false;
  
  while(token != NULL){
    num_tokens++;
   
    if(num_tokens > 4){
      break;
    }
    
    if(num_tokens > 1){
      ip_address <<= 8;      
    }      
    ip_address |= atoi(token) & 0xFF;
    
    token = strtok(NULL, ".");
  }
  
  if(num_tokens == 4){
    ret = true;
    *ip = ip_address;
  }
  
  return ret;
}

void ESP8266_AT_Client::macArrayToString(uint8_t * mac, char * tgt){
  sprintf(tgt, "%02X:%02X:%02X:%02X:%02X:%02X",
    mac[0],
    mac[1],
    mac[2],
    mac[3],
    mac[4],
    mac[5]);
}

boolean ESP8266_AT_Client::stringToMacArray(char * str, uint8_t * mac){
  boolean ret = false;
  char * token = strtok(str, ":");
  uint8_t num_tokens = 0;
  uint32_t ip_address = 0;
  char local_mac[6] = {0};
  
  while(token != NULL){
    num_tokens++;
    if(num_tokens > 6){
      break;
    }
    
    if(strlen(token) == 2){
      char * temp = NULL;
      uint32_t octet = strtoul((char *) token, &temp, 16);
      if (*temp == '\0'){         
        local_mac[num_tokens-1] = octet;
      }
      else{
        break;
      }         
    }
    else{
      break;
    }
    
    token = strtok(NULL, ":");
  }
  
  if(num_tokens == 6){
    ret = true;
    memcpy(mac, local_mac, 6);
  }      
  return ret;
}

boolean ESP8266_AT_Client::getIPAddress(uint32_t * ip, uint32_t * gateway, uint32_t * netmask){
  boolean ret = false;
  char ip_str[16] = {0};
  char gateway_str[16] = {0};
  char netmask_str[16] = {0};
  ret = getIPAddress((char *) &(ip_str[0]), (char *) &(gateway_str[0]), (char *) &(netmask_str[0]));
  if(ret){
    stringToIpUint32((char *) &(ip_str[0]), ip);
    stringToIpUint32((char *) &(gateway_str[0]), gateway); 
    stringToIpUint32((char *) &(netmask_str[0]), netmask);     
  }
  
  return ret;
}

boolean ESP8266_AT_Client::getMacAddress(char * mac_str){
  boolean ret = false;
  
  clearTargetMatchArray();
  addStringToTargetMatchList("+CIFSR:STAMAC,\""); // connected
  addStringToTargetMatchList("OK\r\n");    
  addStringToTargetMatchList("ERROR\r\n");    
  
  flushInput();
  stream->print("AT+CIFSR");
  stream->print("\r\n"); 
  
  uint8_t match_index = 0xFF;
  if(readStreamUntil(&match_index, 100)){   
    if(match_index == 0){
      char tmp[32] = {0};
      if(readStreamUntil("\"", &(tmp[0]), 32, 10)){
        strncpy(mac_str, tmp, 18); // an mac address is at most 17 characters
        ret = true;
      }
      
      readStreamUntil("OK", 100); // clear out the OK   
    }
  }
  else{
    // Timeout
  }
  
  return ret;  
}

boolean ESP8266_AT_Client::getMacAddress(uint8_t * mac){
  boolean ret = false;
  char tmp[18] = {0};  
  if(getMacAddress((char *) tmp)){
    ret = stringToMacArray((char *) tmp, mac);
  }

  return ret;
}

boolean ESP8266_AT_Client::getHostByName(const char *hostname, uint32_t *ip, uint32_t timeout_ms){
  // connect to host name on port 7
  // then read the connection status to get teh remote IPAddress
  boolean ret = false;
  
  socket_type = ESP8266_UDP;
  
  stream->print("AT+CIPSTART=\"UDP\",\"");
  stream->print(hostname);
  stream->print("\",7\r\n");
  
  if(readStreamUntil("OK", timeout_ms)){     
    uint32_t remote_ip = 0;
    ret = getRemoteIp(&remote_ip);    
    if(ret){
      *ip = remote_ip;
    }
  }
  
  stop();
  
  return ret;
}

ESP8266_AT_Client::operator bool(){
  return (connected()==1);
}


/** Adds str to list safely
	@param list			
	@param str	
	@param max_num_entries 
	@return returns true if the item is successfully added and false otherwise
 */
boolean ESP8266_AT_Client::addStringToTargetMatchList(char * str){
  uint16_t free_index = 0xFFFF;
  
  if(strlen(str) <= ESP8266_AT_CLIENT_MAX_STRING_LENGTH){
    
    // search the list for an empty space
    // the last empty space must remain empty
    // so don't include it in the search
    for(uint16_t ii = 0; ii < ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES; ii++){      
      uint16_t len = strlen(&(target_match_array[ii][0]));  
      if(0 == len){
        free_index = ii;
        break;
      }
    }
        
    // free index is the first empty space in the list
    // or 0xFFFF if no free entries were found
    
    // if free index points to a viable position
    // then copy the candidate string into that position
    // and limit the number of characters copied
    if(free_index < ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES){         
      char * tgt_addr = &(target_match_array[free_index][0]);
      strncpy(tgt_addr, str, ESP8266_AT_CLIENT_MAX_STRING_LENGTH + 1);  // copy the string in      
      target_match_lengths[free_index] = strlen(tgt_addr);
      return true;
    }
    
  }
  
  return false;
}

// pass in an array of strings to match against
// the list is presumed to be terminated by a NULL string
// this function can only handle up to ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES matching targets
// TODO: this function should get another argument to reset its internal state
//       because the caller decides it failed irrecoverably for some reason it might never recover?
boolean ESP8266_AT_Client::readStreamUntil(uint8_t * match_idx, char * target_buffer, uint16_t target_buffer_length, int32_t timeout_ms){
  boolean match_found = false;
  static boolean initial_call = true;
  static uint16_t local_target_buffer_index = 0;
  static uint8_t match_char_idx[ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES] = {0};  
  unsigned long previousMillis = millis();      
  
#ifdef ESP8266_AT_CLIENT_ENABLE_DEBUG
  if((debugStream != NULL) && debugEnabled){
    ESP8266_DEBUG("+++");
    for(uint8_t ii = 0; ii < ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES; ii++){
      uint16_t target_match_length = target_match_lengths[ii];
      if(target_match_length == 0){ 
        break;
      }
      ESP8266_DEBUG("  Waiting for ", target_match_array[ii]);  
    }
    ESP8266_DEBUG("===");    
  }
#endif  
  
  if(initial_call){
    initial_call = false;
    //debugStream->println("\nbegin>");
  }
  
  while(!match_found){ // until a match is found
    unsigned long currentMillis = millis();
    if((timeout_ms > 0) && (currentMillis - previousMillis >= timeout_ms)){
       break;
    }

    if(stream->available()){
      previousMillis = millis(); // reset the timeout
      char chr = stream->read(); // read a character

#ifdef ESP8266_AT_CLIENT_DEBUG_ECHO_EVERYTHING     
      if(debugStream != NULL && debugEnabled) debugStream->print(chr); // echo the received characters to the Serial Monitor
#endif
      
      // for each target match
      for(uint8_t ii = 0; ii < ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES; ii++){
        uint16_t target_match_length = target_match_lengths[ii];
        // an empty string in the list signals the end of the list
        if(target_match_length == 0){ 
          break;
        } 
        
        // if the current character is a match for this string
        // advance it's match index, 
        // otherwise reset its match index
        if(chr == target_match_array[ii][match_char_idx[ii]]){
           match_char_idx[ii]++;
        }
        else{
           match_char_idx[ii] = 0;
        }
        
        // if the match index is equal to the length of the string
        // then it's a complete match
        // return the string index that matched into match_idx
        // and return true to the caller
        if(match_char_idx[ii] >= target_match_length){      
          *match_idx = ii;
          match_found = true;
          
          // copy the collected local data into the caller's buffer
          if(target_buffer != NULL){
            if(local_target_buffer_index > target_buffer_length){
              ESP8266_DEBUG("Warn: caller's buffer is smaller than needed to contain", target_buffer);
            }
          }
          
          // reset the stateful variables          
          memset(match_char_idx, 0, ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES);
          local_target_buffer_index = 0;   
          initial_call = true;                     
          //debugStream->println("<end");
          break;
        }        
      }
      
      if(!match_found && target_buffer != NULL){
        if(local_target_buffer_index < target_buffer_length - 1){
          target_buffer[local_target_buffer_index++] = chr;
        }
        else{
          ESP8266_DEBUG("Target buffer would overflow");
          break; // target buffer overflow
        }
      }      
    }
  }
  
  ESP8266_DEBUG("*** ", (uint8_t) match_found);
  ESP8266_DEBUG("==> ", (uint8_t) *match_idx);
   
  return match_found;   
}

// pass a single string to match against
// the string must not be longer than 31 characters
boolean ESP8266_AT_Client::readStreamUntil(char * target_match, char * target_buffer, uint16_t target_buffer_length, int32_t timeout_ms){  
  uint8_t dummy_return;
  
  if(strlen(target_match) > 31){
    return false; 
  }
  else{
    clearTargetMatchArray();
    addStringToTargetMatchList(target_match);
    return readStreamUntil(&dummy_return, target_buffer, target_buffer_length, timeout_ms);
  }
}

boolean ESP8266_AT_Client::readStreamUntil(char * target_match, int32_t timeout_ms){
  return readStreamUntil(target_match, NULL, 0, timeout_ms);
}

boolean ESP8266_AT_Client::readStreamUntil(char * target_match){
  return readStreamUntil(target_match, -1);
}

boolean ESP8266_AT_Client::readStreamUntil(uint8_t * match_idx, int32_t timeout_ms){
  return readStreamUntil(match_idx, NULL, 0, timeout_ms);
}

boolean ESP8266_AT_Client::readStreamUntil(uint8_t * match_idx){
  return readStreamUntil(match_idx, -1);  
}

// writes c to the write pointer in the input buffer
// otherwise the write pointer is advanced
boolean ESP8266_AT_Client::writeToInputBuffer(uint8_t c){
  if(num_free_bytes_in_input_buffer > 0){
    *input_buffer_write_ptr = c; // write the value to the buffer
    // update the write pointer to the next location to write to within the buffer
    if(input_buffer_write_ptr == input_buffer_tail_ptr){
      input_buffer_write_ptr = input_buffer;
    }
    else{
      input_buffer_write_ptr++;
    }
    
    num_consumed_bytes_in_input_buffer++;
    num_free_bytes_in_input_buffer--;
    
    //debugStream->write(c);    
       
    return true;
  }
  else{
    ESP8266_DEBUG("Input Buffer Underflow!");
    return false;
  }
}

// returns the character at the current read pointer
// there are no bytes available to read
// the read pointer is not advanced
// otherwise the read pointer is advanced
uint8_t ESP8266_AT_Client::readFromInputBuffer(void){  
  if(num_consumed_bytes_in_input_buffer > 0){    
    uint8_t ret = *input_buffer_read_ptr;
    
    if(input_buffer_read_ptr == input_buffer_tail_ptr){
      input_buffer_read_ptr = input_buffer;      
    }
    else{
      input_buffer_read_ptr++;
    }
    
    num_consumed_bytes_in_input_buffer--;
    num_free_bytes_in_input_buffer++;

    //debugStream->write(ret);
    
    return ret;
  }
  
  return -1;
}

void ESP8266_AT_Client::flushInput(){
  while(num_consumed_bytes_in_input_buffer > 0){ 
    char chr = stream->read();
    
#ifdef ESP8266_AT_CLIENT_DEBUG_ECHO_EVERYTHING    
    if(debugStream != NULL && debugEnabled) debugStream->println((uint8_t) chr, HEX); // echo the received characters to the Serial Monitor   
#endif    

  }  
}

// 1 is Station Mode
// 2 is SoftAP Mode
// 3 is SoftAP + Station Mode
boolean ESP8266_AT_Client::setNetworkMode(uint8_t mode){
  boolean ret = false;
  
  flushInput();  
  stream->print("AT+CWMODE_CUR=");
  stream->print(mode);
  stream->print("\r\n");    

  // ESP8266 responds with either "OK", "ERROR"
  clearTargetMatchArray();
  addStringToTargetMatchList("OK");
  addStringToTargetMatchList("ERROR");
  
  uint8_t match_index = 0xFF;
  if(readStreamUntil(&match_index)){
     if(match_index == 0){
       ESP8266_DEBUG("Debug: NetworkMode Succeeded");
       ret = true;
     }  
     else{
       ESP8266_DEBUG("Debug: NetworkMode Failed");
       ret = false;
     }     
  }
  
  flushInput();
  return ret;    
}

boolean ESP8266_AT_Client::connectToNetwork(char * ssid, char * pwd, int32_t timeout_ms, void (*onConnect)(void)){
  flushInput();
  stream->print("AT+CWJAP_CUR=\"");
  stream->print(ssid);
  stream->print("\",\"");
  stream->print(pwd);
  stream->print("\"");
  stream->print("\r\n");
  
  // wait for connected status
  if(readStreamUntil("WIFI CONNECTED", timeout_ms)){

    ESP8266_DEBUG("Connected to Network"); 
    if(onConnect != NULL){
      onConnect();
    }
    // wait for got IP status
    if(readStreamUntil("WIFI GOT IP", 60000)){
       ESP8266_DEBUG("Got IP"); 
 
       if(readStreamUntil("OK")){
         return true;
       }      
    }
    else{
       ESP8266_DEBUG("Failed to get IP address"); 
       return false;
    }      
  }
  else{  
     ESP8266_DEBUG("Failed to connect to Network");    
     return false;    
  }
  
  return false;
}

//TODO: Implement
boolean ESP8266_AT_Client::disconnectFromNetwork(){
  boolean ret = false;
  
  flushInput();
  stream->print("AT+CWQAP");
  stream->print("\r\n");  
  
  if(readStreamUntil("WIFI DISCONNECT", 1000)){
     ESP8266_DEBUG("Disconnected from Network");    
     ret = true;
  }
  else{
     ESP8266_DEBUG("Failed to disconnect from Network");
  }

  return ret;
}

boolean ESP8266_AT_Client::setTcpKeepAliveInterval(uint16_t _tcp_seconds){
  if(_tcp_seconds <= 7200){
    tcp_keep_alive_interval_seconds = _tcp_seconds;
    return true;
  }
  return false;
}

void ESP8266_AT_Client::receive(boolean delegate_received_IPD){    
  static enum {WAITING_FOR_IPD, WAITING_FOR_IPD_OR_CLOSED, WAITING_FOR_COLON, PROCESSING_IPD} receive_state = WAITING_FOR_IPD;
  uint8_t match_index = 0xFF;
    
  if(num_free_bytes_in_input_buffer == 0){
    // trying to receive more is unproductive until we get rid of some that we alrady have
    return;
  }
  
  if(stream->available() == 0){
    // there's nothing in the hardware buffer to digest
    return;
  }
      
  if(delegate_received_IPD){
    if(receive_state == WAITING_FOR_IPD || receive_state == WAITING_FOR_IPD_OR_CLOSED){
      receive_state = WAITING_FOR_COLON;
      ESP8266_DEBUG("Rx State = WAITING_FOR_COLON (1)");
    }
    else{
      ESP8266_DEBUG("Unexpected delegate call to receive");
    }    
  }  
    
  if(receive_state == WAITING_FOR_IPD){
    clearTargetMatchArray();
    addStringToTargetMatchList("+IPD,");
    addStringToTargetMatchList("OK");
    if(readStreamUntil(&match_index, 10) && (match_index == 0)){
      // we got +IPD,
      receive_state = WAITING_FOR_COLON;
      ESP8266_DEBUG("Rx State = WAITING_FOR_COLON (2)");
    }
  }
  else if(receive_state == WAITING_FOR_IPD_OR_CLOSED){
    clearTargetMatchArray();
    addStringToTargetMatchList("+IPD,");    
    addStringToTargetMatchList("CLOSED");   
    addStringToTargetMatchList("OK");
    if(readStreamUntil(&match_index, 10)){
      if(match_index == 0){      
        // we got +IPD,
        receive_state = WAITING_FOR_COLON;
        ESP8266_DEBUG("Rx State = WAITING_FOR_COLON (3)");
      }
      else if(match_index == 1){
        // we got CLOSED
        socket_connected = false;        
        receive_state = WAITING_FOR_IPD;
        ESP8266_DEBUG("Rx State = WAITING_FOR_IPD");
        return; // we're done here
      }
    }      
  }  
 
  if(receive_state == WAITING_FOR_COLON){
    clearTargetMatchArray();
    addStringToTargetMatchList(":");    
    char tmp[32] = {0};
    if(readStreamUntil(&match_index, &(tmp[0]), 32, 10)){
      if(match_index == 0){
        char * temp = NULL;
        uint32_t num_characters_expected = strtoul((char *) tmp, &temp, 10);
        if (*temp != '\0'){
          ESP8266_DEBUG("Debug: Receive Error, length parse error on ", temp);  
          //TODO: this is a disaster... caller should be able to know about it if it happens
          receive_state = WAITING_FOR_IPD_OR_CLOSED;
          ESP8266_DEBUG("Rx State = WAITING_FOR_IPD_OR_CLOSED (1)");
        }
        else{
          num_characters_remaining_to_receive = num_characters_expected;
          receive_state = PROCESSING_IPD;
          ESP8266_DEBUG("Rx State = PROCESSING_IPD");
        }
      }
    }
  }  
    
  if(receive_state == PROCESSING_IPD){
    ESP8266_DEBUG("Remaining: ", num_characters_remaining_to_receive);
    uint32_t bytes_read_this_cycle = 0;
    while((num_characters_remaining_to_receive > 0) && (num_free_bytes_in_input_buffer > 0) && stream->available()){
      uint8_t ch = stream->read();
      
      bytes_read_this_cycle++;
#ifdef ESP8266_AT_CLIENT_DEBUG_ECHO_EVERYTHING
      if(debugStream != NULL && debugEnabled) debugStream->write(ch);
#endif
      
      if(writeToInputBuffer(ch)){            
        num_characters_remaining_to_receive--;
      }      
      else{
        ESP8266_DEBUG("writeToInputBuffer failed");
        break;
      }
    }
    
    if(num_characters_remaining_to_receive == 0){
      receive_state = WAITING_FOR_IPD_OR_CLOSED;
      ESP8266_DEBUG("Rx State = WAITING_FOR_IPD_OR_CLOSED (2)");
    }    
  }
}

void ESP8266_AT_Client::ESP8266_DEBUG(char * msg){
#ifdef ESP8266_AT_CLIENT_ENABLE_DEBUG
  if((debugStream != NULL) && debugEnabled){       
    debugStream->print("Debug: ");     
    debugStream->println(msg);
    debugStream->flush();
  }
#endif
}

void ESP8266_AT_Client::ESP8266_DEBUG(char * msg, uint16_t value){
#ifdef ESP8266_AT_CLIENT_ENABLE_DEBUG  
  if((debugStream != NULL) && debugEnabled){  
    debugStream->print("Debug: ");     
    debugStream->print(msg);
    debugStream->println(value);
    debugStream->flush();
  }
#endif
}

void ESP8266_AT_Client::ESP8266_DEBUG(char * msg, char * value){
#ifdef ESP8266_AT_CLIENT_ENABLE_DEBUG  
  if((debugStream != NULL) && debugEnabled){  
    debugStream->print("Debug: ");     
    debugStream->print(msg);
    debugStream->print("\"");
    debugStream->print(value);
    debugStream->println("\"");
    debugStream->flush();
  }
#endif
}

void ESP8266_AT_Client::enableDebug(void){
#ifdef ESP8266_AT_CLIENT_ENABLE_DEBUG  
  debugEnabled = true;
#endif  
}

void ESP8266_AT_Client::disableDebug(void){
#ifdef ESP8266_AT_CLIENT_ENABLE_DEBUG  
  debugEnabled = false;
#endif  
}

void ESP8266_AT_Client::clearTargetMatchArray(void){
  for(uint8_t ii = 0; ii < ESP8266_AT_CLIENT_MAX_NUM_TARGET_MATCHES; ii++){
    target_match_array[ii][0] = NULL;
    target_match_lengths[ii] = 0;
  }
}
