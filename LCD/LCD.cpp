#include "LCD.h"
#include <Arduino.h>

void LCD::printLines(const char* line1, const char* line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(const char* line1, char line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(const char* line1, long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(const char* line1, unsigned long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(const char* line1, float line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(char line1, const char* line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(char line1, char line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(char line1, long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(char line1, unsigned long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(char line1, float line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(long line1, const char* line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(long line1, char line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(long line1, long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(long line1, unsigned long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(long line1, float line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(unsigned long line1, const char* line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(unsigned long line1, char line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(unsigned long line1, long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(unsigned long line1, unsigned long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(unsigned long line1, float line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(float line1, const char* line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(float line1, char line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(float line1, long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(float line1, unsigned long line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}

void LCD::printLines(float line1, float line2)
{
	this->clear();
	this->home();
	this->print(line1);
	this->setCursor(0, 1);
	this->print(line2);
}