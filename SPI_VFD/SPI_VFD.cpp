#include "SPI_VFD.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    N = 1; 2-line display 
//    BR1=BR0=0; (100% brightness)
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// SPI_VFD constructor is called).

SPI_VFD::SPI_VFD(uint8_t data, uint8_t clock, uint8_t strobe, uint8_t brightness)
{
  _clock = clock;
  _data = data;
  _strobe = strobe;
  
  pinMode(_clock, OUTPUT);
  pinMode(_data, OUTPUT);
  pinMode(_strobe, OUTPUT);

  // normal state of these pins should be high. We must bring them high or the first
  // command will not be captured by the display module.
  digitalWrite(_strobe, HIGH);
  digitalWrite(_clock, HIGH);

  begin(20, 2, brightness);  //default to 2x20 display (SAMSUNG 20T202DA2JA)
}

void SPI_VFD::begin(uint8_t cols, uint8_t lines, uint8_t brightness) {
  // set number of lines
  if (lines > 1)
    _displayfunction = VFD_2LINE;
  else
    _displayfunction = VFD_1LINE;

  if (brightness>VFD_BRIGHTNESS25)	//catch bad values
    brightness = VFD_BRIGHTNESS100;

  // set the brightness and push the linecount with VFD_SETFUNCTION
  setBrightness(brightness);
  
  _numlines = lines;
  _currline = 0;
  
  // Initialize to default text direction (for romance languages#include "SPI_VFD.h"
  _displaymode = VFD_ENTRYLEFT | VFD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(VFD_ENTRYMODESET | _displaymode);

  command(VFD_SETDDRAMADDR);  // go to address 0

  // turn the display on with no cursor or blinking default
  _displaycontrol = VFD_DISPLAYON; 
  display();

  clear();
  home();
}
  
/********** high level commands, for the user! */
void SPI_VFD::setBrightness(uint8_t brightness){
  // set the brightness (only if a valid value is passed
  if (brightness <= VFD_BRIGHTNESS25) {
    _displayfunction &= ~VFD_BRIGHTNESS25;
    _displayfunction |= brightness;

	command(VFD_FUNCTIONSET | _displayfunction);
  }
}

uint8_t SPI_VFD::getBrightness(){
  // get the brightness
  return _displayfunction & VFD_BRIGHTNESS25;
}

void SPI_VFD::clear()
{
  command(VFD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void SPI_VFD::home()
{
  command(VFD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void SPI_VFD::setCursor(uint8_t col, uint8_t row)
{
  int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
  if ( row > _numlines ) {
    row = _numlines-1;    // we count rows starting w/0
  }
  
  command(VFD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void SPI_VFD::noDisplay() {
  _displaycontrol &= ~VFD_DISPLAYON;
  command(VFD_DISPLAYCONTROL | _displaycontrol);
}
void SPI_VFD::display() {
  _displaycontrol |= VFD_DISPLAYON;
  command(VFD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void SPI_VFD::noCursor() {
  _displaycontrol &= ~VFD_CURSORON;
  command(VFD_DISPLAYCONTROL | _displaycontrol);
}
void SPI_VFD::cursor() {
  _displaycontrol |= VFD_CURSORON;
  command(VFD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void SPI_VFD::noBlink() {
  _displaycontrol &= ~VFD_BLINKON;
  command(VFD_DISPLAYCONTROL | _displaycontrol);
}
void SPI_VFD::blink() {
  _displaycontrol |= VFD_BLINKON;
  command(VFD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void SPI_VFD::scrollDisplayLeft(void) {
  command(VFD_CURSORSHIFT | VFD_DISPLAYMOVE | VFD_MOVELEFT);
}
void SPI_VFD::scrollDisplayRight(void) {
  command(VFD_CURSORSHIFT | VFD_DISPLAYMOVE | VFD_MOVERIGHT);
}

// This is for text that flows Left to Right
void SPI_VFD::leftToRight(void) {
  _displaymode |= VFD_ENTRYLEFT;
  command(VFD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void SPI_VFD::rightToLeft(void) {
  _displaymode &= ~VFD_ENTRYLEFT;
  command(VFD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void SPI_VFD::autoscroll(void) {
  _displaymode |= VFD_ENTRYSHIFTINCREMENT;
  command(VFD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void SPI_VFD::noAutoscroll(void) {
  _displaymode &= ~VFD_ENTRYSHIFTINCREMENT;
  command(VFD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void SPI_VFD::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(VFD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds, init */

void SPI_VFD::command(uint8_t value) {
  digitalWrite(_strobe, LOW);
  send(VFD_SPICOMMAND);
  send(value);
  digitalWrite(_strobe, HIGH);

  /*
  Serial.print(VFD_SPICOMMAND, HEX);
  Serial.print('\t');
  Serial.println(value, HEX);
  */
}

#if ARDUINO >= 100
size_t SPI_VFD::write(uint8_t value) {
#else
void SPI_VFD::write(uint8_t value) {
#endif
  digitalWrite(_strobe, LOW);
  send(VFD_SPIDATA);
  send(value);
  digitalWrite(_strobe, HIGH);

  /*
  Serial.print(VFD_SPIDATA, HEX);
  Serial.print('\t');
  Serial.println(value, HEX);
  */
#if ARDUINO >= 100
  return 1;
#endif
}

/************ low level data pushing commands **********/

// write spi data
inline void SPI_VFD::send(uint8_t c) {
  int8_t i;

  digitalWrite(_clock, HIGH);

  for (i=7; i>=0; i--) {
    digitalWrite(_clock, LOW);
    
    if (c & _BV(i)) {
      digitalWrite(_data, HIGH);
    } else {
      digitalWrite(_data, LOW);
    }
    
    digitalWrite(_clock, HIGH);
  }
}
