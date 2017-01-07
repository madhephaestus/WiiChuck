/*
  Library created by Andrew Mascolo : 2/18/2014
  VERSION 1.3
  This library is an extension to Henning Karlsen's UTFT, UTouch, ITDB02_Graph16 and ITDB02_Touch.
  
  Function ideas from others:
    rounded_Square :: robtillaart
	drawMoon       :: robtillaart
	smiley_Face	   :: robtillaart
	HourGlass      :: robtillaart
	SpeechBubble   :: robtillaart
	drawGauge      :: cyclegadget 
    HorBarGraph    :: gromgsxr 
	VertBarGraph   :: gromgsxr 
*/

#ifndef TFT_Extension_h
#define TFT_Extension_h
#include <Arduino.h>

#include <UTFT.h>
#include <UTouch.h>

#include <math.h>

//=====================COLOR_PALLET==========================
#define BLACK   0x0
#define LIGHT_RED	0xFD14
#define RED     0xF800
#define DARK_RED    0x6000
#define LIGHT_ORANGE 0xFF52
#define ORANGE  0xFD00
#define DARK_ORANGE 0xFA80
#define LIGHT_YELLOW 0xFFF4
#define YELLOW  0xD7E0
#define DARK_YELLOW 0xCE40
#define LIGHT_GREEN 0xB7F6
#define GREEN   0x07E0
#define DARK_GREEN 0x0320
#define LIGHT_BLUE 0xA51F
#define BLUE    0x001F
#define DARK_BLUE 0x000C
#define PURPLE  0xF81F
#define CYAN    0x07FF
#define GREY    0x8410
#define WHITE   0xFFFF
//==================END_OF_COLOR_PALLET======================

//=====================TOUCH_DEFINES=========================
#define PORTRAIT	0
#define LANDSCAPE	1
#define Tron 2
#define Big 1
#define Small 0
#define FILL 1
#define NOFILL 0
#define ROUNDED 1
#define NOTROUNDED 0
#define PRESSED 1
#define RELEASED 0

#define swap(type, A, B) {type T = A; A = B; B = T;}
#define deg_to_rad	0.01745 + 3.1415 // 0.01745 = degrees to radians
#define Button_Groups 5
#define Num_Of_Buttons 10
//=================END_OF_TOUCH_DEFINES======================

//===================DISPLAY_DEFINES=========================
#define up 0
#define down 180
#define left 90
#define right 270
//==================END_OF_DISPLAY_DEFINES===================

//========================MOON_PHASES========================
#define WANING_GIBBOUS 7
#define THIRD_QUARTER 6
#define WANING_CRESCENT 5 
#define NEW 4
#define WAXING_CRESCENT 3
#define FIRST_QUARTER 2
#define WAXING_GIBBOUS 1
#define FULL 0 
//===========================================================

//=======================SMILEY_FACES========================
#define EVIL_PLAN 5,0
#define HAPPY 0,1
#define JOY 2,1
#define CONTENT 3,1
#define EVIL_PLAN2 5,1
#define DISAPOINTMENT 3,2
#define HATE 5,2
#define MONDAYS 9,2
#define OVER_JOY 0,3
#define OVER_JOY2 2,3
#define XD 8,3
#define EVIL_PLAN3 5,3
#define YAY 6,3
#define NO_THANKS  2,4
#define SAD 3,4
#define WHOA 7,4
#define DULL 9,4
#define GRUMPY_CAT 7,5
#define ASHAMED 3,5
#define HATE 5,5
#define NO 9,5
#define YEA 1,6
#define EXCITED 7,6
#define GAHH 1,7
#define GET_OUT 5,7
#define THE_HORROR 7,7
#define CRY_BABY 8,7
//========================================================

//=======================KEYBOARD=========================
#define BUF 27
//========================================================

//====================Slider/BarGraph=====================
#define LTR true
#define RTL false
//========================================================

