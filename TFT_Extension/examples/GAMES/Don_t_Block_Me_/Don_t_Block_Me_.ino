#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t BigFont[];

#define Orientation LANDSCAPE
#define swap(type, A, B) {type T = A; A = B; B = T;}
#define SIZE 10

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S, A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch);

int Y,X,YDIR = 0,XDIR = 0, score= 0;

#if(Orientation != LANDSCAPE)
const int DX1 = 10, DY1 = 0, DX2 = 230, DY2 = 200; 
byte grid[DY2/SIZE][(DX2-DX1)/SIZE];
#else
const int DX1 = 10, DY1 = 0, DX2 = 310, DY2 = 120; 
byte grid[DY2/SIZE][(DX2-DX1)/SIZE];
#endif

byte TX = 0, TY = 0;
unsigned long time = 0;
byte collision = false, GS = false; // GS = Game Start

void setup()
{
  myGLCD.InitLCD(Orientation);
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);
  myTouch.InitTouch(Orientation);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  myGLCD.fillScr(BLACK);
  myGLCD.setColor(255,255,255);

  if(Orientation != LANDSCAPE)
  {
    swap(int, int(DX1), int(DY1));
    swap(int, int(DX2), int(DY2));
  }

  myGLCD.fillRect(DX1,DY1,DX2,DY2); // grid
#if(Orientation != LANDSCAPE)
  myTFT.fillTriangle(120,230,40,up);//up
  myTFT.fillTriangle(120,290,40,down);//down
  myTFT.fillTriangle(60,260,40,left);//left
  myTFT.fillTriangle(180,260,40,right);//right
#else
  myTFT.fillTriangle(160,150,40,up);//up
  myTFT.fillTriangle(160,210,40,down);//down
  myTFT.fillTriangle(120,180,40,left);//left
  myTFT.fillTriangle(200,180,40,right);//right
#endif
  myTFT.SetTouchButtonColors(0, GREEN, GREEN, FILL, ROUNDED);
  myTFT.SetTouchButtonColors(1, RED, RED, FILL, ROUNDED);
  myTFT.SetTouchButtonText(0, "YES", Big, BLACK);
  myTFT.SetTouchButtonText(1, "NO", Big, BLACK);
  clearGrid();
  score = 0;
  YDIR = 0; 
  XDIR = 0;
  Y = 100;
  X = 110;
}

void loop()
{    
  control();
  CheckBoundaries(200);  
}

void control()
{
#if(Orientation != LANDSCAPE)
  if(myTFT.TouchTriangle(120,230,40,up))
  {
    XDIR=   0; 
    YDIR= -SIZE;
    GS = true;
  } 
  if(myTFT.TouchTriangle(120,290,40,down)) 
  {
    XDIR=  0; 
    YDIR= SIZE;
    GS = true;
  } 
  if(myTFT.TouchTriangle(60,260,40,left))  
  {
    XDIR= -SIZE; 
    YDIR=   0;
    GS = true;
  } 
  if(myTFT.TouchTriangle(180,260,40,right))
  {
    XDIR= SIZE; 
    YDIR=  0;
    GS = true;
  } 
#else
  if(myTFT.TouchTriangle(160,150,40,up))
  {
    XDIR=   0; 
    YDIR= -SIZE;
    GS = true;
  } 
  if(myTFT.TouchTriangle(160,210,40,down)) 
  {
    XDIR=  0; 
    YDIR= SIZE;
    GS = true;
  } 
  if(myTFT.TouchTriangle(120,180,40,left))  
  {
    XDIR= -SIZE; 
    YDIR=   0;
    GS = true;
  } 
  if(myTFT.TouchTriangle(200,180,40,right))
  {
    XDIR= SIZE; 
    YDIR=  0;
    GS = true;
  } 
#endif
}

void CheckBoundaries(unsigned long T)
{
  if(millis() - time > T)
  {
    if( (X >= DX1) && (Y >= DY1) && (X <= (DX2 - SIZE)) && (Y <= (DY2 - SIZE)) && !collision)
    {
      Y += YDIR;
      X += XDIR;
      makeBody(X, Y);
      fillGrid(X, Y);
    }
    else
    {
      myGLCD.setColor(BLACK);
      myGLCD.setBackColor(WHITE);
      myGLCD.setFont(BigFont);
      myGLCD.print("You Crashed", CENTER, 100); 
      PlayAgain(true);
    }
    printScore();
    time = millis();
  }   
}

void makeBody(int x, int y)
{
  myGLCD.setColor(BLUE);
  myGLCD.fillRect(x,y,x+SIZE,y+SIZE);
  myGLCD.setColor(BLACK);
  myGLCD.drawRect(x,y,x+SIZE,y+SIZE);
}

void fillGrid(int x, int y)
{
  if(GS)
  {
    TX = x / SIZE;
    TY = y / SIZE;

    if(grid[TY][TX] == 0)
    {
      score += SIZE;
      grid[TY][TX] = 1;
    }
    else
    {
      collision = true;
    }
  }
}

void printScore()
{
  myGLCD.setColor(YELLOW);
#if(Orientation != LANDSCAPE)
  myGLCD.printNumI(score, 0,210);
#else
  myGLCD.printNumI(score, 0,120);
#endif
}

void clearGrid()
{
  for(char r = 0; r < DY2/SIZE; r++)
  {
    for(char c = 0; c < (DX2-DX1)/SIZE; c++)
    {
      grid[r][c] = false;
    }
  }
  collision = false;
  TX=0; 
  TY=0;
  GS = false;
}

//void makeButton()
//{
//#if(Orientation != LANDSCAPE)
//  myGLCD.drawRect(50,159, 115, 189); //yes
//  myGLCD.drawRect(140,159, 195, 189); //no
//#else
//  myGLCD.drawRect(80,159, 145, 189); //yes
//  myGLCD.drawRect(170,159, 235, 189); //no
//#endif
//}

void PlayAgain(boolean playAgain)
{
  while(playAgain)
  {
    myGLCD.setColor(BLACK);
    myGLCD.setBackColor(WHITE);
    myGLCD.print("Play Again?", CENTER, 131);
    //makeButton();
#if(Orientation != LANDSCAPE)
    if(myTFT.TouchButton_Draw(80, 159, 145, 189, 0))//yes
    {
      setup();
      playAgain = false;
      break;
    }

    else if(myTFT.TouchButton_Draw(170, 159, 235, 189, 1)) // no
    {
      myGLCD.fillScr(BLUE);
      myGLCD.setColor(YELLOW);
      myGLCD.setBackColor(BLUE);
      myGLCD.print("Thanks for", CENTER, 131);
      myGLCD.print("playing", CENTER, 148);
      delay(1000);
      playAgain = false;
      while(1);
    }
#else
    if(myTFT.TouchButton_Draw(80, 159, 145, 189, 0))//yes
    {
      setup();
      playAgain = false;
      break;
    }

    else if(myTFT.TouchButton_Draw(170, 159, 235, 189, 1))//no
    {
      myGLCD.fillScr(BLUE);
      myGLCD.setColor(YELLOW);
      myGLCD.setBackColor(BLUE);
      myGLCD.print("Thanks for playing", CENTER, 131);
      playAgain = false;
      while(1);
    }
#endif
  } 
}
