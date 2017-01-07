/**
 * Library that stores, reads and configures Arduino ethernet settings
 * for boards and shields based on the Wiznet 5100 chipset.
 * Both the MAC address and IP settings are stored and retrieved from EEPROM.
 * The network adapter can be configured to use DHCP or manual IP settings.
 * 24 bytes of EEPROM are required to store the configuration.
 */

/* The pin from which an anlog reading will be taken to generate a random MAC. */
#ifndef NET_RANDOM_ANALOG_PIN
#define NET_RANDOM_ANALOG_PIN 0
#endif

#ifndef NET_EEPROM_H
#define NET_EEPROM_H

#include <Arduino.h>
#include <EEPROM.h>

/* Magic number that starts the network configuration */
#define NET_EEPROM_MAGIC 0x22

class NetEEPROM {

  public:

  NetEEPROM();

  /* Sets the location in EEPROM that network configuration data will be
   * stored. If not called, then it defaults to zero. This method should
   * generally be called before any other methods.
   */
  void setEepromOffset(int offset);

  /* Configures the network adapter from settings stored in EEPROM.
   * If no settings can be found in EEPROM then a random MAC address
   * will be generated and the adapter will use DHCP for IP configuration,
   * and the MAC address will be saved to EEPROM
   */
  void begin();

  /* Writes given six byte MAC address to EEPROM and specifies that DHCP should
   * be used for configuring the IP address.
   * Parameters:
   *    mac: the six byte MAC address
   */
  void writeDhcpConfig(byte mac[]);


  /* Writes the given six byte MAC address and four byte IP configuration.
   * Parameters:
   *       mac: the six byte MAC address
   *        ip: the four byte IP address
   *       dns: the four byte IP address of the DNS server
   *        gw: the four byte IP address of the gateway
   *    subnet: the four byte subnet mask
   */
  void writeManualConfig(byte mac[], byte ip[], byte dns[], byte gw[], byte subnet[]);

  /* Reads the MAC address from EEPROM.
   * Parameters:
   *   mac: array to be populated with the six byte MAC address
   */
  void readMac(byte mac[]);

  /* Checks whether the magic number exists in the expected EEPROM location.
   * If the magic number exists then it is likely that network settings have
   * been stored in EEPROM.
   */
  int checkMagic();

  /* Returns whether the the IP address will be configured using DHCP.
   */
  int isDhcp();

  /* Reads the configured IP address from EEPROM.
   * Parameters:
   *   ip: array to be populated with the four byte IP address
   */
  void readIp(byte ip[]);

  /* Reads the DNS server address from EEPROM.
   * Parameters:
   *   dns: array to be populated with the four byte DNS server IP.
   */
  void readDns(byte dns[]);

  /* Reads the gateway address from EEPROM.
   * Parameters:
   *   gw: array to be populated with the four byte gateway IP.
   */
  void readGateway(byte gw[]);

  /* Reads the subnet mask from EEPROM.
   * Parameters:
   *   subnet: array to be populated with the four byte subnet mask.
   */
  void readSubnet(byte subnet[]);

  private:
  int eepromOffset;
  void generateRandomMac(byte mac[]);
  void readEEPROM(byte data[], int offset, int length);
  void writeEEPROM(byte data[], int offset, int length);
  void writeMac(byte mac[]);
};

extern NetEEPROM NetEeprom;

#endif
