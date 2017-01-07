/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_lcd.h																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo LCD driver for EMoRo Pro2560										*/
/****************************************************************************************************/
#ifndef EMORO_LCD_H_
#define EMORO_LCD_H_

#ifndef LCD_CONTRAST
#define LCD_CONTRAST 7
#endif

#ifndef DISABLE_GYRO_INT
#define DISABLE_GYRO_INT	unsigned char temp_int7 = EIMSK; EIMSK &= ~(1 <<INT7)
#endif

#ifndef RESTORE_GYRO_INT
#define RESTORE_GYRO_INT	EIMSK |= temp_int7 & (1 <<INT7) ? (1 <<INT7) : 0
#endif

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Print.h"

class LcdClass : public Print{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize LCD display connected to I2C.Function must be called before using another*/
	/* LCD function. Execute time for this function is 1ms-2ms.										*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- (0) -> successfully, (-1) -> failed								*/
	/*----------------------------------------------------------------------------------------------*/
	int init(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function clear LCD display. Before using this function LCD must be initialized with InitLcd()*/
	/* function. Execute time for this function is 1ms-2ms.											*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- (0) -> successfully, (-1) -> failed								*/
	/*----------------------------------------------------------------------------------------------*/
	int clear(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function prints string to LCD. Before using this function LCD must be initialized 			*/
	/* with InitLcd() function. Max string length is 16 data bytes + 1 end of string byte.			*/
	/*																								*/
	/* @input value:	(*char)		- string for lcd print											*/
	/* @return value:	(int)		- (0) -> successfully, (-1) -> failed							*/
	/*----------------------------------------------------------------------------------------------*/
	int printString(char *str);

	/*----------------------------------------------------------------------------------------------*/
	/* Function prints one char to LCD. Before using this function LCD must be initialized with 	*/
	/* InitLcd() function.																			*/
	/*																								*/
	/* @input value:	(char)	- char for lcd print												*/
	/* @return value:	(int)	- (0) -> successfully, (-1) -> failed								*/
	/*----------------------------------------------------------------------------------------------*/
	int printChar(char data);

	/*----------------------------------------------------------------------------------------------*/
	/* Function sets position for LcdWrite(). Before using this function LCD must be initialized 	*/
	/* with InitLcd() function. Max row=1, Max column=15.											*/
	/*																								*/
	/* @input value:	(unsigned char) - 	r -> LCD row [0-1]										*/
	/*										c -> LCD column [0-15]									*/
	/* @return value:	(int)			-	(0) -> successfully, (-1) -> failed						*/
	/*----------------------------------------------------------------------------------------------*/
	int locate(unsigned char r, unsigned char c);

	/************************************************************************************************/
	/* Function sets contrast of LCD display. Before using this function LCD must be initialized	*/
	/* with InitLcd() function. Contrast value is between 0 and 63, lowest contrast is 0,			*/
	/* highest is 63.																				*/
	/*																								*/
	/* @input value:	(unsigned char) - 	con -> Contrast [0-63]									*/
	/* @return value:	(int)			-	(0) -> successfully, (-1) -> failed						*/
	/*----------------------------------------------------------------------------------------------*/
	int contrast(unsigned char con);

	/*----------------------------------------------------------------------------------------------*/
	/* Function turn on LCD backlight.																*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(none)																		*/
	/*----------------------------------------------------------------------------------------------*/
	void backlightOn(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function turn off LCD backlight.																*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(none)																		*/
	/*----------------------------------------------------------------------------------------------*/
	void backlightOff(void);


	virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
};

/************************************ L C D   C L A S S *********************************************/
extern LcdClass Lcd;

#endif /* EMORO_LCD_H_																				*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
