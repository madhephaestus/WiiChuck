/*
  Firmata.cpp - Firmata library
  Copyright (C) 2006-2008 Hans-Christoph Steiner.  All rights reserved.

  Modified for Adafruit_BLE_Uart by Limor Fried/Kevin Townsend for
  Adafruit Industries, 2014
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/

//******************************************************************************
//* Includes
//******************************************************************************

#include "Adafruit_BLE_Firmata.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}

//******************************************************************************
//* Support Functions
//******************************************************************************

void Adafruit_BLE_FirmataClass::sendValueAsTwo7bitBytes(int value)
{
  FirmataSerial.write(value & B01111111); // LSB
  FirmataSerial.write(value >> 7 & B01111111); // MSB
}

void Adafruit_BLE_FirmataClass::startSysex(void)
{
  FirmataSerial.write(START_SYSEX);
}

void Adafruit_BLE_FirmataClass::endSysex(void)
{
  FirmataSerial.write(END_SYSEX);
}

//******************************************************************************
//* Constructors
//******************************************************************************

Adafruit_BLE_FirmataClass::Adafruit_BLE_FirmataClass(Stream &s) : FirmataSerial(s)
{
  firmwareVersionCount = 0;
  systemReset();
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/* begin method for overriding default serial bitrate */
void Adafruit_BLE_FirmataClass::begin(void)
{
  printVersion();
  printFirmwareVersion();
}

void Adafruit_BLE_FirmataClass::begin(Stream &s)
{
  FirmataSerial = s;
  systemReset();
  printVersion();
  printFirmwareVersion();
}

// output the protocol version message to the serial port
void Adafruit_BLE_FirmataClass::printVersion(void) {
  FirmataSerial.write(REPORT_VERSION);
  FirmataSerial.write(FIRMATA_MAJOR_VERSION);
  FirmataSerial.write(FIRMATA_MINOR_VERSION);
}

void Adafruit_BLE_FirmataClass::printFirmwareVersion(void)
{
  byte i;

  if(firmwareVersionCount) { // make sure that the name has been set before reporting
    startSysex();
    FirmataSerial.write(REPORT_FIRMWARE);
    FirmataSerial.write(firmwareVersionVector[0]); // major version number
    FirmataSerial.write(firmwareVersionVector[1]); // minor version number
    for(i=2; i<firmwareVersionCount; ++i) {
      sendValueAsTwo7bitBytes(firmwareVersionVector[i]);
    }
    endSysex();
  }
}

void Adafruit_BLE_FirmataClass::setFirmwareNameAndVersion(const char *name, byte major, byte minor)
{
  const char *filename;
  char *extension;

  // parse out ".cpp" and "applet/" that comes from using __FILE__
  extension = strstr(name, ".cpp");
  filename = strrchr(name, '/') + 1; //points to slash, +1 gets to start of filename
  // add two bytes for version numbers
  if(extension && filename) {
    firmwareVersionCount = extension - filename + 2;
  } else {
    firmwareVersionCount = strlen(name) + 2;
    filename = name;
  }
  firmwareVersionVector = (byte *) malloc(firmwareVersionCount);
  firmwareVersionVector[firmwareVersionCount] = 0;
  firmwareVersionVector[0] = major;
  firmwareVersionVector[1] = minor;
  strncpy((char*)firmwareVersionVector + 2, filename, firmwareVersionCount - 2);
  // alas, no snprintf on Arduino
  //    snprintf(firmwareVersionVector, MAX_DATA_BYTES, "%c%c%s", 
  //             (char)major, (char)minor, firmwareVersionVector);
}

//------------------------------------------------------------------------------
// Serial Receive Handling

int Adafruit_BLE_FirmataClass::available(void)
{
  return FirmataSerial.available();
}


