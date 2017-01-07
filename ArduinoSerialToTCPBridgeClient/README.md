# Arduino Serial to TCP Bridge Client
Arduino client for the [Serial To TCP Bridge Protocol](https://github.com/RoanBrand/SerialToTCPBridgeProtocol) gateway service.

Open a TCP connection to a server from the Arduino using just serial. (No shields or network HW)  
See [this](https://github.com/RoanBrand/SerialToTCPBridgeProtocol) for more information on the protocol and for the **Protocol Gateway** you will need to run on the host the Arduino is connected to serially.  

## Dependencies
- [NeoHWSerial](https://github.com/SlashDevin/NeoHWSerial) - Install manually
- [CRC32](https://github.com/bakercp/CRC32) - Can be installed from the Arduino *Library Manager*

After installing the dependencies and this library, make sure you have all 3 present inside your **libraries** sub-directory of your sketchbook directory.  

## How to
- Get the [Protocol Gateway](https://github.com/RoanBrand/SerialToTCPBridgeProtocol) and build it.
- Change the gateway's config to listen on the COM port connected to your Arduino and start it.
- Your Arduino app can then use the `ArduinoSerialToTCPBridgeClient` API which is similar to the `EthernetClient` API to make tcp connections to servers as long as they are reachable from the host and the gateway is running.


## MQTT Client Example
- Install [PubSubClient](https://github.com/knolleary/pubsubclient) - Manually, or from the Arduino *Library Manager*.
- Get a MQTT Broker running on your host, listening at least on `localhost`. I used [HiveMQ](www.hivemq.com).
- Run the **Protocol Gateway** on the same host, on the right COM port.
- Open and load the example from the Arduino *Examples menu*.
- When the Arduino is connected to the MQTT broker, it will publish a message and subscribe to the `led` topic.
- You can use another MQTT client like [MQTT.fx](http://mqttfx.jfx4ee.org) to publish characters `0` and `1` to the topic `led` to toggle the led on and off on the Arduino board.

### Details
- The protocol provides the app an in order, duplicates free and error checked byte stream by adding a CRC32 and simple retry mechanism. See [this](https://en.wikibooks.org/wiki/Serial_Programming/Error_Correction_Methods) for background.
- The **Protocol Gateway** opens a real TCP connection to a set destination on behalf of the **Protocol Client** running on the Arduino, and forwards traffic bi-directionally.
- `ArduinoSerialToTCPBridgeClient` is derived from the standard Arduino `Client` class. This means existing code written for Ethernet/Wi-Fi shields should work with this.
- `NeoHWSerial` is an alternative for `Serial`, used to gain access to the AVR UART Interrupts.
- You cannot use the same serial port in your app that is being used by the protocol, e.g. You cannot use `Serial` (Serial0) when the library uses `NeoSerial`, etc.
