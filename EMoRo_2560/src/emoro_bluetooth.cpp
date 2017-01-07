/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_bluetooth.c														*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Bluetooth driver for EMoRo Pro2560								*/
/****************************************************************************************************/

#ifndef F_CPU
	#define F_CPU				16000000UL
#endif
/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include <avr\io.h>
#include <util\delay.h>
#include "emoro_bluetooth.h"
#include "emoro_switch.h"
#include "emoro_2560.h"
#include "emoro_lcd.h"
#include "HardwareSerial.h"

/*---------------------------- B L U E T O O T H   I O   M A C R O ---------------------------------*/
#define InitBtReset()			(DDRD |= (1<<PD4), PORTD &=~(1<<PD4))	/* PD4-> out, set low		*/
#define InitBtMode()			(DDRD |= (1<<PD6), PORTD &=~(1<<PD6))	/* PD6-> out, set low		*/
#define InitBtConnection()		(DDRD &=~(1<<PD5), PORTD &=~(1<<PD5))	/* PD5-> in, disable pull up*/

#define BtResetOn()				(PORTD &=~(1<<PD4))		/* reset  PD4-> pull bluetooth reset low	*/
#define BtResetOff()			(PORTD |= (1<<PD4))		/* set    PD4-> pull bluetooth reset high	*/
#define SetCrm()				(PORTD |= (1<<PD6))		/* set    PD6-> set command response mode	*/
#define SetAcm()				(PORTD &=~(1<<PD6))		/* reset  PD6-> set auto connection mode	*/

#define BtConnected()			(((PIND&(1<<PD5))==0)?0:1)	/* PD5 high? return TRUE				*/
#define BtNotConnected()		(((PIND&(1<<PD5))==0)?1:0)	/* PD5 low? return TRUE					*/
/*--------------------------------------------------------------------------------------------------*/

/*----------------------------------- D E C L A R A T I O N ----------------------------------------*/
char bt_module_name[32];
char bt_module_passkey[16];
static unsigned char my_bluetooth_available=0;
/*--------------------------------------------------------------------------------------------------*/

/****************************************************************************************************/
void FlushUartRx(void){
	while(Serial1.available())
		Serial1.read();
}

/****************************************************************************************************/
/* Function resets bluetooth module.																*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(none)																			*/
/*--------------------------------------------------------------------------------------------------*/
static void ResetBluetoothHardware(void){
	BtResetOn();								/* pull bluetooth reset low							*/
	_delay_ms(100);								/* delay reset time									*/
	BtResetOff();								/* pull bluetooth reset high						*/
	_delay_ms(800);								/* wait min 600-800ms. (bluetooth start time)		*/
}

