/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_ultrasonic.c														*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Ultrasonic driver for EMoRo 2560									*/
/****************************************************************************************************/

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_ultrasonic.h"
#include <avr\io.h>

//#ifndef EMORO_INTERRUPTS_H_
//#include "emoro_interrupts.cpp"
//#endif
#include "emoro_intdef.h"

/*-------------------------------------- M A C R O -------------------------------------------------*/

static unsigned char init_state;			/* ultrasonic init state								*/

//extern static struct{
//	unsigned char state : 1;				/* state for ultrasonic, 0-disabled, 1-enabled			*/
//	unsigned int distance;
//}ultrasonic[8];


//extern volatile voidFuncPtr PCintFunc[EXTERNAL_NUM_PCINTERRUPTS];
//extern uint8_t PCintLastState[EXTERNAL_NUM_PCINTERRUPTS];

/*------------------------------- ULTRASONIC ENGINE STATE MACHINE ----------------------------------*/

//extern static struct{
//	unsigned char state;
//	unsigned char us_pointer;
//	unsigned int t_start;
//	unsigned int t_end;
//}eng;
/*--------------------------------------------------------------------------------------------------*/

/****************************************************************************************************/
/* Function initialize ultrasonic sensor. Ports GPP_0 - GPP_7 are defined in emoro_io_desc.h file.	*/
/* Every initialized ultrasonic sensor needs 60 ms to be updated with new value, and if application	*/
/* program enable all 8 sensors refresh rate for every sensor will be 8*60ms = 480ms.				*/
/* To increase refresh rate of one sensor in application program stop all unused sensors. 			*/
/* Use macro GPP_0 - GPP_7 as port attribute.														*/
/*																									*/
/* @input value:	(port) 			- ultrasonic port [GPP_0 - GPP_7]. 								*/
/* @return value:	(int)			- (0) -> successfully, (-1) -> ultrasonic port out of range		*/
/*--------------------------------------------------------------------------------------------------*/
int UltrasonicClass::attach(unsigned char port){
	if((port<GPP_0) || (port>GPP_7))				/* ultrasonic port out of range					*/
		return (-1);								/* return error: port out of range				*/
	if(ultrasonic[port-GPP_0].state == 1)			/* ultrasonic sensor enabled?					*/
		return (0);									/* return successfully							*/
	cli();											/* disable global interrupts					*/

	/*----------------------------- TRANSMIT PORT CONFIGURATION ------------------------------------*/
	HC_TRANSMIT_PORT &=~ (1<<(7-(port-GPP_0)));	/* set transmit port low							*/
	HC_TRANSMIT_DDR  |=  (1<<(7-(port-GPP_0)));	/* config transmit port as output					*/
	
	/*------------------------------- ECHO PORT CONFIGURAION ---------------------------------------*/
	/* HC_1 echo port is connected to PK7 pin. (exception from another echo ports)					*/
	if(port == GPP_0){							/* port is GPP_0 on PK7 pin							*/
		DDRK  &=~ (1<<PK7);						/* set echo port as input							*/
		PORTK |=  (1<<PK7);						/* enable pull up resistor on echo port				*/
	}
	else{										/* another echo ports are connected to PORTJ[0-6]	*/
		HC_ECHO_DDR &=~ (1<<(port-GPP_1));		/* set echo port as input							*/
		HC_ECHO_PORT |= (1<<(port-GPP_1));		/* enable pull up resistor on echo port				*/
	}
	
	/*------------------------------- ECHO INTERRUPT CONFIGURATION ---------------------------------*/
	/* GPP_0 echo port is connected to PK7 pin. (exception from another echo ports)					*/
	if(port == GPP_0)							/* port is GPP_0 on PK7 pin							*/
		PCICR |= (1<<PCIE2);					/* enable PC interrupt 2							*/
	else										/* another echo ports are connected to PORTJ[0-6]	*/
		PCICR |= (1<<PCIE1);					/* enable PC interrupt 1							*/

	/*------------------------------- MES TIMER CONFIGURATION --------------------------------------*/
	if(!init_state){					/* there is no initialized ultrasonic sensors?				*/
		init_state++;					/* start timer because one sensor will be initialized		*/
		TCCR4A=TCCR4B=TCCR4C=0;			/* reset Timer/Counter control registers					*/
		TIMSK4 = 0x00;					/* reset timer interrupt mask register						*/
		TCNT4 = 0;						/* reset timer counter 4									*/
		TCCR4A |= (1<<WGM41);			/* set WGM in TCCR4A register 								*/
		TCCR4B |= (3<<3);				/* set WGM in TCCR4B -> Fast PWM, TOP->ICR4					*/
		ICR4 = 200;						/* set timer 4 TOP value in ICR4 register (100us-> overflow)*/
		TCCR4B |= (1<<CS41);			/* select prescaler (/8) and start Timer 4 (16 MHz/8=2 MHz) */
		TIMSK4 |= (1<<TOIE4);			/* enable TIMER 4 overflow interrupt 						*/
		eng.state = ENG_SET_T_HIGH;		/* set ultrasonic engine -> Set senzor transmit line high	*/
		eng.us_pointer = port-GPP_0;	/* set eng ultrasonic pointer for this sensor				*/
	}
	ultrasonic[port-GPP_0].distance=400;/* set ultrasonic distance out of range before first mes	*/
	ultrasonic[port-GPP_0].state = 1;	/* set ultrasonic state -> running							*/
	sei();								/* enable global interrupts									*/
	return (0);							/* return successfully										*/
}

