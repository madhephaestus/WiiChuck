/* TFT_Extension.cpp */

#include "TFT_Extension.h"
#include <avr/pgmspace.h>

#define Save_MainColor 		do{ FrontColor = _Disp->getColor(); BackColor = _Disp->getBackColor(); }while(0)
#define Restore_MainColor	do{ _Disp->setColor(FrontColor); _Disp->setBackColor(BackColor); }while(0)

word TFT_Extension::ConvertRGB(byte R, byte G, byte B){ 
  return ( ((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3) );
}

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t TRONFont[];

const char KB[5][17] PROGMEM ={
  {0,16,0,'`','1','2','3','4','5','6','7','8','9','0','-','='},
  {0,16,10,'q','w','e','r','t','y','u','i','o','p','[',']','\\'},
  {1,14,9,'a','s','d','f','g','h','j','k','l',';','\''},
  {2,13,7,'z','x','c','v','b','n','m','\,','.','/'},
  {2,12,0,'s','p','a','c','e',' ','b','a','r'}
};

const char ShiftKB[4][17] PROGMEM ={
  {0,16,0,'~','!','@','#','$','%','^','&','*','(',')','_','+'},
  {10,6,0,'{','}','|'},
  {10,5,1,':','"'},
  {9,6,2,'<','>','?'}
};

const char Mobile_KB[3][13] PROGMEM ={
  {0,13,10,'Q','W','E','R','T','Y','U','I','O','P'},
  {1,12,9,'A','S','D','F','G','H','J','K','L'},
  {2,10,7,'Z','X','C','V','B','N','M'},
};

const char Mobile_NumKeys[3][13] PROGMEM ={
  {0,13,10,'1','2','3','4','5','6','7','8','9','0'},
  {0,13,10,'-','/',':',';','(',')','$','&','@','"'},
  {5,8,5,'.','\,','?','!','\''}
};

const char Mobile_SymKeys[3][13] PROGMEM ={
  {0,13,10,'[',']','{','}','#','%','^','*','+','='},
  {4,9,6,'_','\\','|','~','<','>'},
  {5,8,5,'.','\,','?','!','\''}
};

struct Radio {
  char * Text[Button_Groups][Num_Of_Buttons];
  bool TxtEnable[Button_Groups][Num_Of_Buttons];
  bool Font_Size[Button_Groups][Num_Of_Buttons];
  bool Button[Button_Groups][Num_Of_Buttons];
  bool lastButton[Button_Groups][Num_Of_Buttons];
  uint8_t number[Button_Groups];
} Block, Circle;

struct TouchButtons{
  char * Text[Num_Of_Buttons];
  bool TxtEnable[Num_Of_Buttons];
  bool Font_Size[Num_Of_Buttons];
  bool Button[Num_Of_Buttons];
  bool lastButton[Num_Of_Buttons];
  bool Fill[Num_Of_Buttons];
  bool Round[Num_Of_Buttons];
}T_Button, T_Circle, L_Button, L_Circle, T_Triangle, L_Triangle;

struct latching{
  bool buttons;
  bool circles;
  bool triangles;
  bool ButLock;
  bool CirLock;
  bool TriLock;
}L[Num_Of_Buttons];

struct
{
  word rgb;
} 
  RB_Text_color[Num_Of_Buttons][Button_Groups], RCB_Text_color[Num_Of_Buttons][Button_Groups],
  RB_Outer_color[Num_Of_Buttons][Button_Groups], RCB_Outer_color[Num_Of_Buttons][Button_Groups],
  RB_Toggled_color[Num_Of_Buttons][Button_Groups], RCB_Toggled_color[Num_Of_Buttons][Button_Groups],
  RB_Untoggled_color[Num_Of_Buttons][Button_Groups], RCB_Untoggled_color[Num_Of_Buttons][Button_Groups];

struct {
  double x , oldx; // setting variable to 0 is not needed, it will be written over anyways, it just looks good.
  double y , oldy;// same as above
  bool locked;
}gauge[10];  
 
struct
{
  struct Colors 
  {
    word rgb;
	struct textColors 
    {
      word rgb;
	}text;
  }buttons, latches;
} ButtonColor1[Num_Of_Buttons], ButtonColor2[Num_Of_Buttons],
  CircleColor1[Num_Of_Buttons], CircleColor2[Num_Of_Buttons], 
  TriangleColor1[Num_Of_Buttons], TriangleColor2[Num_Of_Buttons];
  
struct Sliders{
  int lastx1;
  int lasty1;
  int lastx2;
  int lasty2;
  int lastC;
  int Slide;
  boolean locked;
}VSliders[10], HSliders[10],
VBG[10], HBG[10];

struct {
  int x1;
  int y1;
  int x2;
  int y2;
}SEND,BSP,CAPS, SPACE, NUM;

TFT_Extension::TFT_Extension(UTFT *Disp, UTouch *Touch)
{ 
 _Disp = Disp;
 _Touch = Touch;
} 

void TFT_Extension::ExtSetup()
{
  B_current_time = 0, C_current_time = 0, T_current_time = 0;
  B_Touched = false, B_timeout = false;
  C_Touched = false, C_timeout = false;
  T_Touched = false, T_timeout = false;
  ButLOCK = false;
  Butlocked[Button_Groups];
  CirLOCK = false;
  Cirlocked[Button_Groups];
  B_lastx1 =0, B_lasty1 =0, B_lastx2 =0, B_lasty2 =0;
  C_lastcx =0, C_lastcy =0;

  locked = false;
  flip = 0;
  T = 0;
  
  idx = 0;
  MSG[BUF];
  gotFirst = false;
  Shift = false, capsLock = false, lastState;
  for(byte idx = 0; idx < 10; idx++)
  {
    VSliders[idx].Slide = 0; HSliders[idx].Slide = 0;
    VBG[idx].Slide = 0; HBG[idx].Slide = 0;
    gauge[idx].locked = 0;
  }
  
  DisplaySize(); 
}
// Set the display size if the display is anything other than 320x240 
void TFT_Extension::DisplaySize() 
{
  DispX = _Disp->disp_x_size; // Global variable will be set to X
  DispY = _Disp->disp_y_size; // ------------------------------ Y
  
  if(_Disp->orient == LANDSCAPE)
  swap(int, DispX, DispY);
}
 
// This tells the compiler how many buttons and groups will be made.
int TFT_Extension::TotalRadioButtons(uint8_t *num, uint8_t group) 
{ 
  TotalRB_Group = group; // Set global variable
  for(byte temp = 0; temp < group; temp++) 
  {
    Block.number[temp] = num[temp]; // This stores the number of buttons per group
  }
  return *Block.number; // return a pointer to that array.
}
 
bool TFT_Extension::RadioButton(int x1, int y1, int x2, int y2, uint8_t buttonNumber, uint8_t group)
{
    Save_MainColor;
	
    int strl = strlen(Block.Text[buttonNumber][group]);
    int Xpos = (x2 + x1)/2; // find the center of the button
    int Ypos = (y2 + y1)/2; // -----------------------------
	
	if (x1>x2) // This makes sure the coordinates inputted are not backwards. Must be coords of top left and bottom right.  
	  swap(int, x1, x2);

	if (y1>y2) // same with this, top left and bottom right
	  swap(int, y1, y2);

	if(Butlocked[group] != Block.number[group] && !ButLOCK) // This makes allows the buttons to be made until locked out
	{
		_Disp->setColor(RB_Outer_color[buttonNumber][group].rgb); //Set the outer color of the button.
	   
		_Disp->fillRoundRect(x1, y1, x2, y2); // make the outer part of the button
	   if(Block.Button[group][Butlocked[group]] == true) // This draws the state of the buttons if the screen is cleared and re-entered.
	   {
		  _Disp->setColor(RB_Toggled_color[buttonNumber][group].rgb);//green
		   
		  _Disp->fillRoundRect(B_lastx1 + 4, B_lasty1 + 4, B_lastx2 - 4, B_lasty2 - 4); // This create the toggled button from the same coords, but it gives the appearance of a border.
			
		  if(Block.TxtEnable[buttonNumber][group]) // Set the background color for the text, based on the buttons color
			_Disp->setBackColor(RB_Toggled_color[buttonNumber][group].rgb);
	   }
	   else
	   {
		  _Disp->setColor(RB_Untoggled_color[buttonNumber][group].rgb);//red			
		   
		  _Disp->fillRoundRect(x1 + 4, y1 + 4, x2 - 4, y2 - 4);
			
		  if(Block.TxtEnable[buttonNumber][group]) // Set the background color for the text, based on the buttons color
			_Disp->setBackColor(RB_Untoggled_color[buttonNumber][group].rgb); 
	   }
	   
	   if(Block.TxtEnable[buttonNumber][group]) // If there is text for that button, show it.
	   {
	     _Disp->setColor(RB_Text_color[buttonNumber][group].rgb); // Show the text color
	     if(Block.Font_Size[buttonNumber][group]) //big font
         {
           _Disp->setFont(BigFont);
           _Disp->print(Block.Text[buttonNumber][group], Xpos - (strl*8), Ypos - 8, 0); //print the string in the center of the button. Big font is 16x16
         }
         else
         {
           _Disp->setFont(SmallFont);
           _Disp->print(Block.Text[buttonNumber][group], Xpos - (strl*4), Ypos - 6, 0); // small font is 8x12
         }
	   }
	   if(Butlocked[group] == Block.number[group]) ButLOCK = true; // if all the buttons have been made, then lockout "if(Butlocked[group] != Block.number[group] && ButLOCK)"
	   else Butlocked[group]++; // otherwise keep making the buttons until all are made.
	}
    
	if( TouchButton(x1,y1,x2,y2) ) // this checks to see if the buttons coords were touched
	{
		for(byte cnt = 0; cnt < Block.number[group]; cnt++) // cycle through the buttons in that group
		{ 
		  if(cnt == buttonNumber) // if the cycled button is the requested button, set it true
		  {
			Block.Button[group][cnt] = true;
			B_lastx1 = x1; B_lasty1 = y1; B_lastx2 = x2; B_lasty2 = y2; // record what coords were touched
		  }
		  else 
		  {
			Block.Button[group][cnt] = false; //all other buttons should be false
		  }
		}
	}

	if(Block.Button[group][buttonNumber] != Block.lastButton[group][buttonNumber]) // This makes sure the button cannot be pressed again to repaint the screen.
	{
		if(Block.Button[group][buttonNumber]) // If the button pressed is different, then paint the new button 
		{
		  _Disp->setColor(RB_Toggled_color[buttonNumber][group].rgb);//green
		  _Disp->fillRoundRect(x1 + 4, y1 + 4, x2 - 4, y2 - 4);
		  
		  if(Block.TxtEnable[buttonNumber][group])
		    _Disp->setBackColor(RB_Toggled_color[buttonNumber][group].rgb); // Set the background color for the text, based on the buttons color
		}
	    else // This paints the other buttons to the untoggled color.
		{
		  _Disp->setColor(RB_Untoggled_color[buttonNumber][group].rgb);//red
		  _Disp->fillRoundRect(x1 + 4, y1 + 4, x2 - 4, y2 - 4);
		  
		  if(Block.TxtEnable[buttonNumber][group])
		    _Disp->setBackColor(RB_Untoggled_color[buttonNumber][group].rgb); // Set the background color for the text, based on the buttons color
		}
		
		if(Block.TxtEnable[buttonNumber][group])
		{
		  _Disp->setColor(RB_Text_color[buttonNumber][group].rgb); // Show text color
          if(Block.Font_Size[buttonNumber][group]) //big font
          {
            _Disp->setFont(BigFont);
            _Disp->print(Block.Text[buttonNumber][group], Xpos - (strl*8), Ypos - 8, 0); //print the string in the center of the button. Big font is 16x16
          }
          else
          {
            _Disp->setFont(SmallFont);
            _Disp->print(Block.Text[buttonNumber][group], Xpos - (strl*4), Ypos - 6, 0); // small font is 8x12
          }
        }		  
	}
  Block.lastButton[group][buttonNumber] = Block.Button[group][buttonNumber]; // record the buttons states
  
  Restore_MainColor;
  
  return Block.Button[group][buttonNumber];
}

// See the comments from TotalRadioButtons 
int TFT_Extension::TotalRadioCircleButtons(uint8_t *num, uint8_t group) 
{ 
  TotalRCB_Group = group;
  for(byte temp = 0; temp < group; temp++)
  {
    Circle.number[temp] = num[temp];
  }
  return *Circle.number;
}

// See the comments from RadioButton 
bool TFT_Extension::RadioCircleButton(int cx, int cy, int radius, uint8_t buttonNumber, uint8_t group)
{	
    Save_MainColor;
	
    int strl = strlen(Circle.Text[buttonNumber][group]);
	
	if(Cirlocked[group] != Circle.number[group] && !CirLOCK)
	{
		_Disp->setColor(RCB_Outer_color[buttonNumber][group].rgb); // GREY
		_Disp->fillCircle(cx,cy,radius);
		
		if(Circle.Button[group][Cirlocked[group]] == true) 
	    {
	      _Disp->setColor(RCB_Toggled_color[buttonNumber][group].rgb);//green
		  _Disp->fillCircle(C_lastcx,C_lastcy,(radius - (radius/4)));
		 
		  if(Circle.TxtEnable[buttonNumber][group]) // Set the background color for the text, based on the buttons color
		    _Disp->setBackColor(RCB_Toggled_color[buttonNumber][group].rgb);
	    }
	    else
	    {
		  _Disp->setColor(RCB_Untoggled_color[buttonNumber][group].rgb);//red
		
		  _Disp->fillCircle(cx,cy,(radius - (radius/4)));
		 
		  if(Circle.TxtEnable[buttonNumber][group]) // Set the background color for the text, based on the buttons color
		    _Disp->setBackColor(RCB_Untoggled_color[buttonNumber][group].rgb);
	    }
		
		if(Circle.TxtEnable[buttonNumber][group]) // If there is text for that button, show it.
	    {
	       _Disp->setColor(RCB_Text_color[buttonNumber][group].rgb);
	       if(Circle.Font_Size[buttonNumber][group]) //big font
           {
             _Disp->setFont(BigFont);
             _Disp->print(Circle.Text[buttonNumber][group], cx - (strl*8), cy - 8, 0); //print the string in the center of the button. Big font is 8x8
           }
           else
           {
             _Disp->setFont(SmallFont);
             _Disp->print(Circle.Text[buttonNumber][group], cx - (strl*4), cy - 5, 0); // small font is 4x5
           }
	    }
		if(Cirlocked[group] == Circle.number[group]) CirLOCK = true;
		else Cirlocked[group]++;
	}

	if(TouchCircle(cx,cy,radius))
	{
		for(byte cnt = 0; cnt < Circle.number[group]; cnt++)
		{ 
		  if(cnt == buttonNumber)
          {		  
			Circle.Button[group][cnt] = true;
			C_lastcx = cx; C_lastcy = cy;
		  }
		  else 
			Circle.Button[group][cnt] = false; 
		}
	}

	if(Circle.Button[group][buttonNumber] != Circle.lastButton[group][buttonNumber])
	{
		if(Circle.Button[group][buttonNumber])
		{
		   _Disp->setColor(RCB_Toggled_color[buttonNumber][group].rgb);//green
		   _Disp->fillCircle(cx,cy,(radius - (radius/4)));
		   
		   if(Circle.TxtEnable[buttonNumber][group]) // Set the background color for the text, based on the buttons color
		    _Disp->setBackColor(RCB_Toggled_color[buttonNumber][group].rgb);
		}
		else 
		{
		   _Disp->setColor(RCB_Untoggled_color[buttonNumber][group].rgb);//red
		   _Disp->fillCircle(cx,cy,(radius - (radius/4)));
		   
		   if(Circle.TxtEnable[buttonNumber][group]) // Set the background color for the text, based on the buttons color
		    _Disp->setBackColor(RCB_Untoggled_color[buttonNumber][group].rgb);
		}
		if(Circle.TxtEnable[buttonNumber][group]) // If there is text for that button, show it.
	    {
	       _Disp->setColor(RCB_Text_color[buttonNumber][group].rgb);
	       if(Circle.Font_Size[buttonNumber][group]) //big font
           {
             _Disp->setFont(BigFont);
             _Disp->print(Circle.Text[buttonNumber][group], cx - (strl*8), cy - 8, 0); //print the string in the center of the button. Big font is 8x8
           }
           else
           {
             _Disp->setFont(SmallFont);
             _Disp->print(Circle.Text[buttonNumber][group], cx - (strl*4), cy - 5, 0); // small font is 4x5
           }
	    }
	}
	Circle.lastButton[group][buttonNumber] = Circle.Button[group][buttonNumber];
	
	Restore_MainColor;
	
	return Circle.Button[group][buttonNumber];
}

// This allows the button to be repainted after switching screens.  
void TFT_Extension::ResetRadioCircleButton(uint8_t group) 
{
 CirLOCK = false; // This sets the lockout variable to be false, indicating it can repaint the buttons
  for(byte temp = 0; temp < group; temp++)
  {
    Cirlocked[temp] = false; // Do the same for all the buttons in the group
  }
}

// This allows the button to be repainted after switching screens. 
void TFT_Extension::ResetRadioButton(uint8_t group) 
{
  ButLOCK = false; // This sets the lockout variable to be false, indicating it can repaint the buttons
  for(byte temp = 0; temp < group; temp++)
  {
	Butlocked[temp] = false;// Do the same for all the buttons in the group
  }
}

// Standard touch screen button
bool TFT_Extension::TouchButton(int x1, int y1, int x2, int y2) 
{
  _Touch->read();
  touchX = _Touch->getX();
  touchY = _Touch->getY();
  int xc = (touchX > DispX? 0 : touchX);
  int yc = (touchY > DispY? 0 : touchY); 

 if (x1 > x2)
  swap(int ,x1 ,x2);

 if (y1 > y2)
  swap(int ,y1 ,y2);
 
 if((xc >= x1) && (xc <= x2) && (yc >= y1) && (yc <= y2)) return true; // If the buttons coords are touched, return true.
 return false; // button coords were not touched, return false.
}

/*bool TFT_Extension::DBLTapButton(int x1, int y1, int x2, int y2)
{

}
*/
//This is a standard touch screen button and will draw the actual button.
bool TFT_Extension::TouchButton_Draw(int x1, int y1, int x2, int y2, uint8_t buttonNumber) 
{
    byte strl = strlen(T_Button.Text[buttonNumber]);
    int Xpos = (x2 + x1)/2; // find the center of the button
    int Ypos = (y2 + y1)/2; // -----------------------------
	
 if( TouchButton(x1,y1,x2,y2) ) // sets the button ID to true if touched, otherwise false
   T_Button.Button[buttonNumber] = true;
 else 
   T_Button.Button[buttonNumber] = false;
 
 if(T_Button.Button[buttonNumber] != T_Button.lastButton[buttonNumber]) // allow to only be pressed and not held.
 {
   if(T_Button.Button[buttonNumber]) // if button is touched, show its color. 
      _Disp->setColor(ButtonColor1[buttonNumber].buttons.rgb);
   else 
      _Disp->setColor(ButtonColor2[buttonNumber].buttons.rgb);
	  
   if(_Disp->orient == PORTRAIT)
   {
     swap(int, x1,x2);
     swap(int, y1,y2);
   }
   if(T_Button.Fill[buttonNumber]) // tell weather the button is rounded or not and filled or not.
      T_Button.Round[buttonNumber]? _Disp->fillRoundRect(x1,y1,x2,y2) : _Disp->fillRect(x1,y1,x2,y2);
   else 
	  T_Button.Round[buttonNumber]? _Disp->drawRoundRect(x1,y1,x2,y2) : _Disp->drawRect(x1,y1,x2,y2);
	  
	  if(T_Button.TxtEnable[buttonNumber]) // If there is text for that button, show it.
	   {
	     if((ButtonColor1[buttonNumber].buttons.rgb == ButtonColor2[buttonNumber].buttons.rgb) 
		  )
		  {
			_Disp->setBackColor(ButtonColor1[buttonNumber].buttons.rgb);
	        _Disp->setColor(ButtonColor1[buttonNumber].buttons.text.rgb); // Show the text color
		  }
		 else
		 {
		   if(T_Button.Button[buttonNumber])
		   {
		     _Disp->setBackColor(ButtonColor1[buttonNumber].buttons.rgb);
		     _Disp->setColor(ButtonColor1[buttonNumber].buttons.text.rgb); // Show the text color
		   }
		   else
		   {
		     _Disp->setBackColor(ButtonColor2[buttonNumber].buttons.rgb);
		     _Disp->setColor(~ButtonColor1[buttonNumber].buttons.text.rgb); // Show the text color
		   }
		 }
		 
	     if(T_Button.Font_Size[buttonNumber]) //big font
         {
           _Disp->setFont(BigFont);
           _Disp->print(T_Button.Text[buttonNumber], Xpos - (strl*8), Ypos - 8, 0); //print the string in the center of the button. Big font is 16x16
         }
         else
         {
           _Disp->setFont(SmallFont);
           _Disp->print(T_Button.Text[buttonNumber], Xpos - (strl*4), Ypos - 6, 0); // small font is 8x12
         }
	   }
   T_Button.lastButton[buttonNumber] = T_Button.Button[buttonNumber]; // This allows the button to be redrawn when released.
 }
 return T_Button.Button[buttonNumber];
}

// THIS IS STILL BEING ADDED TO
bool TFT_Extension::TextButton(char *str, byte font_size, int x1, int y1, int x2, int y2, word color)
{
  int strl = strlen(str); // get the length of the string
  int Xpos = (x2 + x1)/2; // find the center of the button
  int Ypos = (y2 + y1)/2; // -----------------------------

  if (x1 > x2)
   swap(int ,x1 ,x2);

  if (y1 > y2)
   swap(int ,y1 ,y2);
  
  _Disp->setColor(color);
  if(font_size == 1) //big font
  {
    _Disp->setFont(BigFont);
    _Disp->print(str, Xpos - (strl*8), Ypos - 8, 0); //print the string in the center of the button. Big font is 16x16
  }
  else if(font_size == 2)
  {
    _Disp->setFont(TRONFont);
    _Disp->print(str, Xpos - (strl*4), Ypos - 6, 0); // tron font is 8x12
  }
  else
  {
    _Disp->setFont(SmallFont);
    _Disp->print(str, Xpos - (strl*4), Ypos - 6, 0); // small font is 8x12
  }
  
 if( TouchButton(x1,y1,x2,y2) ) return true;
 return false;
}
 
bool TFT_Extension ::LatchButton( int x1, int y1, int x2, int y2, uint8_t buttonNumber)
{
  if (x1>x2)
	swap(int ,x1 ,x2);

  if (y1>y2)
	swap(int ,y1 ,y2);
	
  if( TouchButton(x1,y1,x2,y2) )
  {
    if(L[buttonNumber].ButLock == true)// If this button is pressed, set the latch. 
	{
      L[buttonNumber].buttons = !L[buttonNumber].buttons;
      L[buttonNumber].ButLock = false; // Only allow it to be pressed once and not held.
    }
  }
  else L[buttonNumber].ButLock = true; // Once released allow it to be pressed again.
  
  return L[buttonNumber].buttons; // Return the buttons state
} 
 
bool TFT_Extension::LatchButton_Draw( int x1, int y1, int x2, int y2, uint8_t buttonNumber)
{ 
  byte strl = strlen(L_Button.Text[buttonNumber]); // get the length of the string
  int Xpos = (x2 + x1)/2; // find the center of the button
  int Ypos = (y2 + y1)/2; // -----------------------------
  
  if (x1>x2)
	swap(int ,x1 ,x2);

  if (y1>y2)
	swap(int ,y1 ,y2);
	
  if( TouchButton(x1,y1,x2,y2) ) // See the comment from TouchButton
  {
    if(L[buttonNumber].ButLock == true)
	{
      L[buttonNumber].buttons = !L[buttonNumber].buttons;
      L[buttonNumber].ButLock = false;
    }
  }
  else L[buttonNumber].ButLock = true;
  
  if(L[buttonNumber].buttons) // This looks to see if the button is latched or not
    L_Button.Button[buttonNumber] = true;
  else 
    L_Button.Button[buttonNumber] = false;
 
  if(L_Button.Button[buttonNumber] != L_Button.lastButton[buttonNumber]) // Allow button to be pressed and not held 
  {
    if(L[buttonNumber].buttons) // Set the color of the button based on the latched state.
       _Disp->setColor(ButtonColor1[buttonNumber].latches.rgb);
    else 
       _Disp->setColor(ButtonColor2[buttonNumber].latches.rgb);
    if (_Disp->orient == PORTRAIT)
	{
	  swap(int, x1,x2);
	  swap(int, y1,y2);
	}
    if(L_Button.Fill[buttonNumber]) // See if button is told to be rounded or not and filled or not.
       L_Button.Round[buttonNumber]? _Disp->fillRoundRect(x1,y1,x2,y2) : _Disp->fillRect(x1,y1,x2,y2);
    else 
	   L_Button.Round[buttonNumber]? _Disp->drawRoundRect(x1,y1,x2,y2) : _Disp->drawRect(x1,y1,x2,y2);
  }
  L_Button.lastButton[buttonNumber] = L_Button.Button[buttonNumber];
  
  if(L_Button.TxtEnable[buttonNumber]) // If there is text for that button, show it.
	   {
	     if(ButtonColor1[buttonNumber].latches.rgb == ButtonColor2[buttonNumber].latches.rgb)
		  {
			_Disp->setBackColor(ButtonColor1[buttonNumber].latches.rgb);
	        _Disp->setColor(ButtonColor1[buttonNumber].latches.text.rgb); // Show the text color
		  }
		 else
		 {
		   if(L_Button.Button[buttonNumber])
		   {
		     _Disp->setBackColor(ButtonColor1[buttonNumber].latches.rgb);
		     _Disp->setColor(ButtonColor1[buttonNumber].latches.text.rgb); // Show the text color
		   }
		   else
		   {
		     _Disp->setBackColor(ButtonColor2[buttonNumber].latches.rgb);
		     _Disp->setColor(~ButtonColor1[buttonNumber].latches.text.rgb); // Show the text color
		   }
		 }
		 
	     if(L_Button.Font_Size[buttonNumber]) //big font
         {
           _Disp->setFont(BigFont);
           _Disp->print(L_Button.Text[buttonNumber], Xpos - (strl*8), Ypos - 8, 0); //print the string in the center of the button. Big font is 16x16
         }
         else
         {
           _Disp->setFont(SmallFont);
           _Disp->print(L_Button.Text[buttonNumber], Xpos - (strl*4), Ypos - 6, 0); // small font is 8x12
         }
	   }
  return L[buttonNumber].buttons;
}

// See the comments from TouchButton
bool TFT_Extension ::TouchCircle(int cx, int cy, int radius)
{
  _Touch->read();
  touchX = _Touch->getX();
  touchY = _Touch->getY();
  int tx = (touchX > DispX? 0 : touchX);
  int ty = (touchY > DispY? 0 : touchY);
 
  if((pow((cx - tx),2) + pow((cy - ty),2)) <= pow(radius,2)) return true;
  return false;
}

uint8_t TFT_Extension::TouchCircleByQuad(int cx, int cy, int radius, uint8_t Q1, uint8_t Q2, uint8_t Q3)
{
  uint8_t quad = 0;
  _Touch->read();
  touchX = _Touch->getX();
  touchY = _Touch->getY();
  int tx = (touchX > DispX? 0 : touchX);
  int ty = (touchY > DispY? 0 : touchY);
  
  if(TouchCircle(cx,cy,radius)) // makes sure the circle is actually being touched
  {
    if((tx >= cx) && (ty <= cy)) quad = 1;
    if((tx <= cx) && (ty <= cy)) quad = 2;
    if((tx <= cx) && (ty >= cy)) quad = 3;
    if((tx >= cx) && (ty >= cy)) quad = 4;
  }
  if((Q1 & Q2 & Q3) != 8) // If you enter a specified quadrant, this will return either true or false if that quadrant is touched
  {
    if((quad == Q1) || (quad == Q2) || (quad == Q3) )
      return true;
	else return false;
  }
  else return quad; // If you don't specify a quadrant, then it will return the quadrant number that is touched.
}

// See the comments from TouchButton_Draw
bool TFT_Extension ::TouchCircle_Draw(int cx, int cy, int radius, uint8_t circleNumber)
{
  byte strl = strlen(T_Circle.Text[circleNumber]); // get the length of the string
  
  if( TouchCircle(cx,cy,radius) )
    T_Circle.Button[circleNumber] = true;
  else 
    T_Circle.Button[circleNumber] = false;
 
  if(T_Circle.Button[circleNumber] != T_Circle.lastButton[circleNumber])
  {   
    if(T_Circle.Button[circleNumber])
       _Disp->setColor(CircleColor1[circleNumber].buttons.rgb);
    else 
       _Disp->setColor(CircleColor2[circleNumber].buttons.rgb);
    
	//if( _Orient == PORTRAIT)
      //swap(int, cx,cy);
    if(T_Circle.Fill[circleNumber])
       _Disp->fillCircle(cx,cy,radius);
    else 
	   _Disp->drawCircle(cx,cy,radius);
  T_Circle.lastButton[circleNumber] = T_Circle.Button[circleNumber];
  }
  
  if(T_Circle.TxtEnable[circleNumber]) // If there is text for that button, show it.
   {
	 if(CircleColor1[circleNumber].buttons.rgb == CircleColor2[circleNumber].buttons.rgb) 
	  {
		_Disp->setBackColor(CircleColor1[circleNumber].buttons.rgb);
	    _Disp->setColor(CircleColor1[circleNumber].buttons.text.rgb); // Show the text color
	  }
	 else
	 {
	   if(T_Circle.Button[circleNumber])
	   {
		 _Disp->setBackColor(CircleColor1[circleNumber].buttons.rgb);
		 _Disp->setColor(CircleColor1[circleNumber].buttons.text.rgb); // Show the text color
	   }
	   else
	   {
		 _Disp->setBackColor(CircleColor2[circleNumber].buttons.rgb);
		 _Disp->setColor(~CircleColor1[circleNumber].buttons.text.rgb); // Show the text color
	   }
	 }
	 
	 if(T_Circle.Font_Size[circleNumber]) //big font
	 {
	   _Disp->setFont(BigFont);
	   _Disp->print(T_Circle.Text[circleNumber], cx - (strl*8), cy - 8, 0); //print the string in the center of the button. Big font is 16x16
	 }
	 else
	 {
	   _Disp->setFont(SmallFont);
	   _Disp->print(T_Circle.Text[circleNumber], cx - (strl*4), cy - 6, 0); // small font is 8x12
	 }
   }
 
 return T_Circle.Button[circleNumber];
}

// See the comments from LatchButton
bool TFT_Extension ::LatchCircle(int cx, int cy, int radius, uint8_t circleNumber)
{ 
  if(TouchCircle(cx,cy,radius) )
  {
    if(L[circleNumber].CirLock == true) 
	{
      L[circleNumber].circles = !L[circleNumber].circles;
      L[circleNumber].CirLock = false;
    }
  }
  else L[circleNumber].CirLock = true;
  return L[circleNumber].circles;
}

// See the comments from LatchButton_Draw
bool TFT_Extension ::LatchCircle_Draw(int cx, int cy, int radius, uint8_t circleNumber)
{ 
  byte strl = strlen(L_Circle.Text[circleNumber]); // get the length of the string
  
  if( TouchCircle(cx,cy,radius) )
  {
    if(L[circleNumber].CirLock == true) 
	{
      L[circleNumber].circles = !L[circleNumber].circles;
      L[circleNumber].CirLock = false;
    }
  }
  else L[circleNumber].CirLock = true;
  
  if(L[circleNumber].circles)
    L_Circle.Button[circleNumber] = true;
  else 
    L_Circle.Button[circleNumber] = false;
 
  if(L_Circle.Button[circleNumber] != L_Circle.lastButton[circleNumber])
  {  
    if(L[circleNumber].circles)
       _Disp->setColor(CircleColor1[circleNumber].latches.rgb);
    else 
       _Disp->setColor(CircleColor2[circleNumber].latches.rgb);
    
    if(L_Circle.Fill[circleNumber])
       _Disp->fillCircle(cx,cy,radius);
    else 
	   _Disp->drawCircle(cx,cy,radius);
    L_Circle.lastButton[circleNumber] = L_Circle.Button[circleNumber];
  }
  
	if(L_Circle.TxtEnable[circleNumber]) // If there is text for that button, show it.
	   {
		 if(CircleColor1[circleNumber].latches.rgb == CircleColor2[circleNumber].latches.rgb) 
		  
		  {
		    _Disp->setBackColor(CircleColor1[circleNumber].latches.rgb);
		    _Disp->setColor(CircleColor1[circleNumber].latches.text.rgb); // Show the text color
		  }
		 else
		 {
		   if(L_Circle.Button[circleNumber])
		   {
			 _Disp->setBackColor(CircleColor1[circleNumber].latches.rgb);
			 _Disp->setColor(CircleColor1[circleNumber].latches.text.rgb); // Show the text color
		   }
		   else
		   {
			 _Disp->setBackColor(CircleColor2[circleNumber].latches.rgb);
			 _Disp->setColor(~CircleColor1[circleNumber].latches.text.rgb); // Show the text color
		   }
		 }
		 
		 if(L_Circle.Font_Size[circleNumber]) //big font
		 {
		   _Disp->setFont(BigFont);
		   _Disp->print(L_Circle.Text[circleNumber], cx - (strl*8), cy - 8, 0); //print the string in the center of the button. Big font is 16x16
		 }
		 else
		 {
		   _Disp->setFont(SmallFont);
		   _Disp->print(L_Circle.Text[circleNumber], cx - (strl*4), cy - 6, 0); // small font is 8x12
		 }
	   }
  return L[circleNumber].circles;
}

//Calculate the area of an equilateral triangle  
float TFT_Extension::Area(int Ax, int Ay, int Bx, int By, int Cx, int Cy)
{
 return abs((Ax*(By - Cy) + Bx*(Cy - Ay) + Cx*(Ay - By))/2);
}
 
bool TFT_Extension ::TouchTriangle(int x1,int y1,int base, int deg)
{
  if (_Disp->orient == PORTRAIT) 
    swap(int ,x1 ,y1);
	
   _Touch->read();
   touchX = _Touch->getX();
   touchY = _Touch->getY();
   int tx = (touchX > DispX? 0 : touchX);
   int ty = (touchY > DispY? 0 : touchY);
  
  int rad = base/2; int dir;

  if(deg < 90) dir=up;
  else if(deg < 180) dir=left; // down
  else if(deg < 270) dir=down;
  else dir = right;
  
  Cx = x1 + sin(float(deg) * deg_to_rad) * rad;
  Cy = y1 + cos(float(deg) * deg_to_rad) * rad;
  Cx1 = x1 + sin((float(deg) + 120) * deg_to_rad) * rad;
  Cy1 = y1 + cos((float(deg) + 120) * deg_to_rad) * rad;
  Cx2 = x1 + sin((float(deg) + 240) * deg_to_rad) * rad;
  Cy2 = y1 + cos((float(deg) + 240) * deg_to_rad) * rad;
  
  if(base >= 60) // This also ensures the base is perfectly straight, but for large triangles
  {
    switch(dir)
    {
     case up:
       Cy2-=1;//Cy2-1 
       break;
     case down: 
       Cy2+=1;//Cy2+1
       break;
     case left:
       Cx2-=1;//Cx2-1
       break;
     case right:
       Cx2+=1; //Cx2+1
       break;
    }
  }
  
  float A = Area(Cx, Cy, Cx1, Cy1, Cx2, Cy2); // This is the ideal area of the triangle based on the 3 points.
  float A1 = Area(tx, ty, Cx1, Cy1, Cx2, Cy2);// this checks to see if the touch coords are within the created triangle.
  float A2 = Area(Cx, Cy, tx, ty, Cx2, Cy2);  // The reason three are needed is because each one is a section of the triangle.
  float A3 = Area(Cx, Cy, Cx1, Cy1, tx, ty);

  if(A >= A1 + A2 + A3) return true;
  return false;  
}

// See comments from the other _Draw buttons 
bool TFT_Extension ::TouchTriangle_Draw(int x1,int y1,int base, int deg, uint8_t triangleNumber)
{ 
  byte strl = strlen(T_Triangle.Text[triangleNumber]); // get the length of the string

   if( TouchTriangle(x1,y1,base,deg) ) 
    T_Triangle.Button[triangleNumber] = true;
  else 
    T_Triangle.Button[triangleNumber] = false;
 
  if(T_Triangle.Button[triangleNumber] != T_Triangle.lastButton[triangleNumber])
  {
    if(T_Triangle.Button[triangleNumber])
       _Disp->setColor(TriangleColor1[triangleNumber].buttons.rgb);
    else 
       _Disp->setColor(TriangleColor2[triangleNumber].buttons.rgb);
	  
    if(T_Triangle.Fill[triangleNumber])
       fillTriangle(x1, y1, base, deg);
    else 
	   drawTriangle(x1, y1, base, deg);
  }
  T_Triangle.lastButton[triangleNumber] = T_Triangle.Button[triangleNumber];
  
  if(T_Triangle.TxtEnable[triangleNumber]) // If there is text for that button, show it.
   {
	 if(TriangleColor1[triangleNumber].buttons.rgb == TriangleColor2[triangleNumber].buttons.rgb) 
	  {
		_Disp->setBackColor(TriangleColor1[triangleNumber].buttons.rgb);
	    _Disp->setColor(TriangleColor1[triangleNumber].buttons.text.rgb); // Show the text color
	  }
	 else
	 {
	   if(T_Triangle.Button[triangleNumber])
	   {
		 _Disp->setBackColor(TriangleColor1[triangleNumber].buttons.rgb);
		 _Disp->setColor(TriangleColor1[triangleNumber].buttons.text.rgb); // Show the text color
	   }
	   else
	   {
		 _Disp->setBackColor(TriangleColor2[triangleNumber].buttons.rgb);
		 _Disp->setColor(~TriangleColor1[triangleNumber].buttons.text.rgb); // Show the text color
	   }
	 }
	 
	 if(T_Triangle.Font_Size[triangleNumber]) //big font
	 {
	   _Disp->setFont(BigFont);
	   _Disp->print(T_Triangle.Text[triangleNumber], x1 - (strl*8), y1 - 8, 0); //print the string in the center of the button. Big font is 16x16
	 }
	 else
	 {
	   _Disp->setFont(SmallFont);
	   _Disp->print(T_Triangle.Text[triangleNumber], x1 - (strl*4), y1 - 6, 0); // small font is 8x12
	 }
   }
 
 return T_Triangle.Button[triangleNumber]; 
} 
 
// See comments from the other _Draw buttons  
bool TFT_Extension ::LatchTriangle(int x1, int y1, int base, int deg, uint8_t triangleNumber)
{
  if( TouchTriangle(x1,y1,base,deg) )
  {
    if(L[triangleNumber].TriLock == true) 
	{
      L[triangleNumber].triangles = !L[triangleNumber].triangles;
      L[triangleNumber].TriLock = false;
    }
  }
  else L[triangleNumber].TriLock = true;
  
  return L[triangleNumber].triangles;
} 
 
// See comments from the other _Draw buttons  
bool TFT_Extension ::LatchTriangle_Draw(int x1, int y1, int base, int deg, uint8_t triangleNumber)
{
  byte strl = strlen(L_Triangle.Text[triangleNumber]); // get the length of the string
  
  if( TouchTriangle(x1,y1,base,deg) )
  {
    if(L[triangleNumber].TriLock == true) 
	{
      L[triangleNumber].triangles = !L[triangleNumber].triangles;
      L[triangleNumber].TriLock = false;
    }
  }
  else L[triangleNumber].TriLock = true;
  
  if(L[triangleNumber].triangles)
    L_Triangle.Button[triangleNumber] = true;
  else 
    L_Triangle.Button[triangleNumber] = false;
 
  if(L_Triangle.Button[triangleNumber] != L_Triangle.lastButton[triangleNumber])
  {
    if(L[triangleNumber].triangles)
       _Disp->setColor(TriangleColor1[triangleNumber].latches.rgb);
    else 
       _Disp->setColor(TriangleColor2[triangleNumber].latches.rgb);
	  
    if(L_Triangle.Fill[triangleNumber])
       fillTriangle(x1, y1, base, deg);
    else 
	   drawTriangle(x1, y1, base, deg);
  }
  L_Triangle.lastButton[triangleNumber] = L_Triangle.Button[triangleNumber];
  
  if(L_Triangle.TxtEnable[triangleNumber]) // If there is text for that button, show it.
   {
	 if(TriangleColor1[triangleNumber].latches.rgb == TriangleColor2[triangleNumber].latches.rgb) 
	  {
		_Disp->setBackColor(TriangleColor1[triangleNumber].latches.rgb);
	    _Disp->setColor(TriangleColor1[triangleNumber].latches.text.rgb); // Show the text color
	  }
	 else
	 {
	   if(L_Triangle.Button[triangleNumber])
	   {
		 _Disp->setBackColor(TriangleColor1[triangleNumber].latches.rgb);
		 _Disp->setColor(TriangleColor1[triangleNumber].latches.text.rgb); // Show the text color
	   }
	   else
	   {
		 _Disp->setBackColor(TriangleColor2[triangleNumber].latches.rgb);
		 _Disp->setColor(~TriangleColor1[triangleNumber].latches.text.rgb); // Show the text color
	   }
	 }
	 
	 if(L_Triangle.Font_Size[triangleNumber]) //big font
	 {
	   _Disp->setFont(BigFont);
	   _Disp->print(L_Triangle.Text[triangleNumber], x1 - (strl*8), y1 - 8, 0); //print the string in the center of the button. Big font is 16x16
	 }
	 else
	 {
	   _Disp->setFont(SmallFont);
	   _Disp->print(L_Triangle.Text[triangleNumber], x1 - (strl*4), y1 - 6, 0); // small font is 8x12
	 }
   }
  return L[triangleNumber].triangles; 
} 
 
bool TFT_Extension::TouchDelayButton(int x1, int y1, int x2, int y2, unsigned long time)
{ 
  B_Touched = false;
  if(TouchButton(x1,y1,x2,y2) && B_timeout == false)
  {
    B_current_time = millis();
    B_timeout = true;
  }
  while(TouchButton(x1,y1,x2,y2) && B_timeout)
  {
    if((millis() - B_current_time) >= time)
    {
      B_Touched = true;
	  break;
    }
  }
  B_timeout = false;
  B_current_time = 0;
  return B_Touched; 
}

bool TFT_Extension::TouchDelayCircle(int x1, int y1, int base, unsigned long time)
{
  C_Touched = false;
  if(TouchCircle(x1,y1,base) && C_timeout == false)
  {
    C_current_time = millis();
    C_timeout = true;
  }
  while(TouchCircle(x1,y1,base) && C_timeout)
  {
    if((millis() - C_current_time) >= time)
    {
      C_Touched = true;
	  break;
    }
  }
  C_timeout = false;
  C_current_time = 0;
  return C_Touched; 
}

bool TFT_Extension::TouchDelayTriangle(int x1, int y1, int base, int deg, unsigned long time)
{ 
  T_Touched = false;
  if(TouchTriangle(x1,y1,base, deg) && T_timeout == false)
  {
    T_current_time = millis();
    T_timeout = true;
  }
  while(TouchTriangle(x1,y1,base, deg) && T_timeout)
  {
    if((millis() - T_current_time) >= time)
    {
      T_Touched = true;
	  break;
    }
  }
  T_timeout = false;
  T_current_time = 0;
  return T_Touched; 
}

uint8_t TFT_Extension::TouchArc(int cx, int cy, int radius, int thickness, int start, int stop)
{
  int XO,YO,XI,YI;
  
  for(float rotate = start; rotate < stop; rotate++)
  {
    XO = cx + int(-cos(rotate * deg_to_rad) * radius);
    YO = cy + int(sin(rotate * deg_to_rad) * radius);
    XI = cx - int(cos(rotate * deg_to_rad) * (radius - thickness));
    YI = cy + int(sin(rotate * deg_to_rad) * (radius - thickness));
    if( TouchButton(XI,YI,XO,YO) ) 
	{
	  percent = ((rotate/stop) * 100);
      break;
    }	  
  }
  return percent;
}

uint8_t TFT_Extension::VertSlider(int x1, int y1, int x2, int y2, byte _ID, word color)
{
  if(!VSliders[_ID].locked)
  {
    _Disp->setColor(color);
    _Disp->drawRect(x1, y1, x2, y2);
    _Disp->setColor(BLACK);
    _Disp->fillRect(x1 + 1, y1+1, x2 - 1, y2);
    VSliders[_ID].lastC = y1+1;
    VSliders[_ID].locked = true;
  }
  _Touch->read();
  int ty = _Touch->getY(); 
  int YC = ty > DispY? 0 : ty;

  if( TouchButton(x1, y1, x2, y2) )
  {
    if(YC > VSliders[_ID].lastC)
    {
      _Disp->setColor(BLACK);
      for(VSliders[_ID].Slide = y1+1; VSliders[_ID].Slide < (YC-1); VSliders[_ID].Slide++)
        _Disp->drawLine(x1+1,VSliders[_ID].Slide, x2-1, VSliders[_ID].Slide);
      VSliders[_ID].lastC = YC;
    }
    else
    {
      _Disp->setColor(color);
      for(VSliders[_ID].Slide = y2 - 1; VSliders[_ID].Slide > (VSliders[_ID].lastC+1); VSliders[_ID].Slide--)
        _Disp->drawLine(x1+1,VSliders[_ID].Slide, x2-1, VSliders[_ID].Slide);
      VSliders[_ID].lastC = YC;
    } 
  } 
  return map(VSliders[_ID].Slide, y2, y1, 0 ,100);
}

uint8_t TFT_Extension::HorSlider(int x1, int y1, int x2, int y2, byte _ID, word color, bool direction)
{
  if(!HSliders[_ID].locked)
  {
    _Disp->setColor(color);
    _Disp->drawRect(x1, y1, x2, y2);
    _Disp->setColor(BLACK);
    _Disp->fillRect(x1 + 1, y1+1, x2 - 1, y2);
    HSliders[_ID].lastC = x1+1;
    HSliders[_ID].locked = true;
  }
  _Touch->read();
  int tx = _Touch->getX(); 
  int XC = tx > DispX? 0 : tx;

  if(TouchButton(x1, y1, x2, y2))
  {
    if(XC > HSliders[_ID].lastC)
    {
      if(direction)
	  _Disp->setColor(color);
	else
	  _Disp->setColor(BLACK);
	  
      for(HSliders[_ID].Slide = x1+1; HSliders[_ID].Slide < (XC-1); HSliders[_ID].Slide++)
        _Disp->drawLine(HSliders[_ID].Slide, y1+1,HSliders[_ID].Slide, y2);
      HSliders[_ID].lastC = XC;
    }
    else
    {
      if(direction)
	  _Disp->setColor(BLACK);
	else
	  _Disp->setColor(color);
	  
      for(HSliders[_ID].Slide = x2 - 1; HSliders[_ID].Slide > (HSliders[_ID].lastC+1); HSliders[_ID].Slide--)
        _Disp->drawLine(HSliders[_ID].Slide, y1+1,HSliders[_ID].Slide, y2);
      HSliders[_ID].lastC = XC;
    } 
  } 
  if(direction)
    return map(HSliders[_ID].Slide, x1, x2, 0 ,100);
  else
    return map(HSliders[_ID].Slide, x2, x1, 0 ,100);
}

void TFT_Extension::VertBarGraph(int x1, int y1, int x2, int y2, int value, int low, int high, byte _ID, word color)
{
	if(!VBG[_ID].locked)
	{
		_Disp->setColor(color);
		_Disp->drawRect(x1, y1, x2, y2);
		_Disp->setColor(BLACK);
		_Disp->fillRect(x1 + 1, y1+1, x2 - 1, y2);
		VBG[_ID].lastC = y1+1;
		VBG[_ID].locked = true;
	}
	int YC = map(value, low, high, y2, y1);

	if(YC > VBG[_ID].lastC)
	{
	  _Disp->setColor(BLACK);
	  for(VBG[_ID].Slide = y1+1; VBG[_ID].Slide < (YC-1); VBG[_ID].Slide++)
		_Disp->drawLine(x1+1,VBG[_ID].Slide, x2-1, VBG[_ID].Slide);
	  VBG[_ID].lastC = YC;
	}
	else
	{
	  _Disp->setColor(color);
	  for(VBG[_ID].Slide = y2 - 1; VBG[_ID].Slide > (VBG[_ID].lastC+1); VBG[_ID].Slide--)
		_Disp->drawLine(x1+1,VBG[_ID].Slide, x2-1, VBG[_ID].Slide);
	  VBG[_ID].lastC = YC;
	} 
}

void TFT_Extension::HorBarGraph(int x1, int y1, int x2, int y2, int value, int low, int high, byte _ID, word color, bool direction)
{
  int XC;
  if(!HBG[_ID].locked)
  {
    _Disp->setColor(color);
    _Disp->drawRect(x1, y1, x2, y2);
    _Disp->setColor(BLACK);
    _Disp->fillRect(x1 + 1, y1+1, x2 - 1, y2);
    HBG[_ID].lastC = x1+1;
    HBG[_ID].locked = true;
  }
  
  if(direction)
    XC = map(value, low, high, x1, x2);
  else
    XC = map(value, low, high, x2, x1);
	
  if(XC > HBG[_ID].lastC)
  {
    if(direction)
	  _Disp->setColor(color);
	else
	  _Disp->setColor(BLACK);
	  
	for(HBG[_ID].Slide = x1+1; HBG[_ID].Slide < (XC-1); HBG[_ID].Slide++)
		_Disp->drawLine(HBG[_ID].Slide, y1+1,HBG[_ID].Slide, y2);
	HBG[_ID].lastC = XC;
  }
  else
  {
    if(direction)
	  _Disp->setColor(BLACK);
	else
	  _Disp->setColor(color);
	
	for(HBG[_ID].Slide = x2 - 1; HBG[_ID].Slide > (HBG[_ID].lastC+1); HBG[_ID].Slide--)
		_Disp->drawLine(HBG[_ID].Slide, y1+1,HBG[_ID].Slide, y2);
	HBG[_ID].lastC = XC;
  } 
}

void TFT_Extension::ResetV_barGraph(byte _ID)
{
  VBG[_ID].locked = false;
}

void TFT_Extension::ResetH_barGraph(byte _ID)
{
  HBG[_ID].locked = false;
}

byte TFT_Extension::Swipe(int dir, uint8_t lenght)
{
  bool gotFirst = false;
  int FTouchX =0, FTouchY =0, STouchX =0, STouchY =0;
  while(_Touch->dataAvailable() )
  {
    if(!gotFirst)
	{
      _Touch->read();
      touchX = _Touch->getX();
      touchY = _Touch->getY();
      FTouchX = (touchX > DispX? 0 : touchX);
      FTouchY = (touchY > DispY? 0 : touchY);
	  STouchY = FTouchY;
	  STouchX = FTouchX;
	  gotFirst = true;
	}
	
	for(byte i = 0; i < 10; i++)
	{
	  _Touch->read();
      touchX = _Touch->getX();
      touchY = _Touch->getY();
      STouchX = (touchX > DispX? 0 : touchX);
      STouchY = (touchY > DispY? 0 : touchY);
	}
	
	// //look for a specific direction
	// if((dir > 0) && ((abs(STouchX - FTouchX) >= lenght) || (abs(STouchY - FTouchY) >= lenght)) )
	// {
	  // if((STouchY < FTouchY) && dir == 1) return true; 
      // else if((STouchY > FTouchY) && dir == 2) return true;
      // else if((STouchX < FTouchX) && dir == 3) return true;
      // else if((STouchX > FTouchX) && dir == 4) return true; 
	// }
	
	// //get a swipe direction for the Y axis
	// else if((dir == 0) && (abs(STouchY - FTouchY) >= lenght) ) 
	// {
	  // if(STouchY < FTouchY) return 1; // up
      // else if(STouchY > FTouchY) return 2; // down
	// }
	// //get a swipe direction for X axis
	// else if((dir == 0) && (abs(STouchX - FTouchX) >= lenght) )
	// {
	  // if(STouchX < FTouchX) return 3; // left
      // else if(STouchX > FTouchX) return 4; // right
	// }
	if((dir > 0) && ((abs(STouchX - FTouchX) >= lenght) || (abs(STouchY - FTouchY) >= lenght)) )
	{
	  if((STouchY < FTouchY) && dir == 1) return true; // up
      else if((STouchY > FTouchY) && dir == 2) return true; // down
      else if((STouchX < FTouchX) && dir == 3) return true; // left
      else if((STouchX > FTouchX) && dir == 4) return true; // right
	  else if((STouchY < FTouchY) && (STouchX < FTouchX) && dir == 5) return true; // upleft
	  else if((STouchY > FTouchY) && (STouchX < FTouchX) && dir == 6) return true; // downLeft
	  else if((STouchY < FTouchY) && (STouchX > FTouchX) && dir == 7) return true; // upRight
	  else if((STouchY > FTouchY) && (STouchX > FTouchX) && dir == 8) return true; // downRight
	}
	else if((dir == 0) && ((abs(STouchY - FTouchY) >= lenght) && (abs(STouchX - FTouchX) >= lenght)))
	{
	  if(     (STouchY < FTouchY) && (STouchX < FTouchX)) return 5; // upleft
	  else if((STouchY > FTouchY) && (STouchX < FTouchX)) return 6; // downLeft
	  else if((STouchY < FTouchY) && (STouchX > FTouchX)) return 7; // upRight
	  else if((STouchY > FTouchY) && (STouchX > FTouchX)) return 8; // downRight
	}
	//get a swipe direction for the Y axis
	else if((dir == 0) && (abs(STouchY - FTouchY) >= lenght) )
	{
	  if(STouchY < FTouchY) return 1; // up
      else if(STouchY > FTouchY) return 2; // down
	}
	//get a swipe direction for X axis
	else if((dir == 0) && (abs(STouchX - FTouchX) >= lenght))
	{
	  if(STouchX < FTouchX) return 3; // left
      else if(STouchX > FTouchX) return 4; // right
	}
  }
  return false;
}

byte TFT_Extension::SwipeFromArea(int X1, int Y1, int X2, int Y2, int dir, uint8_t lenght)
{  
  if(!_Touch->dataAvailable())
  {
    gotFirst = false;
	FTouchX =0; FTouchY=0; STouchX=0; STouchY=0;
  }
  
  if(!gotFirst && TouchButton(X1, Y1, X2, Y2))
  {
    _Touch->read();
    touchX = _Touch->getX();
    touchY = _Touch->getY();
    FTouchX = (touchX > DispX? 0 : touchX);
    FTouchY = (touchY > DispY? 0 : touchY);
	STouchY = FTouchY;
	STouchX = FTouchX;
	gotFirst = true;
  }
  
  while(_Touch->dataAvailable() && gotFirst == true)
  {
	for(byte i = 0; i < 10; i++)
	{
	  _Touch->read();
      touchX = _Touch->getX();
      touchY = _Touch->getY();
      STouchX = (touchX > DispX? 0 : touchX);
      STouchY = (touchY > DispY? 0 : touchY);
	}
	
	//look for a specific direction
	if((dir > 0) && ((abs(STouchX - FTouchX) >= lenght) || (abs(STouchY - FTouchY) >= lenght)) )
	{
	  if((STouchY < FTouchY) && dir == 1) return true; // up
      else if((STouchY > FTouchY) && dir == 2) return true; // down
      else if((STouchX < FTouchX) && dir == 3) return true; // left
      else if((STouchX > FTouchX) && dir == 4) return true; // right
	  else if((STouchY < FTouchY) && (STouchX < FTouchX) && dir == 5) return true; // upleft
	  else if((STouchY > FTouchY) && (STouchX < FTouchX) && dir == 6) return true; // downLeft
	  else if((STouchY < FTouchY) && (STouchX > FTouchX) && dir == 7) return true; // upRight
	  else if((STouchY > FTouchY) && (STouchX > FTouchX) && dir == 8) return true; // downRight
	}
	else if((dir == 0) && ((abs(STouchY - FTouchY) >= lenght) && (abs(STouchX - FTouchX) >= lenght)))
	{
	  if(     (STouchY < FTouchY) && (STouchX < FTouchX)) return 5; // upleft
	  else if((STouchY > FTouchY) && (STouchX < FTouchX)) return 6; // downLeft
	  else if((STouchY < FTouchY) && (STouchX > FTouchX)) return 7; // upRight
	  else if((STouchY > FTouchY) && (STouchX > FTouchX)) return 8; // downRight
	}
	//get a swipe direction for the Y axis
	else if((dir == 0) && (abs(STouchY - FTouchY) >= lenght) )
	{
	  if(STouchY < FTouchY) return 1; // up
      else if(STouchY > FTouchY) return 2; // down
	}
	//get a swipe direction for X axis
	else if((dir == 0) && (abs(STouchX - FTouchX) >= lenght))
	{
	  if(STouchX < FTouchX) return 3; // left
      else if(STouchX > FTouchX) return 4; // right
	}
  }
  return false;
}
//==================END_OF_TOUCH_FUNCTIONS=======================

//====================DISPLAY_FUNCTIONS==========================

void TFT_Extension::SetTouchButtonColors(uint8_t ButtonNumber, word color1, word color2, bool fill, bool rounded)
{
  ButtonColor1[ButtonNumber].buttons.rgb = color1;
  ButtonColor2[ButtonNumber].buttons.rgb = color2; 
  
  T_Button.lastButton[ButtonNumber] = true;
  T_Button.Fill[ButtonNumber] = fill;
  T_Button.Round[ButtonNumber] = rounded;
}

void TFT_Extension::SetTouchCircleColors(uint8_t ButtonNumber, word color1, word color2, bool fill)
{
  CircleColor1[ButtonNumber].buttons.rgb = color1;
  CircleColor2[ButtonNumber].buttons.rgb = color2;
  
  T_Circle.lastButton[ButtonNumber] = true;
  T_Circle.Fill[ButtonNumber] = fill;
}

void TFT_Extension::SetTouchTriangleColors(uint8_t ButtonNumber, word color1, word color2, bool fill)
{
  TriangleColor1[ButtonNumber].buttons.rgb = color1;
  TriangleColor2[ButtonNumber].buttons.rgb = color2;
  
  T_Triangle.lastButton[ButtonNumber] = true;
  T_Triangle.Fill[ButtonNumber] = fill;
}

void TFT_Extension::SetLatchButtonColors(uint8_t ButtonNumber, word color1, word color2, bool fill, bool rounded)
{
  ButtonColor1[ButtonNumber].latches.rgb = color1;
  ButtonColor2[ButtonNumber].latches.rgb = color2;
  
  L_Button.lastButton[ButtonNumber] = true;
  L_Button.Fill[ButtonNumber] = fill;
  L_Button.Round[ButtonNumber] = rounded;
}

void TFT_Extension::SetLatchCircleColors(uint8_t ButtonNumber, word color1, word color2, bool fill)
{
  CircleColor1[ButtonNumber].latches.rgb = color1;
  CircleColor2[ButtonNumber].latches.rgb = color2;
  
  L_Circle.lastButton[ButtonNumber] = true;
  L_Circle.Fill[ButtonNumber] = fill;
}

void TFT_Extension::SetLatchTriangleColors(uint8_t ButtonNumber, word color1, word color2, bool fill)
{
  TriangleColor1[ButtonNumber].latches.rgb = color1;
  TriangleColor2[ButtonNumber].latches.rgb = color2;
  
  L_Triangle.lastButton[ButtonNumber] = true;
  L_Triangle.Fill[ButtonNumber] = fill;
}

void TFT_Extension::SetTouchButtonText(uint8_t ButtonNumber,char* txt, bool size, word color)
{
  T_Button.Text[ButtonNumber] = txt;
  T_Button.Font_Size[ButtonNumber] = size;
  T_Button.TxtEnable[ButtonNumber] = true;
 
  ButtonColor1[ButtonNumber].buttons.text.rgb = color;
}

void TFT_Extension::SetLatchButtonText(uint8_t ButtonNumber,char* txt, bool size, word color)
{
  L_Button.Text[ButtonNumber] = txt;
  L_Button.Font_Size[ButtonNumber] = size;
  L_Button.TxtEnable[ButtonNumber] = true;
  
  ButtonColor1[ButtonNumber].latches.text.rgb = color;
}

void TFT_Extension::SetTouchCircleText(uint8_t CircleNumber, char* txt, bool size, word color)
{
  T_Circle.Text[CircleNumber] = txt;
  T_Circle.Font_Size[CircleNumber] = size;
  T_Circle.TxtEnable[CircleNumber] = true;
  
  CircleColor1[CircleNumber].buttons.text.rgb = color;
}

void TFT_Extension::SetLatchCircleText(uint8_t CircleNumber, char* txt, bool size, word color)
{
  L_Circle.Text[CircleNumber] = txt;
  L_Circle.Font_Size[CircleNumber] = size;
  L_Circle.TxtEnable[CircleNumber] = true;
  
  CircleColor1[CircleNumber].latches.text.rgb = color;
}

void TFT_Extension::SetTouchTriangleText(uint8_t TriangleNumber, char* txt, bool size, word color)
{
  T_Triangle.Text[TriangleNumber] = txt;
  T_Triangle.Font_Size[TriangleNumber] = size;
  T_Triangle.TxtEnable[TriangleNumber] = true;
  
  TriangleColor1[TriangleNumber].buttons.text.rgb = color;
}

void TFT_Extension::SetLatchTriangleText(uint8_t TriangleNumber, char* txt, bool size, word color)
{
  L_Triangle.Text[TriangleNumber] = txt;
  L_Triangle.Font_Size[TriangleNumber] = size;
  L_Triangle.TxtEnable[TriangleNumber] = true;
  
  TriangleColor1[TriangleNumber].latches.text.rgb = color;
}

void TFT_Extension::ResetTouchButton(byte ID)
{
  T_Button.lastButton[ID] = true;
}

void TFT_Extension::ResetTouchCircle(byte ID)
{
  T_Circle.lastButton[ID] = true;
}

void TFT_Extension::ResetTouchTriangle(byte ID)
{
  T_Triangle.lastButton[ID] = true;
}

void TFT_Extension::ResetAllTouchButton()
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    T_Button.lastButton[ID] = !T_Button.Button[ID];
}

