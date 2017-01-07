#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S, A1,A2,A0,A3,A5);
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
  myGLCD.fillScr(BLACK);

  myTFT.Polygon(160,120,6,60,RED, FILL); // X, Y, sides of shape, diameter of shape.
  myTFT.Polygon(50,50,3,20,WHITE, NOFILL);
  myTFT.Polygon(250,50,5,20,BLUE, FILL);
  myTFT.Polygon(50,190,7,20,PURPLE, FILL);
  myTFT.Polygon(250,190,4,20,YELLOW, NOFILL);
}

void loop() { }
