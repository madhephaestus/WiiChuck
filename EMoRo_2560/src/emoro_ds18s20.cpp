/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_ds18s20.c															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo DS18S20 driver													*/
/****************************************************************************************************/

#ifndef F_CPU
	#define F_CPU				16000000UL
#endif

/*--------------------------------------- I N C L U D E S ------------------------------------------*/
#include "emoro_ds18s20.h"
#include "Arduino.h"
#include <avr/io.h>
#include <util\delay.h>

/****************************************************************************************************/
unsigned char Ds18s20Reset(unsigned char port){
	unsigned char err=100;
							
	pinMode(port, OUTPUT);									/* config port as output, set low		*/	
	digitalWrite(port, 0);
	_delay_ms(1);											/* Reset delay pulse (1 ms)				*/
	
	pinMode(port, INPUT_PULLUP);							/* config port as input, enable pull-up	*/
	_delay_us(66); 										

	/* no presence detect --> err=1 otherwise err=0 												*/
	err = digitalRead(port);
	_delay_ms(1);										
		
	if(digitalRead(port) == 0)								/* short circuit --> err=2				*/
		err = 2;
	return err;
}

/****************************************************************************************************/
void Ds18s20WriteBit(unsigned char wrbit,unsigned char port){
	if (wrbit == 0){
		pinMode(port, OUTPUT);								/* config port as output, set low		*/
		digitalWrite(port, 0);
		_delay_us(60); 
		pinMode(port, INPUT_PULLUP);						/* config port as input, enable pull-up	*/
		_delay_us(4); 
	}
	if (wrbit ==1)	{
		pinMode(port, OUTPUT);								/* config port as output, set low		*/
		digitalWrite(port, 0);
		_delay_us(10); 
		pinMode(port, INPUT_PULLUP);						/* config port as input, enable pull-up	*/
		_delay_us(54); 
	}
}

/****************************************************************************************************/
unsigned char Ds18s20ReadBit(unsigned char port){
	unsigned char rebit;

	pinMode(port, OUTPUT);									/* config port as output, set low		*/
	digitalWrite(port, 0);
	_delay_us(1); 
	pinMode(port, INPUT_PULLUP);							/* config port as input, enable pull-up	*/
	_delay_us(10); 
	rebit = digitalRead(port);								/* Read one bit							*/
	_delay_us(50); 
	return rebit;								
}

/****************************************************************************************************/
unsigned char Ds18s20ReadByte(unsigned char port){
	unsigned char rebyte = 0;
	unsigned char rebit;
	unsigned char i;

	for (i=0;i<8;i++){
		rebit=Ds18s20ReadBit(port);
		if(rebit==1){
			rebyte|=(1<<i);
		}
	}
	return(rebyte);
}

/****************************************************************************************************/
void Ds18s20WrByte(unsigned char wrbyte, unsigned char port){
	unsigned char i;
	for (i=0;i<8;i++){ 									/*  writes byte, one bit at a time 			*/ 
		Ds18s20WriteBit((wrbyte & 0b00000001), port);
		wrbyte = wrbyte >> 1;
	}
	_delay_us(5); 
}

/****************************************************************************************************/
float EmoroDs18s20Class::read(unsigned char port){
	unsigned char i;
	unsigned int j=0;
	int temp;
										
	if(Ds18s20Reset(port) == 0){						/* reset successfully?						*/
		Ds18s20WrByte(0xCC, port); 						/* skip ROM									*/
		Ds18s20WrByte(0x44, port);  					/* ask for temperature conversion			*/
		while (Ds18s20ReadByte(port)==0xFF && j<1000){	/* wait until conversion is finished		*/ 
			_delay_us(1);
			j++;
		}									 
		Ds18s20Reset(port);								/* reset again								*/
		Ds18s20WrByte(0xCC, port); 						/* skip ROM 								*/
		Ds18s20WrByte(0xBE, port); 						/* read entire scratchpad 9 bytes			*/
		
		temp = Ds18s20ReadByte(port);					/* read temperature low byte				*/
		temp |= (Ds18s20ReadByte(port)<<8);				/* read temperature high byte				*/
		for (i=0; i<7; i++)     						/* get scratchpad byte by byte				*/
			Ds18s20ReadByte(port); 						/* ignore scratchpad bytes 2-8				*/													
		temp /= 2;
		return (float)temp;								/* return temperature						*/
	}
	return 200;											/* reset failed -> return 200				*/
}

/****************************************************************************************************/
int EmoroDs18s20Class::attach(unsigned char port){
	unsigned int i=0;
	if(port > EX_IO_15)
		return -1;										/* port out of range						*/
	if(Ds18s20Reset(port) == 0){						/* reset successfully?						*/
		Ds18s20WrByte(0xCC, port); 						/* skip ROM 								*/
		Ds18s20WrByte(0x44, port); 						/* ask for temperature conversion			*/
		while (Ds18s20ReadByte(port)==0xFF && i<1000){	/* wait until conversion is finished		*/ 
			_delay_us(1);
			i++;
		}
		Ds18s20Reset(port);								/* Reset									*/
		Ds18s20WrByte(0xCC, port); 						/* skip ROM 								*/
		Ds18s20WrByte(0xBE, port); 						/* read entire scratchpad 9 bytes 			*/
		return (0);										/* ds18s20 init successfully				*/
	}
	return (-2);										/* reset failed -> return error				*/   
}

/***************************************** D S 1 8 2 0   C L A S S **********************************/
EmoroDs18s20Class DS18S20;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
