#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myGLCD.fillScr(WHITE);
}

void loop()
{  
  //(X1, Y1, X2, Y2, ID Number, Color)
  byte red = map(myTFT.VertSlider(20, 20, 50, 210, 0, RED), 0, 100, 0, 255);
  byte green = map(myTFT.VertSlider(70, 20, 100, 210, 1, GREEN), 0, 100, 0, 255);
  byte blue = map(myTFT.VertSlider(120, 20, 150, 210, 2, BLUE), 0, 100, 0, 255);
  
  myGLCD.setColor(WHITE);
  myGLCD.printNumI(red, 20, 10);
  myGLCD.printNumI(green, 70, 10);
  myGLCD.printNumI(blue, 120, 10);
  
  myGLCD.setColor(red, green, blue);
  myGLCD.fillRect(200, 10, 250, 60);
}
