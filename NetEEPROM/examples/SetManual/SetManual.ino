#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <NetEEPROM.h>

/* Stores network configuration of a MAC address and manual IP setup
 * to EEPROM.
 */
void setup() {
  byte mac[6] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
  byte ip[4] = { 192, 168, 1, 100 };
  byte dns[4] = { 8, 8, 8, 8 };
  byte gw[4] = { 192, 168, 1, 1 };
  byte subnet[4] = { 255, 255, 255, 0 };

  NetEeprom.writeManualConfig(mac, ip, dns, gw, subnet);
}

void loop() {
}
