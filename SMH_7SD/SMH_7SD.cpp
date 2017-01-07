/*
 * AUTHOR:    Samuel M.H. <samuel.mh@gmail.com>
 * LAST REV:  17-May-2012
 * DESCRIPTION:
 *   Arduino C code class for displaying characters and writing strings
 *    with a seven segment display
 *
 * LICENSE: GPL V3 <http://www.gnu.org/licenses/gpl.html>
*/

#include "SMH_7SD.h"


/*
 * CONSTRUCTOR:
 *   Initializes the display
 *
 * PARAMETERS:
 *   pins: int[8] with the pins which control the segments {a,b,c,d,e,f,g,dp}.
 *   type: type of the display to control. Please use the constants:
 *     SMH_7SD_COMMON_ANODE 
 *     SMH_7SD_COMMON_CATHODE
*/
SMH_7SD::SMH_7SD(int* pins, short type){
  this->pins = pins;
  if (type==SMH_7SD_COMMON_CATHODE){
    this->on = HIGH;
    this->off = LOW;
  } else{
    this->on = LOW;
    this->off = HIGH;
  }
  this->timeOn = 500;
  this->timeOff= 50;
  //Configure pins
  short i;
  for(i=0;i<8;i++){
    pinMode(pins[i], OUTPUT);
  }
  //Switch off all the segmens
  this->printChar(' ');
}



/*
 * TimeOn:
 *   The time in ms a character is showed
*/
void SMH_7SD::setTimeOn(unsigned int time){
 this->timeOn = time;
}

/*
 * TimeOff:
 *   The time in ms between characters. This small blink = usability.
*/
void SMH_7SD::setTimeOff(unsigned int time){
 this->timeOff = time;
}



/*
 * Prints a raw byte, display format.
 *
 * PARAMETERS:
 *   b: byte that represent what you want to display.
 *     Each bit of the byte represents a segment {a,b,c,d,e,f,g,dp}
 *
*/
void SMH_7SD::printRaw(byte b){
  int i;
  for(i=0;i<8;i++){
    if (b & B10000000){ //Mask
      digitalWrite(this->pins[i], this->on);
    } else {
      digitalWrite(this->pins[i], this->off);
    }
    b = b << 1; //Shift left
  }
}


/*
 * Prints a character. This function will translate the char with a
 *   dictionary and will use the printRaw function.
 *
 * PARAMETERS:
 *   c: the character you want to print.
 *
*/
void SMH_7SD::printChar(char c){
  byte b;
  if(48<=c && c<=57){           //NUMBER
    b= SMH_7SD_NUMBERS[c-48];
  } else if(65<=c && c<=90){    //LETTER UPPERCASE
    b= SMH_7SD_LETTERS[c-65];
  } else if(97<=c && c<=122){   //LETTER LOWERCASE
    b= SMH_7SD_LETTERS[c-97];
  } else if(c==' ') b= B00000000;
    else if(c=='.') b= B00000001;
    else if(c=='-') b= B00000010;
    else if(c=='_') b= B00010000;
    else if(c=='(' || c=='[') b= B10011100;
    else if(c==')' || c==')') b= B11110000;
    else if(c=='{') b= B01100010;
    else if(c=='}') b= B00001110;
    else if(c=='@') b= B11011110;
    else if(c=='/') b= B01001010;
    else if(c==':') b= B00010010;
  else {                      //UNKNOWN
    b = B11001011;
  }
  this->printRaw(b);
}


/*
 * Prints a string.
 *
 * PARAMETERS:
 *   string: the string you want to print.
 *
*/
void SMH_7SD::print(char* string){
  int i = 0;
  while(string[i]!='\0'){
    this->printChar(string[i]);
    delay(this->timeOn);
    this->printChar(' ');
    delay(this->timeOff);
    i++;
  }
}

