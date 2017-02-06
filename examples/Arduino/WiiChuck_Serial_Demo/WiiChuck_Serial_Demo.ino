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
#include <Wire.h>
#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif


WiiChuck myChuck(3, 2);
char st[500];

void setup() {

	Serial.begin(115200);
	Serial.println("Starting WiiChuck Demo");
	//myChuck.type=OFFICIALWII;
	myChuck.type = THIRDPARTYWII;
	myChuck.usePullUpClock = false;
	//myChuck.type = WIICLASSIC;
	myChuck.begin();
}

void loop() {

	myChuck.readData();
	Serial.print("JoyX BYTE: ");
	//Serial.print(myChuck._dataarray[0], BIN) ;
	Serial.print(myChuck._dataarray[0], HEX) ;
	Serial.print("  JoyY BYTE: ");
	//Serial.print(myChuck._dataarray[1], BIN) ;
	Serial.print(myChuck._dataarray[1], HEX) ;
	sprintf(st,
			"  JoyX: %4d%% | JoyY: %4d%% | Roll: %4d | Pitch: %4d | Buttons: ",
			myChuck.getJoyX(), myChuck.getJoyY(), myChuck.getRollAngle(),
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
	sprintf(st,
			"rightStickX: %4d%% | rightStickY() : %4d%% | leftStickX(): %4d | leftStickY(): %4d ",
			myChuck.rightStickX(), myChuck.rightStickY(), myChuck.leftStickX(), myChuck.leftStickY()
	);

	Serial.print(st);
	sprintf(st,
			" rightShouldPressure() : %4d%% | leftShouldPressure() : %4d%% | Buttons:",
			myChuck.rightShouldPressure(), myChuck.leftShouldPressure());

	Serial.print(st);
	//Dpad
	if (myChuck.bPressed())
		Serial.print("B");
	else
		Serial.print("-");
	if (myChuck.aPressed())
		Serial.print("A");
	else
		Serial.print("-");
	if (myChuck.yPressed())
		Serial.print("Y");
	else
		Serial.print("-");
	if (myChuck.xPressed())
		Serial.print("X");
	else
		Serial.print("-");

	//control buttons
	if (myChuck.startPressed())
		Serial.print("St");
	else
		Serial.print("-");
	if (myChuck. homePressed() )
		Serial.print("H");
	else
		Serial.print("-");
	if (myChuck.selectPressed())
		Serial.print("Sl");
	else
		Serial.print("-");
	
	// arrows
	if (myChuck.upDPressed() )
		Serial.print("U");
	else
		Serial.print("-");
	if (myChuck.downDPressed())
		Serial.print("D");
	else
		Serial.print("-");
	if (myChuck.rightDPressed() )
		Serial.print("R");
	else
		Serial.print("-");
	if (myChuck.leftDPressed())
		Serial.print("L");
	else
		Serial.print("-");	
	
	// Shoulders
	if (myChuck.rzPressed() )
		Serial.print("Rz");
	else
		Serial.print("-");
	if (myChuck.lzPressed())
		Serial.print("Lz");
	else
		Serial.print("-");
	if (myChuck.rightShoulderPressed() )
		Serial.print("Rs");
	else
		Serial.print("-");
	if (myChuck.leftShoulderPressed() )
		Serial.print("Ls");
	else
		Serial.print("-");
	Serial.println();
 
	delay(100);
}