void TFT_Extension::ResetAllTouchCircle()
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    T_Circle.lastButton[ID] = !T_Circle.Button[ID];
}

void TFT_Extension::ResetAllTouchTriangle()
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    T_Triangle.lastButton[ID] = !T_Triangle.Button[ID];
}

void TFT_Extension::ResetLatchButton(byte ID)
{
  L_Button.lastButton[ID] = !L_Button.Button[ID];
}

void TFT_Extension::ResetLatchButtonState(byte ID, byte State)
{
  L[ID].buttons = State;
}

void TFT_Extension::ResetAllLatchButtonState(byte State)
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    L[ID].buttons = State;
}

void TFT_Extension::ResetLatchCircle(byte ID)
{
  L_Circle.lastButton[ID] = !L_Circle.Button[ID];
}

void TFT_Extension::ResetLatchCircleState(byte ID, byte State)
{
  L[ID].circles = State;
}

void TFT_Extension::ResetAllLatchCircleState(byte State)
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    L[ID].circles = State;
}

void TFT_Extension::ResetLatchTriangle(byte ID)
{
  L_Triangle.lastButton[ID] = !L_Triangle.Button[ID];
}

void TFT_Extension::ResetLatchTriangleState(byte ID, byte State)
{
  L[ID].triangles = State;
}

