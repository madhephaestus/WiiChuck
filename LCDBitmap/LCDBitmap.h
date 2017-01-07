// ---------------------------------------------------------------------------
// LCDBitmap Library - v1.6 - 03/26/2015
//
// AUTHOR/LICENSE:
// Created by Tim Eckel - tim@leethost.com
// Copyright 2015 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: https://bitbucket.org/teckel12/arduino-lcd-bitmap
// Blog/Support: http://forum.arduino.cc/index.php/topic,103956.0.html
//
// DISCLAIMER:
// This software is furnished "as is", without technical support, and with no 
// warranty, express or implied, as to its usefulness for any purpose.
//
// PURPOSE:
// Creates a tiny 20x16 pixel addressable bitmap area on a normally text-only
// LCD display. Set of functions for plotting pixels, lines, rectangles, filled
// rectangles inverting, moving and clearing a tiny 20x16 pixel bitmap.
//
// Works with standard Arduino 1.0 LiquidCrystal library as well as
// Francisco Malpartida's New LiquidCrystal library available at the following
// location: https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home
//
// I highly suggest using Francisco's New LiquidCrystal library for speed,
// features, and pin reduction with the addition of a simple shift register.
//
// CONSTRUCTOR:
//   LCDBitmap bitmap(&lcd, 12, 0) - &lcd is from LiquidCrystal, (12,0) is character position of bitmap
//
// SYNTAX:
//   bitmap.begin() - Initalize the LCD bitmap
//   bitmap.clear() - Clear the bitmap display (automatically updates bitmap display) doesn't clear text
//   bitmap.inverse() - Invert the bitmap, automatically updates the bitmap display
//   bitmap.update() - Update bitmap display
//   bitmap.clear_text() - Clear just the text on the display (leaves bitmap alone)
//   bitmap.home() - Move cursor the home position (0,0)
//   bitmap.move(x, y) - Move the LCD bitmap position to this character position
//   bitmap.pixel(x, y, color, update) - Add pixel at (x,y), color is either ON or OFF, update is either UPDATE or NO_UPDATE
//   bitmap.line(x1, y1, x2, y2, color, update) - Draw the line from (x1,y1) to (x2,y2), color & update as in pixel
//   bitmap.rect(x1, y1, x2, y2, color, update) - Draw a rectangle from (x1,y1) to (x2,y2), color & update as in pixel
//   bitmap.rectFill(x1, y1, x2, y2, color, update) - Draw a filled rectangle from (x1,y1) to (x2,y2), color & update as in pixel
//   bitmap.barGraph(bars, *graph, color, update) - Draw bar graph, bars is # of bars (1,2,4,5,10,20), *graph is array containing height values, color & update as in pixel
//
//   bitmap.lineHor - Deprecated, use bitmap.line instead, old code using this function will continue to work.
//   bitmap.lineVert - Deprecated, use bitmap.line instead, old code using this function will continue to work.
//
// HISTORY:
// 04/01/2015 - Moved repository to Bitbucket, updated other web links.
//
// 07/05/2012 v1.6 - BITMAP_RANGE_CHK bug fix.
//
// 06/28/2012 v1.5 - 4bit method now works without the New LiquidCrystal
//   library (was always supposed to work this way, oversight on my part).
//   Update switch now optional on all functions, defaults to NO_UPDATE.
//
// 05/25/2012 v1.4 - Fixed a few problems with the barGraph function. Further
//   optimization with the range checking. Compatible with Arduino 0023
//   through latest release.
//
// 05/10/2012 v1.3 - Many performance tweaks and reduced compiled code size.
//   Optimization were enough to deprecate the lineHor and lineVert commands.
//   Both functions still work, there's just no longer any advantage in using
//   them. Thanks to robtillaart for suggesting the Bresenham line algorithm.
//   Modified range checking removing redundant checks. Added new function
//   barGraph that easily creates a 1 to 20 bar graph.
//
// 05/06/2012 v1.2 - From the suggestion of Francisco Malpartida and bperrybap
//   (author of the Graphical LCD library) I removed the float variables in the
//   line function which reduced the compile size by 1820 bytes. Also did some
//   speed tweaks, added an option for range checking, and removed the "magic
//   numbers". Finally, I believe it will now work with 7 pixels high character
//   displays (change the BITMAP_CHAR_H define to 7). I don't have a display
//   like this, so if someone can test and let me know that would be great.
//
// 05/02/2012 v1.1 - With the help of Francisco Malpartida (author of the New
//   LiquidCrystal library) the library was cleaned up and now more easily
//   supports either the standard LiquidCrystal library or the New
//   LiquidCrystal library.
//
// 05/01/2012 v1.0 - My first Arduino library, so excuse the mess. The library
//   needs to be tweaked so it automatically works with the SR and I2C New LCD
//   libraries without the current slight modification required before
//   compiling. However, I lack the knowledge to do this. So, I'll leave it up
//   to someone who finds this library useful and is interested in tweaking it.
// ---------------------------------------------------------------------------

