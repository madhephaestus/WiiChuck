/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_bluetooth.h														*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Bluetooth driver for EMoRo Pro2560								*/
/****************************************************************************************************/
#ifndef EMORO_BLUETOOTH_H_
#define EMORO_BLUETOOTH_H_

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include <stdio.h>
#include <string.h>			

class EmoroBluetoothClass{
public:
	/*----------------------------------------------------------------------------------------------*/
	/* Function initializes bluetooth module. If button "SW1" is activated, bluetooth module 		*/
	/* will be set to default settings. (Name: EMoRo 2560, Passkey:0000, Baudrate: 38400 bps)		*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(int)				-	(0) -> successfully, (-1) -> Communication failed	*/
	/*----------------------------------------------------------------------------------------------*/
	int init(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function verify bluetooth connection. Make sure the device is connected and  				*/
	/* responds as expected. Return True if connection is valid, false otherwise.					*/
	/*																								*/
	/* @input value:	(none)			 															*/
	/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully	*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned char testConnection(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function change bluetooth name. If successfully changed, function returns (0), otherwise 	*/
	/* returns (-1). 																				*/
	/*																								*/
	/* @input value:	(char* new_name)	- char pointer to new bluetooth name					*/
	/* @return value:	(int)				- (0) -> successfully, (-1) -> error: name not supported*/
	/*----------------------------------------------------------------------------------------------*/
	int changeName(char* new_name);

	/*----------------------------------------------------------------------------------------------*/
	/* Function change bluetooth passkey. If successfully changed, function returns (0), otherwise  */
	/* returns (-1). 																				*/
	/*																								*/
	/* @input value:	(char* new_passkey)	- char pointer to new bluetooth passkey					*/
	/* @return value:	(int)			- (0) -> successfully, (-1) -> error: passkey not supported	*/
	/*----------------------------------------------------------------------------------------------*/
	int changePasskey(char* new_passkey);

	/*----------------------------------------------------------------------------------------------*/
	/* Function changes bluetooth module passkey and name.									 		*/
	/*																								*/
	/* @input value:	(char* new_name)	- char pointer to new bluetooth name					*/
	/*					(char* new_passkey)	- char pointer to new bluetooth passkey					*/
	/* @return value:	(int)				- (0) -> successfully, (-1) -> failed					*/
	/*----------------------------------------------------------------------------------------------*/
	int changeNameAndPasskey(char* new_name, char* new_passkey);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns current bluetooth name.														*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(char*) - pointer to bluetooth name											*/
	/*----------------------------------------------------------------------------------------------*/
	char* readName(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns current bluetooth passkey.													*/
	/*																								*/
	/* @input value:	(none)																		*/
	/* @return value:	(char*)	- pointer to bluetooth passkey										*/
	/*----------------------------------------------------------------------------------------------*/
	char* readPasskey(void);

	/*----------------------------------------------------------------------------------------------*/
	/* Function returns current blueooth connection state.											*/
	/*																								*/
	/*	@input value:	(none)																		*/
	/* @return value:	(unsigned char) - bluetooth connection state (0-Disconnected, 1-Connected)	*/
	/*----------------------------------------------------------------------------------------------*/
	unsigned char connection(void);
};

/************************************ B L U E T O O T H   C L A S S *********************************/
extern EmoroBluetoothClass Bluetooth;

#endif /* EMORO_BLUETOOTH_H_																		*/
/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
