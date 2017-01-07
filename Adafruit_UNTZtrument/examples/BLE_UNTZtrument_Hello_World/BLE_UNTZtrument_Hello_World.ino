// Super-basic Bluefruit Feather UNTZtrument MIDI example.
// Maps buttons to MIDI noteon/off events;
// this is NOT a sequencer or anything fancy.
// Requires an Adafruit Bluefruit LE Feather, the Adafruit 
// BluefruitLE nRF51 library, and a MIDI software synth on your
// host computer or mobile device.
//
// Copyright (c) 2014-2016 Adafruit Industries
// Author: Phil Burgess
// BLE Modifications: Todd Treece
// Licensed under the Modified BSD License.

#include <Wire.h>
#include <Adafruit_Trellis.h>
#include <Adafruit_UNTZtrument.h>

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BLEMIDI.h"

#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE         1
#define MINIMUM_FIRMWARE_VERSION    "0.7.0"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
Adafruit_BLEMIDI midi(ble);

#define LED     13 // Pin for heartbeat LED (shows code is working)
#define CHANNEL 1  // MIDI channel number

#ifndef HELLA
// A standard UNTZtrument has four Trellises in a 2x2 arrangement
// (8x8 buttons total).  addr[] is the I2C address of the upper left,
// upper right, lower left and lower right matrices, respectively,
// assuming an upright orientation, i.e. labels on board are in the
// normal reading direction.
Adafruit_Trellis     T[4];
Adafruit_UNTZtrument untztrument(&T[0], &T[1], &T[2], &T[3]);
const uint8_t        addr[] = { 0x70, 0x71,
                                0x72, 0x73 };
#else
// A HELLA UNTZtrument has eight Trellis boards...
Adafruit_Trellis     T[8];
Adafruit_UNTZtrument untztrument(&T[0], &T[1], &T[2], &T[3],
                                 &T[4], &T[5], &T[6], &T[7]);
const uint8_t        addr[] = { 0x70, 0x71, 0x72, 0x73,
                                0x74, 0x75, 0x76, 0x77 };
#endif // HELLA

// For this example, MIDI note numbers are simply centered based on
// the number of Trellis buttons; each row doesn't necessarily
// correspond to an octave or anything.
#define WIDTH     ((sizeof(T) / sizeof(T[0])) * 2)
#define N_BUTTONS ((sizeof(T) / sizeof(T[0])) * 16)
#define LOWNOTE   ((128 - N_BUTTONS) / 2)

uint8_t       heart        = 0;  // Heartbeat LED counter
unsigned long prevReadTime = 0L; // Keypad polling timer
bool isConnected = false;

void setup() {
  pinMode(LED, OUTPUT);
 
  Serial.begin(115200);
  Serial.print(F("Bluefruit Feather: "));

  if ( !ble.begin(VERBOSE_MODE) ) {
    error(F("Couldn't find Bluefruit, check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE ) {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("Couldn't factory reset"));
    }
  }

  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();
  
  /* Set BLE callbacks */
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);
  Serial.println(F("Enable MIDI: "));
  
  if ( ! midi.begin(true) ) {
    error(F("Could not enable MIDI"));
  }
    
  ble.verbose(false);
  Serial.print(F("Waiting for a connection..."));
  while(!isConnected) {
    ble.update(500);
  }

#ifndef HELLA
  untztrument.begin(addr[0], addr[1], addr[2], addr[3]);
#else
  untztrument.begin(addr[0], addr[1], addr[2], addr[3],
                    addr[4], addr[5], addr[6], addr[7]);
#endif // HELLA
  // Default Arduino I2C speed is 100 KHz, but the HT16K33 supports
  // 400 KHz.  We can force this for faster read & refresh, but may
  // break compatibility with other I2C devices...so be prepared to
  // comment this out, or save & restore value as needed.
#ifdef ARDUINO_ARCH_SAMD
    Wire.setClock(400000L);
#endif
#ifdef __AVR__
      TWBR = 12; // 400 KHz I2C on 16 MHz AVR
#endif
  untztrument.clear();
  untztrument.writeDisplay();
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midi.send(0x90 | channel, pitch, velocity);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midi.send(0x80 | channel, pitch, velocity);
}

void loop() {
  ble.update(1);
 
  if(! isConnected) {
    return;
  }
 
  unsigned long t = millis();
  
  if((t - prevReadTime) >= 20L) { // 20ms = min Trellis poll time
    if(untztrument.readSwitches()) { // Button state change?
      for(uint8_t i=0; i<N_BUTTONS; i++) { // For each button...
        // Get column/row for button, convert to MIDI note number
        uint8_t x, y, note;
        untztrument.i2xy(i, &x, &y);
        note = LOWNOTE + y * WIDTH + x;
        if(untztrument.justPressed(i)) {
          noteOn(CHANNEL, note, 127);
          untztrument.setLED(i);
        } else if(untztrument.justReleased(i)) {
          noteOff(CHANNEL, note, 0);
          untztrument.clrLED(i);
        }
      }
      untztrument.writeDisplay();
    }
    prevReadTime = t;
    digitalWrite(LED, ++heart & 32); // Blink = alive
  }
  
}

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void connected(void) {
  isConnected = true;
  Serial.println(F(" CONNECTED!"));
}

void disconnected(void) {
  Serial.println("disconnected");
  isConnected = false;
}
