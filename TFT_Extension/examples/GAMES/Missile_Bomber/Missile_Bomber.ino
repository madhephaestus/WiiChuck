#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];

UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5); //myGLCD(RS,WR,CS,RST,ALE,mode);
UTouch  myTouch(13,10,11,12,A4); //myTouch(TCLK,TCS,DIN,DOUT,IRQ);
TFT_Extension myTFT(&myGLCD, &myTouch);

unsigned long myScore = 0;
boolean state = false;
char Cmsg[18];
int C=20, lastScore = -1;
byte num= 1, R = 0;

class missile
{
public:

  void makeMissile()
  {
    if(!locked)
    {
      City_Destroyed = false;
      X = random(50, 270);
      shift = (double(random(-2, 2))/2.0);
      yy= 5;
      oldX = (X+xx);
      oldY = 5;
      xx=0;
      locked = true;
    }

    myGLCD.setColor(BLACK);
    myTFT.drawTriangle(oldX, oldY, 7, down);
    myGLCD.setColor(WHITE);
    myTFT.drawTriangle(X+xx, yy, 7, down);
    oldX = (X+xx);
    oldY = yy;

    if(myTFT.TouchTriangle(X+xx, yy, 2, down)/*(Tx == (X+xx) && Ty == yy)*/ || yy  > 225)
    {
      for(byte i = 1; i < 10; i++)
      {
        myTFT.drawStar(X+xx, yy, i,RED, NOFILL);
      }
      myTFT.drawStar(X+xx, yy, 12,BLACK, FILL);
      if(yy < 225)
        Score += 5;
      else
        City_Destroyed = true;
      locked = false;
    }

    else
    {
      yy = (yy+1 < 230? yy+1 : 1);
      xx += shift;
    }
  }

  int GetScore() 
  { 
    return Score;
  }

  byte CitiesRemaining()
  {
    return City_Destroyed;
  }

  void resetScore()
  {
    Score = 0;
  }


private:
  int X,yy, oldX, oldY, Score, Cities;
  double shift, xx;
  boolean locked, City_Destroyed;
};

missile M1,M2,M3,M4,M5,M6;
missile missiles[6] = {
  M1,M2,M3,M4,M5,M6};
  
void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  
  myGLCD.fillScr(BLACK);
  myTFT.SetTouchButtonColors(0,GREEN, WHITE, FILL, NOTROUNDED);
  myGLCD.setColor(GREEN);
  myGLCD.fillRect(0,220,319,239);

}

void loop()
{  
  myTouch.read();

  if(R < num) 
  {
    missiles[R].makeMissile();
    myScore += missiles[R].GetScore();
    missiles[R].resetScore();

    if(missiles[R].CitiesRemaining())
    {
      C--;
      myGLCD.setColor(WHITE);
      myGLCD.setBackColor(GREEN);
      sprintf(Cmsg, "Cities Left %d ", C);
      myGLCD.print(Cmsg, 100, 225);
      if(C == 0)
        GameOver();
    }
    num = num < 6? (myScore/50)+1 : 6;
    R++;
  }
  else R=0;
  if(myScore != lastScore)
  {
    myGLCD.setColor(WHITE);
    myGLCD.setBackColor(GREEN);
    myGLCD.printNumI(myScore, 0,225);
    lastScore = myScore;
  }

}

void GameOver()
{
  myGLCD.fillScr(WHITE);
  myGLCD.setColor(BLACK);
  myGLCD.setBackColor(WHITE);
  myGLCD.print("All Cities were", CENTER, 80);
  delay(1000);
  myGLCD.print("destroyed, you", CENTER, 100);
  delay(1000);
  myGLCD.print("were not able to", CENTER, 120);
  delay(1000);
  myGLCD.print("protect them.", CENTER, 140);
  delay(1000);
  myGLCD.print("Gameover", CENTER, 160);
  while(1);
}
