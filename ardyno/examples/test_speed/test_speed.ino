#include "DynamixelMotor.h"

const uint8_t id=1;

DynamixelInterface &interface=*createSerialInterface(Serial);
DynamixelMotor motor(interface, id);

// demonstrate how to change communication speed
// the led should blink three time
void setup() 
{
  //start at 1MBd
  delay(100);
  interface.begin(1000000);
  
  motor.init();
  motor.led(HIGH);
  delay(1000);
  motor.led(LOW);
  delay(1000);
  
  //switch motor to 9600Bd
  motor.communicationSpeed(9600);
  
  //switch interface speed to 9600Bd
  delay(100);
  interface.begin(9600);
  
  motor.led(HIGH);
  delay(1000);
  motor.led(LOW);
  delay(1000);
  
  //switch back to 1MBd
  motor.communicationSpeed(1000000);
  
  delay(100);
  interface.begin(1000000);
  
  motor.led(HIGH);
  delay(1000);
  motor.led(LOW);
  delay(1000);
}

void loop(){}
