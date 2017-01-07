/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_switch.c															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Switch driver for EMoRo Pro2560									*/
/****************************************************************************************************/

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include <avr\io.h>

/*-------------------------------------- M A C R O -------------------------------------------------*/
#define InitSw1()			(DDRG &= ~(1<<PG0), PORTG |= (1<<PG0));	/* input, enable pull up		*/
#define InitSw2()			(DDRG &= ~(1<<PG1), PORTG |= (1<<PG1));	/* input, enable pull up		*/
#define InitSw3()			(DDRG &= ~(1<<PG2), PORTG |= (1<<PG2));	/* input, enable pull up		*/
#define InitSw4()			(DDRG &= ~(1<<PG3), PORTG |= (1<<PG3));	/* input, enable pull up		*/

/****************************************************************************************************/
/* Function reads switches SW_1 - SW_4 on Extend Module - EMoRo AMG. Use SW_1 - SW_4 macro as sw 	*/
/* attribute. Before using ReadSwitch() function, switches must be initialized with InitSwitch().	*/
/* Macro SW_1 - SW_4 are defined in switch.h file.													*/
/*																									*/
/* @input value:	(sw)			- switch for read SW_1 - SW4												*/
/* @return value:	(unsigned char)	- state of requested switch (0 - 1)								*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char ReadSwitch(unsigned char sw){
	return ((PING>>(sw-1))&0x01);				/* return state of switch SW1 - SW4					*/
}

/****************************************************************************************************/
/* Function initialize switches SW_1 - SW_4 on Extend Module - EMoRo AMG 							*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(none)																			*/
/*--------------------------------------------------------------------------------------------------*/
void InitSwitch(void){
	InitSw1(); InitSw2(); InitSw3(); InitSw4();	/* initialize 4 switch (SW1-SW4)					*/
}

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
