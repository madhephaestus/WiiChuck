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

//===========================READ ME!================================
//These indicate which way the Slider/Bargraph will draw when touched
// LTR = Left to Right : RTL = Right to Left
//===================================================================
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
  //byte red = map(myTFT.HorSlider(20, 20, 210, 50, 0, RED, LTR), 0, 100, 0, 255);
  //byte green = map(myTFT.HorSlider(20, 70, 210,100, 1, GREEN, LTR), 0, 100, 0, 255);
  byte blue = map(myTFT.HorSlider(20, 120, 210,150, 2, BLUE, LTR), 0, 100, 0, 255);
  
  myGLCD.setColor(WHITE);
  //myGLCD.printNumI(red, 20, 9);
  //myGLCD.printNumI(green, 20, 59);
  myGLCD.printNumI(blue, 20, 109);
  
  myGLCD.setColor(0, 0, blue);
  myGLCD.fillRect(230, 10, 280, 60);
                 //(X1, Y1, X2, Y2, value, lowest, high, ID Number, Color)
  myTFT.HorBarGraph(20, 20, 210, 50, blue, 0, 255, 2, BLUE, LTR); 
}










