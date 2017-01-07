//
//  fonts.h
//  

#ifndef _fonts_h
#define _fonts_h

#include "Arduino.h"

// Font selection descriptors - Add an entry for each new font and number sequentially
#define TIMESNR_8	0
#define CENTURY_8	1
#define ARIAL_8		2
#define COMICS_8	3
#define GLCDFONT	4
#define TEST		5

#define FONT_START 0
#define FONT_END 1

struct FontDescriptor
{
	uint8_t	width;		// width in bits
	uint8_t	height; 	// char height in bits
	uint16_t offset;	// offset of char into char array
};

// Font references - add pair of references for each new font
extern const uint8_t timesNewRoman_8ptBitmaps[];
extern const FontDescriptor timesNewRoman_8ptDescriptors[];

extern const uint8_t centuryGothic_8ptBitmaps[];
extern const FontDescriptor centuryGothic_8ptDescriptors[];

extern const uint8_t arial_8ptBitmaps[];
extern const FontDescriptor arial_8ptDescriptors[];

extern const uint8_t comicSansMS_8ptBitmaps[];
extern const FontDescriptor comicSansMS_8ptDescriptors[];

extern const uint8_t glcdfontBitmaps[];
extern const FontDescriptor glcdfontDescriptors[];

extern const uint8_t testBitmaps[];
extern const FontDescriptor testDescriptors[];


#endif
