NetEEPROM
=========

Arduino library that reads stores network settings to EEPROM. This is
useful to associate a network configuration with a specific Arduino
board which can then be read by many sketches without having to be
set in each sketch.

This library stores to EEPROM:
* the MAC address of the interface
* whether DHCP should be used to configure IP settings
* if DHCP is not used, then the manual IP settings for the interface

The library also contains a function to configure the Ethernet adapter
from the configuration stored in EEPROM. If network configuration has
not been stored in EEPROM then the following configuration will apply:
* A random MAC address in the form DE:AD:BE:EF:xx:xx
  (where xx is a random number between 0 and 255) will be generated
  and stored in EEPROM.
* The IP address will be configured using DHCP, and this configuration
  will be stored in EEPROM

Usage
-----

To use the the NetEEPROM library the sketch will need to contain the
following includes:

    #include <EEPROM.h>
    #include <SPI.h>
    #include <Ethernet.h>
    #include <NetEEPROM.h>

To set a MAC address with DHCP configuration, use the following code:

    byte mac[6] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
    NetEeprom.writeDhcpConfig(mac);

To set a MAC address with manual IP configuration, use the following code:

    byte mac[6] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
    byte ip[4] = { 192, 168, 1, 100 };
    byte dns[4] = { 8, 8, 8, 8 };
    byte gw[4] = { 192, 168, 1, 1 };
    byte subnet[4] = { 255, 255, 255, 0 };
    NetEeprom.writeManualConfig(mac, ip, dns, gw, subnet);

To set up the Ethernet adapter with configuration from EEPROM, use the
following line:

    NetEeprom.begin();

If you wish to configure the start address in EEPROM where the network
configuration resides, call the `setEepromOffset()` following method 
**before** calling any other methods. If you do not call this method, then
the default EEPROM location set set to 0. To set the network configuration at
a custom EEPROM offset, use the following code.

    NetEeprom.setEepromOffet(128);
    byte mac[6] = { 0xCA, 0xFE, 0xBA, 0xBE, 0x00, 0x00 };
    NetEeprom.writeDhcpConfig(mac);

To read network configuration at a custom EEPROM offset, use the following
code:

    NetEeprom.setEepromOffset(128);
    NetEeprom.begin();