void TFT_Extension::ResetAllLatchTriangleState(byte State)
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    L[ID].triangles = State;
}

void TFT_Extension::ResetAllLatchButton()
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    L_Button.lastButton[ID] = !L_Button.Button[ID];
}

void TFT_Extension::ResetAllLatchCircle()
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    L_Circle.lastButton[ID] = !L_Circle.Button[ID];
}

void TFT_Extension::ResetAllLatchTriangle()
{
  for(byte ID = 0; ID < Num_Of_Buttons; ID++)
    L_Triangle.lastButton[ID] = !L_Triangle.Button[ID];
}

// This function allows text to be entered into the radio button
void TFT_Extension::RB_Text(char * text, bool size, uint8_t ButtonNumber,uint8_t Group, word color)
{
  Block.Text[ButtonNumber][Group] = text;
  Block.Font_Size[ButtonNumber][Group] = size;
  Block.TxtEnable[ButtonNumber][Group] = true;
  
  RB_Text_color[ButtonNumber][Group].rgb = color;
}

// This function allows text to be entered into the radio circle button
void TFT_Extension::RCB_Text(char * text, bool size, uint8_t ButtonNumber, uint8_t Group, word color)
{
  Circle.Text[ButtonNumber][Group] = text;
  Circle.Font_Size[ButtonNumber][Group] = size;
  Circle.TxtEnable[ButtonNumber][Group] = true;
  
  RCB_Text_color[ButtonNumber][Group].rgb = color;
}

