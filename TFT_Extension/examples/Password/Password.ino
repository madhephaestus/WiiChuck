#include <Wire.h>
#include <UTouch.h>
#include <UTFT.h>
#include <TFT_Extension.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5);   //
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

char * text[4][3] = { 
  {
    "1","2","3"}
  ,
  {
    "4","5","6"}
  ,
  {
    "7","8","9"}
  ,
  {
    "*","0","#"}
};

char Pass[5];
char master[5] = "2651";
int Pcount=0;
int XC,YC;
int PassCounter=0;
byte orient = LANDSCAPE;

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_EXTREME);
  for(int line = 0; line <= 239; line++) // custom background color
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  PassWord_makeButton();
  PassWord_buttonText();
  //RUN();
}

void loop()
{
  myTouch.read();
  int tx = myTouch.TP_X;
  int ty = myTouch.TP_Y;
  XC= constrain (map(tx, 205, 3900,0,239),0,239);
  YC= constrain (map(ty, 300, 3850,0,319),0,319);
  GetNum(XC,YC);
}

void RUN() 
{
  //  while(1) // Not using the Loop() function,
  //  {
  //    myTouch.read();
  //    int tx = myTouch.TP_X;
  //    int ty = myTouch.TP_Y;
  //    XC= constrain (map(tx, 205, 3900,0,239),0,239);
  //    YC= constrain (map(ty, 300, 3850,0,319),0,319);
  //    GetNum(XC,YC);
  //  }
}

void PassWord_makeButton()
{
  for(int y = 0; y < 300;y+=100)
  {
    for(int x = 0; x < 200; x+=50)
    {
      drawButton(20+x,90+y,50+x,30+y); // this will draw the button on the screen by so many pixels 
    }
  }
  return;
}

void drawButton(int x1, int y1,int x2, int y2)
{
  myGLCD.setColor(170,170,170); // grey
  myGLCD.fillRoundRect(y1-3, x1+3, y2-3, x2+3);//Button Shading  

  myGLCD.setColor(255, 255, 255); // white
  myGLCD.fillRoundRect(y1, x1, y2, x2);// outter button color

  myGLCD.setColor(200,0,0); // red
  myGLCD.fillRoundRect(y1-5, x1+5, y2+5, x2-5);//inner button color
  return;
}

void PassWord_buttonText()
{ 
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(200, 0, 0);
  myGLCD.setColor(255,255,255);
  for(int tr = 0; tr < 300; tr+=100)
  {
    for(int tc = 0; tc < 200; tc+=50)
    {
      char *myText = text[tc/50][tr/100];
      myGLCD.print(myText,52+tr,27+tc,0); //this will print the number into the center of the buttons
    }
  } 
}

void GetNum(int tx,int ty) {
  char * lastText;
  for(int tr = 0; tr < 300; tr+=100)
  {
    for(int tc = 0; tc < 200; tc+=50)
    {
      char *myText = text[tc/50][tr/100];
      if(myTFT.TouchButton((90+tr),(20+tc),(30+tr),(50+tc),XC,YC)) // this check to see if any of the buttons were pressed, then it sends the corresponding number to the "check" function
      {
        check(*myText); 
      }
    }
  }
}


void check(char pass) 
{
  myGLCD.setFont(BigFont);
  Pass[Pcount] = pass;  //store incoming numbers in an array
  myGLCD.print(Pass,CENTER, 1); // show the password as it is being entered

  if(Pcount == 3)// only when the array is filled, does it check the numbers
  {
    delay(1000);
    if(!strcmp(Pass, master)) // compare the password to the master password
    {
      myGLCD.print("GOOD",CENTER, 1);
      delay(1000);
      myGLCD.print("    ",CENTER, 1);
    }
    else
    {
      myGLCD.print("FAIL",CENTER, 1);
      delay(1000);
      myGLCD.print("    ",CENTER, 1);
    }
    clearPassword(); // Clear the password
  }

  Pcount++;
  if(Pcount > 3) // this ensure the count does not exceed the maximum lenght of the password
  {
    Pcount = 0; 
    PassCounter = 0;
  }
}

void clearPassword()
{
  byte i = 3;
  while(i != 0)
    Pass[i--] = NULL;
}









