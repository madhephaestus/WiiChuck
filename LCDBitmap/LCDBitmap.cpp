// ---------------------------------------------------------------------------
// Created by Tim Eckel - teckel@leethost.com
// Copyright 2012 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// See "LCDBitmap.h" for purpose, syntax, version history, links, and more.
// ---------------------------------------------------------------------------

#include "LCDBitmap.h"

#ifndef LiquidCrystal_h // Using the New LiquidCrystal library
LCDBitmap::LCDBitmap(LCD *lcd, byte x, byte y) {
#else                   // Using the standard LiquidCrystal library
LCDBitmap::LCDBitmap(LiquidCrystal *lcd, byte x, byte y) {
#endif
  bitmap_x = x; 
  bitmap_y = y;
  _lcd = lcd;
}

void LCDBitmap::updateChar() {
  for (byte i=0; i<BITMAP_CHAR; i++) _lcd->createChar(i, chr[i]);
}

void LCDBitmap::drawChar() {
  _lcd->setCursor(bitmap_x, bitmap_y);
  for (byte i=0; i<BITMAP_CHAR; i++) {
    if (i==4) _lcd->setCursor(bitmap_x, bitmap_y+1);  
    _lcd->print(char(i));
  }
}

#ifdef BITMAP_RANGE_CHK
void LCDBitmap::rangeCheck(byte &x1, byte &y1, byte &x2, byte &y2) {
  x1 = min(x1, BITMAP_W-1);
  x2 = min(x2, BITMAP_W-1);
  y1 = min(y1, BITMAP_H-1);
  y2 = min(y2, BITMAP_H-1);
}
#endif

void LCDBitmap::begin() {
  LCDBitmap::clear();
  LCDBitmap::updateChar();
  LCDBitmap::drawChar();
}

void LCDBitmap::clear() {
  for (byte y=0; y<BITMAP_H; y++) for (byte x=0; x<BITMAP_W; x++) bitmap[x][y]=OFF;
  for (byte c=0; c<BITMAP_CHAR; c++) for (byte a=0; a<BITMAP_CHAR_H; a++) chr[c][a]=OFF;
  LCDBitmap::updateChar();
}

void LCDBitmap::clear_text() {
  _lcd->clear();
  LCDBitmap::drawChar();
}

void LCDBitmap::home() {
  _lcd->setCursor(0, 0);
}
void LCDBitmap::update() {
  byte shift_x;
  byte shift_y=0;
  for (byte c=0; c<BITMAP_CHAR; c++) {
    if (c<4) shift_x=c*BITMAP_CHAR_W;
    else shift_x=(c-4)*BITMAP_CHAR_W;
    if (c==4) shift_y=BITMAP_CHAR_H;
    for (byte a=0; a<BITMAP_CHAR_H; a++) {
      chr[c][a]=bitmap[shift_x][shift_y+a]<<4;
      chr[c][a]+=bitmap[shift_x+1][shift_y+a]<<3;
      chr[c][a]+=bitmap[shift_x+2][shift_y+a]<<2;
      chr[c][a]+=bitmap[shift_x+3][shift_y+a]<<1;
      chr[c][a]+=bitmap[shift_x+4][shift_y+a];
    }
  }
  LCDBitmap::updateChar();
}

void LCDBitmap::inverse() {
  for (byte x=0; x<BITMAP_W; x++) {
    for (byte y=0; y<BITMAP_H; y++) bitmap[x][y]=bitmap[x][y]^1;
  }
  LCDBitmap::update();
}

void LCDBitmap::move(byte x, byte y) {
  _lcd->setCursor(bitmap_x, bitmap_y);
  _lcd->print("    ");
  _lcd->setCursor(bitmap_x, bitmap_y+1);
  _lcd->print("    ");
  bitmap_x = x;
  bitmap_y = y;
  LCDBitmap::drawChar();
}

void LCDBitmap::pixel(byte x, byte y, boolean color, boolean update) {
#ifdef BITMAP_RANGE_CHK
  if (x>=0 && y>=0 && x<BITMAP_W && y<BITMAP_H) bitmap[x][y]=color;
#else
  bitmap[x][y]=color;
#endif
  if (update) LCDBitmap::update();
}

void LCDBitmap::line(byte x1, byte y1, byte x2, byte y2, boolean color, boolean update) {
#ifdef BITMAP_RANGE_CHK
  LCDBitmap::rangeCheck(x1, y1, x2, y2);
#endif
  // Vertical line (faster than diagonal line method)
  if (x1==x2) {
    if (y1 < y2) {
      for (y1=y1; y1<=y2; y1++) bitmap[x1][y1]=color;
    } else {
      for (y2=y2; y2<=y1; y2++) bitmap[x1][y2]=color;
    }
  // Horizontal line (faster than diagonal line method)
  } else if (y1==y2) {
    if (x1 < x2) {
      for (x1=x1; x1<=x2; x1++) bitmap[x1][y1]=color;
    } else {
      for (x2=x2; x2<=x1; x2++) bitmap[x2][y1]=color;
    }
  // Diagonal line
  } else {
    byte dx, dy;
    int8_t sx, sy;
    int err, e2;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    err = dx-dy;
    if (x1 < x2) sx = 1; else sx = -1;
    if (y1 < y2) sy = 1; else sy = -1;
    while (1) {
      bitmap[x1][y1]=color;
      if (x1 == x2 && y1 == y2) break;
      e2 = 2*err;
      if (e2 > -dy) { 
        err -= dy;
        x1 += sx;
      }
      if (e2 <  dx) { 
        err += dx;
        y1 += sy ;
      }
    }
  }
  if (update) LCDBitmap::update();
}

void LCDBitmap::rect(byte x1, byte y1, byte x2, byte y2, boolean color, boolean update) {
#ifdef BITMAP_RANGE_CHK
  LCDBitmap::rangeCheck(x1, y1, x2, y2);
#endif
  LCDBitmap::line(x1, y1, x2, y1, color, NO_UPDATE);
  LCDBitmap::line(x1, y2, x2, y2, color, NO_UPDATE);
  LCDBitmap::line(x1, y1, x1, y2, color, NO_UPDATE);
  LCDBitmap::line(x2, y1, x2, y2, color, NO_UPDATE);
  if (update) LCDBitmap::update();
}

void LCDBitmap::rectFill(byte x1, byte y1, byte x2, byte y2, boolean color, boolean update) {
#ifdef BITMAP_RANGE_CHK
  LCDBitmap::rangeCheck(x1, y1, x2, y2);
#endif
  byte y;
  int8_t sx, sy;
  if (x1 < x2) sx = 1; else sx = -1;
  if (y1 < y2) sy = 1; else sy = -1;
  while (1) {
    y=y1;
    while (1) {
      bitmap[x1][y]=color;
      if (y == y2) break;
    y+=sy;
    }
    if (x1 == x2) break;
    x1+=sx;
  }
  if (update) LCDBitmap::update();
}

void LCDBitmap::barGraph(byte bars, byte *graph, boolean color, boolean update) {
#ifdef BITMAP_RANGE_CHK
  for (byte x=0; x<bars; x++) graph[x] = min(graph[x], BITMAP_H);
  bars = min(bars, BITMAP_W);
#endif
  byte x1, x2, b, alt_color;
  byte bar_W = BITMAP_W/bars;
  if (color == OFF) alt_color=ON; else alt_color=OFF;
  for (b=0; b<bars; b++) {
    x1 = b*bar_W;
    x2 = (b+1)*bar_W-1;
    if (graph[b]>0) {
      LCDBitmap::rectFill(x1, BITMAP_H-1, x2, BITMAP_H-graph[b], color, NO_UPDATE);
    }
    if (graph[b]<BITMAP_H) {
      LCDBitmap::rectFill(x1, 0, x2, BITMAP_H-graph[b]-1, alt_color, NO_UPDATE);
    }
  }
  if (update) LCDBitmap::update();
}