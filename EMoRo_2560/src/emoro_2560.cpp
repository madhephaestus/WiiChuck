/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_2560.c															*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		main initialization file, GLAM EEPROM read and write,					*/
/*							read supply voltage														*/
/****************************************************************************************************/

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_switch.h"
#include "emoro_bluetooth.h"
#include "emoro_lcd.h"
#include "emoro_acc.h"
#include "emoro_gyr.h"
#include "emoro_mag.h"
#include "emoro_i2c.h"
#include "pitches.h"
#include <util\delay.h>
#include <avr\interrupt.h>
#include <string.h>
#include <stdio.h>
#include "emoro_2560.h"
#include "wiring_private.h"
#include "pins_arduino.h"

static int initialized_emoro_hw = 0;

/****************************************************************************************************/
static void WaitSwitch(unsigned char sw){
	int cnt=0;
	while(ReadSwitch(sw)==0);
	while((ReadSwitch(sw)) || (cnt<20)){
		_delay_ms(1);
		if(ReadSwitch(sw)==0)
			cnt++;
		else
			cnt=0;
	}
}
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
unsigned char GLAM_EEPROMReadBytes(unsigned char reg_start, unsigned char bytes, unsigned char *buffer){
	unsigned char data, i;
	signed char error=0;
	
	if( reg_start+bytes >128 )
		return (2);

	cli();
	error += I2cStartWait();						/* send I2C start with deley					*/
	error += I2cWriteWait(GLAM_EEPROM_ADDRESS);		/* send 24AA01 address byte						*/
	error += I2cWriteWait(reg_start);				/* write first address byte for miltiple read	*/
	error += I2cStartWait();						/* send I2C start with deley					*/
    error += I2cWriteWait(GLAM_EEPROM_ADDRESS|0x01);/* send 24AA01 address byte and ¸read bit		*/
	
    for(i=0 ; i<bytes ; i++){						/* read number of bytes							*/
		error += I2cRead(&data);					/* read one byte								*/
		*(buffer+i) = data;							/* store one byte								*/
		if((i+1) == bytes){
			error += I2cSendNak();					/* send NAK to 24AA01 to finish read			*/
            I2cStop();								/* send stop to 24AA01							*/
			break;									/* break for									*/
		}
		else
			error += I2cSendAck();					/* send ACK to 24AA01 slave to read next byte	*/
	}
	sei();
	
	if(error)										/* error occurred?								*/
		return (1);									/* return error (1) -> communication failed		*/
	return (0);										/* return successfully							*/
}
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
unsigned char GLAM_EEPROMWriteBytes(unsigned char reg_start, unsigned char bytes, unsigned char *buffer){
	unsigned char  i;
	signed char error=0;
	
	if( reg_start+bytes >120)
		return (2);									/* it is not allowed modify configuration data	*/
	cli();
	error += I2cStart();							/* send I2C start with deley					*/
	error += I2cWrite(GLAM_EEPROM_ADDRESS);				/* send 24AA01 address byte and write bit	*/
	error += I2cWrite(reg_start);					/* write first address byte for miltiple read	*/
	
	for(i=0 ; i<bytes ; i++){						/* read number of bytes							*/
		error += I2cWrite(*(buffer+i));				/* read one byte								*/
		if((i+1) == bytes){
			I2cStop();								/* send stop to 24AA01							*/
			break;									/* break for									*/
		}
	}
	sei();
	
	if(error)										/* error occurred?								*/
		return (1);									/* return error (1) -> communication failed		*/
	return (0);										/* return successfully							*/
}
/****************************************************************************************************/
/* Function read input voltage on Power supply connector											*/
/* input value:		(none)																			*/
/* return value:	(10bit ADC value 0..1023,														*/
/*					 real voltage is calculated: return value * (Vref/1024) * 3;					*/
/*					 Vref is 5V )																	*/
/*--------------------------------------------------------------------------------------------------*/
int CheckVoltage(void){
	uint8_t low, high;
	InitADC12();		// make ADC12 input, disable  pull up
	ADCSRB = 0x08;		// make ADC on PK4, ADC12
	ADMUX = 0x44;
	// start the conversion
	sbi(ADCSRA, ADSC);
	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));
	// we have to read ADCL first; doing so locks both ADCL
	// and ADCH until ADCH is read.  reading ADCL second would
	// cause the results of each conversion to be discarded,
	// as ADCL and ADCH would be locked when it completed.
	low  = ADCL;
	high = ADCH;

	//combine the two bytes
	return (high << 8) | low;
}
/****************************************************************************************************/
/* Function initialize EMoRo 2560																	*/
/* input value:		(none)																			*/
/* return value:	(initialized hardware)															*/
/*--------------------------------------------------------------------------------------------------*/
int InitEmoro(void){
	int ret_value = 0;
	int ADC_read;
	long Vin, Vin_decimal;

	char lcd[20];
	unsigned char d[7], chk=0;
	
	
	GLAM_EEPROMReadBytes(121,7, d);					/* address 121 -> LCD, 122 -> switches			*/
													/* 123 -> Accelerometer, 124 -> Gyroscope		*/
													/* 125 -> Compas, 126 -> Bluetooth, 127 -> Chk	*/
	for(unsigned char n=0; n<7; n++)				/* calculate checksum							*/
		chk^=*(d+n);
	
	if(chk^0xAA == 0){
		if(d[0] == 0xC8){							/* check LCD									*/
			if(Lcd.init() == 0){					/* initialize lcd								*/
				Lcd.backlightOn();					/* turn on lcd backlight						*/
				ret_value |= (LCD_AVAILABLE);		/* set return value -> LCD Available			*/
				initialized_emoro_hw = ret_value;
			}
		}
		if(d[1] == 0xC8){							/* check switches								*/
			InitSwitch();							/* initialize switch (SW1 - SW4)				*/
			ret_value |= (SW_AVAILABLE);			/* set return value -> Switch Available			*/
		}
		if(Acc.testConnection() & d[2] == 0xC8){	/* test Acc and check 							*/
			Acc.init();								/* initialize acceleration sensor BMA150		*/
			ret_value |= (ACC_AVAILABLE);			/* set return value -> ACC Available			*/
		}
		if(Gyr.testConnection() & d[3] == 0xC8){	/* test Gyr and check 							*/
			Gyr.initBasic();						/* initialize gyroscope L3GD20					*/
			ret_value |= (GYR_AVAILABLE);			/* set return value -> GYR Available			*/
		}
		if(Mag.testConnection() & d[4] == 0xC8){	/* test magnetometer and check 					*/
			Mag.init();								/* initialize magnetometer						*/
			ret_value |= (MAG_AVAILABLE);			/* set return value -> MAG Available			*/
		}
		if(d[5] == 0xC8){
			if(Bluetooth.init() == 0){				/* initialize bluetooth communication			*/				
				ret_value |= (BLUETOOTH_AVAILABLE);	/* set return value -> Bluetooth Available		*/
			}
		}
		
		ADC_read=CheckVoltage();						/* read input voltage				*/
		Vin=(long)(ADC_read*15/1024);					/* calculate voltage; whole number	*/
		Vin_decimal=(long)ADC_read*1500/1024-Vin*100;	/* calculate decimal places			*/
		if( Vin >= 5 & (Vin == 6 & Vin_decimal<20) ){	/* if Vin>=5V & Vin < 6.20V batt is */
			sprintf(lcd, "Vin=%d.%d", (int)Vin, (int)Vin_decimal);
			Serial.begin(9600);				    /* initialize Serial monitor UART 9600 bps	        */
			Serial.println((char*)"Voltage low:");
			Serial.println(lcd);
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();								/* low, for Vin<5V:USB power supply */
				Lcd.printString((char*)"Voltage low:");
				Lcd.locate(1,0);
				Lcd.printString(lcd);
			}
			for (int rep=0; rep<2; rep++){
				tone(BUZ_BUILTIN, NOTE_C6, 50); _delay_ms(100); /* Mores code letter S(...) */
				tone(BUZ_BUILTIN, NOTE_C6, 50); _delay_ms(100);
				tone(BUZ_BUILTIN, NOTE_C6, 50); _delay_ms(200);
					
				tone(BUZ_BUILTIN, NOTE_C6, 150); _delay_ms(200);/* Mores code letter O(---) */
				tone(BUZ_BUILTIN, NOTE_C6, 150); _delay_ms(200);
				tone(BUZ_BUILTIN, NOTE_C6, 150); _delay_ms(300);
					
				tone(BUZ_BUILTIN, NOTE_C6, 50); _delay_ms(100);	/* Mores code letter S(...) */
				tone(BUZ_BUILTIN, NOTE_C6, 50); _delay_ms(100);
				tone(BUZ_BUILTIN, NOTE_C6, 50); _delay_ms(400);
			}
			noTone(BUZ_BUILTIN);
			
			Lcd.clear();
			
		}
	}

	if(ret_value & MAG_AVAILABLE){					
		
		if(ReadSwitch(SW_2) == 1){				/* magnetometer calibration request?				*/
			Serial.begin(9600);
			Serial.println((char*)"Compass");          
			Serial.println((char*)"Calibration.....\n"); 
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();
				Lcd.printString((char*)"Compass         ");
				Lcd.locate(1,0);
				Lcd.printString((char*)"Calibration.....");
			}
			while(ReadSwitch(SW_2)==1);			/* wait button release								*/
			_delay_ms(300);
			
			/* mag calibration step 1																*/
			Serial.println((char*)"Lay board flat");          
			Serial.println((char*)"Press button 1\n");          
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();
				Lcd.printString((char*)"Lay board flat");
				Lcd.locate(1,0);
				Lcd.printString((char*)"Press button 1");
			}
			WaitSwitch(SW_1);	
			Mag.calibrate(1);
			_delay_ms(500);
			
			/* mag calibration step 2																*/
			Serial.println((char*)"Rotate 180 degre");          
			Serial.println((char*)"Press button 1\n");          
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();
				Lcd.printString((char*)"Rotate 180 degre");
				Lcd.locate(1,0);
				Lcd.printString((char*)"Press button 1");						
			}
			WaitSwitch(SW_1);	
			Mag.calibrate(2);
			_delay_ms(300);
			
			/* mag calibration step 2																*/
			Serial.println((char*)"Flip board");          
			Serial.println((char*)"Press button 1\n");          
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();
				Lcd.printString((char*)"Flip board");
				Lcd.locate(1,0);
				Lcd.printString((char*)"Press button 1");						
			}
			WaitSwitch(SW_1);	
			Mag.calibrate(3);
			_delay_ms(300);
			
			Serial.println((char*)"Calibration");          
			Serial.println((char*)"Complete\n");          
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();
				Lcd.printString((char*)"Calibration     ");	
				Lcd.locate(1,0);
				Lcd.printString((char*)"Complete        ");	
			}
			_delay_ms(1500);	
			Serial.println((char*)"Press button 1\n");          
			if(ret_value & LCD_AVAILABLE){
				Lcd.clear();	
				Lcd.locate(1,0);
				Lcd.printString((char*)"Press button 1");
			}
			while(ReadSwitch(SW_1)==0){
				int dir, inc, str;
				Mag.read(&dir, &inc, &str);
				sprintf(lcd, "Direction=%3d", dir);
				Serial.println(lcd);  
				if(ret_value & LCD_AVAILABLE){
					Lcd.locate(0, 0);
					Lcd.printString(lcd);
				}				        
			}
			Lcd.clear();
		}
	}		
	initialized_emoro_hw = ret_value; 
	return initialized_emoro_hw;	
}

/****************************************************************************************************/
/* Function read initialized hardware.																*/
/* input value:		(none)																			*/
/* return value:	(initialized hardware)															*/
/*--------------------------------------------------------------------------------------------------*/
int ReadEmoroHardware(void){
	return initialized_emoro_hw;
}

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