/****************************************************************************************************/
/* Function detach ultrasonic sensor. Ports GPP_0 - GPP_7 are defined in emoro_io_desc.h file.		*/
/* Use macro GPP_0 - GPP_7 as port attribute. Function initialize echo and trig port as input with 	*/
/* disabled pull up resistor.																		*/
/*																									*/
/* @input value:	(port) 			- ultrasonic port [GPP_0 - GPP_7]. 								*/
/* @return value:	(int)			- (0) -> successfully, (-1) -> ultrasonic port out of range		*/
/*--------------------------------------------------------------------------------------------------*/
int UltrasonicClass::detach(unsigned char port){
	register unsigned char i;
	if((port<GPP_0) || (port>GPP_7))				/* ultrasonic port out of range					*/
		return (-1);								/* return error: port out of range				*/
	if(ultrasonic[port-GPP_0].state == 0)			/* ultrasonic sensor disabled?					*/
		return (0);									/* return successfully							*/

	cli();											/* disable global interrupts					*/
	HC_TRANSMIT_DDR  &=~ (1<<(7-(port-GPP_0)));		/* config transmit port as input				*/
	HC_TRANSMIT_PORT &=~ (1<<(7-(port-GPP_0)));		/* set transmit port low						*/
	
	/* GPP_0 echo port is connected to PK7 pin. (exception from another echo ports)					*/
	if(port == GPP_0){								/* port is GPP_0 on PK7 pin						*/
		DDRK   &=~ (1<<PK7);						/* set echo port as input						*/
		PORTK  &=~ (1<<PK7);						/* disable pull up resistor on echo port		*/
		PCICR  &=~ (1<<PCIE2);						/* disable PC interrupt 2						*/
		PCMSK2 &=~ (1<<PCINT23);					/* disable PC interrupt for PCINT23 (PK4)		*/
	}
	else{											/* echo ports are connected to PORTJ[0-6]		*/
		HC_ECHO_DDR  &=~ (1<<(port-GPP_1));			/* set echo port as input						*/
		HC_ECHO_PORT &=~ (1<<(port-GPP_1));			/* disable pull up resistor on echo port		*/
		PCMSK1 &=~ (1<<(port-GPP_0));				/* disable PC interrupt for PJ0 - PJ6			*/
	}
	ultrasonic[port-GPP_0].state = 0;				/* set ultrasonic state -> stoped				*/
	
	for(i=0;i<8;i++){								/* check all ultrasonic sensors					*/
		if(ultrasonic[i].state == 1)				/* one sensor is enabled						*/
			break;									/* break for(;;)								*/
	}
	if(i>=8){										/* all sensors disabled?						*/
		init_state=0;								/* set initialize state = 0						*/
		TCCR4A=TCCR4B=TCCR4C=0;						/* reset Timer/Counter control registers		*/
		TIMSK4 = 0;									/* reset timer interrupt mask register			*/
		TCNT4 = 0;									/* reset timer counter 4						*/
		ICR4 = 0;									/* reset ICR register							*/
	}
	sei();											/* enable global interrupts						*/
	return (0);										/* return successfully							*/
}

/****************************************************************************************************/
/* Function reads ultrasonic sensor. Before using this function ultrasonic sensor must be 			*/
/* initialized with InitUltrasonic() function. Ports GPP_0 - GPP_7 are defined in emoro_io_desc.h 	*/
/* file. Use macro GPP_0 - GPP_7 as port attribute.													*/
/*																									*/
/* @input value:	(port)			-	ultrasonic port [GPP_0 - GPP_7]								*/
/* @return value:	(int)			-	(-1)	-> port out of range								*/
/*										(-2)	-> sensor not initialized							*/
/*										(1-399)	-> sensor distance in cm							*/
/*										(400)	-> distance out of range							*/
/*--------------------------------------------------------------------------------------------------*/
int UltrasonicClass::read(unsigned char port){
	unsigned int temp;								/* temp variable for return value				*/
	if((port<GPP_0) || (port>GPP_7))				/* ultrasonic port out of range					*/
		return (-1);								/* return error: port out of range				*/
	if(ultrasonic[port-GPP_0].state == 0)			/* ultrasonic sensor disabled?					*/
		return (-2);								/* return 0 for distance						*/
	cli();											/* disable global interrupts					*/
	temp = ultrasonic[port-GPP_0].distance;			/* copy ultrasonic distance to temp variable	*/
	sei();											/* enable global interrupts						*/
	return (temp);									/* return temp variable							*/
}

/*************************************** U L T R A S O N I C   C L A S S ****************************/
UltrasonicClass Ultrasonic;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
