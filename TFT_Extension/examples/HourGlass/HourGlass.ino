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


//boolean locked = false;
//byte flip = 0;
//int rad;
//unsigned long T = 0;

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  
  myGLCD.setColor(WHITE);
}

void loop() {
  myTFT.HourGlass(160,120,30,60); // (X, Y, height, time in seconds) function is currently set to 60 seconds
}

//void HourGlass(int cx, int cy, int height, int time)
//{
//  myGLCD.setColor(BLACK);
//  if(!locked)
//  {
//    rad = height;
//    locked = true;
//  }
//
//  if(rad >= 0)
//  {
//    int Cx  = cx + sin((90 + (45))* deg_to_rad) * rad; 
//    int Cy  = cy + cos((90 + (45)) * deg_to_rad) * rad;
//    int Cx2 = cx + sin((90 + (180-45)) * deg_to_rad) * rad; 
//    int Cy2 = cy + cos((90 + (180-45)) * deg_to_rad) * rad;
//    int Cx3 = cx + sin((90 + (45+180)) * deg_to_rad) * rad; 
//    int Cy3 = cy + cos((90 + (45+180)) * deg_to_rad) * rad;
//    int Cx4 = cx + sin((90 + (-45)) * deg_to_rad) * rad; 
//    int Cy4 = cy + cos((90 + (-45)) * deg_to_rad) * rad;
//    switch(flip)
//    {
//    case 0:
//      rad--;
//      myGLCD.setColor(BLACK);
//      myGLCD.drawLine(Cx , Cy, Cx2, Cy2);// bottom 
//      myGLCD.drawLine(Cx2, Cy2, Cx4, Cy4);// left
//      myGLCD.drawLine(Cx3, Cy3, Cx, Cy); // right
//      myGLCD.setColor(WHITE);
//      myGLCD.drawLine(Cx3, Cy3, Cx4, Cy4); // top
//      break;
//
//    case 1:
//      if(millis() - T > float((time*1000UL) / float(height)))
//      {
//       T = millis(); 
//       rad--;
//      }
//      myGLCD.setColor(WHITE);
//      myGLCD.drawLine(Cx+2 , Cy, Cx2-2, Cy2);// bottom 
//      myGLCD.drawLine(cx-1,cy, cx-1,Cy);
//      myGLCD.setColor(BLACK);
//      myGLCD.drawLine(Cx3-2, Cy3, Cx4+2, Cy4); // top
//      break;
//    }
//  }
//  else 
//  {
//    flip = !flip;
//    rad = height;
//    T = millis(); 
//  }
//}






