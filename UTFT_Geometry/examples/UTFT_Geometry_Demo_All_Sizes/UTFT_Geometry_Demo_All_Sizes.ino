// UTFT_Geometry_Demo_All_Sizes 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A small demo to demonstrate the use of some of the
// functions of the UTFT_Geometry add-on library.
//
// This demo was made for modules with of all screen 
// resolutions.
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

int max_x, max_y;

void setup()
{
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(VGA_BLACK);
  max_x = myGLCD.getDisplayXSize();
  max_y = myGLCD.getDisplayYSize();
}

void loop()
{
  int x1,x2,x3,y1,y2,y3,r,as,ae;
  myGLCD.clrScr();

// Draw some random triangles
  for (int i=0; i<50; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x1=random(max_x);
    y1=random(max_y);
    x2=random(max_x);
    y2=random(max_y);
    x3=random(max_x);
    y3=random(max_y);
    geo.drawTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(2000);
  myGLCD.clrScr();

// Draw some random filled triangles
  for (int i=0; i<50; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x1=random(max_x);
    y1=random(max_y);
    x2=random(max_x);
    y2=random(max_y);
    x3=random(max_x);
    y3=random(max_y);
    geo.fillTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(2000);
  myGLCD.clrScr();

// Draw some random pies
  for (int i=0; i<50; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x1=30+random(max_x-60);
    y1=30+random(max_y-60);
    r=10+random(20);
    as=random(360);
    ae=random(360);
    geo.drawPie(x1, y1, r, as, ae);
  }

  delay(2000);
}

