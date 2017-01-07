#include <LiquidCrystal_SR2W.h>
#include <LCDBitmap.h>

LiquidCrystal_SR2W lcd(2, 3); 
LCDBitmap bitmap(&lcd, 12, 0);  // Assign the LCD bitmap to "&lcd" set in the previous line and set display location to (12,0)

unsigned long currentMillis;  // Timer variable
const int sample = 4000;  // Sets how long to run each sample

void setup() {
  lcd.begin(16,2);  // Initialize the LCD 
  bitmap.begin();  // Initalize the LCD bitmap
  bitmap.home();  // Move cursor the home position (0,0)
  lcd.print("LCDBitmap");  
  randomSeed(analogRead(A0));
}

void loop() {
  // bitmap.pixel
  lcd.setCursor(0, 1);
  lcd.print(".pixel   ");
  currentMillis = millis();
  while(millis()-currentMillis<sample) {
    for (byte i=0; i<8; i++) {
      bitmap.pixel(random(0, BITMAP_W), random(0, BITMAP_H), random(0, 2), NO_UPDATE);  // Add one random pixel but don't update bitmap display
    }
    bitmap.update();  // 8 pixels positioned, now update bitmap display
  }

  // bitmap.line sample #1
  lcd.setCursor(0, 1);
  lcd.print(".line #1 ");
  bitmap.clear();  // Clear the bitmap display (automatically updates bitmap display) doesn't clear text
  currentMillis = millis();
  int startdelay=120;
  while(millis()-currentMillis<sample*2) {
    for (byte x=0; x<BITMAP_W; x=x+2) {
      bitmap.line(x, 0, BITMAP_W-x-1, BITMAP_H-1, ON, UPDATE);  // Draw the line from (x, 0) to (BITMAP_W-x-1, BITMAP_H-1) and update bitmap display
      delay(startdelay);
      bitmap.clear();  // Clear the display
    }
    for (byte y=0; y<BITMAP_H; y=y+2) {
      bitmap.line(0, BITMAP_H-y-1, BITMAP_W-1, y, ON, UPDATE);
      delay(startdelay);
      bitmap.clear();  // Clear the display
    }
    startdelay=startdelay*2/3;
  }

  // bitmap.line sample #2
  lcd.setCursor(0, 1);
  lcd.print(".line #2 ");
  currentMillis = millis();
  while(millis()-currentMillis<sample) {
    for (byte x=0; x<BITMAP_W; x++) {
      bitmap.line(x, 0, x, BITMAP_H-1, ON, UPDATE);  // Draw the line from (x, 0) to (x, BITMAP_H-1) and update bitmap display
    }
    for (byte y=0; y<BITMAP_H; y++) {
      bitmap.line(0, y, BITMAP_W-1, y, OFF, UPDATE);
    }
  }
  bitmap.clear();  // Clear the display

  // bitmap.rect
  lcd.setCursor(0, 1);
  lcd.print(".rect    ");
  currentMillis = millis();
  while(millis()-currentMillis<sample) {
    bitmap.rect(random(0, BITMAP_W), random(0, BITMAP_H), random(0, BITMAP_W), random(0, BITMAP_H), ON, UPDATE);  // Draw a rectangle from (x1,y1) to (x2,y2) and update bitmap display
    delay(150);
    bitmap.clear();  // Clear the display
  }

  // bitmap.rectFill
  lcd.setCursor(0, 1);
  lcd.print(".rectFill");
  currentMillis = millis();
  while(millis()-currentMillis<sample*1.5) {
    bitmap.rectFill(random(0, BITMAP_W), random(0, BITMAP_H), random(0, BITMAP_W), random(0, BITMAP_H), ON, NO_UPDATE);  // Draw a filled rectangle from (x1,y1) to (x2,y2) and update bitmap display
    bitmap.rectFill(random(0, BITMAP_W), random(0, BITMAP_H), random(0, BITMAP_W), random(0, BITMAP_H), OFF, NO_UPDATE);  // Draw a filled rectangle from (x1,y1) to (x2,y2) and update bitmap display
    bitmap.update();  // Filled rectangles drawn, now update bitmap display
    delay(10);
  }
  bitmap.clear();  // Clear the display

  // bitmap.barGraph
  lcd.setCursor(0, 1);
  lcd.print(".barGraph ");
  byte bars=10;
  byte graph[bars];  // 10 Bar graph values
  currentMillis = millis();
  while(millis()-currentMillis<sample*2) {
    for (byte x=0; x<bars; x++) {
      graph[x] = random(0, BITMAP_H);  // Fill array with random height values
    }
    bitmap.barGraph(bars, graph, ON, UPDATE);  // Display the bar graph
    delay(10);
    if (bars==10 && millis()-currentMillis>=sample) bars=4;
  }
  bitmap.clear();  // Clear the display

  // This creates a big "X" using the LCDBitmap to show the inverse and move functions
  for (byte x=0; x<=4; x++) {
    bitmap.line(x, BITMAP_H-1, x+BITMAP_H-1, 0, ON, NO_UPDATE);  // Draw a line, but don't update the bitmap display
    bitmap.line(x, 0, x+BITMAP_H-1, BITMAP_H-1, ON, NO_UPDATE);  // Draw a line, but don't update the bitmap display
  }
  bitmap.update();  // Now that everything is written to the bitmap, show the changes

  // bitmap.inverse
  lcd.setCursor(0, 1);
  lcd.print(".inverse ");
  delay(1000);
  int x_status = 1;
  currentMillis = millis();
  while(millis()-currentMillis<sample/2) {
    for (byte a=0; a<6; a++) {
      bitmap.inverse();  // Invert the bitmap, automatically updates the bitmap display
      x_status *= -1;
      delay(75);
    }
  }
  if (x_status==-1) bitmap.inverse();

  // bitmap.move
  bitmap.clear_text();  // Clear just the text on the display (leaves bitmap alone)
  lcd.setCursor(0, 1);
  lcd.print(".move");
  for (byte x=11; x>4; x--) {
    bitmap.move(x,0);  // Move the LCD bitmap position to the left one position
    delay(150);
  }
  for (byte x=6; x<13; x++) {
    bitmap.move(x,0);  // Move the LCD bitmap position to the right one position
    delay(150);
  }
  bitmap.home();  // Move cursor the home position (0,0)
  lcd.print("LCDBitmap");  
  delay(500);
  
  // bitmap.clear
  lcd.setCursor(0, 1);
  lcd.print(".clear   ");
  delay(1000);
  bitmap.clear();  // Clear the bitmap display only (automatically updates bitmap display) doesn't clear text
  delay(2000);
}