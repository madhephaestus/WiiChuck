#include <DynamixelMotor.h>

DynamixelInterface &interface=*createSerialInterface(Serial);
DynamixelDevice device(interface, BROADCAST_ID);
uint8_t on=true;

void setup() {
  interface.begin(1000000);
  delay(100);
}

void loop() {
   device.write(0x19, on);
   on=!on;
   delay(1000);
}

