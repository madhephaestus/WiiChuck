/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_acc.c																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo acceleration sensor BMA150 driver for EMoRo Pro2560				*/
/****************************************************************************************************/

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_acc.h"
#include "emoro_i2c.h"
#include <avr\io.h>

/*--------------------------------------- M A C R O ------------------------------------------------*/
#define BMA150_ADDRESS_WRITE			0x70
#define BMA150_ADDRESS_READ				0x71                               

/*----------------------------------- BMA150 registers ---------------------------------------------*/
#define BMA150_RA_CHIP_ID				0x00
#define BMA150_RA_VERSION				0x01
#define BMA150_RA_X_AXIS_LSB			0x02
#define BMA150_RA_X_AXIS_MSB			0x03
#define BMA150_RA_Y_AXIS_LSB			0x04
#define BMA150_RA_Y_AXIS_MSB			0x05
#define BMA150_RA_Z_AXIS_LSB			0x06
#define BMA150_RA_Z_AXIS_MSB			0x07
#define BMA150_RA_TEMP_RD				0x08
#define BMA150_RA_SMB150_STATUS			0x09
#define BMA150_RA_SMB150_CTRL			0x0A
#define BMA150_RA_SMB150_CONF1			0x0B
#define BMA150_RA_LG_THRESHOLD			0x0C
#define BMA150_RA_LG_DURATION			0x0D
#define BMA150_RA_HG_THRESHOLD			0x0E
#define BMA150_RA_HG_DURATION			0x0F
#define BMA150_RA_MOTION_THRS			0x10
#define BMA150_RA_HYSTERESIS			0x11
#define BMA150_RA_CUSTOMER1				0x12
#define BMA150_RA_CUSTOMER2				0x13
#define BMA150_RA_RANGE_BWIDTH			0x14
#define BMA150_RA_SMB150_CONF2			0x15
#define BMA150_RA_OFFS_GAIN_X			0x16
#define BMA150_RA_OFFS_GAIN_Y			0x17
#define BMA150_RA_OFFS_GAIN_Z			0x18
#define BMA150_RA_OFFS_GAIN_T			0x19
#define BMA150_RA_OFFSET_X				0x1A
#define BMA150_RA_OFFSET_Y				0x1B
#define BMA150_RA_OFFSET_Z				0x1C
#define BMA150_RA_OFFSET_T				0x1D

static unsigned char my_acc_available=0;

/****************************************************************************************************/
/* Function reads one byte from BMA chip. Reg attribute is used for register address, and *data		*/
/* will contain readed byte if reading is successfully.												*/
/*																									*/
/* @input value:	(reg)			- register address												*/
/*					(*data)			- data for readed byte from register address					*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char AccReadByte(unsigned char reg, unsigned char *data){
	signed char error=0;

	DISABLE_GYRO_INT;

	error += I2cStartWait();							/* send I2C start with delay				*/
	error += I2cWriteWait(BMA150_ADDRESS_WRITE);		/* send BMA address byte and write bit		*/
	error += I2cWrite(reg);								/* send register address for read			*/
	I2cStop();											/* send I2C stop							*/
	error += I2cStartWait();							/* send I2C start with delay				*/
	error += I2cWriteWait(BMA150_ADDRESS_READ);			/* send BMA address byte and read bit		*/
	error += I2cRead(data);								/* read one byte from BMA slave				*/
	error += I2cSendNak();								/* send Nak to BMA slave					*/
	
	RESTORE_GYRO_INT;
	
	if(error)											/* error occurred?							*/
		return (1);										/* return error (1) -> communication failed	*/
	return (0);											/* return successfully						*/
}

