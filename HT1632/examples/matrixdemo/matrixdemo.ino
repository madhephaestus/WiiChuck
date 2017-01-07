#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

// use this line for single matrix
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
// use this line for two matrices!
//Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS, HT_CS2);

void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);  
  matrix.fillScreen();
  delay(500);
  matrix.clearScreen(); 
}

void loop() {
  for (uint8_t y=0; y<matrix.height(); y++) {
    for (uint8_t x=0; x< matrix.width(); x++) {
      matrix.setPixel(x, y);
      matrix.writeScreen();
    }
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

  for (uint8_t y=0; y<matrix.height(); y++) {
    for (uint8_t x=0; x< matrix.width(); x++) {
      matrix.clrPixel(x, y);
      matrix.writeScreen();
    }
  }
}