class TFT_Extension
{
	public:
				TFT_Extension(UTFT *Disp, UTouch *Touch);
		void	ExtSetup();
		//========================TOUCH========================
		bool    TouchButton(int x1, int y1, int x2, int y2);
		bool    TouchButton_Draw(int x1, int y1, int x2, int y2, uint8_t buttonNumber);
		bool 	TextButton(char *str, byte font_size, int x1, int y1, int x2, int y2, word color);
		bool    LatchButton(int x1, int y1, int x2, int y2, uint8_t buttonNumber);
		bool    LatchButton_Draw(int x1, int y1, int x2, int y2, uint8_t buttonNumber);
		bool    TouchCircle(int cx, int cy, int radius);
        bool    TouchCircle_Draw(int cx, int cy, int radius, uint8_t circleNumber);
		bool    LatchCircle(int cx, int cy, int radius, uint8_t circleNumber);
		bool    LatchCircle_Draw(int cx, int cy, int radius, uint8_t circleNumber);
        bool    TouchTriangle(int x1, int y1, int base, int deg);
		bool	TouchTriangle_Draw(int x1, int y1, int base, int deg, uint8_t triangleNumber);
		bool	LatchTriangle(int x1, int y1, int base, int deg, uint8_t triangleNumber);
		bool	LatchTriangle_Draw(int x1, int y1, int base, int deg, uint8_t triangleNumber);
		bool    TouchDelayButton(int x1, int y1, int x2, int y2, unsigned long time);
		bool	TouchDelayCircle(int x1, int y1, int base, unsigned long time);
		bool	TouchDelayTriangle(int x1, int y1, int base, int deg, unsigned long time);
		int  	TotalRadioButtons(uint8_t *num, uint8_t group);
		bool 	RadioButton(int x1, int y1, int x2, int y2, uint8_t buttonNumber, uint8_t group);
		int  	TotalRadioCircleButtons(uint8_t *num, uint8_t group);
		bool 	RadioCircleButton(int cx, int cy, int radius, uint8_t buttonNumber, uint8_t group);
		void 	ResetRadioCircleButton(uint8_t group);
		void 	ResetRadioButton(uint8_t group);
		uint8_t	TouchArc(int cx, int cy, int radius, int thickness, int start, int stop);
		uint8_t	HorSlider(int x1, int y1, int x2, int y2, byte count, word color, bool direction = true);
		uint8_t	VertSlider(int x1, int y1, int x2, int y2, byte count, word color);
		void	HorBarGraph(int x1, int y1, int x2, int y2, int value, int low, int high, byte _ID, word color, bool direction = true);
		void	VertBarGraph(int x1, int y1, int x2, int y2, int value, int low, int high, byte _ID, word color);
		void	ResetV_barGraph(byte _ID);
		void	ResetH_barGraph(byte _ID);
		uint8_t	TouchCircleByQuad(int cx, int cy, int radius, uint8_t Q1 = 8, uint8_t Q2 = 8, uint8_t Q3 = 8);
		uint8_t	Swipe(int dir = 0, uint8_t lenght = 50);
		uint8_t SwipeFromArea(int X1, int Y1, int X2, int Y2, int dir = 0, uint8_t lenght = 50);
		//====================END_OF_TOUCH=====================
		
