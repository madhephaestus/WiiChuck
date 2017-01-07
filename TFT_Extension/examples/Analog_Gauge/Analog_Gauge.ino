#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S, A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch);

unsigned long time=0;
double val = 0, val2 = 0;
byte speed = 0;
void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  myGLCD.fillScr(BLACK);  
  myTFT.ResetGauge(0);
}

void loop() 
{
  // I am using my accelerometer for the gauge input value.
  // I have also constrained the mapped values to prevent any spikes from the accelerometer.
 if(millis() - time > 100) // slow down the readings for smoother effect.
  {
    time = millis();
    //myGLCD.printNumI(analogRead(A15),0,0);
    val = map( constrain( analogRead(A15),280,420 ), 280, 420, -45, 225);
    val2 = map( constrain( analogRead(A14),280,420 ), 280, 420, -45, 225);   
  }
  myTFT.drawGauge(0,val,100,100,-45,225, 50); // "ID, value, X,Y, Start, Stop, Radius"
  myTFT.drawGauge(1,val2,200,100,-45,225, 50);
  speed = map(val, -45, 225, 160, 0); // map the analog value to a set speed range, 0 - 160.
  myGLCD.printNumI(speed, 0,20);
}
