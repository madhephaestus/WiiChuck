#include "NetEEPROM.h"
#include "SPI.h"
#include "Ethernet.h"


#define MAC_OFFSET (eepromOffset + 1)
#define DHCP_OFFSET (MAC_OFFSET + 6)
#define IP_OFFSET (DHCP_OFFSET + 1)
#define DNS_OFFSET (IP_OFFSET + 4)
#define GW_OFFSET (DNS_OFFSET + 4)
#define SUBNET_OFFSET (GW_OFFSET + 4)

NetEEPROM::NetEEPROM(void) {
  eepromOffset = 0;
}

void NetEEPROM::setEepromOffset(int offset) {
  eepromOffset = offset;
}

void NetEEPROM::begin() {
  delay(250); // For ethernet instantiation
  byte mac[6];
  if (checkMagic()) {
    readMac(mac);
  } else {
    generateRandomMac(mac);
    writeDhcpConfig(mac);
  }

  if (isDhcp()) {
    Ethernet.begin(mac);
  } else {
    byte ip[4];
    byte dns[4];
    byte gw[4];
    byte subnet[4];

    readIp(ip);
    readDns(dns);
    readGateway(gw);
    readSubnet(subnet);

    Ethernet.begin(mac, ip, dns, gw, subnet);
  }
}

void NetEEPROM::writeDhcpConfig(byte mac[]) {
  writeMac(mac);
  EEPROM.write(DHCP_OFFSET, 1);
}

void NetEEPROM::writeManualConfig(byte mac[], byte ip[], byte dns[], byte gw[], byte subnet[]) {
  writeMac(mac);
  EEPROM.write(DHCP_OFFSET, 0);
  writeEEPROM(ip, IP_OFFSET, 4);
  writeEEPROM(dns, DNS_OFFSET, 4);
  writeEEPROM(gw, GW_OFFSET, 4);
  writeEEPROM(subnet, SUBNET_OFFSET, 4);
}

int NetEEPROM::isDhcp() {
  return EEPROM.read(DHCP_OFFSET) != 0;
}

void NetEEPROM::readMac(byte mac[]) {
  readEEPROM(mac, MAC_OFFSET, 6);
}

void NetEEPROM::readIp(byte ip[]) {
  readEEPROM(ip, IP_OFFSET, 4);
}

void NetEEPROM::readDns(byte dns[]) {
  readEEPROM(dns, DNS_OFFSET, 4);
}

void NetEEPROM::readGateway(byte gw[]) {
  readEEPROM(gw, GW_OFFSET, 4);
}

void NetEEPROM::readSubnet(byte subnet[]) {
  readEEPROM(subnet, SUBNET_OFFSET, 4);
}

int NetEEPROM::checkMagic() {
  return EEPROM.read(eepromOffset) == NET_EEPROM_MAGIC;
}

void NetEEPROM::generateRandomMac(byte mac[]) {
  randomSeed(analogRead(NET_RANDOM_ANALOG_PIN));
  mac[0] = 0xDE;
  mac[1] = 0xAD;
  mac[2] = 0xBE;
  mac[3] = 0xEF;
  mac[4] = random(255);
  mac[5] = random(255);
}

void NetEEPROM::readEEPROM(byte data[], int offset, int length) {
  for (int i = 0; i < length; i++) {
    data[i] = EEPROM.read(offset + i);
  }
}

void NetEEPROM::writeEEPROM(byte data[], int offset, int length) {
  for (int i = 0; i < length; i++) {
    EEPROM.write(offset + i, data[i]);
  }
}

void NetEEPROM::writeMac(byte mac[]) {
  EEPROM.write(eepromOffset, NET_EEPROM_MAGIC);
  writeEEPROM(mac, MAC_OFFSET, 6);
}

NetEEPROM NetEeprom;
