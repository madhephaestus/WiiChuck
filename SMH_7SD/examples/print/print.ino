/*
 * AUTHOR:    Samuel M.H. <samuel.mh@gmail.com>
 * LAST REV:  17-May-2012
 * DESCRIPTION:
 *   Example for the SMH_SSD Arduino library.
 *   You can print strings ona 7 segment display.
 *
 * LICENSE: GPL V3 <http://www.gnu.org/licenses/gpl.html>
*/

#include "SMH_7SD.h"

//Create the object that control the display
SMH_7SD display((int[]){5,4,10,11,12,6,7,9},SMH_7SD_COMMON_ANODE);

void setup(){} 

void loop(){
  display.print("Hello World");
  delay(1000);
}

