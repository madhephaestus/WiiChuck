/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_servo.c															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Servo driver for EMoRo Pro2560									*/
/****************************************************************************************************/

/*--------------------------------------- I N C L U D E S ------------------------------------------*/
#include <avr\io.h>
#include <avr\interrupt.h>
#include "emoro_servo.h"
#include "pins_arduino.h"

/*----------------------------------------- M A C R O ----------------------------------------------*/
#define SERVO_DDR			DDRL			/* servo data direction configuration					*/
#define SERVO_PORT			PORTL			/* servo port configuration								*/

/*------------------------------------ D E C L A R A T I O N S -------------------------------------*/
static struct{
	int position;
	unsigned char status;
}servo[8];
 
static unsigned int t_servo;
static unsigned char p;
static unsigned char init_state;

/****************************************************************************************************/
/* Timer 1 interrupt service routine used for servo pulse execute. 									*/
ISR(TIMER1_OVF_vect){
	if(p < 7){								/* servo pointer < 7?									*/
		if(servo[p].status) 				/* servo p enabled?										*/
			SERVO_PORT &=~(1<<p);			/* set servo p pow										*/
		if(servo[p+1].status) 				/* servo p+1 enabled?									*/
			SERVO_PORT |= (1<<(p+1));		/* set servo p+1 high									*/
	}	
	else if(p == 7){						/* servo pointer = 7?									*/
		if(servo[p].status)					/* servo p enabled?										*/
			SERVO_PORT &=~(1<<p);			/* set servo p low										*/
	}
	else if(p == 8){					
		if(servo[0].status)					/* first servo enabled?									*/
			SERVO_PORT |= (1<<0);			/* set first servo port high							*/
	}
	p++;									/* point to next servo pulse							*/
	t_servo += ICR1;						/* sum servo pulse time									*/

	if(p == 8){								/* end of servo pulses?									*/
		ICR1 = 40000-t_servo;				/* fill TIMER for 20ms servo delay TIME					*/
	}
	else if(p == 9){						/* 20 ms servo delay expired?							*/
		p = 0;								/* reset servo session pointer							*/
		t_servo = 0;						/* reset Servo pulse timer								*/
		ICR1 = servo[0].position*2-2;		/* set T1 overflow for servo 0 pulse					*/
	}
	else									/* servo pulses running?								*/
		ICR1 = servo[p].position*2-2;		/* set T1 overflow for servo pointer pulse				*/
}

/****************************************************************************************************/
/* Function initialize servo port and must be called before using SetServo() and GetServo() 		*/
/* functions. SERVO_0 - SERVO_7 macro are defined in emoro_io_desc.h file. Function initialize		*/
/* servo port with default pulse 1500us. Use macro SERVO_0-SERVO_7 as port attribeute.				*/
/*																									*/
/* @input value:	(port) 	- servo port [SERVO_0 - SERVO_7]										*/
/* @return value:	(int)	- (0) -> successfully, (-1) -> servo port out of range					*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroServoClass::attach(unsigned char port){
	register unsigned char i;
	if((port<SERVO_0) || (port>SERVO_7)) 
		return (-1);					/* return error: servo port out of range. 					*/
	cli();								/* disable global interrupts								*/
	if(!init_state){
		init_state++;					/* set flag for servo init complete							*/
		TCCR1A=TCCR1B=TCCR1C=0;			/* reset Timer/Counter control registers					*/
		TIMSK1 = 0;						/* reset timer interrupt mask register						*/
		TCCR1A |= (1<<WGM11);			/* set WGM in TCCR1A register 								*/
		TCCR1B |= (3<<3);				/* set WGM in TCCR1B -> Fast PWM, TOP->ICR1					*/
		ICR1 = 3000;					/* set timer 1 TOP value in ICR1 register (1500us->overflow)*/
		TCCR1B |= (1<<CS11);			/* select prescaler (/8) and start Timer 1 (16 MHz/ =2 MHz) */
		TIMSK1 |= (1<<TOIE1);			/* enable TIMER 1 overflow interrupt 						*/
		for(i=0 ; i<8 ; i++){			
			servo[i].status = 0;		/* set servo status -> disabled								*/
			servo[i].position = 1500;	/* set servo default position -> 1500 uS					*/
		}
	}
	servo[port-SERVO_0].status = 1;		/* set new servo status -> servo running					*/
	SERVO_DDR |= (1<<(port-SERVO_0));	/* set output for servo port								*/
	sei();								/* enable global interrupts									*/
	return (0);							/* servo initialization successfully						*/
}

