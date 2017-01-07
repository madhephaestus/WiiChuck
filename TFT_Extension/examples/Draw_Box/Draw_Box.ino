#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
//#include <math.h>
// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SmallFont[];
// 
//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(A1,A2,A0,A3,A5,ITDB32S);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);

TFT_Extension myTFT(&myGLCD, &myTouch);

int count, counter;
int coordX[4];
int coordY[4];
long currtime;
boolean buttonState, lastState = 0;
boolean cleared = true;
byte time = 0;

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  
  for(int line = 0; line <= 239; line++)
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  startup();
}

void loop()
{   
  myTouch.read();
  int tx = myTouch.getX();
  int ty = myTouch.getY();
  int XC= tx > 320? 0 : tx; 
  int YC= ty > 240? 0 : ty;

  //data(XC,YC);
  getButton(XC,YC);
}

void startup()
{ 
  myGLCD.setBackColor(255, 0, 0);//background of text red
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);//text color White
  myGLCD.print("DRAW BOX", CENTER, 119);
  myGLCD.setColor(255, 0, 0);//text color Red
  myGLCD.print("Red box clears screen", CENTER, 140);
  myGLCD.setColor(0, 255, 0);//text color Green
  myGLCD.print("Green box switches box point", CENTER, 150);
  
  delay(5000);
  for(int line = 0; line <= 239; line++)
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  makeButton();
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 255, 0);
  myGLCD.setColor(0,0,0);
  myGLCD.printNumI(count, 297, 190);
}

void getButton(int tx, int ty)
{
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(255,0,0);
  myGLCD.setColor(255,255,255);
  myGLCD.printNumI((time > 3? 3 : time), 295, 52);
  
  buttonState = myTFT.TouchButton(290,150,320,240);
  if(buttonState && buttonState != lastState) 
  {
    count > 3 ?count = 0 :count++;
    
    myGLCD.setBackColor(0, 255, 0);
    myGLCD.setColor(0,0,0);
    myGLCD.printNumI(count, 297, 190);
  }
  lastState = buttonState;
  
  if(myTFT.TouchButton(6,40,280,230)) // CANVAS
  {
    if(ty !=319){
      coordX[count] = tx;
      coordY[count] = ty;
      myGLCD.setColor(0,0,0);
      myGLCD.drawPixel(tx,ty);
    }  
  }

  if(myTFT.TouchButton(290,40,320,80) && !cleared)
  {
    time = 3;
    currtime = millis();
    cleared = true;
  }
  if(myTFT.TouchButton(290,40,320,80) && cleared)
  {
    time = (3000 - (millis() - currtime))/1000;
    myGLCD.setBackColor(255,0,0);
    myGLCD.setColor(255,255,255);
    myGLCD.printNumI((time > 3? 3 : time), 295, 52);
    
    if((millis() - currtime > 3000))
    {
      if(myTFT.TouchButton(290,40,320,80))
      {
        myGLCD.setColor(255,255,255);
        myGLCD.fillRect(6,40,280,230);
        cleared = false;
        currtime = millis();
      }
    }
  }
  if(count == 4) 
  {
    myGLCD.setColor(0,0,0);
    myGLCD.drawLine(coordX[0],coordY[0],coordX[1],coordY[1]);
    myGLCD.drawLine(coordX[1],coordY[1],coordX[2],coordY[2]);
    myGLCD.drawLine(coordX[2],coordY[2],coordX[3],coordY[3]);
    myGLCD.drawLine(coordX[3],coordY[3],coordX[0],coordY[0]);  

    count = 0;
    myGLCD.setBackColor(0, 255, 0);
    myGLCD.setColor(0,0,0);
    myGLCD.printNumI(count, 297, 190);
    //myGLCD.printNumI(count, 300, 190); 
  }
}

void makeButton()
{ 
  myGLCD.setColor(0,255,0);
  myGLCD.fillRect(290,150,320,240);
  myGLCD.setColor(255,0,0);
  myGLCD.fillRect(290,40,315,80);
  myGLCD.setColor(255,255,255);
  myGLCD.fillRect(6,40,280,230);
}

void data(int X, int Y)
{
  myGLCD.setBackColor(200, 0, 200);//background of text purple
  myGLCD.setColor(255,255,255);

  myGLCD.print("X: ", 0,0, 0);
  if(X < 100){
    myGLCD.print("  ", 29,0, 0);
    myGLCD.printNumI(X, 24, 0);
  }
  else myGLCD.printNumI(X, 24, 0);

  myGLCD.print("Y: ", 55,0,0);
  if(Y < 100){
    myGLCD.print("  ", 84,0, 0); 
    myGLCD.printNumI(Y, 79, 0);
  }
  else myGLCD.printNumI(Y, 79, 0);
  return; 
}





