#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
UTFT myGLCD(A1,A2,A0,A3,A5,ITDB32S);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);
TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

int cx, cy,count,counter,R,G,B;
int coordX[4];
int coordY[4];
long currtime;
boolean lockout = false;
boolean cleared = true;
byte time = 2;

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(SmallFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision( PREC_MEDIUM );  
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
  myTouch.read();
  int tx = myTouch.getX();
  int ty = myTouch.getY();
  int XC= tx > 320? 0 : tx; 
  int YC= ty > 240? 0 : ty;

  //debug(ty,tx);
  //data(YC,XC);
  getButton(XC,YC);
}

void startup()
{ 
  myGLCD.setColor(255, 255, 255);//text color White
  myGLCD.setBackColor(255, 0, 0);//background of text red
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("ARDUINO PAINT", CENTER, 119);

  delay(2000);

  for(int line = 0; line <= 239; line++)
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  makeButton();
}

void getButton(int tx, int ty)
{
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(200, 0, 0);
  myGLCD.setColor(0,0,0);
  myGLCD.printNumI(count, 297, 217);

  myGLCD.setBackColor(0, 200,0);
  myGLCD.setColor(255,255,255);
  myGLCD.printNumI((time > 2? 2 : time), 295, 52);

  if(myTFT.TouchButton(290,210,320,240))  // brush size button
  {
    if(!lockout)
    {
      myGLCD.setFont(BigFont);
      myGLCD.setColor(0,0,0);
      myGLCD.setBackColor(200,0,0);
      myGLCD.printNumI(count, 297, 217);
      count > 2 ? count = 0 :count++;
      lockout = true;
    }
  }
  else lockout = false;

  if(myTFT.TouchButton(5,200,263,253))// pick a color
  {
    for(int number=0; number <= 2; number++)
    {
      for(int y=0; y <= 2; y++)
      {
        for(int x=0; x <= 255; x++)
        {
          if(myTFT.TouchButton(5+x, 200+(y*3)+(number*13), 8+x, 203+(y*3)+(number*13)))
          {
            R = map(number, 0, 2, 0, 255);
            G = map(y, 0, 2, 0, 255);
            B = map(x, 0, 255, 0, 255);
            myGLCD.setColor(R,G,B);
            myGLCD.fillRect(269,205,284,235);//Sample Color
          }
        }
      }
    }
  }

  if(myTFT.TouchButton(1,1,279,200)) // draw on pad only
  {
    if((count+1) < ty && ty < 200 - (count+1) )
    {
      myGLCD.setColor(R,G,B);
      myGLCD.fillRect(tx - (count+1), ty - (count+2), (count+1) + tx, (count+1) + ty);
    }  
  }

  if(myTFT.TouchButton(290,40,320,80) && cleared == false) // clear pad
  {
    time = 2;
    currtime = millis();
    cleared = true;
  }

  if(myTFT.TouchButton(290,40,320,80) && cleared == true)
  {
    time = (2000 - (millis() - currtime))/1000;
    myGLCD.setBackColor(0, 200,0);
    myGLCD.setColor(255,255,255);
    myGLCD.printNumI((time > 2? 2 : time), 295, 52);
    if((millis() - currtime) > 2000)
    {
      myGLCD.setColor(255,255,255);
      myGLCD.fillRect(0,0,280,200);
      cleared = false;
    }
  }
  else cleared = false;
}

void makeButton()
{ 
  myGLCD.setColor(200,0,0);
  myGLCD.fillRect(290,210,320,240);//Brush size
  myGLCD.setColor(0,200,0);
  myGLCD.fillRect(290,40,315,80); // clear pad
  myGLCD.setColor(255,255,255);
  myGLCD.fillRect(0,0,280,200); // drawing pad
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(269,205,284,235);//Sample box
  for(int number=0; number <= 2; number++) {
    for(int y=0; y <= 2; y++){
      for(int x=0; x <= 255; x++){

        int R = map(number, 0, 2, 0, 255);
        int G = map(y, 0, 2, 0, 255);
        int B = map(x, 0, 255, 0, 255);
        myGLCD.setColor(R,G,B);
        myGLCD.fillRect(5+x, 200+(y*3)+(number*13), 8+x, 203+(y*3)+(number*13));
      }
    } 
  }
}

void debug(int tx,int ty)
{
  myGLCD.setBackColor(200, 0, 200);//background of text purple
  myGLCD.setColor(255,255,255);
  myGLCD.print("tx: ", 0,0, 0);
  myGLCD.printNumF(tx, 1, 30, 0);
  myGLCD.print("ty: ", 100,0,0);
  myGLCD.printNumF(ty, 1, 130, 0);
  return; 
}

void data(int X, int Y)
{
  myGLCD.setBackColor(200, 0, 200);//background of text purple
  myGLCD.setColor(255,255,255);
  myGLCD.print("X: ", 0,20, 0);
  myGLCD.printNumF(X, 0, 30, 20);
  myGLCD.print("Y: ", 100,20,0);
  myGLCD.printNumF(Y, 0, 130, 20);
  return; 
}















