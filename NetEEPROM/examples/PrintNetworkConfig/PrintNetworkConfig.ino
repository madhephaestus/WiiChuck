#include <EEPROM.h>
#include <SPI.h>
#include <Ethernet.h>
#include <NetEEPROM.h>

/* Prints the network configuration stored in EEPROM. */
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    // Wait for serial for Leonardo
  }

  if (NetEeprom.checkMagic()) {
    byte mac[6];
    NetEeprom.readMac(mac);
    Serial.print("MAC: "); printMac(mac); Serial.println();
    if (NetEeprom.isDhcp()) {
      Serial.println("Network configured using DHCP");
    } else {
      byte addr[4];
      NetEeprom.readIp(addr);
      Serial.print("IP: "); printIp(addr); Serial.println();
      NetEeprom.readDns(addr);
      Serial.print("DNS: "); printIp(addr); Serial.println();
      NetEeprom.readGateway(addr);
      Serial.print("GW: "); printIp(addr); Serial.println();
      NetEeprom.readSubnet(addr);
      Serial.print("Subnet: "); printIp(addr); Serial.println();
    }
  } else {
    Serial.println("Network MAC and IP have not been configured");
  }
}

void loop() {
}

void printMac(byte mac[]) {
  for (int i = 0; i < 6; i++) {
    if (i > 0) {
      Serial.print(":");
    }
    if (mac[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
  }
}

void printIp(byte ip[]) {
  for (int i = 0; i < 4; i++) {
    if (i > 0) {
      Serial.print(".");
    }
    Serial.print(ip[i]);
  }
}