#ifndef LCDBitmap_h
#define LCDBitmap_h
 
#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

#include <LiquidCrystal.h>
#ifndef LiquidCrystal_h // Not using the stock LiquidCrystal library
	#ifndef _LCD_H_       // Also not using the New LiquidCrystal library
		#error "You must install New LiquidCrystal library to work with non-4bit projects: http://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home"
	#endif
#endif

#define ON 1   // Color ON (LCD pixel active)
#define OFF 0  // Color OFF (LCD pixel inactive)
#define UPDATE true
#define NO_UPDATE false

#define BITMAP_CHAR 8    // Custom characters (don't change)
#define BITMAP_CHAR_W 5  // Character width in pixels (don't change)
#define BITMAP_CHAR_H 8  // Character height in pixels (this is sometimes 7 instead of 8)
//#define BITMAP_RANGE_CHK  // Uncomment to do range checking, adds 100 bytes or so and a tad slower - Or, leave commented and be careful

// Deprecated commands
#define lineHor(x1, y1, x2, color, update) line(x1, y1, x2, y1, color, update)
#define lineVert(x1, y1, y2, color, update) line(x1, y1, x1, y2, color, update)

const byte BITMAP_W = BITMAP_CHAR_W * 4; // Bitmap pixels wide (4 character columns wide)
const byte BITMAP_H = BITMAP_CHAR_H * 2; // Bitmap pixels high (2 character rows high)

class LCDBitmap {
	public:
#ifndef LiquidCrystal_h // Using the New LiquidCrystal library
		LCDBitmap (LCD *lcd, byte bitmap_x, byte bitmap_y);
#else                   // Using the standard LiquidCrystal library
		LCDBitmap (LiquidCrystal *lcd, byte bitmap_x, byte bitmap_y);
#endif
		void begin();
		void clear();
		void inverse();
		void update();
		void clear_text();
		void home();
		void move(byte x, byte y);
		void pixel(byte x, byte y, boolean color, boolean update=false);
		void line(byte x1, byte y1, byte x2, byte y2, boolean color, boolean update=false);
		void rect(byte x1, byte y1, byte x2, byte y2, boolean color, boolean update=false);
		void rectFill(byte x1, byte y1, byte x2, byte y2, boolean color, boolean update=false);
		void barGraph(byte bars, byte *graph, boolean color, boolean update=false);
	private:
		void updateChar();
		void drawChar();
#ifdef BITMAP_RANGE_CHK
		void rangeCheck(byte &x1, byte &y1, byte &x2, byte &y2);
#endif
		byte bitmap_x;
		byte bitmap_y;
		boolean bitmap[BITMAP_W][BITMAP_H];
		byte chr[BITMAP_CHAR][BITMAP_CHAR_H];
#ifndef LiquidCrystal_h // Using the New LiquidCrystal library
		LCD *_lcd;
#else                   // Using the standard LiquidCrystal library
		LiquidCrystal *_lcd;
#endif
};
 
#endif