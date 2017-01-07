/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_i2c.h																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo I2C driver for EMoRo Pro2560										*/
/****************************************************************************************************/
#ifndef EMORO_I2C_H_
#define EMORO_I2C_H_

#define DISABLE_GYRO_INT	unsigned char temp_int7 = EIMSK; EIMSK &= ~(1 <<INT7)
#define RESTORE_GYRO_INT	EIMSK |= temp_int7 & (1 <<INT7) ? (1 <<INT7) : 0

/*--------------------------------------------------------------------------------------------------*/
/* Function sets clock high, if slave stretches it, the function waits for max  					*/
/* I2C_CLOCK_STRETCH_DELAY i2c_Delay's for the clock to go low. Slaves use this feature to delay  	*/
/* data transfer to master.																			*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
signed char I2cWaitSclH(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function retries byte write until slave responds. Max I2C_RETRIES attempts.						*/
/*																									*/
/* @input value:	(unsigned char)	- data to send													*/
/* @return value:	(signed char)	- (0) successfully, (-1) too many retries						*/
signed char I2cWriteWait(unsigned char data);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function sends P (STOP) to slave. Clock goes high, then data goes high.							*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(none)														  					*/
void I2cStop(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function sends 8 bits to slave. Checks if slave pulls data low for ACK on clock pulse.			*/
/*	Allows for clocks stretching on SCL going high.													*/
/*																									*/
/* @input value:	(unsigned char)	-  data to send													*/
/* @return value:	(signed char)	- 	(0) 	ACK sent from slave  								*/
/*										(1) 	NAK sent from slave									*/
/*										(-1) 	clock stretching timeout							*/
signed char I2cWrite(unsigned char data);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function checks if slave pulls data low for ACK on clock pulse. Allows for clocks stretching  	*/
/* on SCL going high.																				*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- 	(0) 	ACK sent from slave  								*/
/*										(1) 	NAK sent from slave									*/
/*										(-1) 	clock stretching timeout							*/
signed char I2cCheckAck(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function reads 8 bits from slave. Allows for clocks stretching on all SCL going high.			*/
/* This is NOT in the protocol for I2C, but allows I2C slaves to be implemented on slower devices.	*/
/*																									*/
/* @input value:	(unsigned char*)	- 1 data return buffer										*/
/* @return value:	(signed char)		- (0) successfully, (-1) clock stretching timeout			*/
signed char I2cRead(unsigned char *data);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function Sends NAK sequence to slave. NAK is often sent when transfer is finished.				*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
signed char I2cSendNak(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function sends ACK sequence to slave. ACK is usually sent after succesful transfer, where more 	*/
/* bytes to be read																					*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
signed char I2cSendAck(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function initiates i2c transmission by sending S. First waits for possible clock stretching. 	*/
/* Data goes low while clock is high. Inserts delay after S pulse.									*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
signed char I2cStartWait(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function initiates i2c transmission by sending S. First waits for possible clock stretching. 	*/
/* Data goes low while clock is high. 																*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(signed char)	- (0) successfully, (-1) clock stretching timeout				*/
signed char I2cStart(void);
/*--------------------------------------------------------------------------------------------------*/

#endif /* EMORO_I2C_H_																				*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/



