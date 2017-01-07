// Simple UNTZtrument MIDI step sequencer.
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
bool isConnected = false;

#define LED 13 // Pin for heartbeat LED (shows code is working)

#ifndef HELLA
// A standard UNTZtrument has four Trellises in a 2x2 arrangement
// (8x8 buttons total).  addr[] is the I2C address of the upper left,
// upper right, lower left and lower right matrices, respectively,
// assuming an upright orientation, i.e. labels on board are in the
// normal reading direction.
Adafruit_Trellis     T[4];
Adafruit_UNTZtrument untztrument(&T[0], &T[1], &T[2], &T[3]);
const uint8_t        addr[] = { 0x70, 0x71, 0x72, 0x73 };
#else
// A HELLA UNTZtrument has eight Trellis boards...
Adafruit_Trellis     T[8];
Adafruit_UNTZtrument untztrument(&T[0], &T[1], &T[2], &T[3],
                                 &T[4], &T[5], &T[6], &T[7]);
const uint8_t        addr[] = { 0x70, 0x71, 0x72, 0x73,
                                0x74, 0x75, 0x76, 0x77 };
#endif // HELLA

#define WIDTH     ((sizeof(T) / sizeof(T[0])) * 2)
#define N_BUTTONS ((sizeof(T) / sizeof(T[0])) * 16)

// Encoder on pins 4,5 sets tempo.  Optional, not present in
// standard UNTZtrument, but won't affect things if unconnected.
enc e(4, 5);

uint8_t       grid[WIDTH];                 // Sequencer state
uint8_t       heart        = 0,            // Heartbeat LED counter
              col          = WIDTH-1;      // Current column
unsigned int  bpm          = 240;          // Tempo
unsigned long beatInterval = 60000L / bpm, // ms/beat
              prevBeatTime = 0L,           // Column step timer
              prevReadTime = 0L;           // Keypad polling timer

// The note[] and channel[] tables are the MIDI note and channel numbers
// for to each row (top to bottom); they're specific to this application.
// bitmask[] is for efficient reading/writing bits to the grid[] array.
static const uint8_t PROGMEM
  note[8]    = {  72, 71, 69, 67, 65, 64, 62,  60 },
  channel[8] = {   1,  1,  1,  1,  1,  1,  1,   1 },
  bitmask[8] = {   1,  2,  4,  8, 16, 32, 64, 128 };

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
  memset(grid, 0, sizeof(grid));
  e.setBounds(60 * 4, 480 * 4 + 3); // Set tempo limits
  e.setValue(bpm * 4);              // *4's for encoder detents
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midi.send(0x90 | channel, pitch, velocity);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midi.send(0x80 | channel, pitch, velocity);
}

// Turn on (or off) one column of the display
void line(uint8_t x, boolean set) {
  for(uint8_t mask=1, y=0; y<8; y++, mask <<= 1) {
    uint8_t i = untztrument.xy2i(x, y);
    if(set || (grid[x] & mask)) untztrument.setLED(i);
    else                        untztrument.clrLED(i);
  }
}

void loop() {
  uint8_t       mask;
  boolean       refresh = false;
  unsigned long t       = millis();

  enc::poll(); // Read encoder(s)

  if((t - prevReadTime) >= 20L) { // 20ms = min Trellis poll time
    if(untztrument.readSwitches()) { // Button state change?
      for(uint8_t i=0; i<N_BUTTONS; i++) { // For each button...
        uint8_t x, y;
        untztrument.i2xy(i, &x, &y);
        mask = pgm_read_byte(&bitmask[y]);
        if(untztrument.justPressed(i)) {
          if(grid[x] & mask) { // Already set?  Turn off...
            grid[x] &= ~mask;
            untztrument.clrLED(i);
            noteOff(pgm_read_byte(&channel[y]), pgm_read_byte(&note[y]), 127);
          } else { // Turn on
            grid[x] |= mask;
            untztrument.setLED(i);
          }
          refresh = true;
        }
      }
    }
    prevReadTime = t;
    digitalWrite(LED, ++heart & 32); // Blink = alive
  }

  if((t - prevBeatTime) >= beatInterval) { // Next beat?
    // Turn off old column
    line(col, false);
    for(uint8_t row=0, mask=1; row<8; row++, mask <<= 1) {
      if(grid[col] & mask) {
        noteOff(pgm_read_byte(&channel[row]), pgm_read_byte(&note[row]), 127);
      }
    }
    // Advance column counter, wrap around
    if(++col >= WIDTH) col = 0;
    // Turn on new column
    line(col, true);
    for(uint8_t row=0, mask=1; row<8; row++, mask <<= 1) {
      if(grid[col] & mask) {
          noteOn(pgm_read_byte(&channel[row]), pgm_read_byte(&note[row]), 127);
      }
    }
    prevBeatTime = t;
    refresh      = true;
    bpm          = e.getValue() / 4; // Div for encoder detents
    beatInterval = 60000L / bpm;
  }

  if(refresh) untztrument.writeDisplay();

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

