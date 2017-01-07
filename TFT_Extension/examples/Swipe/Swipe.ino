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
  myGLCD.fillScr(0,0,0);
  myGLCD.setColor(BLUE);
  myGLCD.drawRect(140,100,180,140);
}

void loop()
{ 
  myGLCD.setColor(RED); 
  myGLCD.printNumI(myTFT.Swipe(0,50),0,0); // 1 = up, 2 = down, 3 = left, 4 = right, 5 = upLeft, 6 = downLeft, 7 = upRight, 8 = downRight
  myGLCD.setColor(BLUE); 
  myGLCD.printNumI(myTFT.SwipeFromArea(140,100,180,140,0,50),0,20);
}

