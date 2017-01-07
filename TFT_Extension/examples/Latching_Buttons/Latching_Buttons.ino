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

#define convert(x) {myTFT.ConvertRGB(x)}

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  Serial1.begin(115200);
  startup();
}

void loop()
{   
  getButton();
}

void startup()
{ 
  myGLCD.setColor(RED);//red box fill
  myGLCD.fillRect(0, 0, 319, 13);//text box
  myGLCD.setColor(WHITE);//text color White
  myGLCD.setBackColor(RED);//background of text red
  myGLCD.drawLine(0, 14, 319, 14);
  myGLCD.print("LatchButtons", CENTER, 1);
  myGLCD.setBackColor(BLACK);
  myGLCD.print("Touch screen to start", CENTER, 119);
  myGLCD.clrScr();
  myGLCD.print("Latches", CENTER, 50);
  myGLCD.print("Buttons", CENTER, 170);
  myTFT.SetTouchCircleColors(0, ORANGE,  PURPLE, FILL);        // ID number, Pressed Color, Released Color, FILL/NOFILL)
  myTFT.SetTouchCircleText(0,  "Hello", Small,  BLUE);
  myTFT.SetLatchCircleColors(1,  WHITE,  CYAN, FILL);          // ID number, Latched Color, UnLatched Color, FILL/NOFILL)
  myTFT.SetLatchCircleText(1, "Hello", Small,  BLUE);
  myTFT.SetTouchButtonColors(0,  RED,  RED, FILL, ROUNDED);   // ID number, Pressed Color, Released Color, FILL/NOFILL,ROUNDED/NOTROUNDED)
  myTFT.SetTouchButtonText(0, "Hello", Small,  BLUE);
  myTFT.SetLatchButtonColors(1,  RED,  BLACK, FILL, ROUNDED); // ID number, Latched Color, UnLatched Color, FILL/NOFILL, ROUNDED/NOTROUNDED)
  myTFT.SetLatchButtonText(1, "Hello", Small,  YELLOW);
//  myTFT.SetTouchTriangleColors(0, PURPLE, GREEN, FILL);       
//  myTFT.SetLatchTriangleColors(1, YELLOW, BLUE, FILL);
  
}

void getButton()
{
  myTFT.TouchCircle_Draw(55,180,50,0); // (x,y,radius, ID number)
  if(myTFT.LatchCircle_Draw(265,55,50,1)) // (x,y,radius, ID number)
  {
    delay(5000);
    myTFT.ResetLatchCircleState(1, RELEASED);
  }
  myTFT.TouchButton_Draw(220,135,310,225,0);// (x1,y1,x2,y2, ID number)
  if(myTFT.LatchButton_Draw(10,10,100,100,1))// (x1,y1,x2,y2, ID number)
  {
    delay(5000);
    myTFT.ResetLatchButtonState(1, RELEASED);
  }
  myTFT.TouchTriangle_Draw(160,140,50,down,0); //(x,y,radius, degree, ID number)
  myTFT.LatchTriangle_Draw(160,100,50,up,1); //(x,y,radius, degree, ID number) 
}

