/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_lcd.c																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo LCD driver for EMoRo Pro2560										*/
/****************************************************************************************************/

#ifndef F_CPU
	#define F_CPU				16000000UL
#endif

#define LCD_CONTRAST			23								/* set contrast 0 - 63				*/


/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_lcd.h"
#include "emoro_i2c.h"
#include <avr\io.h>
#include <util\delay.h>

/*---------------------------------- L C D   M A C R O ---------------------------------------------*/
#define LcdBacklightOn()		(DDRG  |= (1<<PG4), PORTG |= (1<<PG4))
#define LcdBacklightOff()		(DDRG  |= (1<<PG4), PORTG &=~(1<<PG4))

#define LcdResetOn()			(DDRK |= (1<<PK5), PORTK &=~(1<<PK5))	/* reset low				*/
#define LcdResetOff()			(PORTK |= (1<<PK5))		/* set PK5-> pull lcd reset high			*/

#define LCD_ADDRESS				0x7C			/* LCD address (I2C)								*/
#define CONTROL_BYTE			0x00			/* control byte command descriptor					*/
#define DATA_BYTE				0x40			/* data byte command descriptor						*/

static unsigned char my_lcd_available;

/****************************************************************************************************/
unsigned char LcdExecuteCommand(unsigned char cmd){
	signed char test=0;
	
	DISABLE_GYRO_INT;

	test += I2cStartWait();						/* send I2C start with delay						*/
	test += I2cWriteWait(LCD_ADDRESS);			/* send LCD address byte							*/
	test += I2cWrite(CONTROL_BYTE);				/* send control byte								*/
	test += I2cWrite(cmd);						/* send command										*/
	I2cStop();									/* send I2C stop									*/
	
	RESTORE_GYRO_INT;
	
	_delay_us(30);								/* wait command execute time						*/
	if(test)									/* error occurred?									*/
		return (1);								/* return I2C error									*/
	return (0);									/* return successfully								*/
}

/****************************************************************************************************/
/* Function initialize LCD display connected to I2C. Function must be called before using another	*/
/* LCD function. Execute time for this function is 1ms-2ms.											*/
/*																									*/
/* @input value:	(none) 																			*/
/* @return value:	(int)	- (0) -> successfully, (-1) -> failed									*/
/*--------------------------------------------------------------------------------------------------*/
int LcdClass::init(void){
	signed char test=0;
	LcdResetOn();											/* pull lcd reset low					*/
	_delay_ms(5);											/* delay reset time						*/
	LcdResetOff();											/* pull lcd reset high					*/
	_delay_ms(5);											/* wait lcd reset time					*/
	
	DISABLE_GYRO_INT;

	test += LcdExecuteCommand(0x39);						/* function Set - table 1				*/
	test += LcdExecuteCommand(0x14);						/* set frame freq 183 Hz				*/
	test += LcdExecuteCommand(0x70+(LCD_CONTRAST&0x0F));	/* set contrast 0x70-0x7F				*/
	test += LcdExecuteCommand(0x5C+((LCD_CONTRAST>>4)&0x3));/* set power/ICON/Contrast				*/
	test += LcdExecuteCommand(0x6D);						/* follower control						*/
	test += LcdExecuteCommand(0x0C);					/* display ON/OFF, display on, cursor off	*/
	test += LcdExecuteCommand(0x06);						/* entry mode set, shift right			*/
	test += LcdExecuteCommand(0x80);						/* set DDRAM address = 0				*/
	test += LcdExecuteCommand(0x01);						/* clear LCD							*/
	
	RESTORE_GYRO_INT;
	
	_delay_ms(1);											/* wait clear lcd execute time			*/
	if(test)												/* initialization error occurred?		*/
		return (-1);										/* return error: initialization failed	*/
	my_lcd_available = 1;									/* lcd is available						*/
	return (0);												/* initialization successfully			*/
}

/****************************************************************************************************/
/* Function clear LCD display. Before using this function LCD must be initialized with InitLcd()	*/
/* function. Execute time for this function is 1ms-2ms.												*/
/*																									*/
/* @input value:	(none) 																			*/
/* @return value:	(int)	- (0) -> successfully, (-1) -> failed									*/
/*--------------------------------------------------------------------------------------------------*/
int LcdClass::clear(void){
	if(!my_lcd_available)							/* lcd not available							*/
		return (-1);								/* return error: communication failed			*/
	if(LcdExecuteCommand(0x01)==0){					/* clear LCD successfully?						*/
		_delay_ms(2);								/* wait command execute time					*/
		return (0);									/* return successfully							*/
	}
	return (-1);									/* return error: clear LCD failed				*/
}

/****************************************************************************************************/
/* Function prints string to LCD. Before using this function LCD must be initialized with InitLcd()	*/
/* function. Max string length is 16 data bytes + 1 end of string byte.								*/
/*																									*/
/* @input value:	(*char)		- string for lcd print												*/
/* @return value:	(int)		- (0) -> successfully, (-1) -> failed								*/
/*--------------------------------------------------------------------------------------------------*/
int LcdClass::printString(char *str){
	signed char test=0;
	if(!my_lcd_available)							/* lcd not available							*/
		return (-1);								/* return error: communication failed			*/
	
	DISABLE_GYRO_INT;
	
	test += I2cStartWait();							/* send I2C start with delay					*/
	test += I2cWriteWait(LCD_ADDRESS);				/* send lcd address with delay					*/
	test += I2cWrite(DATA_BYTE);					/* send data byte								*/
	while(*str){									/* write string									*/
		test += I2cWrite(*str);										
		str++;
	}
	I2cStop();										/* send I2C stop								*/
	
	RESTORE_GYRO_INT;
	
	if(test)										/* lcd write failed?							*/
		return (-1);								/* return error: lcd write failed				*/
	return (0);										/* successfully									*/
}

