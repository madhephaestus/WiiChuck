/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_i2c.c																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo I2C driver for EMoRo Pro2560										*/
/****************************************************************************************************/

#ifndef F_CPU
	#define F_CPU				16000000UL
#endif

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_i2c.h"
#include <avr\io.h>
#include <util\delay.h>

/*----------------------------------------- M A C R O ----------------------------------------------*/
#define I2C_CLOCK_STRETCH_DELAY 2000	/* counts of I2CDelay to allow for clock stretching			*/
#define I2C_RERRIES 			1000	/* retries to get an answer from slave in i2c_write_wait	*/

#define SclH() 		(DDRD &=~(1<<PD0))
#define SclL() 		(DDRD |= (1<<PD0))
#define SdaH() 		(DDRD &=~(1<<PD1))
#define SdaL() 		(DDRD |= (1<<PD1))
#define Scl()  		((PIND>>PD0)&0x01)
#define Sda()   	((PIND>>PD1)&0x01)

/****************************************************************************************************/
void I2cWait(void){
	_delay_us(1);
}

/****************************************************************************************************/
/* Function sets clock high, if slave stretches it, the function waits for max  					*/
/* I2C_CLOCK_STRETCH_DELAY i2c_Delay's for the clock to go low. Slaves use this feature to delay  	*/
/* data transfer to master.																			*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cWaitSclH(void){
	int delcnt=0;	
	SclH();								/* Sets SCL high and waits for clock stretching. 			*/
   I2cWait();
	while(Scl()==0 && delcnt<I2C_CLOCK_STRETCH_DELAY){
		I2cWait();
		delcnt++;
	}
	if(Scl()==0) 
		return (-1); 					/* error: too long clock stretching 						*/
	return (0);							/* return successfully										*/
}

/****************************************************************************************************/
/* Function retries byte write until slave responds. Max I2C_RETRIES attempts.						*/
/*																									*/
/* @input value:	(unsigned char)	- data to send													*/
/* @return value:	(signed char)	- (0) successfully, (-1) too many retries						*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cWriteWait(unsigned char data){
	unsigned int cnt=0;
	
	while(I2cWrite(data) && cnt<I2C_RERRIES){
		I2cWait();
		I2cStop();						/* Not ready yet !											*/
		I2cWait();
		if(I2cStartWait()) 
			return (-1); 				/* error: too long clock stretching 						*/
 		cnt++;
  	}
   if (cnt>=I2C_RERRIES)
   	return (-1);  						/* number of retries exceeded								*/
	return (0);							/* return successfully										*/
}

/****************************************************************************************************/
/* Function sends P (STOP) to slave. Clock goes high, then data goes high.							*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(none)														  					*/
/*--------------------------------------------------------------------------------------------------*/
void I2cStop(void){
	SdaL();								/* set SDA LOW												*/
	I2cWait();							/* I2C delay												*/
	SclH();								/* set SCL high												*/
	I2cWait();							/* I2C delay												*/
	SdaH();								/* set SDA high												*/
}

/****************************************************************************************************/
/* Function sends 8 bits to slave. Checks if slave pulls data low for ACK on clock pulse.			*/
/*	Allows for clocks stretching on SCL going high.													*/
/*																									*/
/* @input value:	(unsigned char)	-  data to send													*/
/* @return value:	(signed char)	- 	(0) 	ACK sent from slave  								*/
/*										(1) 	NAK sent from slave									*/
/*										(-1) 	clock stretching timeout							*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cWrite(unsigned char data){
	unsigned char i;		
	for (i=0;i<8;i++){				/* write one byte												*/
		if(data & 0x80)				/* last bit high? (bit 7)										*/
			SdaH();					/* set SDA high													*/
		else						/* else															*/
			SdaL();					/* set SDA low													*/
		I2cWait();					/* I2C delay													*/
		if(I2cWaitSclH()) 			/* wait SCL high failed?										*/
			return (-1);			/* error: too long clock stretching 							*/
		SclL();						/* set SCL low													*/
		I2cWait();					/* I2C delay													*/
		data = data<<1;				/* shift data for next bit										*/
	}
	return I2cCheckAck();			/* return ACK or NAK											*/
}