		//======================DISPLAY========================
		void	DisplaySize();
		void	SetTouchButtonColors(uint8_t ButtonNumber, word color1,word color2, bool fill, bool rounded);
		void	SetTouchCircleColors(uint8_t ButtonNumber, word color1,word color2, bool fill);
		void	SetTouchTriangleColors(uint8_t ButtonNumber, word color1,word color2, bool fill);
		void	SetLatchButtonColors(uint8_t ButtonNumber, word color1,word color2, bool fill, bool rounded);
		void	SetLatchCircleColors(uint8_t ButtonNumber, word color1,word color2, bool fill);
		void	SetLatchTriangleColors(uint8_t ButtonNumber, word color1,word color2, bool fill);
		void	SetTouchButtonText(uint8_t ButtonNumber, char* txt, bool size, word color);
		void	SetLatchButtonText(uint8_t ButtonNumber, char* txt, bool size, word color);
		void	SetTouchCircleText(uint8_t CircleNumber, char* txt, bool size, word color);
		void	SetLatchCircleText(uint8_t CircleNumber, char* txt, bool size, word color);
		void	SetTouchTriangleText(uint8_t TriangleNumber, char* txt, bool size, word color);
		void	SetLatchTriangleText(uint8_t TriangleNumber, char* txt, bool size, word color);
		void	ResetTouchButton(byte ID);
		void	ResetTouchCircle(byte ID);
		void	ResetTouchTriangle(byte ID);
		void	ResetLatchButton(byte ID);
		void	ResetLatchButtonState(byte ID, byte State);
		void	ResetAllLatchButtonState(byte State);
		void	ResetLatchCircle(byte ID);
		void	ResetLatchCircleState(byte ID, byte State);
		void	ResetAllLatchCircleState(byte State);
		void	ResetLatchTriangle(byte ID);
		void	ResetLatchTriangleState(byte ID, byte State);
		void	ResetAllLatchTriangleState(byte State);
		void	ResetAllTouchButton();
		void	ResetAllTouchCircle();
		void	ResetAllTouchTriangle();
		void	ResetAllLatchButton();
		void	ResetAllLatchCircle();
		void	ResetAllLatchTriangle();
		void	RB_Text(char * text, bool size, uint8_t ButtonNumber, uint8_t Group, word color);
		void	RCB_Text(char * text, bool size, uint8_t ButtonNumber, uint8_t Group, word color);
		void    RB_Outer_Color(uint8_t ButtonNumber, uint8_t Group, word color);
		void    RCB_Outer_Color(uint8_t ButtonNumber, uint8_t Group, word color);
		void 	RB_Toggled_Color(uint8_t ButtonNumber, uint8_t Group, word color);
		void	RCB_Toggled_Color(uint8_t ButtonNumber, uint8_t Group, word color);
		void	RB_Untoggled_Color(uint8_t ButtonNumber, uint8_t Group, word color);
		void	RCB_Untoggled_Color(uint8_t ButtonNumber, uint8_t Group, word color);
		void	RB_Outer_Color_By_Group(uint8_t Group, word color);
		void	RCB_Outer_Color_By_Group(uint8_t Group, word color);
		void	RB_Toggled_Color_By_Group(uint8_t Group, word color);
		void	RCB_Toggled_Color_By_Group(uint8_t Group, word color);
		void	RB_Untoggled_Color_By_Group(uint8_t Group, word color);
		void	RCB_Untoggled_Color_By_Group(uint8_t Group, word color);
		void	SetAll_RB_Outer_Color( word color);
		void	SetAll_RCB_Outer_Color( word color);
		void	SetAll_RB_Toggled_Color( word color);
		void	SetAll_RCB_Toggled_Color( word color);
		void	SetAll_RB_Untoggled_Color( word color);
		void	SetAll_RCB_Untoggled_Color( word color);
		void	Triangle(int x1, int y1,int x2,int y2,int x3,int y3);
        void 	drawTriangle(int x1,int y1,int base, int deg);
		void	fillTriangle(int x1,int y1,int base, int deg);
		void	Polygon(int cx, int cy, int sides, int diameter, word color, bool fill, float deg = 0);
		void	fillPoly(int x1, int y1, int x2, int y2, int x3, int y3);
		void	drawStar(int cx, int cy, int diameter, word color, bool fill, float factor = 2.0);
		void	drawArc(int cx, int cy, int radius, int thickness,int start, int stop, word color, bool lines = false);
		void	drawOvalArc(int cx, int cy, int Xradius, int Yradius, int Xthickness, int Ythickness, int start, int stop, word color);
		void	drawMoon(int cx, int cy, int radius, uint8_t phase);
		void	rounded_Square(int cx,int cy,int h, int w, float radius, word color, bool fill);
		void	smiley_Face(int cx, int cy, int radius, byte eyes, byte mouth);
		void	HourGlass(int cx, int cy, int height, int time);
		void	SpeechBubble(char * str, int cx, int cy, float radius, int deg);
		void	drawGauge(byte _ID, int cur, int pos_x, int pos_y, int start, int stop, int rad);
		void	ResetGauge(byte _ID);
		void	SetupStandardKB();
		void	SetupMobileKB();
		char* 	Standard_KeyBoard(word color);
		char*	Mobile_KeyBoard(word color);
		void	ReceiveMsg(int X, int Y, word color);
		word    ConvertRGB(byte R, byte G, byte B);
		/*void	SmileyFaces(boolean draw);*/
		//==================END_OF_DISPLAY=====================	
	
	protected:
	    UTFT	 *_Disp;
		UTouch	 *_Touch;		
		
	private:
		float   Area(int Ax, int Ay, int Bx, int By, int Cx, int Cy);
        double 	Cx,Cx1,Cx2;
        double 	Cy,Cy1,Cy2;
		int		touchX, touchY;
		bool    _Orient;
		int		DispX, DispY;
		uint8_t TotalRB_Group;
		uint8_t TotalRCB_Group;	
		uint8_t percent;
		bool 	locked;
		uint8_t flip;
		int 	rad;
		unsigned long T;
		unsigned long B_current_time, C_current_time, T_current_time;
		bool 	B_Touched, B_timeout;
		bool 	C_Touched, C_timeout;
		bool 	T_Touched, T_timeout;
		uint8_t Butlocked[Button_Groups], Cirlocked[Button_Groups];
		bool	ButLOCK, CirLOCK;
		int 	B_lastx1, B_lasty1, B_lastx2, B_lasty2;
		int 	C_lastcx, C_lastcy;
		byte 	XoffSet, YoffSet,TxtoffSet;
		int		idx;
        char    MSG[BUF];
		char    RET_MSG[BUF];
        bool    Shift, capsLock, lastState, Sfaces, lastSface;
		void	makeKeyboard();
		void	make_Mobile_Keyboard();
		void	makeNumberKeys();
		void	makeSymbolKeys();
		void	makeShiftKeys();
		void	makeCapsKeys();
		void	clearMSG();
		void	clearRCVMSG();
		byte 	RCV_cnt;
		bool	RCV_locked;
		char 	ReceiveMSG[BUF];
		byte 	_FONT;
		bool	_TYPE;
		/*byte	Face[3];
		bool	gotFace;*/
		bool    gotFirst;
		int 	FTouchX, FTouchY, STouchX, STouchY;	
		bool	Type, _symbol;
		word	FrontColor, BackColor;
};
#endif