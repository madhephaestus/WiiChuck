#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <NetEEPROM.h>

/* Stores network configuration of a MAC address and DHCP IP setup
 * to EEPROM.
 */void setup() {
  byte mac[] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
  NetEeprom.writeDhcpConfig(mac);
}

void loop() {
}