/****************************************************************************************************/
/* Function initializes bluetooth module. If button "SW1" is activated, bluetooth module 			*/
/* will be set to default settings. (Name: EMoRo 2560, Passkey:0000, Baudrate: 38400 bps)			*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(int)				-	(0) -> successfully, (-1) -> Communication failed		*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroBluetoothClass::init(void){
	unsigned char i=0;
	unsigned int my_t;
	char buf[16];
	InitBtReset();							/* initialize Bluetooth IO								*/
	InitBtMode();
	InitBtConnection();

	InitSwitch();							/* initialize EMoRo AMG switches (SW1 - SW4)			*/
	Serial1.begin(38400);					/* init UART1: 38400 bps, 8 data bits, 1 stop bit		*/
	
	/*----------------------- Start Bluetooth module with command response mode --------------------*/
	SetCrm();								/* set Bluetooth command response mode 					*/
	ResetBluetoothHardware();				/* reset Bluetooth module								*/
	FlushUartRx();
	Serial1.flush();						/* flush Rx1 buffer										*/

	/*------------------------------------ Get Bluetooth name --------------------------------------*/
	Serial1.print("AT+NAME?\r\n");			/* get bluetooth module name							*/
	my_t=0;
	while(i<6){								/* ignore +NAME: received from bluetooth module			*/
		while(Serial1.available()==0){
			_delay_ms(1);
			if(++my_t > 200){				/* timeout -> bluetooth not found						*/
				Serial.end();
				return -1;
			}
		}
		buf[i] = Serial1.read();			/* get one byte from UART1								*/
		i++;
	}
	buf[6]=0;								/* force end of string									*/
	if(strcmp(buf, "+NAME:") == 0){			/* received +NAME:?										*/
		my_bluetooth_available = 1;			/* set bluetooth available								*/
	}
	else{
		Serial1.end();
		return -1;							/* bluetooth not available								*/
	}

	i=0;									/* reset control variable i								*/
	while(i<32){							/* max name = 32 bytes									*/
		while(Serial1.available()==0);
		bt_module_name[i] = Serial1.read();	/* read one char										*/
		if(bt_module_name[i] == 13){		/* end of string?										*/
			bt_module_name[i] = 0;			/* set end of string in my name variable				*/
			i = 32;							/* module name -> complete								*/
		}		
		else
			i++;							/* get next char										*/
	}
	i=0;									/* reset control variable								*/
	while(i<5){								/* ignore 0x10 'O' 'K' 0x13 0x10 from bluetooth module	*/
		while(Serial1.available()==0);
		Serial1.read();						/* get one byte											*/
		i++;											
	}
	FlushUartRx();

	/*------------------------------------ Get Bluetooth passkey -----------------------------------*/
	Serial1.print("AT+PSWD?\r\n");			/* get bluetooth passkey								*/
	i=0;									/* reset control varijable								*/
	while(i<6){								/* ignore "+PSWD:" received from bluetooth module		*/
		while(Serial1.available()==0);
		Serial1.read();						/* get one byte											*/
		i++;											
	}
	i=0;									/* reset control variable i								*/
	while(i<32){							/* max passkey = 32 bytes								*/
		while(Serial1.available()==0);
		bt_module_passkey[i] = Serial1.read();	/* read one char									*/
		if(bt_module_passkey[i] == 13){		/* end of string?										*/
			bt_module_passkey[i] = 0;		/* set end of string in my passkey variable				*/
			i = 32;							/* module passkey -> complete							*/
		}		
		else
			i++;							/* get next char										*/
	}
	i=0;									/* reset control variable								*/
	while(i<5){								/* ignore 0x10 'O' 'K' 0x13 0x10 from bluetooth module	*/
		while(Serial1.available()==0);
		Serial1.read();						/* get one byte											*/
		i++;											
	}
	FlushUartRx();

	/* Bluetooth Format requested with SW button from EMoRo AMG?									*/
	if(ReadSwitch(SW_1)){								
		char lcd[20];
		if(ReadEmoroHardware() & LCD_AVAILABLE){							/* if LCD available	    */
			Lcd.locate(0,0);						/* set lcd position 0,0 						*/
			Lcd.printString((char*)"Bluetooth Format");	/* print bluetooth format message				*/
			Lcd.locate(1,0);
			Lcd.printString((char*)"Please Wait.....");
		}
		Serial.begin(9600);
		Serial.println((char*)"Bluetooth Format");
		Serial.println((char*)"Please Wait.....\n");
			
		strcpy(bt_module_name,		"EMoRo 2560");
		strcpy(bt_module_passkey,	"0000");
		
		Serial1.print("AT+NAME=EMoRo 2560\r\n");		/* set default bluetooth name				*/
		_delay_ms(200);						/* wait a bluetooth module to execute commands (200ms)	*/
	
		Serial1.print("AT+PSWD=0000\r\n");	/* set default bluetooth passkey "0000"					*/
		_delay_ms(200);						/* wait a bluetooth module to execute commands (200ms)	*/
	
		Serial1.print("AT+UART=38400,0,0\r\n");			/* set default bluetooth baudrate 38400		*/
		_delay_ms(200);						/* wait a bluetooth module to execute commands (200ms)	*/

		Serial1.print("AT+ROLE=0\r\n");		/* set default bluetooth role	0						*/
		_delay_ms(200);						/* wait a bluetooth module to execute commands (200ms)	*/

		Serial1.flush();					/* flush UART1 receive buffer							*/
		
		_delay_ms(2200);					/* show format message 3000 ms							*/
		
		if(ReadEmoroHardware() & LCD_AVAILABLE){							/* if LCD available	    */
			Lcd.clear();						/* clear LCD										*/
			Lcd.locate(0,0);					/* set lcd position 0,0 							*/
			Lcd.printString((char*)"New Name:");		/* print new name									*/
			Lcd.locate(1,0);					/* set lcd position 1,0								*/
			Lcd.printString(bt_module_name);	/* show bluetooth name								*/
		}
		Serial.println("New Name:");
		Serial.println(bt_module_name);
		_delay_ms(3000);					/* show bluetooth name 3000 ms							*/
		
		if(ReadEmoroHardware() & LCD_AVAILABLE){							/* if LCD available	    */
			Lcd.clear();						/* clear LCD										*/
			Lcd.locate(0,0);					/* set lcd position 0,0 							*/
			Lcd.printString((char*)"New Passkey:");	/* print new passkey								*/
			Lcd.locate(1,0);					/* set lcd position 1,0								*/
			Lcd.printString(bt_module_passkey);	/* show bluetooth passkey							*/
		}
		Serial.println("\nNew Passkey:");
		Serial.println(bt_module_passkey);
		_delay_ms(3000);					/* show bluetooth passkey 3000 ms						*/
		Lcd.clear();						/* clear LCD											*/
	}
	
	/*----------------------- Start Bluetooth module with auto connection mode ---------------------*/
	SetAcm();								/* set bluetooth auto connection mode					*/
	FlushUartRx();
	ResetBluetoothHardware();				/* reset Bluetooth module								*/
	return (0);								/* return successfully									*/
}

