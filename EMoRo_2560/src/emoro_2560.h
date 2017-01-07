/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_2560.h															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		main initialization file												*/
/****************************************************************************************************/
#ifndef EMORO_2560_H_
#define EMORO_2560_H_

/*--------------------------------------------------------------------------------------------------*/
/* Return value bit MASK (Supported Hardware) for InitEmoro nad ReadEmoroHardware()					*/
#define LCD_AVAILABLE					(0x01)
#define SW_AVAILABLE					(0x02)
#define BLUETOOTH_AVAILABLE				(0x04)
#define ACC_AVAILABLE					(0x08)
#define GYR_AVAILABLE					(0x10)
#define MAG_AVAILABLE					(0x20)

/*--------------------------------------------------------------------------------------------------*/
/* Starting address of GLAM configuration data														*/
#define GLAM_EEPROM_ADDRESS 0xA0

/*--------------------------------------------------------------------------------------------------*/
/* Init pin PK4 (ADC 12) as input without pull up													*/
#define InitADC12()				DDRK &=~(1<<PK4); PORTK &= ~(1<<PK4)

/****************************************************************************************************/
/* Function read data from configuration EEPROM in GLAM module. Memory locations from address 0 to	*/
/* 120 are free for general use. Locations from 121 to 127 are configuration setings for GLAM module*/
/* input value:		(reg_start	->	first address to write data										*/
/*					 bytes		->	number of bytes to write										*/
/*					 *buffer	->  pointer to data array which contain data to write )				*/
/* return value:	(0	->	execution successfuly													*/
/*					 1  ->  communication failed													*/
/*					 2  ->  address out of range													*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char GLAM_EEPROMReadBytes(unsigned char reg_start, unsigned char bytes, unsigned char *buffer);

/****************************************************************************************************/
/* Function write data to configuration EEPROM in GLAM module. Memory locations from address 0 to	*/
/* 120 are free for general use. Locations from 121 to 127 are configuration setings for GLAM module*/
/* input value:		(reg_start	->	first address to write data										*/
/*					 bytes		->	number of bytes to write										*/
/*					 *buffer	->  pointer to data array which contain data to write )				*/
/* return value:	(0	->	execution successfuly													*/
/*					 1  ->  communication failed													*/
/*					 2  ->  address out of range													*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char GLAM_EEPROMWriteBytes(unsigned char reg_start, unsigned char bytes, unsigned char *buffer);

/****************************************************************************************************/
/* Function read input voltage on Power supply connector											*/
/* input value:		(none)																			*/
/* return value:	(10bit ADC value 0..1023,														*/
/*					 real input voltage is calculated as: return value * (Vref/1024) * 3;			*/
/*					 Vref is 5V )																	*/
/*--------------------------------------------------------------------------------------------------*/
int CheckVoltage(void);

/*--------------------------------------------------------------------------------------------------*/
/* Function initialize EMoRo 2560																	*/
/* input value:		(none)																			*/
/* return value:	(initialized hardware)															*/
int InitEmoro(void);
/*--------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------*/
/* Function read initialized hardware.																*/
/* input value:		(none)																			*/
/* return value:	(initialized hardware)															*/
int ReadEmoroHardware(void);
/*--------------------------------------------------------------------------------------------------*/

#endif /* EMORO_2560_H_																				*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
