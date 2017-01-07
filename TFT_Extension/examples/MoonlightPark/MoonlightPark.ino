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
  randomSeed(1000);
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);

  myGLCD.fillScr(BLACK);
  myTFT.drawMoon(270, 50, 30, 1);

  for(byte S=0; S < 30; S++)
    myTFT.drawStar(random(20,240), random(10, 100), random(1,7), WHITE, FILL);
                   // X, Y, Xrad, Yrad, XT, YT, Start, Stop, R,G,B);
  myTFT.drawOvalArc(160, 239, 160, 10, 160, 10, 0, 180, GREEN);

  for(byte Sf=0; Sf < 15; Sf++)
    stickFigures();

}

void loop() {
}

void stickFigures()
{
  int X = random(10,310);
  int Y = random(180,200);
  myTFT.smiley_Face(X,Y, 5, OVER_JOY);
  myGLCD.setColor(random(0,255),random(0,255),random(0,255));
  myGLCD.drawLine(X,Y+5, X, Y+20); // body
  myGLCD.drawLine(X,Y+10, X-6, Y+5); // left arm
  myGLCD.drawLine(X,Y+10, X+6, Y+5); // right arm
  myGLCD.drawLine(X,Y+20, X-6, Y+25); // left leg
  myGLCD.drawLine(X,Y+20, X+6, Y+25); // right leg
}

