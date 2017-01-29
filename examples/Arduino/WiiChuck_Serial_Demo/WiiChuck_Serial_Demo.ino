// WiiChuck_Serial_Demo
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my WiiChuck library to 
// send joystick and accelerometer information over a serial link
//
// To use the hardware I2C (TWI) interface of the Arduino you must connect
// the pins as follows:
//
// Arduino Uno/2009:
// ----------------------
// NunChuck:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//            SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
//
// Arduino Leonardo:
// ----------------------
// NunChuck:  SDA pin   -> Arduino Digital 2 or the dedicated SDA pin
//            SCL pin   -> Arduino Digital 3 or the dedicated SCL pin
//
// Arduino Mega:
// ----------------------
// NunChuck:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//            SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//
// Arduino Due:
// ----------------------
// NunChuck:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA1 (Digital 70) pin
//            SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL1 (Digital 71) pin
//
// The internal pull-up resistors will be activated when using the 
// hardware I2C interfaces.
//
// You can connect the NunChuck to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//

#include <WiiChuck.h>

#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif
WiiChuck myChuck( SDA,SCL);

char st[100];

void setup()
{
	
  Serial.begin(115200);
  Serial.println("Starting WiiChuck Demo");
  //myChuck.type=OFFICIALWII;
  myChuck.type=THIRDPARTYWII;
  myChuck.begin();
}

void loop()
{
	
  myChuck.readData();

  sprintf(st, 
		  "JoyX: %4d%% | JoyY: %4d%% | Roll: %4d | Pitch: %4d | Buttons: ", 
		  myChuck.getJoyX(),
		  myChuck.getJoyY(),
		  myChuck.getRollAngle(), 
		  myChuck.getPitchAngle());

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