//The function names tell just what the function does. RB = Radiobutton, RCB = RadioCirclebutton
void TFT_Extension::RB_Outer_Color(uint8_t ButtonNumber, uint8_t Group, word color)
{
  RB_Outer_color[ButtonNumber][Group].rgb = color;
}
 
void TFT_Extension::RCB_Outer_Color(uint8_t ButtonNumber, uint8_t Group, word color)
{
  RCB_Outer_color[ButtonNumber][Group].rgb = color;
}
//======================================================================
 
void TFT_Extension::RB_Toggled_Color(uint8_t ButtonNumber, uint8_t Group, word color)
{
  RB_Toggled_color[ButtonNumber][Group].rgb = color;
}
 
void TFT_Extension::RCB_Toggled_Color(uint8_t ButtonNumber, uint8_t Group, word color)
{
  RCB_Toggled_color[ButtonNumber][Group].rgb = color;
}
//======================================================================
 
void TFT_Extension::RB_Untoggled_Color(uint8_t ButtonNumber, uint8_t Group, word color)
{
  RB_Untoggled_color[ButtonNumber][Group].rgb = color;
}
 
void TFT_Extension::RCB_Untoggled_Color(uint8_t ButtonNumber, uint8_t Group, word color)
{
  RCB_Untoggled_color[ButtonNumber][Group].rgb = color;
}
//======================================================================
//
//======================================================================
void TFT_Extension::RB_Outer_Color_By_Group(uint8_t Group, word color)
{
  for(int temp = 0; temp < Block.number[Group]; temp++)
  {
   RB_Outer_color[temp][Group].rgb = color;
  }
}
 
