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
#include <avr\interrupt.h>
#include <avr/pgmspace.h>
#include "emoro_intdef.h"

//#include "emoro_interrupts.h"

//#include "emoro_ultrasonic.h"





volatile voidFuncPtr PCintFunc[EXTERNAL_NUM_PCINTERRUPTS];
uint8_t  PCintLastState[EXTERNAL_NUM_PCINTERRUPTS];

ULTRASONIC ultrasonic[8];

ENG eng;



/****************************************************************************************************/
ISR(TIMER4_OVF_vect){
	register unsigned char i;	

	/*------------------------- running sensor disabled from application ---------------------------*/
	/* running ultrasonic sensor is disabled and engine state is not ENG_WAIT_ECHO_DELAY?			*/
	if((ultrasonic[eng.us_pointer].state == 0) && (eng.state != ENG_WAIT_ECHO_DELAY)){			
		ICR4 = 60000-1;									/* overflow after 60000*0.5us = 30ms		*/
		eng.state = ENG_WAIT_ECHO_DELAY;				/* set state machine END_WAIT_ECHO_DELAY	*/
	}
	/*----------------------------- set trig pulse high state --------------------------------------*/
	else if(eng.state == ENG_SET_T_HIGH){
		PCIFR |= 7;										/* clear all external ISR flags				*/
		
		/* HC_1 echo port is connected to PK7 pin. (exception from another echo ports)				*/
		if(eng.us_pointer==0)
			PCMSK2 |= (1<<PCINT23);						/* enable PC interrupt for PCINT23 (PK4)	*/
		else												
			PCMSK1 |= (1<<eng.us_pointer);				/* enable PC interrupt for PORTJ[0-6]		*/
				
		HC_TRANSMIT_PORT |= (1<<(7-eng.us_pointer));	/* set trig high							*/
		eng.state = ENG_SET_T_LOW;						/* set state machine ENG_SET_T_LOW			*/
		eng.t_start = 0;								/* reset echo start value					*/
		eng.t_end = 0;									/* reset echo end value						*/
		ICR4 = 20;										/* set overflow after 20*0.5us = 10us		*/
	}
	/*----------------------------- set trig pulse low state ---------------------------------------*/
	else if(eng.state == ENG_SET_T_LOW){							
		HC_TRANSMIT_PORT &=~ (1<<(7-eng.us_pointer));	/* set trig low								*/
		eng.state = ENG_WAIT_ECHO;						/* set state machine ENG_WAIT_ECHO			*/
		ICR4 = 60000-1;									/* overflow after 60000*0.5us = 30ms		*/
	}
	/*----------------------------- wait echo state ------------------------------------------------*/
	else if(eng.state == ENG_WAIT_ECHO){
		PCIFR |= 7;										/* clear all external ISR flags				*/
		
		/* GPP_0 echo port is connected to PK7 pin. (exception from another echo ports)				*/
		if(eng.us_pointer==0)
			PCMSK2 &=~(1<<PCINT23);						/* disable PC interrupt for PCINT23 (PK4)	*/
		else												
			PCMSK1 &=~(1<<eng.us_pointer);				/* disable PC interrupt for PORTJ[0-6]		*/
		if(eng.t_end == 0)								/* echo not complete?						*/
			ultrasonic[eng.us_pointer].distance = 400;	/* force distance to 400 cm, out of range	*/
		eng.state = ENG_WAIT_ECHO_DELAY;				/* set state machine END_WAIT_ECHO_DELAY	*/
	}
	/*----------------------------- wait echo delay state ------------------------------------------*/
	else if(eng.state == ENG_WAIT_ECHO_DELAY){
		for(i=0;i<8;i++){								/* find enabled ultrasonic sensor			*/
			if(++eng.us_pointer > 7)					/* set pointer to next sensor				*/
				eng.us_pointer = 0;
			if(ultrasonic[eng.us_pointer].state == 1){	/* sensor enabled?							*/
				
				/* GPP_0 echo port is connected to PK7 pin. (exception from another echo ports)		*/
				if(eng.us_pointer == 0){
					if((PINK>>PK7)&0x01){				/* echo pin is still high?					*/
						ultrasonic[0].distance = 400;	/* force distance to 400 cm, out of range	*/
						//continue;						/* try find available ultrasonic			*/
					}
				}
				else{									/* another echo are connected to POJ[0-6]	*/
					if((PINJ>>(eng.us_pointer-1))&0x01){	/* echo pin is still high?				*/
						ultrasonic[eng.us_pointer].distance = 400;	/* force distance to 400 cm		*/
						//continue;								/* try find available ultrasonic	*/
					}
				}
				break;								/* break for(;;)								*/
			}											
		}
		if(i<8){									/* we have available ultrasonic sensor			*/
			eng.state = ENG_SET_T_HIGH;				/* set state machine ENG_SET_T_HIGH				*/
			ICR4 = 200;								/* overflow after 200*0.5us = 100us				*/
		}
	}
	TCNT4 = 0;										/* reset TCRN4 from isr to avoid overflow		*/
	TIFR4 = 0xFF;									/* clear ISR flag if overflow already occurred	*/
}

