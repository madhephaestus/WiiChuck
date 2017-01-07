/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1325 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1325.h>

// If using software SPI, define CLK and MOSI
#define OLED_CLK 13
#define OLED_MOSI 11

// These are neede for both hardware & softare SPI
#define OLED_CS 10
#define OLED_RESET 9
#define OLED_DC 8

// this is software SPI, slower but any pins
Adafruit_SSD1325 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// this is for hardware SPI, fast! but fixed oubs
//Adafruit_SSD1325 display(OLED_DC, OLED_RESET, OLED_CS);


void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  display.begin();
  
  Serial.println("init");

  display.fillScreen(BLACK);
  display.display();
  
  Serial.println("This is a test of the rotation capabilities of the GFX library!");
  Serial.println("Press <SEND> (or type a character) to advance");
}

void loop(void) {
  rotatePixel();
  rotateLine();
  rotateText();
  rotateFastline();
  rotateDrawrect();
  rotateFillrect();
  rotateDrawcircle();
  rotateFillcircle();
  rotateTriangle();
  rotateFillTriangle();
  rotateRoundRect();
  rotateFillRoundRect();
}

void rotateText() {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);
    display.display();

    display.setCursor(0, 0);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println("Hello");
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.println("World!");
    display.display();
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();
  
    display.setRotation(display.getRotation()+1);
  }
}

void rotateFillcircle(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.fillCircle(10, 30, 10, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateDrawcircle(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawCircle(10, 30, 10, WHITE);
    display.display();
 
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();
  
    display.setRotation(display.getRotation()+1);
  }
}

void rotateFillrect(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.fillRect(10, 20, 10, 20, WHITE);
    display.display();
 
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateDrawrect(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawRect(5, 10, 5, 10, WHITE);
    display.display();
 
    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateFastline(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawFastHLine(0, 10, display.width(), WHITE);
    display.drawFastVLine(10, 0, display.height(), WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateLine(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawLine(display.width()/2, display.height()/2, 0, 0, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotatePixel(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawPixel(10,5, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateTriangle(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawTriangle(20, 10, 10, 30, 30, 30, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateFillTriangle(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.fillTriangle(20, 10, 10, 30, 30, 30, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateRoundRect(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawRoundRect(20, 10, 25, 15, 5, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateFillRoundRect(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.fillRoundRect(20, 10, 25, 15, 5, WHITE);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateChar(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.drawChar(25, 15, 'A', WHITE, BLACK, 1);
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}

void rotateString(void) {
  for (uint8_t i=0; i<4; i++) {
    display.fillScreen(BLACK);
    Serial.println(display.getRotation(), DEC);

    display.setCursor(8, 25);
    display.setTextColor(WHITE);
    display.print("Adafruit Industries");
    display.display();

    while (!Serial.available());
    Serial.read();  Serial.read();  Serial.read();

    display.setRotation(display.getRotation()+1);
  }
}
