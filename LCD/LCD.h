#ifndef __LCD_H__
#define __LCD_H__

#include <LiquidCrystal.h>

class LCD : public LiquidCrystal
{
public:
	LCD(int rs, int enable, int d4, int d5, int d6, int d7) : LiquidCrystal(rs, enable, d4, d5, d6, d7) {}
	LCD(int rs, int rw, int enable, int d4, int d5, int d6, int d7) : LiquidCrystal(rs, rw, enable, d4, d5, d6, d7) {}
	LCD(int rs, int enable, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) : LiquidCrystal(rs, enable, d0, d1, d2, d3, d4, d5, d6, d7) {}
	LCD(int rs, int rw, int enable, int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7) : LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7) {}

	void printLines(const char* line1, const char* line2);
	void printLines(const char* line1, char line2);
	void printLines(const char* line1, long line2);
	void printLines(const char* line1, unsigned long line2);
	void printLines(const char* line1, float line2);
	void printLines(char line1, const char* line2);
	void printLines(char line1, char line2);
	void printLines(char line1, long line2);
	void printLines(char line1, unsigned long line2);
	void printLines(char line1, float line2);
	void printLines(long line1, const char* line2);
	void printLines(long line1, char line2);
	void printLines(long line1, long line2);
	void printLines(long line1, unsigned long line2);
	void printLines(long line1, float line2);
	void printLines(unsigned long line1, const char* line2);
	void printLines(unsigned long line1, char line2);
	void printLines(unsigned long line1, long line2);
	void printLines(unsigned long line1, unsigned long line2);
	void printLines(unsigned long line1, float line2);
	void printLines(float line1, const char* line2);
	void printLines(float line1, char line2);
	void printLines(float line1, long line2);
	void printLines(float line1, unsigned long line2);
	void printLines(float line1, float line2); 
};

#endif