void TFT_Extension::RCB_Outer_Color_By_Group(uint8_t Group, word color)
{
  for(int temp = 0; temp < Block.number[Group]; temp++)
  {
   RCB_Outer_color[temp][Group].rgb = color;
  }
}
//======================================================================
 
void TFT_Extension::RB_Toggled_Color_By_Group(uint8_t Group, word color)
{
  for(int temp = 0; temp < Block.number[Group]; temp++)
  {
   RB_Toggled_color[temp][Group].rgb = color;
  }
}
 
void TFT_Extension::RCB_Toggled_Color_By_Group(uint8_t Group, word color)
{
  for(int temp = 0; temp < Block.number[Group]; temp++)
  {
   RCB_Toggled_color[temp][Group].rgb = color;
  }
}
//======================================================================
 
void TFT_Extension::RB_Untoggled_Color_By_Group(uint8_t Group, word color)
{
  for(int temp = 0; temp < Block.number[Group]; temp++)
  {
   RB_Untoggled_color[temp][Group].rgb = color;
  }
}

void TFT_Extension::RCB_Untoggled_Color_By_Group(uint8_t Group, word color)
{
  for(int temp = 0; temp < Block.number[Group]; temp++)
  {
   RCB_Untoggled_color[temp][Group].rgb = color;
  }
}
//======================================================================
//
//======================================================================
 
void TFT_Extension::SetAll_RB_Outer_Color(word color)
{
  for( uint8_t G_idx = 0; G_idx < TotalRB_Group; G_idx++)
  {
    for(uint8_t temp = 0; temp < Block.number[G_idx]; temp++)
    {
      RB_Outer_color[temp][G_idx].rgb = color;
    }
  }
}
 
void TFT_Extension::SetAll_RCB_Outer_Color(word color)
{
  for( uint8_t G_idx = 0; G_idx < TotalRCB_Group; G_idx++)
  {
    for(uint8_t temp = 0; temp < Block.number[G_idx]; temp++)
    {
      RCB_Outer_color[temp][G_idx].rgb = color;
    }
  }
}
//===========================================================================
 
void TFT_Extension::SetAll_RB_Toggled_Color(word color)
{
  for( uint8_t G_idx = 0; G_idx < TotalRB_Group; G_idx++)
  {
    for(uint8_t temp = 0; temp < Block.number[G_idx]; temp++)
    {
      RB_Toggled_color[temp][G_idx].rgb = color;
    }
  }
}
 
void TFT_Extension::SetAll_RCB_Toggled_Color(word color)
{
  for( uint8_t G_idx = 0; G_idx < TotalRCB_Group; G_idx++)
  {
    for(uint8_t temp = 0; temp < Block.number[G_idx]; temp++)
    {
      RCB_Toggled_color[temp][G_idx].rgb = color;
    }
  }
}
//=============================================================================
 
void TFT_Extension::SetAll_RB_Untoggled_Color(word  color)
{
  for( uint8_t G_idx = 0; G_idx < TotalRB_Group; G_idx++)
  {
    for(uint8_t temp = 0; temp < Block.number[G_idx]; temp++)
    {
      RB_Untoggled_color[temp][G_idx].rgb = color;
    }
  }
}

 
void TFT_Extension::SetAll_RCB_Untoggled_Color(word color)
{
  for( uint8_t G_idx = 0; G_idx < TotalRCB_Group; G_idx++)
  {
    for(uint8_t temp = 0; temp < Block.number[G_idx]; temp++)
    {
      RCB_Untoggled_color[temp][G_idx].rgb = color;
    }
  }
}

//======================================================================
//
//======================================================================
void TFT_Extension::Triangle(int x1, int y1,int x2,int y2,int x3,int y3)
{
  _Disp->drawLine(x1,y1,x2,y2);// lean left
  _Disp->drawLine(x2,y2,x3,y3);// _ base
  _Disp->drawLine(x3,y3,x1,y1);// / lean right
}
 
void TFT_Extension::drawTriangle(int x1, int y1, int base, int deg)
{  
  int rad = base/2; int dir;

  if(deg < 90) dir=up;
  else if(deg < 180) dir=left; // down
  else if(deg < 270) dir=down;
  else dir = right;
  
  Cx = x1 + sin(float(deg) * deg_to_rad) * rad;
  Cy = y1 + cos(float(deg) * deg_to_rad) * rad;
  Cx1 = x1 + sin((float(deg) + 120) * deg_to_rad) * rad;
  Cy1 = y1 + cos((float(deg) + 120) * deg_to_rad) * rad;
  Cx2 = x1 + sin((float(deg) + 240) * deg_to_rad) * rad;
  Cy2 = y1 + cos((float(deg) + 240) * deg_to_rad) * rad;
  
  if(base >= 60)
  {
    switch(dir)
    {
     case up:
       Triangle(Cx,Cy,Cx1,Cy1,Cx2-1,Cy2-1);
       break;
	   
     case down:
	   Triangle(Cx+1,Cy,Cx1+1,Cy1,Cx2+1,Cy2+1);
       break;
	   
     case left:
	   Triangle(Cx,Cy,Cx1,Cy1,Cx2-1,Cy2+1);
       break;
	   
     case right:
	   Triangle(Cx,Cy,Cx1,Cy1,Cx2+1,Cy2);
       break;
    }
  }
  else 
    Triangle(Cx,Cy,Cx1,Cy1,Cx2,Cy2); 
}
 
void TFT_Extension ::fillTriangle(int x1,int y1,int base, int deg)
{   
  int rad = base/2; int dir;

  if(deg < 90) dir=up;
  else if(deg < 180) dir=left; 
  else if(deg < 270) dir=down;
  else dir = right;
  
  Cx = x1 + sin(float(deg) * deg_to_rad) * rad;
  Cy = y1 + cos(float(deg) * deg_to_rad) * rad;
  Cx1 = x1 + sin((float(deg) + 120) * deg_to_rad) * rad;
  Cy1 = y1 + cos((float(deg) + 120) * deg_to_rad) * rad;
  Cx2 = x1 + sin((float(deg) + 240) * deg_to_rad) * rad;
  Cy2 = y1 + cos((float(deg) + 240) * deg_to_rad) * rad;
  
  //if(base >= 60)
  //{
    switch(dir)
    {
     case up:
       Triangle(Cx,Cy,Cx1,Cy1,Cx2-1,Cy2-1);
	   fillPoly(Cx,Cy,Cx1,Cy1,Cx2-1,Cy2-1);
	   fillPoly(Cx1,Cy1,Cx2-1,Cy2-1,Cx,Cy);
       break;
	   
     case down:
	   Triangle(Cx+1,Cy,Cx1+1,Cy1,Cx2+1,Cy2+1);
	   fillPoly(Cx+1,Cy,Cx1+1,Cy1,Cx2+1,Cy2+1);
	   fillPoly(Cx1+1,Cy1,Cx2+1,Cy2+1,Cx+1,Cy);
       break;
	   
     case left:
	   Triangle(Cx,Cy,Cx1,Cy1,Cx2-1,Cy2+1);
	   fillPoly(Cx,Cy,Cx1,Cy1,Cx2-1,Cy2+1);
	   fillPoly(Cx1,Cy1,Cx,Cy,Cx2-1,Cy2+1);
       break;
	   
     case right:
	   Triangle(Cx,Cy,Cx1,Cy1,Cx2+1,Cy2);
	   fillPoly(Cx,Cy,Cx1,Cy1,Cx2+1,Cy2);
	   fillPoly(Cx1,Cy1,Cx,Cy,Cx2+1,Cy2);
       break;
    }
 // }
  //else 
    //Triangle(Cx,Cy,Cx1,Cy1,Cx2,Cy2); 
  
  //having two does take longer, but this ensures that the triangle is completely filled. 
  //fillPoly(Cx1,Cy1,Cx2,Cy2,Cx,Cy);
}

void TFT_Extension::Polygon(int cx, int cy, int sides, int diameter, word color, bool fill, float deg)
{ 
  sides = (sides > 2? sides : 3); // This ensures the minimum side number is 3.
  int Xpoints[sides], Ypoints[sides];// Set the arrays based on the number of sides entered
  int rads = 360 / sides; // This equally spaces the points.
 
  for(int idx = 0; idx < sides; idx++) 
  { 
    Xpoints[idx] = cx + sin(float(idx*rads + deg) * deg_to_rad) * diameter; 
    Ypoints[idx] = cy + cos(float(idx*rads + deg) * deg_to_rad) * diameter;
  }
  
  _Disp->setColor(color);
  for(int idx = 0; idx < sides; idx++) // draws the polygon on the screen.
  {
    if( (idx+1) < sides) 
      _Disp->drawLine(Xpoints[idx],Ypoints[idx],Xpoints[idx+1],Ypoints[idx+1]); // draw the lines 
    else
      _Disp->drawLine(Xpoints[idx],Ypoints[idx],Xpoints[0],Ypoints[0]); // finishes the last line to close up the polygon.
  }
  if(fill)
    for(int idx = 0; idx < sides; idx++)
    {
      if((idx+1) < sides)
        fillPoly(cx,cy,Xpoints[idx],Ypoints[idx],Xpoints[idx+1],Ypoints[idx+1]);
      else
        fillPoly(cx,cy,Xpoints[idx],Ypoints[idx],Xpoints[0],Ypoints[0]);
    }
}

void TFT_Extension::fillPoly(int x1, int y1, int x2, int y2, int x3, int y3)
{
  float xx, yy;
  xx = abs(x3-x2);
  yy = abs(y3-y2)/(xx == 0? 1:xx);
  for(float i = 0; i <= xx; i+=0.2)
  {
    _Disp->drawLine(x1,y1, x2 + (x2>x3 ? -i:i), y2 + ((yy*i) * (y2 > y3 ? -1 : 1) ));
  }
}

// Similar to the Polygon function.
void TFT_Extension::drawStar(int cx, int cy, int diameter, word color, bool fill, float factor)
{
  factor = constrain(factor, 1.0, 4.0);
  uint8_t sides = 5;
  uint8_t rads = 360 / sides;
  
  int Xpoints_O[sides], Ypoints_O[sides], Xpoints_I[sides], Ypoints_I[sides];//Xpoints_T[5], Ypoints_T[5];

  for(int idx = 0; idx < sides; idx++)
  { 
    Xpoints_O[idx] = cx + sin(float(idx*rads + 72) * deg_to_rad) * diameter; // makes the outer points
    Ypoints_O[idx] = cy + cos(float(idx*rads + 72) * deg_to_rad) * diameter;
    Xpoints_I[idx] = cx + sin(float(idx*rads + 36) * deg_to_rad) * (float(diameter)/factor); // makes the inner points
    Ypoints_I[idx] = cy + cos(float(idx*rads + 36) * deg_to_rad) * (float(diameter)/factor); // 36 is half of 72, and this will allow the inner and outer points to line up like a triangle. 
  }
  
  _Disp->setColor(color);
  for(int idx = 0; idx < sides; idx++)
  {   
	if((idx+1) < sides)
	{
	  if(fill) // this part below should be self explanatory. It fills in the star.
	  {
		fillPoly(cx,cy,Xpoints_I[idx],Ypoints_I[idx],Xpoints_O[idx],Ypoints_O[idx]);
		fillPoly(cx,cy,Xpoints_O[idx],Ypoints_O[idx],Xpoints_I[idx+1],Ypoints_I[idx+1]);
	  }
	  else
	  {
		_Disp->drawLine(Xpoints_O[idx],Ypoints_O[idx],Xpoints_I[idx+1],Ypoints_I[idx+1]);
		_Disp->drawLine(Xpoints_I[idx],Ypoints_I[idx],Xpoints_O[idx],Ypoints_O[idx]);
	  }
	}
    else
    {
	  if(fill)
	  {
		fillPoly(cx,cy,Xpoints_I[0],Ypoints_I[0],Xpoints_O[idx],Ypoints_O[idx]); 
		fillPoly(cx,cy,Xpoints_O[idx],Ypoints_O[idx],Xpoints_I[idx],Ypoints_I[idx]);
	  }
	  else
	  {
	    _Disp->drawLine(Xpoints_O[idx],Ypoints_O[idx],Xpoints_I[idx],Ypoints_I[idx]);
	    _Disp->drawLine(Xpoints_I[0],Ypoints_I[0],Xpoints_O[idx],Ypoints_O[idx]);
	  }
    }
  }
}

