/*
  UTFT_SPIflash.cpp - Add-on library to integrate UTFT and SPIflash
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

#include "UTFT_SPIflash.h"

UTFT_SPIflash::UTFT_SPIflash(UTFT *ptrUTFT, SPIflash *ptrSPIflash)
{
	_UTFT		= ptrUTFT;
	_SPIflash	= ptrSPIflash;
}

word UTFT_SPIflash::loadBitmap(uint16_t fileid, int x, int y)
{
	uint32_t	res, imgX, imgY;
	uint16_t	bread;
	uint8_t		fhandle;
	int			cx, cy;

	res = _SPIflash->getFileType(fileid);
	if (res != 4)
		return ERR_FILETYPE_INCORRECT;

	fhandle = _SPIflash->fileOpen(fileid);
	if (fhandle!=ERR_FILE_DOES_NOT_EXIST)
	{
		imgX = _SPIflash->getImageXSize(fileid);
		imgY = _SPIflash->getImageYSize(fileid);
		if (imgX < (sizeof(imgbuf)/2))
		{
			_SPIflash->fileClose(fhandle);
			return loadBitmap(fileid, x, y, 0, 0, imgX, imgY);
		}
		else
		{
			cbi(_UTFT->P_CS, _UTFT->B_CS);
			if (_UTFT->orient==PORTRAIT)
			{
				_UTFT->setXY(x, y, x+imgX-1, y+imgY-1);
				for (uint32_t cnt=0; cnt<((imgX*imgY)*2); cnt+=sizeof(imgbuf))
				{
					bread = _SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));
					for (int bc=0; bc<bread; bc+=2)
						_UTFT->LCD_Write_DATA(imgbuf[bc],imgbuf[bc+1]);
				}

			}
			else
			{
				cx = 0;
				cy = 0;
				for (uint32_t cnt=0; cnt<((imgX*imgY)*2); cnt+=sizeof(imgbuf))
				{
					bread = _SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));
					if ((imgX-cx)>(bread/2))
					{
						_UTFT->setXY(x+cx, y+cy, x+cx+(sizeof(imgbuf)/2)-1, y+cy);
						for (int bc=0; bc<bread; bc+=2)
						{
							_UTFT->LCD_Write_DATA(imgbuf[sizeof(imgbuf)-bc-2],imgbuf[sizeof(imgbuf)-bc-1]);
						}
						cx += (sizeof(imgbuf)/2);
						if (cx>=imgX)
						{
							cx = 0;
							cy++;
						}
					}
					else
					{
						int r = imgX-cx;
						int bc = r*2;

						_UTFT->setXY(x+cx-1, y+cy, x+imgX-1, y+cy);
						for (int c=cx; c<imgX; c++)
						{
							_UTFT->LCD_Write_DATA(imgbuf[bc-2],imgbuf[bc-1]);
							bc-=2;
						}

						cy++;
						cx=(bread/2)-r;
						bc = bread;
						_UTFT->setXY(x, y+cy, x+cx-1, y+cy);
						for (int c=0; c<(bread/2)-r; c++)
						{
							_UTFT->LCD_Write_DATA(imgbuf[bc-2],imgbuf[bc-1]);
							bc-=2;
						}
					}
				}
			}
			_SPIflash->fileClose(fhandle);
			_UTFT->setXY(0,0,_UTFT->getDisplayXSize()-1,_UTFT->getDisplayYSize()-1);
			sbi(_UTFT->P_CS, _UTFT->B_CS);
			return ERR_NO_ERROR;
		}
	}
}

word UTFT_SPIflash::loadBitmap(uint16_t fileid, int x, int y, int ox, int oy, int sx, int sy)
{
	uint32_t	res, imgX, imgY, cpos;
	uint16_t	bread, bc;
	uint8_t		fhandle;
	uint16_t	cx, cy;

	res = _SPIflash->getFileType(fileid);
	if (res != 4)
		return ERR_FILETYPE_INCORRECT;

	fhandle = _SPIflash->fileOpen(fileid);
	if (fhandle!=ERR_FILE_DOES_NOT_EXIST)
	{
		imgX = _SPIflash->getImageXSize(fileid);
		imgY = _SPIflash->getImageYSize(fileid);
		cbi(_UTFT->P_CS, _UTFT->B_CS);
		for (cy=y; cy<(y+sy); cy++)
		{
			_SPIflash->fileSeek(fhandle, 0);
			cpos = (((oy+(cy-y))*imgX)+ox)*2;
			_SPIflash->fileSeek(fhandle, cpos);
			bread = _SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));
			bc = 0;
			for (cx = 0; cx<sx; cx++)
			{
				_UTFT->setXY(x+cx, cy, x+cx, cy);
				_UTFT->LCD_Write_DATA(imgbuf[bc],imgbuf[bc+1]);
				bc+=2;
				if (bc == sizeof(imgbuf))
				{
					bread = _SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));
					bc = 0;
				}
			}
		}
		_SPIflash->fileClose(fhandle);
		_UTFT->setXY(0,0,_UTFT->getDisplayXSize()-1,_UTFT->getDisplayYSize()-1);
		sbi(_UTFT->P_CS, _UTFT->B_CS);
		return ERR_NO_ERROR;
	}
}

word UTFT_SPIflash::loadMonoBitmap(uint16_t fileid, int x, int y)
{
	uint16_t	res, imgX, imgY;
	uint8_t		fhandle;
	int			cx, cy;
	uint8_t		bitcount = 7;
	uint16_t	bufcount = 0;

	res = _SPIflash->getFileType(fileid);
	if (res != 5)
		return ERR_FILETYPE_INCORRECT;

	fhandle = _SPIflash->fileOpen(fileid);
	if (fhandle!=ERR_FILE_DOES_NOT_EXIST)
	{
		imgX = _SPIflash->getImageXSize(fileid);
		imgY = _SPIflash->getImageYSize(fileid);
		cbi(_UTFT->P_CS, _UTFT->B_CS);

		_SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));
		for (uint16_t ycnt=0; ycnt<imgY; ycnt++)
			for (uint16_t xcnt=0; xcnt<imgX; xcnt++)
			{
				_UTFT->setXY(x+xcnt, y+ycnt, x+xcnt+1, y+ycnt+1);
				if (imgbuf[bufcount] & (1<<bitcount))
					_UTFT->LCD_Write_DATA(_UTFT->fch,_UTFT->fcl);
				else
					if (!_UTFT->_transparent)
						_UTFT->LCD_Write_DATA(_UTFT->bch,_UTFT->bcl);
				if (bitcount == 0)
				{
					bitcount = 7;
					bufcount++;
					if (bufcount == sizeof(imgbuf))
					{
						bufcount = 0;
						_SPIflash->fileRead(fhandle, imgbuf, sizeof(imgbuf));
					}
				}
				else
					bitcount--;
			}
		_SPIflash->fileClose(fhandle);
		_UTFT->setXY(0,0,_UTFT->getDisplayXSize()-1,_UTFT->getDisplayYSize()-1);
		sbi(_UTFT->P_CS, _UTFT->B_CS);
		return ERR_NO_ERROR;
	}
}
