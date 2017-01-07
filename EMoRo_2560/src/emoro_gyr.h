/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_gyr.c																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Gyroscope L3GD20 driver for EMoRo Pro2560							*/
/****************************************************************************************************/
#ifndef EMORO_GYR_H_
#define EMORO_GYR_H_

#ifndef DISABLE_GYRO_INT
#define DISABLE_GYRO_INT	unsigned char temp_int7 = EIMSK; EIMSK &= ~(1 <<INT7)
#endif

#ifndef RESTORE_GYRO_INT
#define RESTORE_GYRO_INT	EIMSK |= temp_int7 & (1 <<INT7) ? (1 <<INT7) : 0
#endif


class EmoroGyrClass{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize gyroscope L3GD20 in Basic mode (without FIFO and automatic angle	axis	*/
	/* calculation). Must be called before using gyro functions: read(*x, *y, *z), readX(), readY() */
	/* readZ()																						*/
	/* input value:		(none)																		*/
	/* return value:	(int)	- (0) -> successfully, (-1) -> initialization failed				*/
	/*----------------------------------------------------------------------------------------------*/
	int initBasic(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize gyroscope L3GD20 in Basic mode (without FIFO and automatic angle	axis    */
	/* calculations). Gyroscope is initialized with provided output data rate and measuring scale   */
	/* range. Must be called before using gyro functions: read(*x, *y, *z), readX(), readY(), 	    */
	/* readZ()                                                                               		*/
	/*																								*/
	/* @input value:	(unsigned int odr)		- gyro data rate generation	[Hz]					*/
	/*											  possible values: 95, 190, 380 and 760				*/
	/*					(unsigned int range)	- full scale selection	[DPS] (degrees per second)	*/
	/*											  possible values: 250, 500 and 2000				*/
	/* return value:	(int)	- (0) -> successfully, (-1) -> wrong parameters,					*/
	/*							- (-2) -> initialization failed										*/
	/*----------------------------------------------------------------------------------------------*/
	int initBasic(unsigned int odr, unsigned int range);

	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize gyroscope L3GD20 in Advanced mode (with FIFO buffering, filtering data	*/
	/* and automatic angle axis calculations). 														*/
	/* This function must be called befor using gyro functions related with axis angles	data		*/
	/*																								*/
	/* input value:		(none)																		*/
	/* return value:	(int)	- (0) -> successfully, (-1) -> initialization failed				*/
	/*----------------------------------------------------------------------------------------------*/
	int init(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize gyroscope L3GD20 in Advanced mode (with FIFO buffering, filtering data	*/
	/* and automatic angle axis calculations). Gyroscope is initialiyed with default output data 	*/
	/* rate and default measuring scale range. (defined in: L3G_RANGE_DPS and L3G_DATA_RATE_HZ)		*/
	/* This function must be called befor using gyro functions related with axis angles	data		*/
	/*																								*/
	/* @input value:	(unsigned int odr)		- gyro data rate generation	[Hz]					*/
	/*											  possible values: 95, 190, 380 and 760				*/
	/*					(unsigned int range)	- full scale selection	[DPS] (degrees per second)	*/
	/*											  possible values: 250, 500 and 2000				*/
	/* return value:	(int)	- (0) -> successfully, (-1) -> initialization failed				*/
	/*----------------------------------------------------------------------------------------------*/
	int init(unsigned int odr, unsigned int range);


	
	/*----------------------------------------------------------------------------------------------*/
	/* Function stop gyro L3GD20 Advanced mode and save controller time for data and interrupt		*/ 
	/* processing. Can be called after Advanced mode initialization.							 	*/
	/*																								*/
	/* input value:		(none)																		*/
	/* return value:	(int)	- (0) -> successfully, (-1) -> Stop failed							*/
	/*----------------------------------------------------------------------------------------------*/
	int stop(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function verifies gyroscope I2C connection. Make sure the device is connected and responds 	*/
	/* as expected. Return True if connection is valid (read correct data in Device identification	*/
	/* register), false otherwise. Can be used before any gyro initialization.						*/
	/*																								*/
	/* @input value:	(none)			 															*/
	/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully	*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned char testConnection(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function reads calculated all 3-axes angles.					 								*/
	/*																								*/
	/* @input value:	(double *x) 	- containter for X-axis angle			 					*/
	/*					(double *y) 	- containter for Y-axis angle								*/
	/*					(double *z)		- containter for Z-axis angle								*/
	/* @return value:	(int)			- (0) -> successfully, (-1) - reading failed				*/
	/*----------------------------------------------------------------------------------------------*/
	int readDegrees(double *x, double *y, double *z);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function reads calculated X axis angle.					 									*/
	/*																								*/
	/* @input value:	(none)													 					*/
	/* @return value:	(double)		-  X axis angle												*/
	/*----------------------------------------------------------------------------------------------*/
	double readDegreesX(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function reads calculated Y axis angle.					 									*/
	/*																								*/
	/* @input value:	(none)													 					*/
	/* @return value:	(double)		-  Y axis angle												*/
	/*----------------------------------------------------------------------------------------------*/
	double readDegreesY(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function reads calculated Z axis angle.					 									*/
	/*																								*/
	/* @input value:	(none)													 					*/
	/* @return value:	(double)		-  Z axis angle												*/
	/*----------------------------------------------------------------------------------------------*/
	double readDegreesZ(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function sets values of all 3-axes angles as current gyro position.							*/
	/*																								*/
	/* @input value:	(double x) 		- containter for X-axis angle			 					*/
	/*					(double y) 		- containter for Y-axis angle								*/
	/*					(double z)		- containter for Z-axis angle								*/
	/* @return value:	(int)			- (0) -> successfully, (-1) - gyro is not in Advance mode	*/
	/*----------------------------------------------------------------------------------------------*/
	int setDegrees(double x, double y, double z);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function sets values of all 3-axes angles as 0 degrees position.	(Reset all 3-axes angles)	*/
	/*																								*/
	/* @input value:	(none)													 					*/
	/* @return value:	(int)			- (0) -> successfully, (-1) - gyro is not in Advance mode	*/
	/*----------------------------------------------------------------------------------------------*/
	int resetDegrees(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function reads 3-axis gyroscope. (L3GD20 axis registers) 									*/
	/*																								*/
	/* @input value:	(int *x) 		- containter for X-axis angular rate data 					*/
	/*					(int *y) 		- containter for Y-axis angular rate data					*/
	/*					(int *z)		- containter for Z-axis angular rate data					*/
	/* @return value:	(int)			- (0) -> successfully, (-1) - reading failed				*/
	/*----------------------------------------------------------------------------------------------*/
	int read(int *x, int *y, int *z);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function returns X axis gyroscope reading.													*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- X-axis gyroscope measurment (16-bit 2's complement format)		*/
	/*----------------------------------------------------------------------------------------------*/
	int readX(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns Y axis gyroscope reading.													*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- Y-axis gyroscope measurment (16-bit 2's complement format)		*/
	/*----------------------------------------------------------------------------------------------*/
	int readY(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns Z axis gyroscope reading.													*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- Z-axis gyroscope measurment (16-bit 2's complement format)		*/
	/*----------------------------------------------------------------------------------------------*/
	int readZ(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns number of calculated angular data rates of X axis which pass all filtering. */
	/* This counter data can be used for filter tuning. While stationary position changing of		*/
	/* this counter should be as minimum as possible. 					 	  						*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(unsigned long)	- numbers of calculated X-axis angular data values			*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned long readCounterX(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns number of calculated angular data rates of Y axis which pass all filtering. */
	/* This counter data can be used for filter tuning. While stationary position changing of		*/
	/* this counter should be as minimum as possible. 					 	  						*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(unsigned long)	- numbers of calculated Y-axis angular data values			*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned long readCounterY(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns number of calculated angular data rates of Z axis which pass all filtering. */
	/* This counter data can be used for filter tuning. While stationary position changing of		*/
	/* this counter should be as minimum as possible. 					 	  						*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(unsigned long)	- numbers of calculated Z-axis angular data values			*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned long readCounterZ(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns state of DRDY signal from gyro. This signal is used for interrupt generation*/
	/* in Advanced mode when FIFO level is over watermark level.									*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(unsigned char)		- (0) -> DRDY/INT2 low level, (1) - DRDY/INT2 high level*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned char Dready(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function calculates dc_offset drift and noise of all 3 axes. Function can be used only in	*/
	/* Advanced mode. While calibration, gyro should be without movement.							*/
	/*																								*/
	/* @input value:	(unsigned int)		- number of samples which are used for calculation		*/
	/* @return value:	(int)				- (0) -> successfully, (-1) - gyr is't in Advanced mode */
	/*										  (-2) - calibration failed								*/
	/*----------------------------------------------------------------------------------------------*/
	int Calibration(unsigned int samples);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function returns calculated dc_offset for X axis.											*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- dc_offset for X axis												*/
	/*----------------------------------------------------------------------------------------------*/
	int dc_offsetX(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns calculated dc_offset for Y axis.											*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- dc_offset for Y axis												*/
	/*----------------------------------------------------------------------------------------------*/
	int dc_offsetY(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns calculated dc_offset for Z axis.											*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(int)	- dc_offset for Z axis												*/
	/*----------------------------------------------------------------------------------------------*/
	int dc_offsetZ(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns calculated noise for X axis.												*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(double)	- noise for X axis												*/
	/*----------------------------------------------------------------------------------------------*/
	double noiseX(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function returns calculated noise for Y axis.												*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(double)	- noise for Y axis												*/
	/*----------------------------------------------------------------------------------------------*/
	double noiseY(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function returns calculated noise for Z axis.												*/
	/*																								*/
	/* @input value:	(none) 																		*/
	/* @return value:	(double)	- noise for Z axis												*/
	/*----------------------------------------------------------------------------------------------*/
	double noiseZ(void);
};

/*************************************** G Y R   C L A S S ******************************************/
extern EmoroGyrClass Gyr;

#endif /* EMORO_GYR_H_																				*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