/****************************************************************************************************/
/* Function prints one char to LCD. Before using this function LCD must be initialized with 		*/
/* InitLcd() function.																				*/
/*																									*/
/* @input value:	(char)		- char for lcd print												*/
/* @return value:	(int)		- (0) -> successfully, (-1) -> failed								*/
/*--------------------------------------------------------------------------------------------------*/
int LcdClass::printChar(char data){
	signed char test=0;
	if(!my_lcd_available)							/* lcd not available							*/
		return (-1);								/* return error: communication failed			*/
	
	DISABLE_GYRO_INT;
	
	test += I2cStartWait();							/* send I2C start with delay					*/
	test += I2cWriteWait(LCD_ADDRESS);				/* send lcd address with delay					*/
	test += I2cWrite(DATA_BYTE);					/* send data byte								*/
	test += I2cWrite(data);							/* send one char								*/
	I2cStop();										/* send I2C stop								*/
	
	RESTORE_GYRO_INT;

	if(test)										/* lcd write failed?							*/
		return (-1);								/* return error: lcd write failed				*/
	return (0);										/* successfully									*/
}

/****************************************************************************************************/
/* Function sets position for LcdWrite(). Before using this function LCD must be initialized with	*/
/* InitLcd() function. Max row=1, Max column=15.													*/
/*																									*/
/* @input value:	(unsigned char) - 	r -> LCD row [0-1]											*/
/*										c -> LCD column [0-15]										*/
/* @return value:	(int)			-	(0) -> successfully, (-1) -> failed							*/
/*--------------------------------------------------------------------------------------------------*/
int LcdClass::locate(unsigned char r, unsigned char c){
	signed char test=0;
	if(!my_lcd_available)							/* lcd not available							*/
		return (-1);								/* return error: communication failed			*/
	if((r > 1) || (c > 15))		
		return (-1);											
	
	DISABLE_GYRO_INT;

	test += I2cStartWait();							/* send I2C start with delay					*/
	test += I2cWriteWait(LCD_ADDRESS);				/* send lcd address with delay					*/
	test += I2cWrite(CONTROL_BYTE);					/* send control byte							*/
	if(r)											/* row 1?										*/
		test += I2cWrite(0xC0|c);					/* 0xC0 or line number							*/
	else											/* row 2?										*/
		test += I2cWrite(0x80|c);					/* 0c80 or line number							*/
	I2cStop();										/* send I2C stop								*/
	
	RESTORE_GYRO_INT;

	if(test)										/* lcd set position failed?						*/
		return (-1);								/* return error: lcd set position failed		*/
	return (0);										/* successfully									*/
}

/****************************************************************************************************/
/* Function sets contrast of LCD display. Before using this function LCD must be initialized with	*/
/* InitLcd() function. Contrast value is between 0 and 63, lowest contrast is 0, highest is 63.		*/
/*																									*/
/* @input value:	(unsigned char) - 	con -> Contrast [0-63]										*/
/* @return value:	(int)			-	(0) -> successfully, (-1) -> failed							*/
/*--------------------------------------------------------------------------------------------------*/
int LcdClass::contrast(unsigned char con){
	signed char test=0;
	if(!my_lcd_available)								/* lcd not available						*/
		return (-1);									/* return error: communication failed		*/
	
	DISABLE_GYRO_INT;

	test += LcdExecuteCommand(0x70+(con&0x0F));			/* set contrast 0x70-0x7F					*/
	test += LcdExecuteCommand(0x5C+((con>>4)&0x3));		/* set contrast 0x5C-0x5F					*/

	RESTORE_GYRO_INT;

	_delay_ms(1);										/* wait clear lcd execute time				*/
	if(test)											/* initialization error occurred?			*/
		return (-1);									/* return error: initialization failed		*/
	my_lcd_available = 1;								/* lcd is available							*/
	return (0);											/* initialization successfully				*/

}

/****************************************************************************************************/
/* Function turn on LCD backlight.																	*/
/*																									*/
/* @input value:	(none) 																			*/
/* @return value:	(none)																			*/
/*--------------------------------------------------------------------------------------------------*/
void LcdClass::backlightOn(void){
	if(!my_lcd_available)							/* lcd not available							*/
		return;										/* return error: communication failed			*/
	LcdBacklightOn();								/* turn on lcd backlight						*/
}

/****************************************************************************************************/
/* Function turn off LCD backlight.																	*/
/*																									*/
/* @input value:	(none) 																			*/
/* @return value:	(none)																			*/
/*--------------------------------------------------------------------------------------------------*/
void LcdClass::backlightOff(void){
	if(!my_lcd_available)							/* lcd not available							*/
		return;										/* return error: communication failed			*/
	LcdBacklightOff();								/* turn off lcd backlight						*/
}

/****************************************************************************************************/
size_t LcdClass::write(uint8_t c){
	signed char test=0;
	if(!my_lcd_available)							/* lcd not available							*/
		return (-1);								/* return error: communication failed			*/
	
	DISABLE_GYRO_INT;
	
	test += I2cStartWait();							/* send I2C start with delay					*/
	test += I2cWriteWait(LCD_ADDRESS);				/* send lcd address with delay					*/
	test += I2cWrite(DATA_BYTE);					/* send data byte								*/
	test += I2cWrite(c);							/* send one char								*/
	I2cStop();										/* send I2C stop								*/
	
	RESTORE_GYRO_INT;
	
	if(test)										/* lcd write failed?							*/
		return (-1);								/* return error: lcd write failed				*/
	return (0);										/* successfully									*/
}

/************************************ L C D   C L A S S *********************************************/
LcdClass Lcd;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