/****************************************************************************************************/
/* Function reads bytes from BMA chip. Reg_start points to address of the first byte for reading,	*/
/* and number of readed bytes will be stored in attribute buffer.									*/
/*																									*/
/* @input value:	(reg_start)			- first register address									*/
/*					(bytes)				- number of bytes for reading								*/
/*					(*buffer)			- buffer for readed bytes									*/
/* @return value:	(unsigned char)		- (0) -> successfully, (1) -> reading failed				*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char AccReadBytes(unsigned char reg_start, unsigned char bytes, unsigned char *buffer){
	unsigned char data, i;
	signed char error=0;
	
	DISABLE_GYRO_INT;

	error += I2cStartWait();							/* send I2C start with delay				*/
	error += I2cWriteWait(BMA150_ADDRESS_WRITE);		/* send BMA address byte and write bit		*/
	error += I2cWrite(reg_start);						/* send BMA start register address			*/
	I2cStop();											/* send I2C stop							*/
	error += I2cStartWait();							/* send I2C start with delay				*/
	error += I2cWriteWait(BMA150_ADDRESS_READ);			/* send BMA address byte and read bit		*/
	
	for(i=0 ; i<bytes ; i++){							/* read number of bytes						*/
		error += I2cRead(&data);						/* read one byte							*/
		*(buffer+i) = data;								/* store one byte							*/
		if((i+1) == bytes){
			error += I2cSendNak();						/* send Nak to BMA slave					*/
			break;										/* break for								*/
		}
		else
			error += I2cSendAck();						/* send ACK to BMA slave to read next byte	*/
	}
	RESTORE_GYRO_INT;
	
	if(error)											/* error occurred?							*/
		return (1);										/* return error (1) -> communication failed	*/
	return (0);											/* return successfully						*/
}

/****************************************************************************************************/
/* Function initialize acceleration sensor BMA150, and must be called before using another 			*/
/* acc function except AccTestConnection().															*/
/*																									*/
/* input value:		(none)																			*/
/* return value:	(unsigned char)	- (0) -> successfully, (-1) -> initialization failed (I2C error)*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroAccClass::init(void){
	signed char error = 0;
	unsigned char data=0;

	DISABLE_GYRO_INT;

	/* set bandwidth 25 Hz and range 2G																*/
	error += I2cStartWait();								/* send I2C start with delay			*/
	error += I2cWriteWait(BMA150_ADDRESS_WRITE);			/* send BMA address byte and write bit	*/
	error += I2cWrite(BMA150_RA_RANGE_BWIDTH);				/* send register address for read		*/
	I2cStop();												/* send I2C stop						*/
	error += I2cStartWait();								/* send I2C start with delay			*/
	error += I2cWriteWait(BMA150_ADDRESS_READ);				/* send BMA address byte and read bit	*/
	error += I2cRead(&data);								/* read one byte from BMA slave			*/
	error += I2cSendNak();									/* send Nak to BMA slave				*/
	data = data & 0xE0;										/* set bandwidth 25 Hz, range 2G		*/
	
	error += I2cStartWait();								/* send I2C start with delay			*/
	error += I2cWriteWait(BMA150_ADDRESS_WRITE);			/* send BMA address byte and write bit	*/
	error += I2cWrite(BMA150_RA_RANGE_BWIDTH);				/* send register address for write		*/
	error += I2cWrite(data);								/* write data							*/
	I2cStop();												/* send I2C stop						*/

	/* set bandwidth 25 Hz and range 2G																*/
	error += I2cStartWait();								/* send I2C start with delay			*/
	error += I2cWriteWait(BMA150_ADDRESS_WRITE);			/* send BMA address byte and write bit	*/
	error += I2cWrite(BMA150_RA_SMB150_CONF1);				/* send register address for read		*/
	I2cStop();												/* send I2C stop						*/
	error += I2cStartWait();								/* send I2C start with delay			*/
	error += I2cWriteWait(BMA150_ADDRESS_READ);				/* send BMA address byte and read bit	*/
	error += I2cRead(&data);								/* read one byte from BMA slave			*/
	error += I2cSendNak();									/* send Nak to BMA slave				*/
	data = 0x01;										/* set bandwidth 25 Hz, range 2G		*/
	
	error += I2cStartWait();								/* send I2C start with delay			*/
	error += I2cWriteWait(BMA150_ADDRESS_WRITE);			/* send BMA address byte and write bit	*/
	error += I2cWrite(BMA150_RA_SMB150_CONF1);				/* send register address for write		*/
	error += I2cWrite(data);								/* write data							*/
	I2cStop();												/* send I2C stop						*/

	RESTORE_GYRO_INT;

	if(error)												/* I2C error occurred?					*/
		return (-1);										/* return error: initialize failed		*/
	my_acc_available = 1;									/* acc is available						*/
	return (0);												/* initialize successfully				*/
}

/****************************************************************************************************/
/* Function verify acceleration sensor I2C connection. Make sure the device is connected and  		*/
/* responds as expected. Return True if connection is valid, false otherwise.						*/
/*																									*/
/* @input value:	(none)			 																*/
/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully		*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char EmoroAccClass::testConnection(void){
	unsigned char data;
	if(AccReadByte(BMA150_RA_CHIP_ID, &data) != 0)		/* Read BMA device ID failed?				*/
		return (0);										/* return 0 								*/
	if(data == 0x02)									/* device id should be 0x02					*/
		return (1);										/* return connection successfully			*/
	return (0);											/* return connection failed					*/
}

