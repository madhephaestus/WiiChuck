#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>

#include "pitches.h"

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

UTFT myGLCD(A1,A2,A0,A3,A5,ITDB32S); //myGLCD(RS,WR,CS,RST,ALE,mode);
UTouch  myTouch(13,10,11,12,A4); //myTouch(TCLK,TCS,DIN,DOUT,IRQ);
TFT_Extension myTFT(&myGLCD, &myTouch);

int XC,YC,X = random(3,316),Y = random(3,236),BallX = 5,BallY = 2,oldX,oldY;
int BallRadius = 3, Balls = 3, lastball = -1; // Changed
int NY, old_YC = -1;

byte _Piezopin = 43;
byte _PiezoGnd = 49;

void setup()
{
  pinMode(_PiezoGnd, OUTPUT);
  digitalWrite(_PiezoGnd, LOW); //piezo GND

  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  myGLCD.fillScr(BLACK);
  myGLCD.drawLine(0,12, 319 , 12);
  myGLCD.print("Balls Remaining:",0,0);
  myGLCD.printNumI(Balls, 140, 0);
  myGLCD.setColor(GREY);
  myGLCD.fillRect(0,12,3,239);      
}

void loop()
{ 

  //data(XC,YC);

  myGLCD.setColor(WHITE);
  if(X - BallRadius <= 7) // wall
  {
    BallX = -BallX;
    PlayTone(NOTE_GS1,20); 
  } 

  if(Y + BallRadius >= 236 || Y - BallRadius <= 13)// top and bottom boundaries 
  {
    BallY = -BallY;
    PlayTone(NOTE_FS1,20); 
  }

  if(X + BallRadius >= 316) // paddle area
  {
    PlayTone(NOTE_B0,3000);
    PlayTone(NOTE_B0,3000);
    PlayTone(NOTE_B0,3000); 
    Balls--; 
    if(Balls != lastball)
    {
      myGLCD.setFont(SmallFont);
      myGLCD.printNumI(Balls, 140, 0);
      lastball = Balls;
    }
    delay(1000); 
    X = random(20,100); 
    Y = random(20,100);
  }

  if(Balls > 0)
  {
    oldX = X;
    X += BallX;
    oldY = Y;
    Y += BallY;
    myGLCD.setColor(BLUE);
    myGLCD.fillCircle(X,Y,BallRadius);
    myGLCD.setColor(BLACK);
    myGLCD.fillCircle(oldX,oldY,BallRadius);

    check(paddle(), X, Y);
  }
  else 
  {
    GameOverScreen();
  }
}

void data(int X, int Y)
{
  myGLCD.setBackColor(PURPLE);//background of text purple
  myGLCD.setColor(WHITE);
  myGLCD.print("X: ", 0,20, 0);
  myGLCD.printNumF(X, 0, 30, 20);
  myGLCD.print("Y: ", 100,20,0);
  myGLCD.printNumF(Y, 0, 130, 20);
  return; 
}

int paddle()
{
  if(myTouch.dataAvailable())
  {
    myTouch.read();
    int tx = myTouch.getX();
    int ty = myTouch.getY();
    XC= tx > 320? 0 : tx; 
    YC= ty > 240? 0 : ty; 
  }
  
  myGLCD.setColor(WHITE);
  if(myTFT.TouchButton(310,1,319,239))
  {
    if((YC + 25) < 239 && (YC - 25) > 0)
    {
      NY = YC;
      myGLCD.fillRect(310,YC - 25,319,YC + 25);

      if( YC != old_YC )
      {
        myGLCD.setColor(BLACK);
        if(YC >= old_YC)
          myGLCD.fillRect(310,1,319,YC - 26);
        else  
          myGLCD.fillRect(310,YC + 26,319,239);
        old_YC = YC;
      }
    }
  }
  else 
  {
    myGLCD.setColor(WHITE);
    myGLCD.fillRect(310,NY - 25,319,NY + 25);
  }
  return NY;
}

void check(int paddlePos, int BallXP, int BallYP)
{
  if(BallXP >= 307 && BallYP >= (paddlePos - 25) && BallYP <= (paddlePos + 25))
  {
    BallX = -BallX;
    PlayTone(NOTE_C1, 20);
  }
}  

void makeButton()
{
  myGLCD.drawRect(80,139, 145, 169); //yes
  myGLCD.drawRect(170,139, 235, 169); //no
}

void PlayTone(byte Tone, byte _duration)
{
  tone(_Piezopin, Tone, _duration);
}

void GameOverScreen()
{
  myGLCD.setColor(200,0,0); // Dark red
  myGLCD.setFont(BigFont);
  myGLCD.print("GAME OVER", CENTER, 90);
  delay(1000);
  myGLCD.setColor(GREEN);
  myGLCD.print("Play Again?", CENTER, 111);
  makeButton();
  if(myTFT.TextButton("Yes", Big, 80, 139, 145, 169, YELLOW))
  {
    Balls = 3;
    setup();
  }
  else if(myTFT.TextButton("No", Big, 170, 139, 235, 169, RED))
  {
    myGLCD.fillScr(BLACK);
    myGLCD.setColor(BLUE);
    myGLCD.print("Thanks for playing", CENTER, 111);
    while(1);
  } 
}






