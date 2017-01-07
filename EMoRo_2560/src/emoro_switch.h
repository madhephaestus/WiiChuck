/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_switch.h															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Switch driver for EMoRo Pro2560									*/
/****************************************************************************************************/
#ifndef EMORO_SWITCH_H_
#define EMORO_SWITCH_H_

/*--------------------------------------------------------------------------------------------------*/
/* Function reads switches SW_1 - SW_4 on Extend Module - EMoRo AMG. Use SW_1 - SW_4 macro as sw 	*/
/* attribute. Before using ReadSwitch() function, switches must be initialized with InitSwitch().	*/
/* Switch SW_1 - SW_4 MACRO:																		*/
#define SW_1			(1)				
#define SW_2			(2)
#define SW_3			(3)
#define SW_4			(4)
/*																									*/
/* @input value:	(sw)			- switch for read SW_1 - SW4									*/
/* @return value:	(unsigned char)	- state of requested switch (0 - 1)								*/
unsigned char ReadSwitch(unsigned char sw);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function initialize switches SW_1 - SW_4 on Extend Module - EMoRo AMG 							*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(none)																			*/
void InitSwitch(void);
/*--------------------------------------------------------------------------------------------------*/

#endif /* EMORO_SWITCH_H_																			*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
