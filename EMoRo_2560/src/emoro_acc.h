/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_acc.h																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo acceleration sensor BMA150 driver for EMoRo Pro2560				*/
/****************************************************************************************************/
#ifndef EMORO_ACC_H_
#define EMORO_ACC_H_

#ifndef DISABLE_GYRO_INT
#define DISABLE_GYRO_INT	unsigned char temp_int7 = EIMSK; EIMSK &= ~(1 <<INT7)
#endif

#ifndef RESTORE_GYRO_INT
#define RESTORE_GYRO_INT	EIMSK |= temp_int7 & (1 <<INT7) ? (1 <<INT7) : 0
#endif


class EmoroAccClass{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize acceleration sensor BMA150, and must be called before using another 		*/
	/* acc function except AccTestConnection().														*/
	/*																								*/
	/* input value:		(none)																		*/
	/* return value:	(int)			- (0) -> successfully, (-1) -> initialization failed		*/
	/*----------------------------------------------------------------------------------------------*/
	int init(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function verify acceleration sensor I2C connection. Make sure the device is connected and  	*/
	/* responds as expected. Return True if connection is valid, false otherwise.					*/
	/*																								*/
	/* @input value:	(none)			 															*/
	/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully	*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned char testConnection(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function reads 3-axis accelerometer. (BMA axis registers) 									*/
	/*																								*/
	/* @input value:	(int *x) 			- containter for X-axis acceleration					*/
	/*					(int *y) 			- containter for Y-axis acceleration		 			*/
	/*					(int *z)			- containter for Z-axis acceleration					*/
	/* @return value:	(int)				- (0) -> successfully, (-1) - reading failed			*/
	/*----------------------------------------------------------------------------------------------*/
	int read(int *x, int *y, int *z);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns X axis accelerometer reading.												*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)		- X-axis acceleration measurment (16-bit 2's complement format)	*/
	/*----------------------------------------------------------------------------------------------*/
	int readX(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns Y axis accelerometer reading.												*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)		- Y-axis acceleration measurment (16-bit 2's complement format)	*/
	/*----------------------------------------------------------------------------------------------*/
	int readY(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns Z axis accelerometer reading.												*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)		- Z-axis acceleration measurment (16-bit 2's complement format)	*/
	/*----------------------------------------------------------------------------------------------*/
	int readZ(void);
};

/**************************************** A C C  C L A S S ******************************************/
extern EmoroAccClass Acc;

#endif /* EMORO_ACC_H_																				*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