void TFT_Extension::drawArc(int cx, int cy, int radius, int thickness, int start, int stop, word color, bool lines)
{
  stop-=1;
  if(start > stop)
    swap(int, start, stop);
	
  int XI,YI,XR,YR,XL,YL;
  thickness = constrain(thickness, 0, radius);
  
  _Disp->setColor(color);
  for(float thick = (radius - thickness); thick <= radius; thick += 0.5)
  {
	for(int rotate = start; rotate <= stop; rotate++)
	{ 
	
	  XI = cx + int(-cos(rotate * deg_to_rad) * thick);
	  if(rotate == 0) XI-=1; // this gets rid of the extra pixel at 0 deg
      YI = cy + int(sin(rotate * deg_to_rad) * thick);
	  if(rotate == start){ XR = XI; YR = YI;}
	  if(rotate == stop){XL = XI; YL = YI;}
	  _Disp->drawPixel(XI,YI);
	}
    if(stop - start >= 359)
      lines = false;
	
    if(lines)
    {
      _Disp->drawLine(cx,cy, XR,YR); // draw the line for the bottom of the arc
      _Disp->drawLine(cx,cy, XL,YL); // =======================================
    }
  }
}

void TFT_Extension::drawOvalArc(int cx, int cy, int Xradius, int Yradius, int Xthickness, int Ythickness, int start, int stop, word color)
{
  stop-=1;
  if(start > stop)
    swap(int, start, stop);
	
  int XI,YI,XR,YR,XL,YL;
  Xthickness = constrain(Xthickness, 0, Xradius);
  Ythickness = constrain(Ythickness, 0, Yradius);
  
  
  _Disp->setColor(color);
  for(float Xthick = (Xradius - Xthickness), Ythick = (Yradius - Ythickness)/Xradius; Xthick <= Xradius; Xthick++, Ythick+=0.1)
  {
	for(float rotate = start; rotate <= stop; rotate+=0.5)
	{ 
	  XI = cx + sin((rotate - 90) * deg_to_rad) * float(Xthick);
	  YI = cy + cos((rotate - 90) * deg_to_rad) * float(Ythick);
	  if(rotate == start){ XR = XI; YR = YI;}
	  if(rotate == stop){XL = XI; YL = YI;}
	  _Disp->drawPixel(XI,YI);
	}
  }
  _Disp->drawLine(cx,cy, XR,YR); // draw the line for the bottom of the arc
  _Disp->drawLine(cx,cy, XL,YL); // =======================================
}

void TFT_Extension::drawMoon(int cx, int cy, int radius, uint8_t phase)
{
  byte C = 0, N;
  int i=0;
  
  unsigned int Rad_D2 = radius/2;
  unsigned int Rad_D1_5 = radius/1.5;
  unsigned int Rad_M2 = radius*2;
  unsigned int Rad_M4 = radius*4;
  
  _Disp->setColor(240,240,240);
  _Disp->fillCircle(cx,cy,radius);
  for(N = 0; N < (Rad_D2); N++) // make craters when the moon is first made.
  {
    C = random(0, 20);
    _Disp->setColor(230 - C, 230 - C, 230 - C);
    _Disp->fillCircle(cx + random(-Rad_D1_5, Rad_D1_5), cy + random(-Rad_D1_5, Rad_D1_5), random(2, radius/7));
  }

  _Disp->setColor(100,100,100);
  _Disp->drawCircle(cx,cy, radius);
  
  int rad = map(phase, 0, 8, 0, Rad_M4);
  for(i = 0 ; i < rad; i++ ) // this draws the phase of the moon
  {
    if(i > Rad_M2) // change the fill color based on the phase
      _Disp->setColor(240,240,240);   
    else 
      _Disp->setColor(100,100,100);
      
    for(float rotate = 0 ; rotate <= 180 ; rotate+=1)
    {
      int XI = cx + sin(rotate * deg_to_rad) * float(radius - (i>(Rad_M2)? i-(Rad_M2):i));
      int YI = cy + cos(rotate * deg_to_rad) * float(radius);
      _Disp->drawPixel(XI,YI);
    }
  }
  //_Disp->printNumI(i, 0,0);
  if(i == 0 || i == Rad_M2) {} // disable craters if phase is FULL or NEW
  else if(i > Rad_M2)
  {
    for(N = 0; N < (Rad_D2); N++) // make craters after the moon has gotten to it's phase.
    {
      int tmp = random(-Rad_D1_5, map(i-5, Rad_M2, Rad_M4, -Rad_D1_5, Rad_D1_5)); // make craters based on the lighter area
      int tmp2 = random(-Rad_D1_5, Rad_D1_5); // confine the y-axis to the radius divided by 1.5.
      C = random(0, 20);
      _Disp->setColor(230 - C, 230 - C, 230 - C); //give each crater a random color from 230 to 250
      _Disp->fillCircle(cx + tmp, cy + tmp2, random(2, radius/7));
    }
  }
}

void TFT_Extension::rounded_Square(int cx, int cy, int h, int w, float radius, word color, bool fill)
{ 
  byte angle;
  float i;

  int newW = (w/2) - (radius/2);
  int newH = (h/2) - (radius/2);

  int XPstart[4], XPend[4], tmp;
  int YPstart[4], YPend[4], tmp2;

  _Disp->setColor(color);

  for(angle = 0; angle < (fill? 2:4); angle++)
  {
    XPstart[angle] = ((angle <= 1)? cx - newW : cx + newW) + sin(90*angle * deg_to_rad) * (radius/2);
    YPstart[angle] = ((angle == 0 || angle == 3) ? cy - newH : cy + newH) + cos(90*angle * deg_to_rad) * (radius/2);
    for(i = 90*angle; i <= 90*(angle+1); i++)
    { 
      XPend[angle] = ((angle <= 1)? cx - newW : cx + newW) + (sin(i * deg_to_rad) * (radius/2));
      YPend[angle] = ((angle == 0 || angle == 3) ? cy - newH : cy + newH) + (cos(i * deg_to_rad) * (radius/2));
	  
      if(fill) // fill
	  {
	    tmp = (cx - newW) - XPend[angle]; // get the last Xcoord from the first angle
        tmp2 = (cx - newW) - XPstart[angle]; // get the first Xcoord from the second angle
        _Disp->drawLine(XPend[angle], YPend[angle],XPend[angle] + (tmp*2) + (w-radius), YPend[angle]); 
		}
      else // no fill
        _Disp->drawPixel(XPend[angle], YPend[angle]);
    }
  }
  if(fill) //fill 
      _Disp->fillRect(XPend[0], YPend[0], XPstart[1]+ (tmp2*2) + (w-radius), YPstart[1]);
  else
    for(angle=0; angle <= 3; angle++)// draw the outer lines
      _Disp->drawLine(XPend[angle], YPend[angle], XPstart[angle+1 == 4? 0 : angle+1], YPstart[angle+1 == 4? 0 : angle+1]); 
} 

void TFT_Extension::smiley_Face(int cx, int cy, int radius, byte eyes, byte mouth)
{
  int XI,YI,XI2;
  _Disp->setColor(YELLOW);
  _Disp->fillCircle(cx,cy,radius);
  _Disp->setColor(BLACK);
  _Disp->drawCircle(cx,cy,radius);

  switch(eyes)
  {
  case 0:// normal eyes
    for(float rotate = 0 ; rotate < 360 ; rotate+=1) 
    {
      XI = (cx - radius/3) - cos(rotate * deg_to_rad) * (radius/10);
      YI = (cy - radius/3) + sin(rotate * deg_to_rad) * (radius/3.3);
      XI2 = (cx + radius/3) - cos(rotate * deg_to_rad) * (radius/10);
	  if(rotate == 0) { XI -= 1; XI2 -= 1; } // this gets rid of the extra pixel at 0 deg
      _Disp->drawLine(cx - (radius/3),cy - (radius/3),XI,YI);
      _Disp->drawLine(cx + (radius/3),cy - (radius/3),XI2,YI);
    }
    break;

  case 1: // round eyes
    _Disp->fillCircle(cx - radius/3, cy - radius/3, radius/8);
    _Disp->fillCircle(cx + radius/3, cy - radius/3, radius/8);
    break;

  case 2: // over joyed
    drawArc(cx - radius/3, cy - radius/3, radius/5, radius/25, 0, 180, BLACK); 
    drawArc(cx + radius/3, cy - radius/3, radius/5, radius/25, 0, 180, BLACK);
    break;

  case 3: // content
    drawArc(cx - (radius/3), cy - (radius/3), radius/5, radius/25, 180, 360, BLACK);
    drawArc(cx + (radius/3), cy - (radius/3), radius/5, radius/25, 180, 360, BLACK);
    break;

  case 4: // angry
      _Disp->drawLine((cx - radius/5) - 1, cy - radius/5, (cx - radius/2) - 1, cy - radius/2);
      _Disp->drawLine((cx + radius/5) - 1, cy - radius/5, (cx + radius/2) - 1, cy - radius/2);
    break;

  case 5://very angry
      drawArc(cx - radius/3, cy - radius/3, radius/6, 0, 135, 305, BLACK, true);
      drawArc(cx + radius/3, cy - radius/3, radius/6, 0, 240, 395, BLACK, true);
    break;

  case 6: // big eyes
    drawArc(cx - radius/3, cy - radius/3, radius/6, radius/25, 0, 360, BLACK);
    drawArc(cx + radius/3, cy - radius/3, radius/6, radius/25, 0, 360, BLACK);
    break;

  case 7:
    _Disp->setColor(WHITE);
    _Disp->fillCircle(cx - radius/4, cy - radius/3, radius/5);
    _Disp->fillCircle(cx + radius/4, cy - radius/3, radius/5);
    _Disp->setColor(BLACK);
    _Disp->drawCircle(cx - radius/4, cy - radius/3, radius/5);
    _Disp->drawCircle(cx + radius/4, cy - radius/3, radius/5);
    _Disp->drawPixel(cx - radius/4, cy - radius/3);
    _Disp->drawPixel(cx + radius/4, cy - radius/3);
    break;

  case 8:
    _Disp->drawLine(cx - radius/3, cy - radius/1.5, cx, cy - radius/2.5);
    _Disp->drawLine(cx - radius/3, cy - radius/3.5, cx, cy - radius/2.5);
    _Disp->drawLine(cx + radius/3, cy - radius/1.5, cx, cy - radius/2.5);
    _Disp->drawLine(cx + radius/3, cy - radius/3.5, cx, cy - radius/2.5);
    break;

  case 9:
    drawArc(cx - radius/3, cy - radius/3, radius/5, radius/5, 180, 361, BLACK);
    drawArc(cx + radius/3, cy - radius/3, radius/5, radius/5, 180, 361, BLACK); 
    break;
  }

  switch(mouth)
  {
  case 0:
    drawArc(cx,cy, int(float(radius)/1.6), radius/25, 180, 360,  BLACK); // big happy smile
    break;

  case 1:
    drawArc(cx,cy, int(float(radius)/1.6), radius/25, 210, 330,  BLACK); // happy smile
    break;

  case 2:
    drawArc(cx,cy + radius/1.5 , radius/2, radius/25, 30, 150,  BLACK); // frown
    break;

  case 3: // big open smile
    drawArc(cx,cy, int(float(radius)/1.6), int(float(radius)/1.6), 181, 360,  WHITE);
    for(byte i = 0; i <= radius/25; i++)
    {
      drawArc(cx,cy, int(float(radius)/1.6), i, 181, 360,  BLACK, true);
      //_Disp->drawLine(cx - radius/1.8, cy+i, cx + radius/1.8, cy+i);
    }
    break;

  case 4:
    drawArc(cx,cy + radius , radius/2, radius/25, 60, 120,  BLACK); //big frown
    break;

  case 5:
    drawArc(cx,cy + radius/1.5 , radius/2, radius/25, 0, 180,  BLACK); // bigger frown
    break;

  case 6:
    drawArc(cx,cy + radius/1.5 , radius/2, radius/3, 70, 110,  BLACK); // shocked open mouth
    break;

  case 7:
    drawArc(cx,cy + radius , radius/1.2, radius/3, 50, 130,  BLACK); // crying mouth
    break;
  }
}

void TFT_Extension::HourGlass(int cx, int cy, int height, int time)
{
  _Disp->setColor(BLACK);
  if(!locked)
  {
    rad = height;
    locked = true;
  }

  if(rad >= 0)
  {
    int Cx  = cx + sin(135 * deg_to_rad) * rad; 
    int Cy  = cy + cos(135 * deg_to_rad) * rad;
    int Cx2 = cx + sin(225 * deg_to_rad) * rad; 
    int Cy2 = cy + cos(225 * deg_to_rad) * rad;
    int Cx3 = cx + sin(315 * deg_to_rad) * rad; 
    int Cy3 = cy + cos(315 * deg_to_rad) * rad;
    int Cx4 = cx + sin(45 * deg_to_rad) * rad; 
    int Cy4 = cy + cos(45 * deg_to_rad) * rad;
	
	if(flip)
	{
	  if(millis() - T > int(float(time*1000UL) / float(height)) )
      {
        T = millis(); 
        rad--;
      }
      _Disp->setColor(LIGHT_ORANGE);
      _Disp->drawLine(Cx+2 , Cy, Cx2-2, Cy2);// bottom 
      _Disp->drawLine(cx-1,cy, cx-1,Cy);
      _Disp->setColor(BLACK);
      _Disp->drawLine(Cx3-2, Cy3, Cx4+2, Cy4); // top
	}
    else
	{
      rad--;
      _Disp->setColor(BLACK);
      _Disp->drawLine(Cx , Cy, Cx2, Cy2);// bottom 
      _Disp->drawLine(Cx2, Cy2, Cx4, Cy4);// left
      _Disp->drawLine(Cx3, Cy3, Cx, Cy); // right
      _Disp->setColor(LIGHT_ORANGE);
      _Disp->drawLine(Cx3, Cy3, Cx4, Cy4); // top
    }
  }
  else 
  {
    flip = !flip;
    rad = height;
    T = millis(); 
  }
}

