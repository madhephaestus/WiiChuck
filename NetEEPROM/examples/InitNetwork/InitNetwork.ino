#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <NetEEPROM.h>

/* Example of how to configure the network interface from configuration stored in EEPROM. */
void setup() {
  // Instantiate network from stored netowrk paramters in EEPROM.
  // If not in EEPROM then a random MAC will be generated and stored in EEPROM
  // in the form DE:AD:BE:EF:xx:xx where xx is a random number from 0 to 255.
  // If the netowrk is not configured in EEPROM, then DHCP will be used.
  NetEeprom.begin();
}

void loop() {
}

