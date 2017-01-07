// Demo_Mono_Images 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the loadMonoBitmap()-function.
//
// This program requires UTFT_SPIflash, UTFT v2.41 or higher, 
// as well as SPIflash.
//
// This demo expects the "TestImages_Mono_For_Colordisplays"
// (MONO_C.SFD) dataset to be loaded into the flash chip.
// (Demo datasets are supplied with the SPIflash library)
//
// This demo should work for all display sizes, and will
// also work in PORTRAIT mode.
//

#include <SPIflash.h>
#include <UTFT.h>
#include <UTFT_SPIflash.h>

// Remember to set the model code for your display and the appropriate pins for your setup.
// See the UTFT manual for further information.
// myGLCD(<Model Code>, <RS>, <WR>, <CS>, <RST>);
UTFT          myGLCD(ITDB32S, 38, 39, 40, 41);

// myFlash(<SS>); (if using the hardware SPI pins. See the manual for other configurations.)
SPIflash      myFlash(45);

UTFT_SPIflash myFiles(&myGLCD, &myFlash);

// Declare which fonts we will be using
extern uint8_t SmallFont[];

uint8_t  fileid;
uint16_t filetype;
uint16_t display_x, display_y;

void setup()
{
  randomSeed(analogRead(0));
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setColor(VGA_LIME);
  myGLCD.setBackColor(VGA_TRANSPARENT);
  myGLCD.setFont(SmallFont);
  display_x = myGLCD.getDisplayXSize();
  display_y = myGLCD.getDisplayYSize();
  myFlash.begin();
  
  if (myFlash.ID_device==0)
  {
    myGLCD.print("Unknown flash device!", 0, 0);
    while (true) {};
  }    
}

void loop()
{
  fileid = 0;
  filetype = 0;
  
  while (filetype != ERR_FILE_DOES_NOT_EXIST)
  {
    filetype = myFlash.getFileType(fileid);
    if (filetype == 5)
    {
      myGLCD.setColor(random(256), random(256), random(256));
      myFiles.loadMonoBitmap(fileid, random(display_x-myFlash.getImageXSize(fileid)), random(display_y-myFlash.getImageYSize(fileid)));
      fileid++;
    }
  }
}

