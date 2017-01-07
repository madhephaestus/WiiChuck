// UTFT_Geometry_Demo_320x240 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A small demo to demonstrate the use of some of the
// functions of the UTFT_Geometry add-on library.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library in addition to
// the UTFT_Geometry add-on library.
//

#include <UTFT.h>
#include <UTFT_Geometry.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino 2009/Uno/Leonardo shield   : <display model>,19,18,17,16
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Standard chipKit Uno32/uC32                 : <display model>,34,35,36,37
// Standard chipKit Max32                      : <display model>,82,83,84,85
// AquaLEDSource All in One Super Screw Shield : <display model>,82,83,84,85
// CC3200 LaunchPad (pins used in the examples): <display model>,15,18,11,32
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ITDB32S,38,39,40,41);

UTFT_Geometry geo(&myGLCD);

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

  myGLCD.setBackColor(VGA_BLACK);
}

void clearField()
{
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRect(1,15,318,224);
}

void loop()
{
  int x1,x2,x3,y1,y2,y3,r,as,ae;
  long smillis = millis();
  
  myGLCD.clrScr();
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRect(0, 0, 319, 13);
  myGLCD.setColor(VGA_GRAY);
  myGLCD.fillRect(0, 226, 319, 239);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.print("*** UTFT Geometry Add-on Library ***", CENTER, 1);
  myGLCD.setBackColor(VGA_GRAY);
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.print("<http://www.RinkyDinkElectronics.com/>", CENTER, 227);

  myGLCD.setColor(VGA_BLUE);
  myGLCD.drawRect(0, 14, 319, 225);

  myGLCD.setColor(VGA_LIME);
  geo.drawArc(160,240,210,-40,40,3);
  for (int i=-40; i<=40; i+=8)
    geo.drawArc(160,240,205,i,i,10);
  
  int l = 0;
  for (int i=0; i>=-40; i-=1)
  {
    myGLCD.setColor(VGA_BLACK);
    geo.drawArc(160,240,110,l,l,170);
    l=i;
    myGLCD.setColor(VGA_YELLOW);
    geo.drawArc(160,240,110,i,i,170);
    delay(40-abs(i));
  }
  for (int i=-40; i<=40; i+=1)
  {
    myGLCD.setColor(VGA_BLACK);
    geo.drawArc(160,240,110,l,l,170);
    l=i;
    myGLCD.setColor(VGA_YELLOW);
    geo.drawArc(160,240,110,i,i,170);
    delay(40-abs(i));
  }
  for (int i=40; i>=0; i-=1)
  {
    myGLCD.setColor(VGA_BLACK);
    geo.drawArc(160,240,110,l,l,170);
    l=i;
    myGLCD.setColor(VGA_YELLOW);
    geo.drawArc(160,240,110,i,i,170);
    delay(40-i);
  }
  
  delay(2000);
  clearField();

// Draw some random triangles
  for (int i=0; i<50; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x1=2+random(316);
    y1=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    x3=2+random(316);
    y3=16+random(207);
    geo.drawTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(2000);
  clearField();

// Draw some random filled triangles
  for (int i=0; i<50; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x1=2+random(316);
    y1=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    x3=2+random(316);
    y3=16+random(207);
    geo.fillTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(2000);
  clearField();

// Draw some random pies
  for (int i=0; i<50; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x1=32+random(256);
    y1=45+random(146);
    r=10+random(20);
    as=random(360);
    ae=random(360);
    geo.drawPie(x1, y1, r, as, ae);
  }

  delay(2000);

  myGLCD.fillScr(VGA_BLUE);
  myGLCD.setColor(VGA_RED);
  myGLCD.fillRoundRect(80, 70, 239, 169);
  
  myGLCD.setColor(VGA_WHITE);
  myGLCD.setBackColor(VGA_RED);
  myGLCD.print("That's it!", CENTER, 93);
  myGLCD.print("Restarting in a", CENTER, 119);
  myGLCD.print("few seconds...", CENTER, 132);
  
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRect(0, 220, 319, 239);
  myGLCD.setColor(VGA_LIME);
  myGLCD.setBackColor(VGA_BLACK);
  myGLCD.print("Processing time: (msecs)", LEFT, 225);
  myGLCD.printNumI(millis()-smillis-11120, RIGHT, 225);
  
  delay (10000);
}