void TFT_Extension::SpeechBubble(char * str, int cx, int cy, float radius, int deg)
{
  Save_MainColor;
  float start, stop;
  int x,y,xo,yo,fx,fy;
  int Lx1,Ly1,Lx2,Ly2;
  byte strl = strlen(str);
  byte MidTxt = int((radius*2) - (radius/2))/8;
  
  //_Disp->printNumI(strl, 0,0);
  //_Disp->printNumI(MidTxt ,0,10);
   //  byte Ytxtlen = (radius / 16);
  //  int CX_Offset[Ytxtlen];
  //  byte OuterTxt = (radius/8);
  //  int txtTotal = (MidTxt * (Ytxtlen/2)) + (OuterTxt*2);

  //  for(byte i = 0; i < Ytxtlen; i++)
  //  {
  //   if(i == 0 || i == (Ytxtlen-1) )
  //     CX_Offset[i] = OuterTxt;
  //   else 
  //     CX_Offset[i] = MidTxt;
  //  }

  //_Disp->printNumI(txtTotal, 0,0);

  if(deg < 180)
  {
    start = deg + 15;
    stop = (deg - 15 > 0? (360 + (deg - 15)) : (360 - (deg + 15)));
  }
  else 
  {
    start = deg - 360 + 15;
    stop = deg-15;
  }

  Lx1 = cx + sin((start - 90) * deg_to_rad) * radius;
  Ly1 = cy + cos((start - 90) * deg_to_rad) * (radius/2);
  Lx2 = cx + sin((stop - 90) * deg_to_rad) * radius;
  Ly2 = cy + cos((stop - 90) * deg_to_rad) * (radius/2);

  for (float i = start; i < stop; i += 1) 
  {
    x = cx + sin((i - 90) * deg_to_rad) * radius;
    y = cy + cos((i - 90) * deg_to_rad) * (radius/2);
    xo = cx + sin((deg - 90) * deg_to_rad) * (radius*1.5);
    yo = cy + cos((deg - 90) * deg_to_rad) * (radius);
    _Disp->setColor(0,0,0);
    _Disp->drawPixel(x, y);
  } 
  _Disp->setColor(WHITE); 
  for(byte f= 1; f < radius*2; f++)
  {
    for(float i = 0; i < 180; i+= 1)
    {
      fx = cx + sin(i * deg_to_rad) * (radius-f);
      fy = cy + cos(i * deg_to_rad) * ((radius/2)-1);
      _Disp->drawPixel(fx,fy);
    }
  }
  fillPoly(xo,yo, Lx2, Ly2,Lx1, Ly1);
  _Disp->setColor(BLACK);
  _Disp->drawLine(xo,yo, Lx1, Ly1);
  _Disp->drawLine(xo,yo, Lx2, Ly2);
  _Disp->setBackColor(WHITE);

  _Disp->setFont(SmallFont);
  //  byte i = 0;
  //  if(strl > int(txtTotal))
  //    _Disp->print("ERR", cx - (3*4), cy - 6, 0);
  //  else
  //  {
  //    byte S = 0, CY = 0, CX = 0;
  //    for(int s = -(strl/2); s < (strl/2); s++, S++)
  //    {
  //            _Disp->printNumI( strl, 0,20);
  //            //_Disp->printNumI(txtTotal, 0,30);
  //      //      _Disp->printNumI(CX_Offset, 0,40);
  //
  //      if(strl > ((radius*2)/8) )
  //      {
  //        delay(1000);
  //        _Disp->printNumI(CX_Offset[i], 0,30);
  //        _Disp->printNumI(S % CX_Offset[i], 0,40);
  //        if((S % CX_Offset[i]) == (CX_Offset[i] - 1)) i++;
  //        CY = map((S / CX_Offset[i]), 0, Ytxtlen,  -(Ytxtlen * 16), (Ytxtlen * 16) );
  //        CX = map(S % CX_Offset[i], 0, CX_Offset[i], -CX_Offset[i]/2, CX_Offset[i]/2 );
  //        _Disp->printChar(str[S], cx + (CX * 8), cy + CY ); // small font is 8x12
  //      }
  //      else 
  if(strl <= MidTxt)
    _Disp->print(str, cx - (strl*4), cy - 6, 0);
  else 
    _Disp->print("TTL", cx - (3*4), cy - 6, 0); // TTL = Text Too Long
  //}
//}
  Restore_MainColor;
}

void TFT_Extension::drawGauge(byte _ID, int value, int pos_x, int pos_y, int start, int stop, int rad)
{
  _Disp->setColor(WHITE);
  if(!gauge[_ID].locked)
  {
   drawArc(pos_x, pos_y, rad, rad, start, stop,  WHITE, true);
   drawArc(pos_x, pos_y, rad, 0, start, stop,  BLACK, true);
   gauge[_ID].oldx = pos_x; 
   gauge[_ID].oldy = pos_y;
   gauge[_ID].locked = true;
  }
  gauge[_ID].x = pos_x + int(-cos(value * deg_to_rad) * (rad-2));
  gauge[_ID].y = pos_y + int(sin(value * deg_to_rad) * (rad-2));

  if(gauge[_ID].x != gauge[_ID].oldx && gauge[_ID].y != gauge[_ID].oldy)
  {
    _Disp->setColor(WHITE);
    _Disp->drawLine(gauge[_ID].oldx, gauge[_ID].oldy, pos_x, pos_y);  
    _Disp->setColor(BLACK);
    _Disp->drawLine(gauge[_ID].x, gauge[_ID].y,pos_x,pos_y); // (cx, cy, calculated x, calculated y)
    gauge[_ID].oldx = gauge[_ID].x; 
    gauge[_ID].oldy = gauge[_ID].y;
    //_Disp->printNumF(value,2,CENTER,10);//used for debugging  
  }
  _Disp->setColor(WHITE);	
}

void TFT_Extension::ResetGauge(byte _ID)
{
  gauge[_ID].locked = false;
}
//========================KEYBOARD====================
void TFT_Extension::SetupStandardKB()
{
   SetLatchButtonColors(0,  GREEN,  WHITE, NOFILL, NOTROUNDED);
   
   RCV_locked= false;
   RCV_cnt = 0;
	if(_Disp->orient == LANDSCAPE)
	{
		_FONT = Small;
		XoffSet=60;
		YoffSet=160;
		TxtoffSet = 20;
		SEND.x1 = 270; SEND.y1 =160; SEND.x2 =310; SEND.y2 =180;
		BSP.x1 = 270; BSP.y1 = 190; BSP.x2 = 310; BSP.y2 = 210;
		CAPS.x1 = 10; CAPS.y1 = 160; CAPS.x2 = 50; CAPS.y2 = 180;
	}
	else
	{ 
		_FONT = Small;
		XoffSet=25;
		YoffSet=240;
		TxtoffSet = 40;
		SEND.x1 =180; SEND.y1 = 215; SEND.x2 = 220; SEND.y2 = 235;
		BSP.x1 = 130; BSP.y1 = 215; BSP.x2 = 170; BSP.y2 = 235;
		CAPS.x1 = 25; CAPS.y1 = 215; CAPS.x2 = 65; CAPS.y2 = 235;
	}
	Save_MainColor;
	
	_Disp->setColor(0x0);
	_Disp->fillRect(0,YoffSet,_Disp->getDisplayXSize() - 1, _Disp->getDisplayYSize() - 1);
    _Disp->setColor(0xFFFF);
    _Disp->setFont(SmallFont);	
    _Disp->print(">>",0, YoffSet-TxtoffSet);
	
	makeKeyboard();
	
    clearMSG();
	Restore_MainColor; 
}

void TFT_Extension::SetupMobileKB()
{
    RCV_locked= false;
    RCV_cnt = 0;
	if(_Disp->orient == LANDSCAPE)
	{
		_FONT = Big;
		XoffSet=18;
		YoffSet=120;
		TxtoffSet = 0;
		SEND.x1 = 240; SEND.y1 =215; SEND.x2 = 310; SEND.y2 = 235;
		BSP.x1 = 253; BSP.y1 = 187; BSP.x2 = 310; BSP.y2 = 207;
		CAPS.x1 = 5; CAPS.y1 = 187; CAPS.x2 = 40; CAPS.y2 = 207;
		SPACE.x1 = 98; SPACE.y1 = 215; SPACE.x2 = 218; SPACE.y2 = 235;
		NUM.x1 = 5; NUM.y1 = 215; NUM.x2 = 75; NUM.y2 = 235;
		Type = 0;
		_symbol = 0;
	}
	else
	{
		_FONT = Small;
		XoffSet=25;
		YoffSet=230;
		TxtoffSet = 0;
		SEND.x1 =180; SEND.y1 = 300; SEND.x2 = 230; SEND.y2 = 315;
		BSP.x1 = 190; BSP.y1 = 281; BSP.x2 = 230; BSP.y2 = 296;
		CAPS.x1 = 5; CAPS.y1 = 281; CAPS.x2 = 25; CAPS.y2 = 296;
		SPACE.x1 = 85; SPACE.y1 = 300; SPACE.x2 = 150; SPACE.y2 = 315;
		NUM.x1 = 5; NUM.y1 = 300; NUM.x2 = 55; NUM.y2 = 315;
		Type = 0;
		_symbol = 0;
	}
	Save_MainColor;
	
	_Disp->setColor(0x0);
	_Disp->fillRect(0,YoffSet,_Disp->getDisplayXSize() - 1, _Disp->getDisplayYSize() - 1);
    _Disp->setColor(0xFFFF);
    _Disp->setFont(SmallFont);	
    _Disp->print(">>",0, YoffSet-TxtoffSet);
	
    make_Mobile_Keyboard();
    clearMSG();
	
    Restore_MainColor;
}

void TFT_Extension::makeKeyboard()
{ 
  //SmileyFaces(false);
  _Disp->setColor(WHITE);
  _Disp->drawRect(XoffSet, YoffSet-1, XoffSet+ 12*16, YoffSet+(14*5)-1);

  for(byte row = 0; row < 5; row++)
  {
    _Disp->drawLine(XoffSet,(YoffSet-1) + (14*row), XoffSet+ 12*16,(YoffSet-1) + (14*row));
    for(byte col = 3; col < 17; col++)
    {
      if(pgm_read_byte(&(KB[row][1])) == col) break; 
      _Disp->printChar( pgm_read_byte(&(KB[row][col])), XoffSet + 15*(col-3) + (15* pgm_read_byte(&(KB[row][0]))), YoffSet + 14*row);
      if(row != 4) 
        _Disp->drawLine(XoffSet + 15*(col-3)+ (15*pgm_read_byte(&(KB[row][0])) + (col!=3? -3:0)), YoffSet + 14*row, XoffSet + 15*(col-3)+ (15*pgm_read_byte(&(KB[row][0])) + (col!=3? -3:0)),YoffSet + (14*row) + 14);
    }
  }
  _Disp->print("LS", XoffSet+10, YoffSet + 14*3);
  _Disp->drawRect(SEND.x1, SEND.y1, SEND.x2, SEND.y2);
  _Disp->drawRect(BSP.x1, BSP.y1, BSP.x2, BSP.y2);
   TextButton("CAPS",_FONT, CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2,  WHITE);
}

void TFT_Extension::make_Mobile_Keyboard()
{  
  Save_MainColor;
  if(_FONT == Big)
    _Disp->setFont(BigFont);
  else 
    _Disp->setFont(SmallFont);
  //SmileyFaces(false);
  _Disp->setColor(WHITE);

  for(byte row = 0; row < 3; row++)
  {
    for(byte col = 3; col < 17; col++)
    {
	  _Disp->drawRoundRect(XoffSet + (_FONT? 30:20)*(col-3) + (15* pgm_read_byte(&(Mobile_KB[row][0])))-4,(YoffSet) + ((_FONT? 23:18)*(row+1)) - 2, XoffSet + (_FONT? 30:20)*(col-3) + (15* pgm_read_byte(&(Mobile_KB[row][0]))) + (_FONT? 16:8) + 1, (YoffSet) + ((_FONT? 23:18)*(row+1)) + (_FONT? 16:12)); 
      _Disp->printChar( pgm_read_byte(&(Mobile_KB[row][col])), XoffSet + (_FONT? 30:20)*(col-3) + (15* pgm_read_byte(&(Mobile_KB[row][0]))), (YoffSet) + (_FONT? 23:18)*(row+1));
	  if(pgm_read_byte(&(Mobile_KB[row][2])) == col-2) break; 
      
    }
  }
  _Disp->setColor(BLACK);
  _Disp->fillRoundRect(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2); // shift key
  _Disp->setColor(WHITE);
  _Disp->drawRoundRect(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2); // shift key
  _Disp->print("^", ((CAPS.x1 + CAPS.x2)/2) - (_FONT? 8:4), ((CAPS.y1 + CAPS.y2)/2) - (_FONT? 8:4));
  _Disp->drawRoundRect(SEND.x1, SEND.y1, SEND.x2, SEND.y2);
  _Disp->print("RTRN", ((SEND.x1 + SEND.x2)/2) - (_FONT? 8:4)*4, ((SEND.y1 + SEND.y2)/2) - (_FONT? 8:4));
  _Disp->drawRoundRect(BSP.x1, BSP.y1, BSP.x2, BSP.y2);
  _Disp->print("BSP", ((BSP.x1 + BSP.x2)/2) - (_FONT? 8:4)*3, ((BSP.y1 + BSP.y2)/2) - (_FONT? 8:4));
  _Disp->drawRoundRect(SPACE.x1, SPACE.y1, SPACE.x2, SPACE.y2); // spacebar
  _Disp->print("SPACE", ((SPACE.x1 + SPACE.x2)/2) - (_FONT? 8:4)*5, ((SPACE.y1 + SPACE.y2)/2) - (_FONT? 8:4));
  _Disp->drawRoundRect(NUM.x1, NUM.y1, NUM.x2, NUM.y2); //numKeys
  _Disp->print("123", ((NUM.x1 + NUM.x2)/2) - (_FONT? 8:4)*3, ((NUM.y1 + NUM.y2)/2) - (_FONT? 8:4));
  
  Restore_MainColor;
}

void TFT_Extension::makeShiftKeys()
{ 
  _Disp->drawRect(XoffSet, YoffSet-1, XoffSet+ 12*16, YoffSet+(14*4)-1);
  _Disp->setColor(RED);
  for(byte row = 0; row < 4; row++)
  {
    _Disp->drawLine(XoffSet,(YoffSet-1) + (14*row), XoffSet+ 12*16,(YoffSet-1) + (14*row));
    for(byte col = 3; col < 17; col++)
    {
      if(pgm_read_byte(&(ShiftKB[row][1])) == col) break; 
      _Disp->printChar( pgm_read_byte(&(ShiftKB[row][col])), XoffSet + 15*(col-3) + (15* pgm_read_byte(&(ShiftKB[row][0]))), YoffSet + 14*row);
      if(row == 4)break;
    }
  }
}

void TFT_Extension::makeNumberKeys()
{ 
  _Disp->setColor(BLACK);
  _Disp->fillRect(XoffSet - 4,(YoffSet) + (_FONT? 23:18) - 2, XoffSet + (_FONT? 30:20)*9 + 17,(YoffSet) + ((_FONT? 23:15)*2) +18);
  
  _Disp->fillRect(50,(YoffSet) + ((_FONT? 23:17)*3) - 2, XoffSet + (_FONT? 30:15)*6 +(_FONT? 15:28)*2 + 17,(YoffSet) + ((_FONT? 23:16)*3) +20);
  
  if(_FONT == Big)
    _Disp->setFont(BigFont);
  else 
    _Disp->setFont(SmallFont);
  //SmileyFaces(false);
  _Disp->setColor(WHITE);

  for(byte row = 0; row < 3; row++)
  {
    for(byte col = 3; col < 17; col++)
    {
	  _Disp->drawRoundRect(XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_NumKeys[row][0])))-4, (YoffSet) + ((_FONT? 23:18)*(row+1)) - 2, XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_NumKeys[row][0]))) + (_FONT? 16:8) + 1, (YoffSet) + ((_FONT? 23:18)*(row+1)) + (_FONT? 16:12)); 
      
	  _Disp->printChar( pgm_read_byte(&(Mobile_NumKeys[row][col])), XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_NumKeys[row][0]))), (YoffSet) + (_FONT? 23:18)*(row+1));
	  
	  if(pgm_read_byte(&(Mobile_NumKeys[row][2])) == col-2) break; 
    }
  }
  _Disp->drawRoundRect(NUM.x1, NUM.y1, NUM.x2, NUM.y2); //numKeys
  _Disp->print("ABC", ((NUM.x1 + NUM.x2)/2) - (_FONT? 8:4)*3, ((NUM.y1 + NUM.y2)/2) - (_FONT? 8:4));
  _Disp->drawRoundRect(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2);
  _Disp->print("#", ((CAPS.x1 + CAPS.x2)/2) - (_FONT? 8:4), ((CAPS.y1 + CAPS.y2)/2) - (_FONT? 8:4));
}

