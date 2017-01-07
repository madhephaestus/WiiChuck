// Demo_Partial_Images 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// This program is a demo of the loadBitmap()-function when
// loading and displaying parts of stored images.
//
// This program requires UTFT_SPIflash, UTFT v2.41 or higher, 
// as well as SPIflash.
//
// This demo expects the appropriate dataset to be loaded
// into the flash chip.
//
// Appropriate datasets:
// | Full name      | Short name   |
// |----------------|--------------|
// | Earth_Map.*    | EARTH.SFD    | Best suited for displays up to 320x240 pixels
// | Earth_Map_HR.* | EARTH_HR.SFD | Best suited for displays over 320x240 pixels
// |----------------|--------------|
// (Demo datasets are supplied with the SPIflash library)
//
// This demo should work for all display sizes provided and
// will also work in PORTRAIT mode.
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
uint16_t dx, dy, ix, iy, ox, oy;
char     buf[20];

void setup()
{
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  dx = myGLCD.getDisplayXSize();
  dy = myGLCD.getDisplayYSize();
  myFlash.begin();

  if (myFlash.ID_device==0)
  {
    myGLCD.print("Unknown flash device!", 0, 0);
    while (true) {};
  }
  randomSeed(analogRead(0));
}

void loop()
{
  fileid = 0;
  filetype = 0;

  while (filetype != ERR_FILE_DOES_NOT_EXIST)
  {
    filetype = myFlash.getFileType(fileid);
    ix = myFlash.getImageXSize(fileid);
    iy = myFlash.getImageYSize(fileid);
    myFlash.readFileNote(fileid, buf);
    if (filetype == 4)
    {
      ox = random(ix-dx);
      oy = random(iy-dy);
      myFiles.loadBitmap(fileid, 0, 0, ox, oy, dx, dy);
      myGLCD.print("Offset:", 0, 0);
      myGLCD.print("X:", 0, myGLCD.getFontYsize());
      myGLCD.print("Y:", 0, myGLCD.getFontYsize()*2);
      myGLCD.printNumI(ox, myGLCD.getFontXsize()*3, myGLCD.getFontYsize());
      myGLCD.printNumI(oy, myGLCD.getFontXsize()*3, myGLCD.getFontYsize()*2);
      myGLCD.print(buf, CENTER, dy-myGLCD.getFontYsize());
      delay(3000);
      fileid++;
    }
  }
}


