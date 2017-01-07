/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_ultrasonic.h														*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Ultrasonic driver for EMoRo 2560									*/
/****************************************************************************************************/
#include <avr/pgmspace.h>

#ifndef EMORO_INTERRUPTS_H_
#define EMORO_INTERRUPTS_H_

#define EXTERNAL_NUM_PCINTERRUPTS 8

typedef void (*voidFuncPtr)(void);

void attachPCInterrupt(uint8_t interruptNum, void (*userFunc)(void));
void detachPCInterrupt(uint8_t interruptNum);

#endif /* EMORO_INTERRUPTS_H_																		*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