/****************************************************************************************************/
/* Function reads 3-axis accelerometer. (BMA axis registers) 										*/
/*																									*/
/* @input value:	(int *x) 		- containter for X-axis acceleration				 			*/
/*					(int *y) 		- containter for Y-axis acceleration					 		*/
/*					(int *z)		- containter for Z-axis acceleration							*/
/* @return value:	(int)			- (0) -> successfully, (-1) - reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroAccClass::read(int *x, int *y, int *z){
	unsigned char buffer[6];
	if(!my_acc_available)									/* acc not available					*/
		return (-1);										/* return error: communication failed	*/
	if(AccReadBytes(BMA150_RA_X_AXIS_LSB, 6, buffer) != 0)	/* read axis failed?					*/
		return (-1);										/* return error: communication failed	*/
	
	/* sort bites for x, y and z axis																*/
	*x = ((((int)buffer[0] >> 6) & 0x03) | ((int)buffer[1] << 2)); 
	*y = ((((int)buffer[2] >> 6) & 0x03) | ((int)buffer[3] << 2));
	*z = ((((int)buffer[4] >> 6) & 0x03) | ((int)buffer[5] << 2));
	
	if((*x>>9)&0x01)	*x |= 0xFC00;						/* sign extension for x axis			*/
	if((*y>>9)&0x01)	*y |= 0xFC00;						/* sign extension for y axis			*/
	if((*z>>9)&0x01)	*z |= 0xFC00;						/* sign extension for z axis			*/
	return (0);												/* return successfully					*/
} 

/****************************************************************************************************/
/* Function returns X axis accelerometer reading.													*/
/*																									*/
/* @input value:	(none) 													 						*/
/* @return value:	(int)	- X-axis acceleration measurment (16-bit 2's complement format)*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroAccClass::readX(void){
	int x;
	unsigned char buffer[2];
	if(!my_acc_available)									/* acc not available					*/
		return (0);											/* return error: communication failed	*/
	if(AccReadBytes(BMA150_RA_X_AXIS_LSB, 2, buffer) != 0)			/* read X axis failed?			*/
		return (0);													/* return 0 for X axis			*/
	x = ((((int)buffer[0] >> 6) & 0x03) | ((int)buffer[1] << 2));	/* sort bites for x axis		*/
	if((x>>9)&0x01)	x |= 0xFC00;									/* sign extension for x axis	*/
	return (x);														/* return x axis				*/
}

/****************************************************************************************************/
/* Function returns Y axis accelerometer reading.													*/
/*																									*/
/* @input value:	(none) 																			*/
/* @return value:	(int)	- Y-axis acceleration measurment (16-bit 2's complement format)			*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroAccClass::readY(void){
	int y;
	unsigned char buffer[2];
	if(!my_acc_available)									/* acc not available					*/
		return (0);											/* return error: communication failed	*/
	if(AccReadBytes(BMA150_RA_Y_AXIS_LSB, 2, buffer) != 0)			/* read Y axis failed?			*/
		return (0);													/* return 0 for Y axis			*/
	y = ((((int)buffer[0] >> 6) & 0x03) | ((int)buffer[1] << 2));	/* sort bites for y axis		*/
	if((y>>9)&0x01)	y |= 0xFC00;									/* sign extension for y axis	*/
	return (y);														/* return y axis				*/
}

/****************************************************************************************************/
/* Function returns Z axis accelerometer reading.													*/
/*																									*/
/* @input value:	(none) 																			*/
/* @return value:	(int)	- Z-axis acceleration measurment (16-bit 2's complement format)			*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroAccClass::readZ(void){
	int z;
	unsigned char buffer[2];
	if(!my_acc_available)									/* acc not available					*/
		return (0);											/* return error: communication failed	*/
	if(AccReadBytes(BMA150_RA_Z_AXIS_LSB, 2, buffer) != 0)			/* read Z axis failed?			*/
		return (0);													/* return 0 for Z axis			*/
	z = ((((int)buffer[0] >> 6) & 0x03) | ((int)buffer[1] << 2));	/* sort bites for z axis		*/
	if((z>>9)&0x01)	z |= 0xFC00;									/* sign extension for z axis	*/
	return (z);														/* return z axis				*/
}

/**************************************** A C C  C L A S S ******************************************/
EmoroAccClass Acc;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
