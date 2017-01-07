// WiiChuck_Serial_Demo
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my WiiChuck library to 
// send joystick and accelerometer information over a serial link
//
// To use the hardware I2C (TWI) interface of the chipKit you must connect
// the pins as follows:
//
// chipKit Uno32/uC32:
// ----------------------
// NunChuck:  SDA pin   -> Analog 4
//            SCL pin   -> Analog 5
// *** Please note that JP6 and JP8 must be in the I2C position (closest to the analog pins)
//
// chipKit Max32:
// ----------------------
// NunChuck:  SDA pin   -> Digital 20 (the pin labeled SDA)
//            SCL pin   -> Digital 21 (the pin labeled SCL)
//
// The chipKit boards does not have pull-up resistors on the hardware I2C interface
// so external pull-up resistors on the data and clock signals are required.
//
// You can connect the NunChuck to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used.
//

#include <WiiChuck.h>

WiiChuck myChuck(SDA, SCL);

char st[100];

void setup()
{
  Serial.begin(115200);
  myChuck.begin();
}

void loop()
{
  myChuck.readData();

  sprintf(st, "JoyX: %4d%% | JoyY: %4d%% | Roll: %4d%c | Pitch: %4d%c | Buttons: ", myChuck.getJoyX(), myChuck.getJoyY(), myChuck.getRollAngle(), (char)176, myChuck.getPitchAngle(), (char)176);

  Serial.print(st);

  if (myChuck.checkButtonC())
    Serial.print("C");
  else
    Serial.print("-");
  if (myChuck.checkButtonZ())
    Serial.print("Z");
  else
    Serial.print("-");
  Serial.println();
  delay(100);
}