void TFT_Extension::makeSymbolKeys()
{ 
  _Disp->setColor(BLACK);
  _Disp->fillRect(XoffSet - 4,(YoffSet) + (_FONT? 23:18) - 2, XoffSet + (_FONT? 30:20)*9 + 17,(YoffSet) + ((_FONT? 23:15)*2) +18);
  
  _Disp->fillRect(5,(YoffSet) + ((_FONT? 23:17)*3) - 2, XoffSet + (_FONT? 30:15)*6 +(_FONT? 15:28)*2 + 17,(YoffSet) + ((_FONT? 23:16)*3) +20);
  if(_FONT == Big)
    _Disp->setFont(BigFont);
  else 
    _Disp->setFont(SmallFont);
  //SmileyFaces(false);
  _Disp->setColor(WHITE);

  for(byte row = 0; row < 3; row++)
  {
    for(byte col = 3; col < 17; col++)
    {
	  _Disp->drawRoundRect(XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_SymKeys[row][0])))-4, (YoffSet) + ((_FONT? 23:18)*(row+1)) - 2, XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_SymKeys[row][0]))) + (_FONT? 16:8) + 1, (YoffSet) + ((_FONT? 23:18)*(row+1)) + (_FONT? 16:12)); 
      
	  _Disp->printChar( pgm_read_byte(&(Mobile_SymKeys[row][col])), XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_SymKeys[row][0]))), (YoffSet) + (_FONT? 23:18)*(row+1));
	  if(pgm_read_byte(&(Mobile_SymKeys[row][2])) == col-2) break; 
    }
  }
  _Disp->drawRoundRect(CAPS.x1, CAPS.y1,NUM.x2,CAPS.y2); //numKeys
  _Disp->print("123", ((CAPS.x1 + NUM.x2)/2) - (_FONT? 8:4)*3, ((CAPS.y1 + CAPS.y2)/2) - (_FONT? 8:4));
  _Disp->drawRoundRect(NUM.x1, NUM.y1, NUM.x2, NUM.y2); //numKeys
  _Disp->print("ABC", ((NUM.x1 + NUM.x2)/2) - (_FONT? 8:4)*3, ((NUM.y1 + NUM.y2)/2) - (_FONT? 8:4));
  //_Disp->print("#", ((CAPS.x1 + CAPS.x2)/2) - 7, ((CAPS.y1 + CAPS.y2)/2) - 8);
}

void TFT_Extension::makeCapsKeys()
{
  _Disp->setColor(GREEN);
  _Disp->drawRect(XoffSet, YoffSet-1, XoffSet+ 12*16, YoffSet+(14*4)-1);
  for(byte row = 0; row < 5; row++)
  {
    _Disp->drawLine(XoffSet,(YoffSet-1) + (14*row), XoffSet+ 12*16,(YoffSet-1) + (14*row));
    for(byte col = 3; col < 17; col++)
    {
      if(pgm_read_byte(&(KB[row][2]))+3 == col) break; 
      _Disp->printChar( pgm_read_byte(&(KB[row][col])), XoffSet + 15*(col-3) + (15* pgm_read_byte(&(KB[row][0]))), YoffSet + 14*row);
      if(row != 4) 
        _Disp->drawLine(XoffSet + 15*(col-3)+ (15* pgm_read_byte(&(KB[row][0])) + (col!=3? -3:0)), YoffSet + 14*row, XoffSet + 15*(col-3)+ (15* pgm_read_byte(&(KB[row][0])) + (col!=3? -3:0)),YoffSet + (14*row) + 14);
    }
  }
  _Disp->setColor(WHITE);
}

void TFT_Extension::clearMSG()
{
  int C = BUF-1;
  while( C != 0)
  {
    C--;
	MSG[C] = ' ';
    //MSG[C] = '\0';
  }
}

char* TFT_Extension::Standard_KeyBoard(word color)
{
  Save_MainColor;
  _Disp->setColor(WHITE); 
  for(byte row = 0; row < 5; row++)
  {
    for(byte col = 3; col < 17; col++)
    {
      if( TouchButton(XoffSet+10, YoffSet + 14*3, XoffSet + 18,YoffSet + (14*4)))
      {
        _Disp->setColor(RED);
        _Disp->drawRect(XoffSet, YoffSet + (14*3)-1, XoffSet + 30,YoffSet + (14*4)-1);
        Shift = true;
        makeShiftKeys();
        _Disp->setColor(WHITE);
      }

      if(KB[row][1] == col) break;

      if( TouchButton(XoffSet + 15*(col-3)+ (15*pgm_read_byte(&(KB[row][0])) + (col!=3? -3:0)), YoffSet + 14*row, XoffSet + 15*(col-2)+ (15*pgm_read_byte(&(KB[row][0])) + (col!=3? -3:0)),YoffSet + (14*row) + 14))
      {
        
        if(row != 4)
        {
          if(Shift)
          { 
            if(row == 0 || col > pgm_read_byte(&(ShiftKB[row][0])) + (2-pgm_read_byte(&(ShiftKB[row][2]))) ) 
              MSG[idx] = pgm_read_byte(&(ShiftKB[row][(col - pgm_read_byte(&(ShiftKB[row][0])) ) + pgm_read_byte(&(ShiftKB[row][2]))
			  ])); 
            else
              MSG[idx] = (pgm_read_byte(&(KB[row][col])) )-('a'-'A');
            Shift = false;
            _Disp->setColor(WHITE);
            _Disp->drawRect(XoffSet, YoffSet + (14*3)-1, XoffSet + 30,YoffSet + (14*4)-1);
            makeKeyboard();
          }
          else
            MSG[idx] = pgm_read_byte(&(KB[row][col]));
            
          if(capsLock)
          {
            if(row == 0 || col >= pgm_read_byte(&(KB[row][2]))+3 ) 
              MSG[idx] = pgm_read_byte(&(KB[row][col]));
            else
              MSG[idx] = pgm_read_byte(&(KB[row][col])) - ('a'-'A');
          }
        }
        else
          MSG[idx] = ' ';

        (idx+1) < BUF? idx++: idx;
		_Disp->setBackColor(0xFFFFFFFF);
        _Disp->print(MSG, 20, YoffSet - TxtoffSet);
      } 
    }
  }
  if( TextButton("Send", _FONT, SEND.x1, SEND.y1, SEND.x2, SEND.y2,  WHITE))
  {
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      Serial1.println(MSG);
	#endif
	strncpy(RET_MSG, MSG, sizeof(MSG));
    idx = 0;  
    clearMSG();
	_Disp->setBackColor(0xFFFFFFFF);
    _Disp->print(MSG, 20, YoffSet - TxtoffSet);
  }
  if( TextButton("BSP", _FONT, BSP.x1, BSP.y1, BSP.x2, BSP.y2,  WHITE))
  {
    idx < 1? 0 : idx--;
    MSG[idx] = ' ';
	//MSG[idx] = '\0';
	_Disp->setBackColor(0xFFFFFFFF);
    _Disp->print(MSG, 20, YoffSet - TxtoffSet);
  }

  if(capsLock =  LatchButton_Draw(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2, 0))
  {
    if(capsLock != lastState)
      makeCapsKeys();
    lastState = capsLock;
  }
  if(capsLock != lastState)
  {
    lastState = capsLock;
     makeKeyboard();
  }
  _Disp->setColor(color); 

  /*if(Sfaces = TouchCircle(30,200, 10))
  {
    lastSface = false;
    SmileyFaces(true); 
  }
  if(Sfaces != lastSface)
  {
    lastSface = Sfaces;
	if(Sfaces == false)
	{
	  _Disp->setColor( WHITE));
	  _Disp->print(">>",0, YoffSet-TxtoffSet);
      makeKeyboard();
	}
  }*/
  Restore_MainColor;
  return RET_MSG;
}

char* TFT_Extension::Mobile_KeyBoard(word color)
{
  _Disp->setColor(WHITE);
  
  for(byte row = 0; row < 3; row++)
  {
    for(byte col = 3; col < 16; col++)
    {
	  if(Type == 0)
	  {
        if( TouchButton(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2))
        {
	      _Disp->setColor(WHITE);
	      _Disp->fillRoundRect(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2); // shift key
		  _Disp->setColor(BLACK);
		  _Disp->setBackColor(WHITE);
          _Disp->print("^", ((CAPS.x1 + CAPS.x2)/2) - (_FONT? 8:4), ((CAPS.y1 + CAPS.y2)/2) - (_FONT? 8:4));
          Shift = true;
          //makeShiftKeys();
          _Disp->setColor(WHITE);
		  _Disp->setBackColor(BLACK);
        }
      }
	  else if(Type == 1)
	  {
	    if( TouchButton(CAPS.x1, CAPS.y1, _symbol? NUM.x2 : CAPS.x2, CAPS.y2))
        {
		  if(_symbol == 0)
		  {
		    _symbol = true;
		    makeSymbolKeys();
		  }
		  else 
		  {
		    _symbol = false;
			_Disp->setColor(BLACK);
			_Disp->fillRect(5,(YoffSet) + ((_FONT? 23:17)*3) - 2, XoffSet + (_FONT? 30:15)*6 +(_FONT? 15:28)*2 + 17,(YoffSet) + ((_FONT? 23:16)*3) +20);
			_Disp->setColor(WHITE);
			_Disp->drawRoundRect(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2); // shift key
		    makeNumberKeys();
		  }
		}
	  }
      if(Mobile_KB[row][1] == col) break;
      
	  if(Type == 0)
	  {
        if( TouchButton( XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:15)* pgm_read_byte(&(Mobile_KB[row][0])))-4, (YoffSet) + ((_FONT? 23:18)*(row+1)) - 2, XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:15)* pgm_read_byte(&(Mobile_KB[row][0]))) + (_FONT? 16:8) + 1, (YoffSet) + ((_FONT? 23:18)*(row+1)) + (_FONT? 16:12)) )
        {  
          if(Shift)
          { 
            MSG[idx] = (pgm_read_byte(&(Mobile_KB[row][col])) ); // show upper case
            Shift = false;
            _Disp->setColor(WHITE);
            make_Mobile_Keyboard();
          }
          else
            MSG[idx] = pgm_read_byte(&(Mobile_KB[row][col])) + ('a' - 'A'); // show lower case

          (idx+1) < BUF? idx++: idx;
		  _Disp->setBackColor(0xFFFFFFFF);
          _Disp->print(MSG, 20, YoffSet - TxtoffSet);
        }
	  }
	  else
	  {
	    if(_symbol == 0)
		{
	      if( TouchButton(XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_NumKeys[row][0])))-4, (YoffSet) + ((_FONT? 23:18)*(row+1)) - 2, XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_NumKeys[row][0]))) + (_FONT? 16:8) + 1, (YoffSet) + ((_FONT? 23:18)*(row+1)) + (_FONT? 16:12)) )
          {
		    MSG[idx] = pgm_read_byte(&(Mobile_NumKeys[row][col]));
		    (idx+1) < BUF? idx++: idx;
			_Disp->setBackColor(0xFFFFFFFF);
            _Disp->print(MSG, 20, YoffSet - TxtoffSet);
		  }
		}
		else 
		{
		  if( TouchButton(XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_SymKeys[row][0])))-4, (YoffSet) + ((_FONT? 23:18)*(row+1)) - 2, XoffSet + (_FONT? 30:20)*(col-3) + ((_FONT? 15:10)* pgm_read_byte(&(Mobile_SymKeys[row][0]))) + (_FONT? 16:8) + 1, (YoffSet) + ((_FONT? 23:18)*(row+1)) + (_FONT? 16:12)) )
          {
		    MSG[idx] = pgm_read_byte(&(Mobile_SymKeys[row][col]));
		    (idx+1) < BUF? idx++: idx;
			_Disp->setBackColor(0xFFFFFFFF);
            _Disp->print(MSG, 20, YoffSet - TxtoffSet);
		  }
		}
	  }
      if(TouchButton(SPACE.x1, SPACE.y1, SPACE.x2, SPACE.y2) ) // spacebar
      {
		MSG[idx] = ' ';
        (idx+1) < BUF? idx++: idx;
		_Disp->setBackColor(0xFFFFFFFF);
        _Disp->print(MSG, 20, YoffSet - TxtoffSet);	
      }		
    }
  }
  if(TouchButton( SEND.x1, SEND.y1, SEND.x2, SEND.y2))
  {
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
      Serial1.println(MSG);
	#endif
	strncpy(RET_MSG, MSG, sizeof(MSG));
    idx = 0;  
    clearMSG();
	_Disp->setBackColor(0xFFFFFFFF);
    _Disp->print(MSG, 20, YoffSet - TxtoffSet);
	
	Restore_MainColor;
	_Disp->setColor(color);
	return RET_MSG;
  }
  
  if( TouchButton(BSP.x1, BSP.y1, BSP.x2, BSP.y2))
  {
    idx < 1? 0 : idx--;
    MSG[idx] = ' ';
	//MSG[idx] = '\0';
	_Disp->setBackColor(0xFFFFFFFF);
    _Disp->print(MSG, 20, YoffSet - TxtoffSet);
  }
  
  if( TouchButton(NUM.x1, NUM.y1, NUM.x2, NUM.y2)) // numbers
  {
    if(_symbol == 0)
	{
      Type = !Type;
	  _Disp->setColor(BLACK);
      _Disp->fillRect(XoffSet - 4,(YoffSet) + (_FONT? 23:18) - 2, XoffSet + (_FONT? 30:20)*9 + 17,(YoffSet) + ((_FONT? 23:15)*2) +18);
  
      _Disp->fillRect(5,(YoffSet) + ((_FONT? 23:17)*3) - 2, XoffSet + (_FONT? 30:15)*6 +(_FONT? 15:28)*2 + 17,(YoffSet) + ((_FONT? 23:16)*3) +20);
	  if(Type)
        makeNumberKeys();
	  else 
	    make_Mobile_Keyboard();
	  _symbol = false;
	}
	else
	{
	  Type = 0;
	  _Disp->setColor(BLACK); // ok
	  _Disp->fillRect(XoffSet - 4,(YoffSet) + (_FONT? 23:18) - 2, XoffSet + (_FONT? 30:20)*9 + 17,(YoffSet) + ((_FONT? 23:15)*2) +18);
      _Disp->fillRect(5,(YoffSet) + ((_FONT? 23:17)*3) - 2, XoffSet + (_FONT? 30:15)*6 +(_FONT? 15:28)*2 + 17,(YoffSet) + ((_FONT? 23:16)*3) +20);
	  
	  _Disp->setColor(WHITE);
	  _Disp->drawRoundRect(CAPS.x1, CAPS.y1, CAPS.x2, CAPS.y2); // shift key
	  make_Mobile_Keyboard();
	  _symbol = false;
	}
  }
  Restore_MainColor;
  return NULL;
}

void TFT_Extension::ReceiveMsg(int X, int Y, word color)
{
  Save_MainColor;
  
  #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if(Serial1.available() > 0)
  {
    if(!RCV_locked)
    {
       clearRCVMSG();
       _Disp->print(ReceiveMSG,X,Y);
       RCV_locked = true;
    }
    char R_MSG = Serial1.read(); 
    if( R_MSG != '\n')
    { 
      ReceiveMSG[RCV_cnt] = R_MSG;
      RCV_cnt++;
    }
    else
    {
	  Restore_MainColor;
      _Disp->setColor(color);
      _Disp->print(ReceiveMSG,X,Y);
      RCV_cnt=0;
      RCV_locked = false;
    }	
  }
  #endif
  Restore_MainColor;
}

void TFT_Extension::clearRCVMSG()
{
  int C = BUF-1;
  while( C != 0)
  {
    C--;
    ReceiveMSG[C] = ' ';
  }
}

/*
void TFT_Extension::SmileyFaces(boolean draw)
{
  byte E = 0, M = 0;
  _Disp->setColor( BLACK));
  smiley_Face(30,200, 10, HAPPY);
  if(draw)
  {
    _Disp->fillRect(0, YoffSet - 20, _Disp->getDisplayXSize(), _Disp->getDisplayYSize());
    for(byte r = 0; r < 5; r++)
    {
      for(byte c = 0; c < 12; c++)
      {
        smiley_Face(10 + 25*c, (YoffSet-20) + 20*r, 10, E, M);
        E < 9? E++ : (E=0, M++);
        M > 7? M=0 : M;
      }
    }
    
    E = 0, M = 0;
    while(1)
    {
      for(byte r = 0; r < 5; r++)
      {
        for(byte c = 0; c < 12; c++)
        {
          if(TouchCircle(10 + 25*c, (YoffSet-20) + 20*r, 10))
          {
            sprintf(MSG, "<%d,%d,%d>", (idx+1), E, M);
			idx += 7;
            _Disp->fillRect(0, YoffSet - 30, DispX, DispY);
            draw = false;
			//break;
			return;
          }
		  E < 9? E++ : (E=0, M++);
          M > 7? M=0 : M;
        }
		//if(draw == false) break;
      }
	  //if(draw == false) break;
    }
  }
}*/

//================END_OF_DISPLAY_FUNCTIONS=======================