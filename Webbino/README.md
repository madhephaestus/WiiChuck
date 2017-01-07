# Webbino

Webbino is a webserver for Arduino.  I originally wrote it for my [SmartStrip
project](https://github.com/SukkoPera/SmartStrip), which required several unique
features which were not present in other webservers that were available at the
time:
- Compatible with many network interfaces:
  - WizNet W5100: [Ethernet Shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield)
  - WizNet W5500: [Ethernet Shield 2](http://www.arduino.org/products/shields/arduino-ethernet-shield-2)
  - ENC28J60: [KMTronic DINo](http://sigma-shop.com/product/72/web-internet-ethernet-controlled-relay-board-arduino-compatible-rs485-usb.html) (first version)
  - WINC1500 (Wi-Fi): [WiFi Shield 101](https://www.arduino.cc/en/Main/ArduinoWiFiShield101), [MKR1000](https://www.arduino.cc/en/Main/ArduinoMKR1000)
  - ESP8266 (Wi-Fi): Either as an add-on to Arduino, like the [MLT shield](http://www.mlt-group.com/Products/Wifi-Wireless-Modules/ESP8266-WIFI-Shield-with-ESP-01-ESP-07-ESP-12-for-Arduino), or standalone like [NodeMCU](http://www.nodemcu.com), [WeMos D1](https://www.wemos.cc/product/d1.html)/[D1 Mini](https://www.wemos.cc/product/d1-mini.html), etc...
- HTML pages and other data can be embedded in the code. This avoids the need of
  an SD card for simple sites.
- If you prefer, HTML pages can also be stored on an SD card (long file names are
  supported!)
- Tags like "#WHATEVER#" in HTML pages can be replaced at page request time,
  allowing the delivery of dynamic contents.
- Arbitrary functions can be associated to a page, to perform any needed
  actions.

To switch between the supported chipsets, and to configure various working aspects of the webserver, have a look at the _webbino_config.h_ file: the _WEBBINO_USE\_*_ #defines will enable support for the chosen network interface.

Included with Webbino are a lot of examples showing how to use all the different features.

## Supported boards
### WizNet W5100
Support for this chip is included in the standard distribution of the Arduino IDE, so it can be used stright away.

### WizNet W5500
This chip is used on the Ethernet Shield 2 by arduino.org. It is supported through the Ethernet 2 library which (I suppose, as I don't use it) is included in the standard distribution of the arduino.org IDE (1.7.x), but it can also be installed through the Library Manager in the arduino.cc IDE (1.6.x).

### ENC28J60
The ENC28J60 Ethernet Controller is supported it through the [EtherCard library](https://github.com/jcw/ethercard/).

When ENC28J60 support is enabled, a lot of RAM is used for the packet buffer, as EtherCard currently needs it to be allocated in the main RAM. It is currently set at 800 bytes, which include TCP/IP headers, leaving about 750 bytes for data to be sent to the client. Since the library also forces all communications to be single-packet, about 750 bytes is the maximum size of any data that is exchanged between server and client (i.e.: of webpages).

### WINC1500
WINC1500 support is obtained through the WiFi101 library that can be installed through the Library Manager.

### ESP8266
ESP8266 is supported either as an add-on to Arduino or standalone:
- In the former case, you will need Bruno Portaluri's WiFiEsp library available in the Library Manager or at https://github.com/bportaluri/WiFiEsp.
- In the latter case you will need to install the ESP8266 core, available at https://github.com/esp8266/Arduino.

## Storing pages
Web pages can be stored in Arduino's flash memory (where code is stored) and/or on an SD card.

### Flash memory
If you only need to serve one or a few pages, storing them together with the code is a great idea, as you will spare your SD card for some better job.

Any file that must be available on the webserver can be converted to a C header file throught a Python script that is available in the "tools" directory. It will also produce some code that you need to include in your sketch. Please have a look at the examples if you need help.

### SD card
If flash space is an issue or if you just prefer to store your pages on an SD card (to make them easily editable, for instance), you are welcome to do so. You will need to enable SD support by uncommenting one of two #defines in _webbino_config.h_:
- WEBBINO_ENABLE_SD: This will use Arduino's SD library, which only allows DOS-style (i.e. 8+3 characters) file names. This implies that you will have to name your pages with a _.htm_ extension, instead of _.html_.
- WEBBINO_ENABLE_SDFAT: This will use the [SDFat library](https://github.com/greiman/SdFat), which allows access to files with long names (LFNs), if properly configured (see _SdFatConfig.h_ in the library sources).

## Replacement Tags
Webbino allows easy embedding of small pieces of dynamic content in webpages. If, for instance, all you have to do is show some instant weather data in an otherwise static page, just use the Tag feature: put placeholders like _#TEMPERATURE#_, _#HUMIDITY#_ and such in your HTML page, and have Webbino replace them with actual data measured when the page is loaded! Have a look at the _ReplacementTags_ example to get started.

## License
Webbino is free software: you can redistribute it and/or modify
it under the terms of version 3 of the GNU General Public License as published by the Free Software Foundation.

Webbino is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Webbino. If not, see <http://www.gnu.org/licenses/>.

## Support
Please use the [official github page](https://github.com/SukkoPera/Webbino) to report issues, suggestions and such.
