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
#include "emoro_interrupts.h"
#include "emoro_intdef.h"

//extern static volatile voidFuncPtr PCintFunc[EXTERNAL_NUM_PCINTERRUPTS];
//extern static uint8_t  PCintLastState[EXTERNAL_NUM_PCINTERRUPTS];

void attachPCInterrupt(uint8_t interruptNum, void (*userFunc)(void)) {
	//for debug
	//DDRK |=(1<<PK3);		//set PK3 to output 
	//DDRK |=(1<<PK2);		//set PK2 to output 
	//DDRA |=(1<<PA0);		//set PA0 to output 

	if(interruptNum>=9 && interruptNum<=15){	//PCINT9-PCINT15
		PCintFunc[interruptNum-8] = userFunc;
		PCintLastState[interruptNum-8]=((PINJ>>(interruptNum-9)) & 0x01);
		PCMSK1|=(1<<(interruptNum-8));
		PCICR |= (1<<PCIE1);					//Enable PCIE1
	}
	else if(interruptNum==23){					//PCINT23
		PCintFunc[0] = userFunc;
		PCintLastState[0]=((PINK>>PK7) & 0x01);
		PCMSK2|=(1<<7);
		PCICR |= (1<<PCIE2);					//Enable PCIE2
	}
}
void detachPCInterrupt(uint8_t interruptNum) {
	if(interruptNum>=9 && interruptNum<=15){	//PCINT9-PCINT15
		PCintFunc[interruptNum-8] = 0;
		PCMSK1&=~(1<<(interruptNum-8));
	}
	else if(interruptNum==23){					//PCINT23
		PCintFunc[0] = 0;
		PCMSK1&=~(1<<7);
	}

}
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
