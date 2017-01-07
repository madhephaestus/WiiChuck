#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"

/* 
This is a basic demo program showing how to write to a HT1632
These can be used for up to 16x24 LED matrix grids, with internal memory
and using only 3 pins - data, write and select.
Multiple HT1632's can share data and write pins, but need unique CS pins.
*/

#define HT_DATA 2
#define HT_WR 3
#define HT_CS 4

Adafruit_HT1632 matrix = Adafruit_HT1632(HT_DATA, HT_WR, HT_CS);

void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);
  
  delay(100);
  matrix.clearScreen();
}

void testMatrix(Adafruit_HT1632 matrix) {
  for (int i=0; i<24*16; i++) {
    matrix.setPixel(i);
    matrix.writeScreen();
  }
  
  // blink!
  matrix.blink(true);
  delay(2000);
  matrix.blink(false);
  
  // Adjust the brightness down 
  for (int8_t i=15; i>=0; i--) {
   matrix.setBrightness(i);
   delay(100);
  }
  // then back up
  for (uint8_t i=0; i<16; i++) {
   matrix.setBrightness(i);
   delay(100);
  }

  // Clear it out
  for (int i=0; i<24*16; i++) {
    matrix.clrPixel(i);
    matrix.writeScreen();
  }
}

void loop() {
  testMatrix(matrix);
}