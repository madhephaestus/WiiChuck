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

int XI,YI,XI2;
void setup()
{
  randomSeed(100);
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myGLCD.fillScr(GREY);
  
  for(byte n = 0; n < 50; n++)
  myTFT.smiley_Face(160 + random(-100, 100), 120 + random(-80, 80), random(10, 40), random(0,10), random(0,8));
}

void loop()
{

}

void smiley_face(int cx, int cy, int radius, byte eyes, byte mouth)
{
  myGLCD.setColor(YELLOW);
  myGLCD.fillCircle(cx,cy,radius);
  myGLCD.setColor(BLACK);
  myGLCD.drawCircle(cx,cy,radius);

  switch(eyes)
  {
  case 0:// normal eyes
    for(float rotate = 0 ; rotate <= 360 ; rotate+=1) 
    {
      XI = (cx - radius/3) + sin((rotate - 90) * deg_to_rad) * (radius/10);
      YI = (cy - radius/3) + cos((rotate - 90) * deg_to_rad) * (radius/3.3);
      XI2 = (cx + radius/3) + sin((rotate - 90) * deg_to_rad) * (radius/10);
      myGLCD.drawLine(cx - radius/3,cy - radius/3,XI,YI);
      myGLCD.drawLine(cx + radius/3,cy - radius/3,XI2,YI);
    }
    break;

  case 1: // round eyes
    myGLCD.fillCircle(cx - radius/3, cy - radius/3, radius/8);
    myGLCD.fillCircle(cx + radius/3, cy - radius/3, radius/8);
    break;

  case 2: // over joyed
    myTFT.drawArc(cx - radius/3, cy - radius/3, radius/5, 0, 180, radius/25, BLACK); 
    myTFT.drawArc(cx + radius/3, cy - radius/3, radius/5, 0, 180, radius/25, BLACK);
    break;

  case 3: // content
    myTFT.drawArc(cx - (radius/3), cy - (radius/3), radius/5, 180, 360, radius/25, BLACK);
    myTFT.drawArc(cx + (radius/3), cy - (radius/3), radius/5, 180, 360, radius/25, BLACK);
    break;

  case 4: // angry
    for(byte i = 0; i < 2; i++)
    {
      myGLCD.drawLine((cx - radius/5) - i, cy - radius/5, (cx - radius/2) - i, cy - radius/2);
      myGLCD.drawLine((cx + radius/5) - i, cy - radius/5, (cx + radius/2) - i, cy - radius/2);
    }
    break;

  case 5://very angry
    for(byte i = 0; i < 2; i++)
    {
      myGLCD.drawLine((cx - radius/5) - i, cy - radius/5, (cx - radius/2) - i, cy - radius/2);
      myGLCD.drawLine((cx + radius/5) - i, cy - radius/5, (cx + radius/2) - i, cy - radius/2);
      myTFT.drawArc(cx - radius/3, cy - radius/3, radius/6, 135, 305, i, BLACK);
      myTFT.drawArc(cx + radius/3, cy - radius/3, radius/6, 240, 395, i, BLACK);
    }
    break;

  case 6: // big eyes
    myTFT.drawArc(cx - radius/3, cy - radius/3, radius/6, 0, 360, radius/25, BLACK);
    myTFT.drawArc(cx + radius/3, cy - radius/3, radius/6, 0, 360, radius/25, BLACK);
    break;

  case 7:
    myGLCD.setColor(WHITE);
    myGLCD.fillCircle(cx - radius/4, cy - radius/3, radius/5);
    myGLCD.fillCircle(cx + radius/4, cy - radius/3, radius/5);
    myGLCD.setColor(BLACK);
    myGLCD.drawCircle(cx - radius/4, cy - radius/3, radius/5);
    myGLCD.drawCircle(cx + radius/4, cy - radius/3, radius/5);
    myGLCD.drawPixel(cx - radius/4, cy - radius/3);
    myGLCD.drawPixel(cx + radius/4, cy - radius/3);
    break;

  case 8:
    myGLCD.drawLine(cx - radius/3, cy - radius/1.5, cx, cy - radius/2.5);
    myGLCD.drawLine(cx - radius/3, cy - radius/3.5, cx, cy - radius/2.5);
    myGLCD.drawLine(cx + radius/3, cy - radius/1.5, cx, cy - radius/2.5);
    myGLCD.drawLine(cx + radius/3, cy - radius/3.5, cx, cy - radius/2.5);
    break;

  case 9:
    myTFT.drawArc(cx - radius/3, cy - radius/3, radius/5, 180, 361, radius/5, BLACK);
    myTFT.drawArc(cx + radius/3, cy - radius/3, radius/5, 180, 361, radius/5, BLACK); 
    break;
  }

  switch(mouth)
  {
  case 0:
    myTFT.drawArc(cx,cy, int(float(radius)/1.6), 180, 360, radius/25, BLACK); // big happy smile
    break;

  case 1:
    myTFT.drawArc(cx,cy, int(float(radius)/1.6), 210, 330, radius/25, BLACK); // happy smile
    break;

  case 2:
    myTFT.drawArc(cx,cy + radius/1.5 , radius/2, 30, 150, radius/25, BLACK); // frown
    break;

  case 3: // big open smile
    myTFT.drawArc(cx,cy, int(float(radius)/1.6), 181, 360, int(float(radius)/1.6), WHITE);
    for(byte i = 0; i <= radius/25; i++)
    {
      myTFT.drawArc(cx,cy, int(float(radius)/1.6), 181, 360, i, BLACK);
      myGLCD.drawLine(cx - radius/1.8, cy+i, cx + radius/1.8, cy+i);
    }
    break;

  case 4:
    myTFT.drawArc(cx,cy + radius , radius/2, 60, 120, radius/25, BLACK); //big frown
    break;

  case 5:
    myTFT.drawArc(cx,cy + radius/1.5 , radius/2, 0, 180, radius/25, BLACK); // bigger frown
    break;

  case 6:
    myTFT.drawArc(cx,cy + radius/1.5 , radius/2, 70, 110, radius/3, BLACK); // shocked open mouth
    break;

  case 7:
    myTFT.drawArc(cx,cy + radius , radius/1.2, 50, 130, radius/3, BLACK); // crying mouth
    break;

  }
}

