/****************************************************************************************************/
/* Function detach servo port. SERVO_0 - SERVO_7 are defined in emoro_io_desc.h file. Before		*/
/* initializing SERVO port as input or output, and servo is already initialized, servo port must	*/
/* be stoped with StopServo() function. Function initialize port as input with disabled pull up		*/
/* resistor. Use macro SERVO_0-SERVO_7 as port attibute.											*/
/*																									*/
/* @input value:	(port)  - 	stop servo port [SERVO_0 - SERVO_7]									*/
/* @return value:	(int)	-	(0) -> successfully. (-1) -> servo port out of range				*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroServoClass::detach(unsigned char port){
	register unsigned char i;
	if((port<SERVO_0) || (port>SERVO_7)) 
		return (-1);							/* return error: servo port out of range. 			*/
	cli();										/* disable global interrupts						*/
	servo[port-SERVO_0].status = 0;				/* stop servo port									*/
	servo[port-SERVO_0].position = 1500;		/* reset servo position 3000*500us = 1.5ms			*/
	SERVO_DDR  &=~ (1<<(port-SERVO_0));			/* set servo port as input							*/
	SERVO_PORT &=~ (1<<(port-SERVO_0));			/* disable hardware pull up resistor				*/
	for(i=0; i<8 ; i++){						/* check servo states								*/
		if(servo[i].status == 1){				/* if one servo running? 							*/
			sei();								/* enable global interrupts							*/
			return (0);							/* servo stoped successfully 						*/
		}
	}
	TCCR1A = TCCR1B = TCCR1C = TIMSK1 = 0;		/* reset timer 1									*/
	init_state = 0;								/* there is no initialized servo					*/
	sei();										/* enable global interrupts							*/
	return (0);									/* servo stoped successfully 						*/
}

/****************************************************************************************************/
/* Function sets servo port position. SERVO_0 - SERVO_7 are defined in emoro_io_desc.h.				*/
/* Before using SetServo() function, servo port must be initialized with InitServo() function.		*/
/* Use macro SERVO_0 - SERVO_7 as port attribute. Servo position range is 500-2500 us.				*/
/*																									*/
/* @input value:	(port)		- servo port [SERVO_0 - SERVO_7]									*/
/*					(position) 	- servo pulse in us. position range: [500 - 2500] us 				*/
/* @return value:	(int)		- (0)  -> successfuly 												*/
/*								- (-1) -> error: servo position out of range						*/
/*								- (-2) -> error: servo port out of range							*/
/*								- (-3) -> error: servo not running									*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroServoClass::write(unsigned char port, int position){
	if((position>2500) || (position<500))		/* check position range [500-2500]					*/
		return (-1);							/* return error: position out of range 				*/
	else if((port<SERVO_0) || (port>SERVO_7))	/* port out of range?								*/
		return (-2);							/* return error: port out of range 					*/
	else if(servo[port-SERVO_0].status){		/* servo port enabled?								*/
		cli();									/* disable global interrupts						*/
		servo[port-SERVO_0].position=position;	/* set new position for servo port					*/
		sei();									/* enable global interrupts							*/
		return (0);								/* servo set successfully							*/
	}
	return (-3);								/* return error: servo not running					*/
}

/****************************************************************************************************/
/* Function reads servo position for port SERVO_0 - SERVO_7. Before reading servo position, servo	*/
/* port must be initialized with InitServo() function. SERVO_0 - SERVO_7 are defined in				*/
/* emoro_io_desc.h file. Use macro SERVO_0 - SERVO_7 as port attribute.								*/
/*																									*/
/* @input value: 	(port) 	- servo port [SERVO_0 - SERVO_7]										*/
/* @return value:	(int)	- (500-2500)	-> servo position 	 									*/
/*							- (-1)			-> error: servo port out of range						*/
/*							- (-2)			-> error: servo not running								*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroServoClass::read(unsigned char port){
	if((port<SERVO_0) || (port>SERVO_7)) 
		return (-1);							/* return error: servo port out of range. 			*/
	else if(servo[port-SERVO_0].status == 0)	/* servo not running?								*/
		return (-2);							/* return error: servo not running					*/
	return (servo[port-SERVO_0].position);		/* return servo position							*/
}

/************************************ S E R V O   C L A S S *****************************************/
EmoroServoClass EmoroServo;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
