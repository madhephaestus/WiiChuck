/****************************************************************
Example code demonstrating the use of the Arduino Library for
the SparkFun MiniMoto board, which uses the TI DRV8830 IC for I2C
low-voltage DC motor control.

This code is beerware; if you use it, please buy me (or any other
SparkFun employee) a cold beverage next time you run into one of
us at the local.

17 Sep 2013- Mike Hord, SparkFun Electronics

Code developed in Arduino 1.0.5, on a Fio classic board.

**Updated for Arduino 1.6.4 5/2015**
****************************************************************/

#include <SparkFunMiniMoto.h>  // Include the MiniMoto library

// Create two MiniMoto instances, with different address settings.
MiniMoto motor0(0xCE); // A1 = 1, A0 = clear
MiniMoto motor1(0xD0); // A1 = 1, A0 = 1 (default)

#define FAULTn  16     // Pin used for fault detection.

// Nothing terribly special in the setup() function- prep the
//  serial port, print a little greeting, and set up our fault
//  pin as an input.
void setup()
{
  Serial.begin(9600);
  Serial.println("Hello, world!");
  pinMode(FAULTn, INPUT);
}

// The loop() function just spins the motors one way, then the
//  other, while constantly monitoring for any fault conditions
//  to occur. If a fault does occur, it will be reported over
//  the serial port, and then operation continues.
void loop()
{
  Serial.println("Forward!");
  motor0.drive(10);
  motor1.drive(10);
  delayUntil(10000);
  Serial.println("Stop!");
  motor0.stop();
  motor1.stop();
  delay(2000);
  Serial.println("Reverse!");
  motor0.drive(-10);
  motor1.drive(-10);
  delayUntil(10000);
  Serial.println("Brake!");
  motor0.brake();
  motor1.brake();
  delay(2000);
}

// delayUntil() is a little function to run the motor either for
//  a designated time OR until a fault occurs. Note that this is
//  a very simple demonstration; ideally, an interrupt would be
//  used to service faults rather than blocking the application
//  during motion and polling for faults.
void delayUntil(unsigned long elapsedTime)
{
  // See the "BlinkWithoutDelay" example for more details on how
  //  and why this loop works the way it does.
  unsigned long startTime = millis();
  while (startTime + elapsedTime > millis())
  {
    // If FAULTn goes low, a fault condition *may* exist. To be
    //  sure, we'll need to check the FAULT bit.
    if (digitalRead(FAULTn) == LOW)
    {
      // We're going to check both motors; the logic is the same
      //  for each...
      byte result = motor0.getFault();
      // If result masked by FAULT is non-zero, we've got a fault
      //  condition, and we should report it.
      if (result & FAULT)
      {
        Serial.print("Motor 0 fault: ");
        if (result & OCP) Serial.println("Chip overcurrent!");
        if (result & ILIMIT) Serial.println("Load current limit!");
        if (result & UVLO) Serial.println("Undervoltage!");
        if (result & OTS) Serial.println("Over temp!");
        break; // We want to break out of the motion immediately,
               //  so we can stop motion in response to our fault.
      }
      result = motor1.getFault();
      if (result & FAULT)
      {
        Serial.print("Motor 1 fault: ");
        if (result & OCP) Serial.println("Chip overcurrent!");
        if (result & ILIMIT) Serial.println("Load current limit!");
        if (result & UVLO) Serial.println("Undervoltage!");
        if (result & OTS) Serial.println("Over temp!");
        break;
      }
    }
  }
}