/****************************************************************************************************/
/* Function checks if slave pulls data low for ACK on clock pulse. Allows for clocks stretching  	*/
/* on SCL going high.																				*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- 	(0) 	ACK sent from slave  								*/
/*										(1) 	NAK sent from slave									*/
/*										(-1) 	clock stretching timeout							*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cCheckAck(void){
	SdaH();							/* set SDA high													*/
	I2cWait();						/* I2C delay													*/
	if(I2cWaitSclH()) 				/* wait SCL high failed?										*/
		return (-1); 				/* error: too long clock stretching 							*/
	if(Sda()){						/* SDA still high?												*/
		SclL();						/* set SCL low													*/
		return (1);					/* return NAK													*/
	}
	SclL();							/* set SCL low													*/
	return (0);						/* return ACK													*/
}

/****************************************************************************************************/
/* Function reads 8 bits from slave. Allows for clocks stretching on all SCL going high.			*/
/* This is NOT in the protocol for I2C, but allows I2C slaves to be implemented on slower devices.	*/
/*																									*/
/* @input value:	(unsinged char*)	- 1 data return buffer										*/
/* @return value:	(signed char)		- (0) successfully, (-1) clock stretching timeout			*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cRead(unsigned char *data){
	unsigned char res = 0;
	unsigned char  i;
	for (i=0;i<8;i++){				/* read one byte												*/
		SdaH();						/* set SDA high													*/
		I2cWait();					/* I2C delay													*/
		if(I2cWaitSclH()) 			/* wait scl high failed?										*/
			return (-1); 			/* error: too long clock stretching 							*/
		res<<=1;					/* shift res for one bit										*/
		if(Sda())					/* sda still high?												*/
			res |= 0x01;			/* set 1 to first bit											*/
		SclL();						/* set SCL low													*/
		I2cWait();					/* I2C delay													*/
	}
	*data = res; 					/* update data attribute										*/
	return (0);						/* return successfully											*/
}

/****************************************************************************************************/
/* Function Sends NAK sequence to slave. NAK is often sent when transfer is finished.				*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cSendNak(void){
	SdaH();							/* set SDA high													*/
	I2cWait();						/* I2C delay													*/
	if(I2cWaitSclH()) 				/* wait SCL high failed?										*/
		return (-1); 				/* error: too long clock stretching 							*/
	I2cWait();						/* I2C delay													*/
	SclL();							/* set SCL low													*/
	I2cWait();						/* I2C delay													*/
	SdaH();							/* send SDA high												*/
	return (0);						/* return successfully											*/
}

/****************************************************************************************************/
/* Function sends ACK sequence to slave. ACK is usually sent after successfully transfer, where more*/
/* bytes to be read																					*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cSendAck(void){
	SdaL();							/* set SDA low													*/
	I2cWait();						/* I2C delay													*/
	if(I2cWaitSclH()) 				/* wait SCL high failed?										*/
		return (-1);				/* error: too long clock stretching 							*/
	I2cWait();						/* I2C delay													*/
	SclL();							/* set SCL low													*/
	I2cWait();						/* I2C delay													*/
	SdaH();							/* set SDA high													*/
	return (0);						/* return successfully											*/
}

/****************************************************************************************************/
/* Function initiates i2c transmission by sending S. First waits for possible clock stretching. 	*/
/* Data goes low while clock is high. Inserts delay after S pulse.									*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) success, (-1) clock stretching timeout					*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cStartWait(void){
	if(I2cWaitSclH()) 				/* wait SCL high failed?										*/
		return (-1); 				/* error: too long clock stretching 							*/
   SdaH();							/* set SDA high													*/
	I2cWait();						/* I2C delay													*/
   SdaL();							/* set SDA low													*/
	I2cWait();						/* I2C delay													*/
   SclL();							/* set SCL low													*/
	I2cWait();						/* I2C delay													*/
   return (0);						/* return successfully											*/
}

/****************************************************************************************************/
/* Function initiates i2c transmission by sending S. First waits for possible clock stretching. 	*/
/* Data goes low while clock is high. 																*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) success, (-1) clock stretching timeout					*/
/*--------------------------------------------------------------------------------------------------*/
signed char I2cStart(void){
   if(I2cWaitSclH())				/* wait SCL high failed?										*/
		return (-1);				/* error: too long clock stretching 							*/
   SdaH();							/* set SDA high													*/
	I2cWait();						/* I2C delay													*/
   SdaL();							/* set SDA low													*/
	I2cWait();						/* I2C delay													*/
   SclL();							/* set SCL low													*/
   return (0);						/* return successfully											*/
}

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/

