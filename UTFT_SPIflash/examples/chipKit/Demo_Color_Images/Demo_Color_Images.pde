// Demo_Color_Images 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the loadBitmap()-function.
//
// This program requires UTFT_SPIflash, UTFT v2.41 or higher, 
// as well as SPIflash.
//
// This demo expects the appropriate dataset to be loaded
// into the flash chip.
//
// Appropriate datasets:
// | Full name            | Short name  |
// |----------------------|-------------|
// | TestImages_240x320.* | 240X320.SFD |
// | TestImages_240x400.* | 240X400.SFD |
// | TestImages_320x240.* | 320X240.SFD |
// | TestImages_400x240.* | 400X240.SFD |
// | TestImages_480x272.* | 480X272.SFD |
// | TestImages_800x480.* | 800X480.SFD |
// |----------------------|-------------|
// (Demo datasets are supplied with the SPIflash library)
//
// This demo should work for all display sizes provided the
// images loaded on the flash chip aren't too large, and will
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
SPIflash      myFlash(8);

UTFT_SPIflash myFiles(&myGLCD, &myFlash);

// Declare which fonts we will be using
extern uint8_t SmallFont[];

uint8_t  fileid;
uint16_t filetype;

void setup()
{
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
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
    if (filetype == 4)
    {
      myFiles.loadBitmap(fileid, 0, 0);
      delay(3000);
      fileid++;
    }
  }
}


