#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);

// If you are not using any function that require the UTouch 
// library, then you can make it NULL (Not &NULL).
TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myGLCD.fillScr(BLUE);
  myTFT.SpeechBubble("Hello", 160,120,30,235);
}

void loop() {
}
