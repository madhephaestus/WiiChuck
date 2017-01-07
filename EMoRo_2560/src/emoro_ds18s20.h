/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_ds18s20.h															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo DS18S20 driver													*/
/****************************************************************************************************/
#ifndef EMORO_DS18S20_H_
#define EMORO_DS18S20_H_

class EmoroDs18s20Class{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize DS18S20 temperature senzor, and must be called before using read().		*/
	/*																								*/
	/* @input value:	(port)	- DS18S20 port IO_0-IO_16, ADC_0-ADC_7, SERVO_0-SERVO_7,			*/
	/*										  GPP_0_A - GPP_7_B, PWM_0 - PWM_5, EX_IO_0 - EX_IO_17	*/
	/* @return value:	(int)	-	(0)		Successfully											*/  
	/*							-	(-1)	Port out of range										*/
	/*							-	(-2)	Communication error										*/
	/*----------------------------------------------------------------------------------------------*/
	int attach(unsigned char port);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function reads temperature from DS18S20 sensor connected to port IO_0-IO_16, ADC_0-ADC_7,  	*/
	/* SERVO_0-SERVO_7, GPP_0_A - GPP_7_B, PWM_0 - PWM_5, EX_IO_0 - EX_IO_17						*/
	/*																								*/
	/* @input value:	(port)	- DS18S20 port IO_0-IO_16, ADC_0-ADC_7, SERVO_0-SERVO_7				*/
	/* @return value:	(float)	- Successfully = temperature, Failed = 200			  				*/
	/*----------------------------------------------------------------------------------------------*/
	float read(unsigned char port);
};

/***************************************** D S 1 8 2 0   C L A S S **********************************/
extern EmoroDs18s20Class DS18S20;

#endif /* EMORO_DS18S20_H_																			*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