/****************************************************************************************************/
/* PCINT vector 1 for ultrasonic echo PJ0 - PJ6 (GPP_1 - GPP_7)										*/
ISR(PCINT1_vect){
	//PORTK |= (1<<PK3);
	if(eng.state == ENG_WAIT_ECHO){
		if((PINJ>>(eng.us_pointer-1))&0x01){				/* echo pin high?						*/
			eng.t_start = TCNT4;							/* get echo timer start					*/
		}
		else if(eng.t_start>0){								/* valid echo timer start?				*/
			eng.t_end = TCNT4;								/* get echo timer end					*/
			ultrasonic[eng.us_pointer].distance = ((eng.t_end-eng.t_start)/2)/58;	
		}
	}
	//if( (PINJ>>PJ0) && 0x01 ){
	/*if( (PINJ>>PJ0) & 0x01 ){
		PORTA |= (1<<PA0);
	}
	else{
		PORTA &= ~(1<<PA0);
	}*/
	if(PCMSK1 && 0xFE){
		if((PCMSK1 && (1<<PCINT9))){
			if((PCintFunc[1]!=0) && (PCintLastState[1]!=((PINJ>>PJ0) & 0x01))){
				//PORTK |= (1<<PK2);
				PCintLastState[1]=((PINJ>>PJ0) & 0x01);
				PCintFunc[1]();
				//PORTK &= ~(1<<PK2);
			}
		}
		if((PCMSK1 && (1<<PCINT10))){
			if((PCintFunc[2]!=0) && (PCintLastState[2]!=((PINJ>>PJ1) & 0x01))){
				PCintLastState[2]=((PINJ>>PJ1) & 0x01);
				PCintFunc[2]();
			}
		}
		if((PCMSK1 && (1<<PCINT11))){
			if((PCintFunc[3]!=0) && (PCintLastState[3]!=((PINJ>>PJ2) & 0x01))){
				PCintLastState[3]=((PINJ>>PJ2) & 0x01);
				PCintFunc[3]();
			}
		}
		if((PCMSK1 && (1<<PCINT12))){
			if((PCintFunc[4]!=0) && (PCintLastState[4]!=((PINJ>>PJ3) & 0x01))){
				PCintLastState[4]=((PINJ>>PJ3) & 0x01);
				PCintFunc[4]();
			}
		}
		if((PCMSK1 && (1<<PCINT13))){
			if((PCintFunc[5]!=0) && (PCintLastState[5]!=((PINJ>>PJ4) & 0x01))){
				PCintLastState[5]=((PINJ>>PJ4) & 0x01);
				PCintFunc[5]();
			}
		}
		if((PCMSK1 && (1<<PCINT14))){
			if((PCintFunc[6]!=0) && (PCintLastState[6]!=((PINJ>>PJ5) & 0x01))){
				PCintLastState[6]=((PINJ>>PJ5) & 0x01);
				PCintFunc[6]();
			}
		}
		if((PCMSK1 && (1<<PCINT15))){
			if((PCintFunc[7]!=0) && (PCintLastState[7]!=((PINJ>>PJ6) & 0x01))){
				PCintLastState[7]=((PINJ>>PJ6) & 0x01);
				PCintFunc[7]();
			}
		}
	}
	//PORTK &= ~(1<<PK3);
}

/****************************************************************************************************/
/* PCINT vector 2 for ultrasonic echo PK7 (GPP_0) 													*/
ISR(PCINT2_vect){
	if(eng.state == ENG_WAIT_ECHO){
		if((PINK>>PK7)&0x01){								/* echo pin high?						*/
			eng.t_start = TCNT4;							/* get echo timer start					*/
		}
		else if(eng.t_start>0){								/* valid echo timer start?				*/
			eng.t_end = TCNT4;								/* get echo timer end					*/
			ultrasonic[0].distance = ((eng.t_end-eng.t_start)/2)/58;	
		}
	}
	if((PCMSK2 && (1<<PCINT23))  ){
		if((PCintFunc[0]!=0) && (PCintLastState[0]!=((PINK>>PK7) & 0x01))){
			PCintLastState[0]=((PINK>>PK7) & 0x01);
			PCintFunc[0]();
		}
	}
}

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
