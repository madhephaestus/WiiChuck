#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>

// Declare which fonts we will be using
extern uint8_t BigFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myGLCD.fillScr(BLACK);
  myGLCD.setColor(RED);
  myGLCD.fillCircle(160,120,50);
  myGLCD.setColor(BLUE);
  myGLCD.fillCircle(60,60,30);
  
}

void loop()
{ 
  myGLCD.printNumI(myTFT.TouchCircleByQuad(160,120,50),0,0);// RED circle, will return the quadrant touched
  myGLCD.printNumI(myTFT.TouchCircleByQuad(60,60,30,2,4),0,20);// BLUE circle, Will return true if quadrants 2 or 4 are touched
}


