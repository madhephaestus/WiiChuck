/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_servo.h															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Servo driver for EMoRo Pro2560									*/
/****************************************************************************************************/
#ifndef EMORO_SERVO_H_
#define EMORO_SERVO_H_

class EmoroServoClass{
public:	
	/*----------------------------------------------------------------------------------------------*/
	/* Function initialize servo port and must be called before using write() and read()			*/
	/* functions. SERVO_0 - SERVO_7 macro are defined in pins_arduino.h file. Function initialize	*/
	/* servo port with default pulse 1500us. Use macro SERVO_0-SERVO_7 as port attribute.			*/
	/*																								*/
	/* @input value:	(port) 	- servo port [SERVO_0 - SERVO_7]									*/
	/* @return value:	(int)	- (0) -> successfully, (-1) -> servo port out of range				*/
	/*----------------------------------------------------------------------------------------------*/
	int attach(unsigned char port);					/* attach servo SERVO_0 - SERVO_7				*/
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function detach servo port. SERVO_0 - SERVO_7 are defined in pins_arduino.h file. Before		*/
	/* initializing SERVO port as input or output, and servo is already initialized, servo port must*/
	/* be stoped with detach() function. Function initialize port as input with disabled pull up	*/
	/* resistor. Use macro SERVO_0-SERVO_7 as port attribute.										*/
	/*																								*/
	/* @input value:	(port)  - 	stop servo port [SERVO_0 - SERVO_7]								*/
	/* @return value:	(int)	-	(0) -> successfully. (-1) -> servo port out of range			*/
	/*----------------------------------------------------------------------------------------------*/
	int detach(unsigned char port);					/* detach servo SERVO_0 - SERVO_7				*/
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function sets servo port position. SERVO_0 - SERVO_7 are defined in pins_arduino.h file.		*/
	/* Before using write() function, servo port must be initialized with attach() function.		*/
	/* Use macro SERVO_0 - SERVO_7 as port attribute. Servo position range is 500-2500 us.			*/
	/*																								*/
	/* @input value:	(port)		- servo port [SERVO_0 - SERVO_7]								*/
	/*					(position) 	- servo pulse in us. position range: [500 - 2500] us 			*/
	/* @return value:	(int)		- (0)  -> successfuly 											*/
	/*								- (-1) -> error: servo position out of range					*/
	/*								- (-2) -> error: servo port out of range						*/
	/*								- (-3) -> error: servo not running								*/
	/*----------------------------------------------------------------------------------------------*/
	int write(unsigned char port, int position);	/* set new position SERVO_0 - SERVO_7			*/
	
	/*----------------------------------------------------------------------------------------------*/
	/* Function reads servo position for port SERVO_0 - SERVO_7. Before reading servo position,		*/
	/* servo port must be initialized with attach() function. SERVO_0 - SERVO_7 are defined in		*/
	/* pins_arduino.h file. Use macro SERVO_0 - SERVO_7 as port attribute.							*/
	/*																								*/
	/* @input value: 	(port) 	- servo port [SERVO_0 - SERVO_7]									*/
	/* @return value:	(int)	- (500-2500)	-> servo position 	 								*/
	/*							- (-1)			-> error: servo port out of range					*/
	/*							- (-2)			-> error: servo not running							*/
	/*----------------------------------------------------------------------------------------------*/
	int read(unsigned char port);					/* read position SERVO_0 - SERVO_7				*/
};

/************************************ S E R V O   C L A S S *****************************************/
extern EmoroServoClass EmoroServo;

#endif /* EMORO_SERVO_H_																			*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
