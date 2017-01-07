
/***************************************************
  Adafruit CC3000 Multicast DNS Echo Server
    
  This is a simple example of an echo server that responds
  to multicast DNS queries on the 'cc3kserver.local' address.

  See the CC3000 tutorial on Adafruit's learning system
  for more information on setting up and using the
  CC3000:
    http://learn.adafruit.com/adafruit-cc3000-wifi  
    
  Requirements:
  
  This sketch requires the Adafruit CC3000 library.  You can
  download the library from:
    https://github.com/adafruit/Adafruit_CC3000_Library
  
  For information on installing libraries in the Arduino IDE
  see this page:
    http://arduino.cc/en/Guide/Libraries
  
  You must also have MDNS/Bonjour support installed on machines
  that will try to access the server:
  - For Mac OSX support is available natively in the operating
    system.
  - For Linux, install Avahi: http://avahi.org/
  - For Windows, install Bonjour: http://www.apple.com/support/bonjour/
  
  Usage:
    
  Update the SSID and, if necessary, the CC3000 hardware pin 
  information below, then run the sketch and check the 
  output of the serial port.  Once listening for connections, 
  connect to the server from your computer  using a telnet 
  client on port 7.  
           
  For example on Linux or Mac OSX, if your CC3000 has an
  IP address 192.168.1.100 you would execute in a command
  window:
  
    telnet cc3kserver.local 7
           
  After connecting, notice that as you type input and 
  press enter to send it the CC3000 will echo back exactly
  what you typed.  Press ctrl-] and type quit at the prompt 
  to close the telnet session.
           
  On Windows you'll need to download a telnet client.  PuTTY 
  is a good, free GUI client: 
    http://www.chiark.greenend.org.uk/~sgtatham/putty/
  
  License:
 
  This example is copyright (c) 2013 Tony DiCola (tony@tonydicola.com)
  and is released under an open source MIT license.  See details at:
    http://opensource.org/licenses/MIT
  
  This code was adapted from Adafruit CC3000 library example 
  code which has the following license:
  
  Designed specifically to work with the Adafruit WiFi products:
  ----> https://www.adafruit.com/products/1469

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution      
 ****************************************************/
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <CC3000_MDNS.h>

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed

#define WLAN_SSID       "myNetwork"           // cannot be longer than 32 characters!
#define WLAN_PASS       "myPassword"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           7    // What TCP port to listen on for connections.

Adafruit_CC3000_Server echoServer(LISTEN_PORT);

MDNSResponder mdns;

void setup(void)
{  
  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n")); 

  // Set up CC3000 and get connected to the wireless network.
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  Serial.println();
  
  // Start multicast DNS responder
  if (!mdns.begin("cc3kserver", cc3000)) {
    Serial.println(F("Error setting up MDNS responder!"));
    while(1); 
  }
   
  // Start server
  echoServer.begin();
  
  Serial.println(F("Listening for connections..."));
}

void loop(void)
{
  // Handle any multicast DNS requests
  mdns.update();
  
  // Handle any connected clients
  Adafruit_CC3000_ClientRef client = echoServer.available();
  if (client) {
     // Check if there is data available to read and echo it back.
     if (client.available() > 0) {
       uint8_t ch = client.read();
       client.write(ch);
     }
  }
}

