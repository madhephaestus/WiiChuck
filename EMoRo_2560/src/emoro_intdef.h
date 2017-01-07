/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_interrupts.cpp														*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Ultrasonic driver for EMoRo 2560									*/
/****************************************************************************************************/

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#ifndef EMORO_INTDEEF_H_
#define EMORO_INTEDEF_H_

#define EXTERNAL_NUM_PCINTERRUPTS 8

#define HC_TRANSMIT_DDR				(DDRH)
#define HC_TRANSMIT_PORT			(PORTH)
#define HC_TRANSMIT_PIN				(PINH)
#define HC_ECHO_DDR					(DDRJ)
#define HC_ECHO_PORT				(PORTJ)
#define HC_ECHO_PIN					(PINJ)

#define ENG_SET_T_HIGH				(0)
#define ENG_SET_T_LOW				(1)
#define ENG_WAIT_ECHO				(2)
#define ENG_WAIT_ECHO_DELAY			(3)

typedef void (*voidFuncPtr)(void);

extern volatile voidFuncPtr PCintFunc[EXTERNAL_NUM_PCINTERRUPTS];
extern uint8_t  PCintLastState[EXTERNAL_NUM_PCINTERRUPTS];

typedef struct{
	unsigned char state : 1;				/* state for ultrasonic, 0-disabled, 1-enabled			*/
	unsigned int distance;
}ULTRASONIC;

typedef struct{
	unsigned char state;
	unsigned char us_pointer;
	unsigned int t_start;
	unsigned int t_end;
}ENG;

extern ULTRASONIC ultrasonic[8];
extern ENG eng;

#endif

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
