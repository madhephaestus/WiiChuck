#include "Adafruit_GFX.h"
#include "Adafruit_HT1632.h"

#define HT_DATA 2
#define HT_WR   3
#define HT_CS   4
#define HT_CS2  5

// use this line for single matrix
//Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS);
// use this line for two matrices!
Adafruit_HT1632LEDMatrix matrix = Adafruit_HT1632LEDMatrix(HT_DATA, HT_WR, HT_CS, HT_CS2);

void setup() {
  Serial.begin(9600);
  matrix.begin(ADA_HT1632_COMMON_16NMOS);  
  matrix.fillScreen();
  delay(500);

}

void loop() {
  matrix.clearScreen(); 
   // draw a pixel!
  matrix.drawPixel(0, 0, 1);
  matrix.writeScreen();
  
  delay(500);
   // clear a pixel!
  matrix.drawPixel(0, 0, 0); 
  matrix.writeScreen();
  
  // draw a big rect on the screen
  matrix.fillRect(matrix.width()/4, matrix.height()/4, 
                  matrix.width()/2, matrix.height()/2, 1);
  matrix.writeScreen();
  delay(500);

  // draw an outline
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), 1);
  matrix.writeScreen();
  delay(500);
  
  // draw an 'X' in red
  matrix.clearScreen(); 
  matrix.drawLine(0, 0, matrix.width()-1, matrix.height()-1, 1);
  matrix.drawLine(matrix.width()-1, 0, 0, matrix.height()-1, 1);
  matrix.writeScreen();
  delay(500);

  // fill a circle
  matrix.fillCircle(matrix.width()/2-1, matrix.height()/2-1, 7, 1);
  matrix.writeScreen();
  delay(500);

  // draw an inverted circle
  matrix.drawCircle(matrix.width()/2-1, matrix.height()/2-1, 4, 0);
  matrix.writeScreen();
  delay(500);
  
  matrix.clearScreen(); 
  // draw some text!
  matrix.setTextSize(1);    // size 1 == 8 pixels high
  matrix.setTextColor(1);   // 'lit' LEDs
  matrix.setTextWrap(true);

  matrix.setCursor(0, 0);   // start at top left, with one pixel of spacing
  matrix.print("AdafruitIndustries");
  matrix.writeScreen();
  
  delay(2000);

  // whew!
}