/****************************************************************************************************/
/* Function verify bluetooth connection. Make sure the device is connected and  					*/
/* responds as expected. Return True if connection is valid, false otherwise.						*/
/*																									*/
/* @input value:	(none)			 																*/
/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully		*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char EmoroBluetoothClass::testConnection(void){
	return my_bluetooth_available;
}

/****************************************************************************************************/
/* Function change bluetooth name. If successfully changed, function returns (0), otherwise 		*/
/* returns (-1). 																					*/
/*																									*/
/* @input value:	(char* new_name)	- char pointer to new bluetooth name						*/
/* @return value:	(int)				- (0) -> successfully, (-1) -> error: name not supported	*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroBluetoothClass::changeName(char* new_name){
	unsigned char string[3], i;
	char buf[64];
	
	if(my_bluetooth_available == 0)				/* bluetooth not available							*/
		return -1;

	/*-------------------- Start Bluetooth module with command response mode -----------------------*/
	SetCrm();								/* set Bluetooth command response mode 					*/
	ResetBluetoothHardware();				/* reset bluetooth module								*/
	
	sprintf(buf, "AT+NAME=%s\r\n", new_name);		/* change bluetootu name						*/
	Serial1.print(buf);
	_delay_ms(200);							/* wait a bluetooth module to execute commands (200ms)	*/
	
	/*-------------------- Start Bluetooth module with auto connection mode ------------------------*/
	SetAcm();								/* set bluetooth auto connection mode					*/
	ResetBluetoothHardware();				/* reset bluetooth module								*/
	
	string[0] = 0; string[1] = 0;			/* reset first 2 byte									*/
	for(i=0;i<2;i++){						/* try read 2 bytes from UART1 incoming buffer			*/
		if(Serial1.available() != 0)		/* UART1 buffer not empty?								*/
			string[i] = Serial1.read();		/* Get char from UART1 buffer							*/
	}	
	Serial1.flush();						/* Flush UART1 receive buffer							*/
	FlushUartRx();

	if(memcmp(string, "OK", 2) == 0){		/* compare bluetooth response with the result OK.		*/
		strcpy(bt_module_name, new_name);
		return (0);							/* return -> change name successfully					*/
	}	
	return (-1);							/* return error -> name not supported					*/
}

/****************************************************************************************************/
/* Function change bluetooth passkey. If successfully changed, function returns (0), otherwise  	*/
/* returns (-1). 																					*/
/*																									*/
/* @input value:	(char* new_passkey)	- char pointer to new bluetooth passkey						*/
/* @return value:	(int)				- (0) -> successfully, (-1) -> error: passkey not supported	*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroBluetoothClass::changePasskey(char* new_passkey){
	unsigned char string[3], i;
	char buf[64];
	
	if(my_bluetooth_available == 0)				/* bluetooth not available							*/
		return -1;

	/*-------------------- Start Bluetooth module with command response mode -----------------------*/
	SetCrm();								/* set bluetooth command response mode 					*/
	ResetBluetoothHardware();				/* reset bluetooth module								*/

	sprintf(buf, "AT+PSWD=%s\r\n", new_passkey);	/* set new passkey 								*/
	Serial1.print(buf);
	_delay_ms(200);							/* wait a bluetooth module to execute commands (200ms)	*/
	
	/*-------------------- Start Bluetooth module with auto connection mode ------------------------*/
	SetAcm();								/* set bluetooth auto connection mode					*/
	ResetBluetoothHardware();				/* reset bluetooth module								*/
	
	string[0] = 0; string[1] = 0;			/* reset first 2 byte									*/
	for(i=0;i<2;i++){						/* try read 2 bytes from UART1 incoming buffer			*/
		if(Serial1.available() != 0)		/* UART1 buffer not empty?								*/
			string[i] = Serial1.read();		/* Get char from UART1 buffer							*/
	}	
	Serial1.flush();						/* Flush UART1 receive buffer							*/
	FlushUartRx();

	if(memcmp(string, "OK", 2) == 0){		/* compare bluetooth response with the result OK.		*/
		strcpy(bt_module_passkey, new_passkey);
		return (0);							/* return -> change passkey successfully				*/
	}	
	return (-1);							/* return error -> passkey not supported				*/
}

