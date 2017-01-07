#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch);

byte Percent = 0;
char tmp[6];

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  myGLCD.fillScr(WHITE);
 
  //X, Y, radius, thickness, startAngle, stopAngle, color
  myTFT.drawArc(160, 120, 60, 20, 0, 180, BLUE);
}

void loop()
{
  //X, Y, radius, thickness, startAngle, stopAngle
  Percent = myTFT.TouchArc(160, 120, 60, 20, 0, 180);
  
  myGLCD.setBackColor(WHITE);
  myGLCD.setColor(BLACK);
  sprintf(tmp, "%d%%  ", Percent);
  myGLCD.print(tmp, 150, 115);
  
  
}
