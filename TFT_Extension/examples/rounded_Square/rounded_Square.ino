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
  myTFT.rounded_Square(160,120, 80,40,20, BLUE, FILL);
  myTFT.rounded_Square(60,120, 40,40,10, RED, FILL);
  myTFT.rounded_Square(260,120, 80,80,40, GREEN, NOFILL);
}

void loop()
{

}

//void rounded_square(int cx,int cy,int h, int w, float radius, byte R, byte G, byte B, bool fill)
//{ 
//  byte angle;
//  float i;
//
//  int newW = (w/2) - (radius/2);
//  int newH = (h/2) - (radius/2);
//
//  int XPstart[4], XPend[4];
//  int YPstart[4], YPend[4];
//
//  myGLCD.setColor(R,G,B);
//
//  for(angle = 0; angle < 4; angle++)
//  {
//    XPstart[angle] = ((angle <= 1)? cx - newW : cx + newW) + sin(90*angle * deg_to_rad) * (radius/2);
//    YPstart[angle] = ((angle == 0 || angle == 3) ? cy - newH : cy + newH) + cos(90*angle * deg_to_rad) * (radius/2);
//    for(i = 90*angle; i <= 90*(angle+1); i++)
//    { 
//      XPend[angle] = ((angle <= 1)? cx - newW : cx + newW) + (sin(i * deg_to_rad) * (radius/2));
//      YPend[angle] = ((angle == 0 || angle == 3) ? cy - newH : cy + newH) + (cos(i * deg_to_rad) * (radius/2));
//
//      if(!fill) // no fill
//        myGLCD.drawPixel(XPend[angle], YPend[angle]); 
//    }
//    if(fill) //fill
//      myTFT.drawArc(
//       ((angle <= 1)? cx - newW : cx + newW), // Xcoords
//       ((angle == 0 || angle == 3) ? cy - newH : cy + newH), //Ycoords
//       radius/2, // radius of arc
//       90*angle + 90, // start angle + 90* to correctly position the Arc
//       i + 90, // end angle + 90*
//       radius/2, // must match arc radius
//       R, G, B // color
//      );
//  }
//  
//  myGLCD.setColor(R,G,B); 
//  if(fill) // fill in the area between the outer lines
//  {
//    myGLCD.fillRect(XPend[0], YPend[0], XPend[2], YPend[2]); //fill horizontally
//    myGLCD.fillRect(XPstart[0], YPstart[0], XPstart[2], YPstart[2]+1); // fill vertically
//  }
//  else
//  {
//    for(angle=0; angle <= 3; angle++)// draw the outer lines
//    {
//      myGLCD.drawLine(XPend[angle], YPend[angle], XPstart[angle+1 == 4? 0 : angle+1], YPstart[angle+1 == 4? 0 : angle+1]);
//    }
//  }
//}










