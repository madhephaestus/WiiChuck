/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_ultrasonic.h														*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Ultrasonic driver for EMoRo 2560									*/
/****************************************************************************************************/
#include <avr\interrupt.h>

#ifndef EMORO_ULTRASONIC_H_
#define EMORO_ULTRAOSNIC_H_

#define GPP_0			(0)
#define GPP_1			(1)
#define GPP_2			(2)
#define GPP_3			(3)
#define GPP_4			(4)
#define GPP_5			(5)
#define GPP_6			(6)
#define GPP_7			(7)


class UltrasonicClass{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize ultrasonic sensor.														*/
	/* Every initialized ultrasonic sensor needs 60 ms to be updated with new value, and if 		*/
	/* application program enable all 8 sensors refresh rate for every sensor will be 8*60ms=480ms.	*/
	/* To increase refresh rate of one sensor in application program stop all unused sensors. 		*/
	/* Use macro GPP_0 - GPP_7 as port attribute.													*/
	/*																								*/
	/* @input value:	(port) 			- ultrasonic port [GPP_0 - GPP_7]. 							*/
	/* @return value:	(int)			- (0) -> successfully, (-1) -> ultrasonic port out of range	*/
	/*----------------------------------------------------------------------------------------------*/
	int attach(unsigned char port);

	/*----------------------------------------------------------------------------------------------*/
	/* Function detach ultrasonic sensor. Use GPP_0 - GPP_7 as port attribute.						*/
	/* Function initialize echo and trig port as input with disabled pull up resistor.				*/
	/*																								*/
	/* @input value:	(port) 			- ultrasonic port [GPP_0 - GPP_7]. 							*/
	/* @return value:	(int)			- (0) -> successfully, (-1) -> ultrasonic port out of range	*/
	/*----------------------------------------------------------------------------------------------*/
	int detach(unsigned char port);

	/*----------------------------------------------------------------------------------------------*/
	/* Function reads ultrasonic sensor. Before using this function ultrasonic sensor must be 		*/
	/* initialized with attach() function. Use macro GPP_0 - GPP_7 as port attribute.				*/
	/*																								*/
	/* @input value:	(port)			-	ultrasonic port [GPP_0 - GPP_7]							*/
	/* @return value:	(int)			-	(-1)	-> port out of range							*/
	/*										(-2)	-> sensor not initialized						*/
	/*										(1-399)	-> sensor distance in cm						*/
	/*										(400)	-> distance out of range						*/
	/*----------------------------------------------------------------------------------------------*/
	int read(unsigned char port);


};

/*************************************** U L T R A S O N I C   C L A S S ****************************/
extern UltrasonicClass Ultrasonic;

#endif /* EMORO_ULTRASONIC_H_																		*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
