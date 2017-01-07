#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
#include <math.h>

extern uint8_t SmallFont[];

//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);

TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

int cx, cy;

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  for(int line = 0; line <= 239; line++)
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  Serial1.begin(9600);
  startup();
}

void loop()
{   
  getButton();
}

void startup()
{ 
  myGLCD.setColor(255, 0, 0);//red box fill
  myGLCD.fillRect(0, 0, 319, 13);//text box
  myGLCD.setColor(255, 255, 255);//text color White
  myGLCD.setBackColor(255, 0, 0);//background of text red
  myGLCD.drawLine(0, 14, 319, 14);
  myGLCD.print("ARDUINO ROBOT CONTROLLER", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Touch screen to start", CENTER, 119);

  for(int line = 0; line <= 239; line++)
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  
  myTFT.SetTouchCircleColors(0, GREEN, RED, FILL);
  myTFT.SetTouchCircleColors(1, BLUE, YELLOW, FILL);
  myTFT.SetTouchCircleColors(2, ORANGE, CYAN, FILL);
}

void getButton()
{
  myTFT.TouchCircle_Draw(159,119,50,0); // (x,y,radius, ID)
  myTFT.TouchCircle_Draw(35,200,20,1); // (x,y,radius, ID)
  myTFT.TouchCircle_Draw(280,35,20,2); // (x,y,radius, ID)
}