void Adafruit_BLE_FirmataClass::processSysexMessage(void)
{
  switch(storedInputData[0]) { //first byte in buffer is command
  case REPORT_FIRMWARE:
    printFirmwareVersion();
    break;
  case STRING_DATA:
    if(currentStringCallback) {
      byte bufferLength = (sysexBytesRead - 1) / 2;
      char *buffer = (char*)malloc(bufferLength * sizeof(char));
      byte i = 1;
      byte j = 0;
      while(j < bufferLength) {
        buffer[j] = (char)storedInputData[i];
        i++;
        buffer[j] += (char)(storedInputData[i] << 7);
        i++;
        j++;
      }
      (*currentStringCallback)(buffer);
    }
    break;
  default:
    if(currentSysexCallback)
      (*currentSysexCallback)(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
  }
}

int Adafruit_BLE_FirmataClass::processInput(void)
{
  int inputData = FirmataSerial.read(); // this is 'int' to handle -1 when no data
  int command;
    
  if (inputData == -1) return -1;

  //Serial.print(F(" 0x")); Serial.print(inputData, HEX); 

  if (parsingSysex) {
    if(inputData == END_SYSEX) {
      //stop sysex byte      
      parsingSysex = false;
      //fire off handler function
      processSysexMessage();
    } else {
      //normal data byte - add to buffer
      storedInputData[sysexBytesRead] = inputData;
      sysexBytesRead++;
    }
  } else if( (waitForData > 0) && (inputData < 128) ) {  
    waitForData--;
    storedInputData[waitForData] = inputData;
#ifdef BLE_DEBUG
        Serial.print(F(" 0x")); Serial.print(inputData, HEX); 
#endif

    if( (waitForData==0) && executeMultiByteCommand ) { // got the whole message

#ifdef BLE_DEBUG
      Serial.println(); 
#endif


      switch(executeMultiByteCommand) {
      case ANALOG_MESSAGE:
        if(currentAnalogCallback) {
          (*currentAnalogCallback)(multiByteChannel,
                                   (storedInputData[0] << 7)
                                   + storedInputData[1]);
        }
        break;
      case DIGITAL_MESSAGE:
        if(currentDigitalCallback) {
          (*currentDigitalCallback)(multiByteChannel,
                                    (storedInputData[0] << 7)
                                    + storedInputData[1]);
        }
        break;
      case SET_PIN_MODE:
        if(currentPinModeCallback)
          (*currentPinModeCallback)(storedInputData[1], storedInputData[0]);
        break;
      case REPORT_ANALOG:
        if(currentReportAnalogCallback)
          (*currentReportAnalogCallback)(multiByteChannel,storedInputData[0]);
        break;
      case REPORT_DIGITAL:
        if(currentReportDigitalCallback)
          (*currentReportDigitalCallback)(multiByteChannel,storedInputData[0]);
        break;
      }
      executeMultiByteCommand = 0;
    }	
  } else {
#ifdef BLE_DEBUG
    Serial.print(F("\tReceived 0x")); Serial.print(inputData, HEX); 
#endif
    // remove channel info from command byte if less than 0xF0
    if(inputData < 0xF0) {
      command = inputData & 0xF0;
      multiByteChannel = inputData & 0x0F;
    } else {
      command = inputData;
      // commands in the 0xF* range don't use channel data
    }
    switch (command) {
    case ANALOG_MESSAGE:
    case DIGITAL_MESSAGE:
    case SET_PIN_MODE:
      waitForData = 2; // two data bytes needed
      executeMultiByteCommand = command;
      break;
    case REPORT_ANALOG:
    case REPORT_DIGITAL:
      waitForData = 1; // two data bytes needed
      executeMultiByteCommand = command;
      break;
    case START_SYSEX:
      parsingSysex = true;
      sysexBytesRead = 0;
      break;
    case SYSTEM_RESET:
      systemReset();
      break;
    case REPORT_VERSION:
      printVersion();
      break;
    }
  }

  return inputData;
}

//------------------------------------------------------------------------------
// Serial Send Handling

// send an analog message
void Adafruit_BLE_FirmataClass::sendAnalog(byte pin, int value) 
{
  // create a three byte buffer
  uint8_t sendbuffer[3];

  // pin can only be 0-15, so chop higher bits
  //FirmataSerial.write(ANALOG_MESSAGE | (pin & 0xF));
  sendbuffer[0] = ANALOG_MESSAGE | (pin & 0xF);

  //sendValueAsTwo7bitBytes(value);  
  sendbuffer[1] = value % 128; // Tx bits 0-6
  sendbuffer[2] = (value >> 7) &0x7F;  // Tx bits 7-13

  FirmataSerial.write(sendbuffer, 3);
}

// send a single digital pin in a digital message
void Adafruit_BLE_FirmataClass::sendDigital(byte pin, int value) 
{
  /* TODO add single pin digital messages to the protocol, this needs to
   * track the last digital data sent so that it can be sure to change just
   * one bit in the packet.  This is complicated by the fact that the
   * numbering of the pins will probably differ on Arduino, Wiring, and
   * other boards.  The DIGITAL_MESSAGE sends 14 bits at a time, but it is
   * probably easier to send 8 bit ports for any board with more than 14
   * digital pins.
   */

  // TODO: the digital message should not be sent on the serial port every
  // time sendDigital() is called.  Instead, it should add it to an int
  // which will be sent on a schedule.  If a pin changes more than once
  // before the digital message is sent on the serial port, it should send a
  // digital message for each change.

  //    if(value == 0)
  //        sendDigitalPortPair();
}


// send 14-bits in a single digital message (protocol v1)
// send an 8-bit port in a single digital message (protocol v2)
void Adafruit_BLE_FirmataClass::sendDigitalPort(byte portNumber, int portData)
{
  // create a three byte buffer
  uint8_t sendbuffer[3];

  sendbuffer[0] = DIGITAL_MESSAGE | (portNumber & 0xF);
  sendbuffer[1] = (byte)portData % 128; // Tx bits 0-6
  sendbuffer[2] = portData >> 7;  // Tx bits 7-13
  FirmataSerial.write(sendbuffer, 3);
}


void Adafruit_BLE_FirmataClass::sendSysex(byte command, byte bytec, byte* bytev) 
{
  byte i;
  startSysex();
  FirmataSerial.write(command);
  for(i=0; i<bytec; i++) {
    sendValueAsTwo7bitBytes(bytev[i]);        
  }
  endSysex();
}

void Adafruit_BLE_FirmataClass::sendString(byte command, const char* string) 
{
  sendSysex(command, strlen(string), (byte *)string);
}


// send a string as the protocol string type
void Adafruit_BLE_FirmataClass::sendString(const char* string) 
{
  sendString(STRING_DATA, string);
}


// Internal Actions/////////////////////////////////////////////////////////////

// generic callbacks
void Adafruit_BLE_FirmataClass::attach(byte command, callbackFunction newFunction)
{
  switch(command) {
  case ANALOG_MESSAGE: currentAnalogCallback = newFunction; break;
  case DIGITAL_MESSAGE: currentDigitalCallback = newFunction; break;
  case REPORT_ANALOG: currentReportAnalogCallback = newFunction; break;
  case REPORT_DIGITAL: currentReportDigitalCallback = newFunction; break;
  case SET_PIN_MODE: currentPinModeCallback = newFunction; break;
  }
}

void Adafruit_BLE_FirmataClass::attach(byte command, systemResetCallbackFunction newFunction)
{
  switch(command) {
  case SYSTEM_RESET: currentSystemResetCallback = newFunction; break;
  }
}

void Adafruit_BLE_FirmataClass::attach(byte command, stringCallbackFunction newFunction)
{
  switch(command) {
  case STRING_DATA: currentStringCallback = newFunction; break;
  }
}

void Adafruit_BLE_FirmataClass::attach(byte command, sysexCallbackFunction newFunction)
{
  currentSysexCallback = newFunction;
}

void Adafruit_BLE_FirmataClass::detach(byte command)
{
  switch(command) {
  case SYSTEM_RESET: currentSystemResetCallback = NULL; break;
  case STRING_DATA: currentStringCallback = NULL; break;
  case START_SYSEX: currentSysexCallback = NULL; break;
  default:
    attach(command, (callbackFunction)NULL);
  }
}

// sysex callbacks
/*
 * this is too complicated for analogReceive, but maybe for Sysex?
 void Adafruit_BLE_FirmataClass::attachSysex(sysexFunction newFunction)
 {
 byte i;
 byte tmpCount = analogReceiveFunctionCount;
 analogReceiveFunction* tmpArray = analogReceiveFunctionArray;
 analogReceiveFunctionCount++;
 analogReceiveFunctionArray = (analogReceiveFunction*) calloc(analogReceiveFunctionCount, sizeof(analogReceiveFunction));
 for(i = 0; i < tmpCount; i++) {
 analogReceiveFunctionArray[i] = tmpArray[i];
 }
 analogReceiveFunctionArray[tmpCount] = newFunction;
 free(tmpArray);
 }
*/

//******************************************************************************
//* Private Methods
  //******************************************************************************/


// resets the system state upon a SYSTEM_RESET message from the host software
void Adafruit_BLE_FirmataClass::systemReset(void)
{
  byte i;

  waitForData = 0; // this flag says the next serial input will be data
  executeMultiByteCommand = 0; // execute this after getting multi-byte data
  multiByteChannel = 0; // channel data for multiByteCommands


  for(i=0; i<MAX_DATA_BYTES; i++) {
    storedInputData[i] = 0;
  }

  parsingSysex = false;
  sysexBytesRead = 0;

  if(currentSystemResetCallback)
    (*currentSystemResetCallback)();

  //flush(); //TODO uncomment when Firmata is a subclass of HardwareSerial
}



void Adafruit_BLE_FirmataClass::setUsablePins(uint8_t *digitaliopins, uint8_t num_digitaliopins,     
					      uint8_t *analogiopins, uint8_t num_analogiopins,
					      uint8_t *pwmpins, uint8_t num_pwmpins,
					      uint8_t *servopins, uint8_t num_servopins,
					      uint8_t sdapin, uint8_t sclpin)
{
  _digitaliopins = digitaliopins;
  _num_digitaliopins = num_digitaliopins;
  _analogiopins = analogiopins;
  _num_analogiopins = num_analogiopins;
  _pwmpins = servopins;
  _num_pwmpins = num_pwmpins;
  _servopins = servopins;
  _num_servopins = num_servopins;
  _sdapin = sdapin;
  _sclpin = sclpin;
}



boolean  Adafruit_BLE_FirmataClass::contains(uint8_t *set, uint8_t num, uint8_t test) {
  for (uint8_t i=0; i<num; i++) { 
    if (set[i] == test) return true;
  }
  return false;
}

uint8_t  Adafruit_BLE_FirmataClass::location(uint8_t *set, uint8_t num, uint8_t test) {
  for (uint8_t i=0; i<num; i++) { 
    if (set[i] == test) return i;
  }
  return 255;
}

uint8_t Adafruit_BLE_FirmataClass::PIN_TO_ANALOG(uint8_t p) {
  return location(_analogiopins, _num_analogiopins, p);
}

/*==============================================================================
 * readPort() - Read an 8 bit port
 *============================================================================*/

unsigned char  Adafruit_BLE_FirmataClass::readPort(byte port, byte bitmask)
{
  unsigned char out=0, pin=port*8;
  if (IS_PIN_DIGITAL(pin+0) && (bitmask & 0x01) && digitalRead(PIN_TO_DIGITAL(pin+0))) out |= 0x01;
  if (IS_PIN_DIGITAL(pin+1) && (bitmask & 0x02) && digitalRead(PIN_TO_DIGITAL(pin+1))) out |= 0x02;
  if (IS_PIN_DIGITAL(pin+2) && (bitmask & 0x04) && digitalRead(PIN_TO_DIGITAL(pin+2))) out |= 0x04;
  if (IS_PIN_DIGITAL(pin+3) && (bitmask & 0x08) && digitalRead(PIN_TO_DIGITAL(pin+3))) out |= 0x08;
  if (IS_PIN_DIGITAL(pin+4) && (bitmask & 0x10) && digitalRead(PIN_TO_DIGITAL(pin+4))) out |= 0x10;
  if (IS_PIN_DIGITAL(pin+5) && (bitmask & 0x20) && digitalRead(PIN_TO_DIGITAL(pin+5))) out |= 0x20;
  if (IS_PIN_DIGITAL(pin+6) && (bitmask & 0x40) && digitalRead(PIN_TO_DIGITAL(pin+6))) out |= 0x40;
  if (IS_PIN_DIGITAL(pin+7) && (bitmask & 0x80) && digitalRead(PIN_TO_DIGITAL(pin+7))) out |= 0x80;
  return out;

}

/*==============================================================================
 * writePort() - Write an 8 bit port, only touch pins specified by a bitmask
 *============================================================================*/

unsigned char  Adafruit_BLE_FirmataClass::writePort(byte port, byte value, byte bitmask)
{
	byte pin=port*8;
	for (uint8_t i=0; i<8; i++) {
	  if (bitmask & (1 << i)) {
	    // dont touch non-digital pins
	    if (IS_PIN_DIGITAL(pin+i))
	      digitalWrite(PIN_TO_DIGITAL(pin+i), (value & (1 << i)));
	  }
	}
}
