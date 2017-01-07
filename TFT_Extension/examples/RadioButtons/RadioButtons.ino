#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
#include <math.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
 
//myGLCD(RS,WR,CS,RST,ALE,mode);
UTFT myGLCD(A1,A2,A0,A3,A5,ITDB32S);
//myTouch(TCLK,TCS,DIN,DOUT,IRQ);
UTouch  myTouch(13,10,11,12,A4);

TFT_Extension myTFT(&myGLCD, &myTouch, LANDSCAPE);

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
  myGLCD.print("ARDUINO RADIOBUTTONS", CENTER, 1);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.print("Touch screen to start", CENTER, 119);

  for(int line = 0; line <= 239; line++)
  {
    myGLCD.setColor(0, 0, line);//text color White  
    myGLCD.drawLine(0, line, 319, line);
  }
  
  uint8_t nums[1] = {4};
  myTFT.TotalRadioButtons(nums,1);
  myTFT.TotalRadioCircleButtons(nums,1);
  
  myTFT.SetAll_RB_Outer_Color(BLACK);
  myTFT.SetAll_RB_Toggled_Color(GREEN);
  myTFT.SetAll_RB_Untoggled_Color(YELLOW);
  
  myTFT.SetAll_RCB_Outer_Color(PURPLE); 
  myTFT.SetAll_RCB_Toggled_Color(BLUE);
  myTFT.SetAll_RCB_Untoggled_Color(RED);
  
  myTFT.RB_Text("A", Big, 0,0, BLACK); // text, Font size, button number, group number, text color
  myTFT.RB_Text("B", Big, 1,0, BLUE);
  myTFT.RB_Text("C", Small, 2,0, WHITE);
  
  myTFT.RCB_Text("A", Small, 0,0, BLACK);
  myTFT.RCB_Text("B", Small, 1,0, BLACK);
  myTFT.RCB_Text("C", Big, 2,0, BLACK);
   
  makeButton();
}

void getButton()
{
  myTFT.RadioButton(10,10,50,50,0,0);
  myTFT.RadioButton(10,60,50,100,1,0 );
  myTFT.RadioButton(10,110,50,150,2,0 );
  myTFT.RadioButton(10,160,50,200,3,0 );
  
//=================================================  
  myTFT.RadioCircleButton(80,30,20,0,0 );
  myTFT.RadioCircleButton(80,80,20,1,0 );
  myTFT.RadioCircleButton(80,130,20,2,0 );
  myTFT.RadioCircleButton(80,180,20,3,0 );
}



