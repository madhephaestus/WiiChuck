/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_mag.h																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo magnetometer HMC5883L driver for EMoRo Pro2560					*/
/****************************************************************************************************/
#ifndef EMORO_MAG_H_
#define EMORO_MAG_H_

#ifndef DISABLE_GYRO_INT
#define DISABLE_GYRO_INT	unsigned char temp_int7 = EIMSK; EIMSK &= ~(1 <<INT7)
#endif

#ifndef RESTORE_GYRO_INT
#define RESTORE_GYRO_INT	EIMSK |= temp_int7 & (1 <<INT7) ? (1 <<INT7) : 0
#endif


class EmoroMagClass{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize magnetometer HMC5883L, and must be called before using another 			*/
	/* magnetometer function.																		*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(int)	- (0) -> successfully, (-1) -> initialization failed (I2C error)	*/
	/*----------------------------------------------------------------------------------------------*/
	int init(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function verify mag sensor I2C connection. Make sure the device is connected and  			*/
	/* responds as expected. Return True if connection is valid, false otherwise.					*/
	/*																								*/
	/* @input value:	(none)			 															*/
	/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully	*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned char testConnection(void);

	/*----------------------------------------------------------------------------------------------*/
	/* This function reads direction inclination and strength from compass device.					*/
	/*																								*/
	/* @input value:	(*direction)																*/
	/*					(*inclination)																*/
	/*					(*strength)																	*/
	/* @return value:	(int)			- (0) -> successfully, (-1) -> failed (I2C error)			*/
	/*----------------------------------------------------------------------------------------------*/
	int read(int *direction, int *inclination, int *strength);

	/*----------------------------------------------------------------------------------------------*/
	/* This function reads direction from compass device.											*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(int)	- direction															*/
	/*----------------------------------------------------------------------------------------------*/
	int readDirection(void);
	
	/*----------------------------------------------------------------------------------------------*/
	/* This function reads inclination from compass device.											*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(int)	- inclination														*/
	/*----------------------------------------------------------------------------------------------*/
	int readInclination(void);

	/*----------------------------------------------------------------------------------------------*/
	/* This function reads strength from compass device.											*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(int)	- strength															*/
	/*----------------------------------------------------------------------------------------------*/
	int readStrength(void);

	/*----------------------------------------------------------------------------------------------*/
	/* This routine must be called 3 times total, with the "step" parameter indicating what stage of*/
	/* the calibration is being performed.															*/
	/* Calibration step 1:	Lay board flat!															*/
	/* Calibration step 2:  Rotate 180 degrees!														*/
	/* Calibration step 3:	Flip board!																*/
	/*																								*/
	/* @input value:	(step)			-  The calibration stage number (1 to 3).					*/
	/* @return value:	(int)			- (0)  -> successfully 										*/
	/*									- (-1) -> failed (I2C error)								*/
	/*									- (-2) -> step out of range									*/
	/*----------------------------------------------------------------------------------------------*/
	int calibrate(unsigned char step);
};

/****************************************** M A G   C L A S S ***************************************/
extern EmoroMagClass Mag;

#endif /* EMORO_MAG_H_																				*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
