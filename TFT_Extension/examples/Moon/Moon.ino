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


//int XI,YI, cx,cy;

#define WANING_GIBBOUS 7
#define THIRD_QUARTER 6
#define WANING_CRESCENT 5 
#define NEW 4
#define WAXING_CRESCENT 3
#define FIRST_QUARTER 2
#define WAXING_GIBBOUS 1
#define FULL 0 

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myGLCD.fillScr(BLACK);
  
  myTFT.drawMoon(160, 120, 20, FULL);
}

void loop()
{

}

//void Moon(int cx, int cy, int radius, byte phase)
//{
//  byte C = 0, N;
//  int i=0;
//  
//  int Rad_D2 = radius/2;
//  int Rad_D1_5 = radius/1.5;
//  int Rad_M2 = radius*2;
//  int Rad_M4 = radius*4;
//  
//  myGLCD.setColor(240,240,240);
//  myGLCD.fillCircle(cx,cy,radius);
//  for(N = 0; N < (Rad_D2); N++)
//  {
//    C = random(0, 20);
//    myGLCD.setColor(230 - C, 230 - C, 230 - C);
//    myGLCD.fillCircle(cx + random(-Rad_D1_5, Rad_D1_5), 120 + random(-Rad_D1_5, Rad_D1_5), random(2, radius/7));
//  }
//
//  myGLCD.setColor(100,100,100);
//  myGLCD.drawCircle(cx,cy, radius);
//  
//  int rad = map(phase, 0, 8, 0, Rad_M4);
//  for(i = 0 ; i < rad; i++ )
//  {
//    if(i > Rad_M2)
//      myGLCD.setColor(240,240,240);   
//    else 
//      myGLCD.setColor(100,100,100);
//      
//    for(float rotate = 90 ; rotate <= 270 ; rotate+=1)
//    {
//      XI = cx + sin((rotate - 90) * deg_to_rad) * float(radius - (i>(Rad_M2)? i-(Rad_M2):i));
//      YI = cy + cos((rotate - 90) * deg_to_rad) * float(radius);
//      myGLCD.drawPixel(XI,YI);
//    }
//  }
//  //myGLCD.printNumI(i, 0,0);
//  if(i == 0 || i == Rad_M2) {}
//  else if(i > Rad_M2)
//  {
//    for(N = 0; N < (Rad_D2); N++)
//    {
//      int tmp = random(-Rad_D1_5, map(i-5, Rad_M2, Rad_M4, -Rad_D1_5, Rad_D1_5));
//      int tmp2 = random(-Rad_D1_5, Rad_D1_5);
//      C = random(0, 20);
//      myGLCD.setColor(230 - C, 230 - C, 230 - C);
//      myGLCD.fillCircle(cx + tmp, 120 + tmp2, random(2, radius/7));
//    }
//  }
//}








