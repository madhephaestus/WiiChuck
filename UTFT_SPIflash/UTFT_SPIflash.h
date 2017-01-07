/*
  UTFT_SPIflash.h - Add-on library to integrate UTFT and SPIflash
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library is an add-on to UTFT and will not work on its own. This add-on 
  library also requires the SPIflash library.

  This library adds a simple way to load images from SPI flash chip. The images 
  must be contained within the SPIflash file system. Images can be added to the 
  flash chips using the FlashUploader tool supplied with the SPIflash library.

  You can find the latest version of the library at 
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/

#ifndef UTFT_SPIflash_h
#define UTFT_SPIflash_h

#include <UTFT.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif
#include "SPIflash.h"

class UTFT_SPIflash
{
	public:
		UTFT_SPIflash(UTFT *ptrUTFT, SPIflash *ptrSPIflash);

		word loadBitmap(uint16_t fileid, int x, int y);
		word loadBitmap(uint16_t fileid, int x, int y, int ox, int oy, int sx, int sy);
		word loadMonoBitmap(uint16_t fileid, int x, int y);

protected:
		UTFT		*_UTFT;
		SPIflash	*_SPIflash;

		char		imgbuf[160];  // Optimal buffer size: 160 bytes
};

#endif