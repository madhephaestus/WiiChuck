/*
  UTFT_Geometry.h - Add-on Library for UTFT: Geometric functions
  Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
  
  This library adds some geometric functions to UTFT which are not
  included in the main library.

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

#ifndef UTFT_Geometry_h
#define UTFT_Geometry_h

#define UTFT_GEOMETRY_VERSION	101

#include <UTFT.h>
#if ((!defined(UTFT_VERSION)) || (UTFT_VERSION<241))
	#error : You will need UTFT v2.41 or higher to use this add-on library...
#endif

class UTFT_Geometry
{
	public:
		UTFT_Geometry(UTFT *ptrUTFT);

		void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness = 1);
		void	drawPie(int x, int y, int r, int startAngle, int endAngle);

	protected:
		UTFT *_UTFT;
};

#endif