/****************************************************************************************************/
/* Function changes bluetooth module passkey and name.					  							*/
/*																									*/
/* @input value:	(char* new_name)	- char pointer to new bluetooth name						*/
/*					(char* new_passkey)	- char pointer to new bluetooth passkey						*/
/* @return value:	(int)				- (0) -> successfully, (-1) -> failed						*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroBluetoothClass::changeNameAndPasskey(char* new_name, char* new_passkey){
	unsigned char string[3], i, error = 0;
	char buf[64];
	
	if(my_bluetooth_available == 0)				/* bluetooth not available							*/
		return -1;

	/*-------------------- Start Bluetooth module with command response mode -----------------------*/
	SetCrm();								/* set bluetooth command response mode 					*/
	ResetBluetoothHardware();				/* reset bluetooth module								*/
	
	/*---------------------------------- Change Bluetooth name -------------------------------------*/
	sprintf (buf, "AT+NAME=%s\r\n", new_name);		/* change bluetooth name						*/
	Serial1.print(buf);
	_delay_ms(200);							/* wait a bluetooth module to execute commands (200ms)	*/
	
	string[0] = 0; string[1] = 0;			/* reset first 2 byte									*/
	for(i=0;i<2;i++){						/* try read 2 bytes from UART1 incoming buffer			*/
		if(Serial1.available() != 0)		/* UART1 buffer not empty?								*/
			string[i] = Serial1.read();		/* get char from UART1 buffer							*/
	}	
	Serial1.flush();						/* flush UART1 receive buffer							*/
	FlushUartRx();

	if(memcmp(string, "OK", 2) != 0)		/* compare bluetooth response with the result OK.		*/
		error++;							/* Change name failed									*/
	else
		strcpy(bt_module_name, new_name);
		
	/*---------------------------------- Change Bluetooth passkey ----------------------------------*/
	sprintf(buf, "AT+PSWD=%s\r\n", new_passkey);	/* set new passkey 								*/
	Serial1.print(buf);
	_delay_ms(200);							/* wait a bluetooth module to execute commands (200ms)	*/
	
	string[0] = 0; string[1] = 0;			/* reset first 2 byte									*/
	for(i=0;i<2;i++){						/* try read 2 bytes from UART1 incoming buffer			*/
		if(Serial1.available() != 0)		/* UART1 buffer not empty?								*/
			string[i] = Serial1.read();		/* get char from UART1 buffer							*/
	}	
	Serial1.flush();						/* flush UART1 receive buffer							*/
	FlushUartRx();

	if(memcmp(string, "OK", 2) != 0)		/* compare bluetooth response with the result OK.		*/
		error++;							/* change passkey failed								*/
	else
		strcpy(bt_module_passkey, new_passkey);

	/*-------------------- Start Bluetooth module with auto connection mode ------------------------*/
	SetAcm();								/* set bluetooth auto connection mode					*/
	ResetBluetoothHardware();				/* reset bluetooth module								*/
	
	if(error)
		return (-1);						/* return error -> change name or passkey failed		*/
	return (0);								/* return -> change name and passkey successfully		*/
}

/****************************************************************************************************/
/* Function returns current bluetooth name.															*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(char*)	- pointer to bluetooth name												*/
/*--------------------------------------------------------------------------------------------------*/
char* EmoroBluetoothClass::readName(void){	
	return (bt_module_name);
}

/****************************************************************************************************/
/* Function returns current bluetooth passkey.														*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(char*)	- pointer to bluetooth passkey											*/
/*--------------------------------------------------------------------------------------------------*/
char* EmoroBluetoothClass::readPasskey(void){	
	return (bt_module_passkey);
}

/****************************************************************************************************/
/* Function returns current blueooth connection state.												*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(unsigned char) - bluetooth connection state (0-Disconnected, 1-Connected)		*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char EmoroBluetoothClass::connection(void){
	if(my_bluetooth_available == 0)				/* bluetooth not available							*/
		return 0;
	return BtConnected();						/* return 0 if bluetooth module disconnected		*/
}

/************************************ B L U E T O O T H   C L A S S *********************************/
EmoroBluetoothClass Bluetooth;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/

