/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_gyr.c																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo Gyroscope L3GD20 driver for EMoRo Pro2560							*/
/****************************************************************************************************/

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_gyr.h"
#include "emoro_i2c.h"
#include <avr\io.h>
#include <util/delay.h>
#include <avr\interrupt.h>

/*-------------------------------------- S E T  U P ------------------------------------------------*/
#define NUM_SAMPLES			100				/* Number of samples for calculating DC offset			*/
#define L3G_RANGE_DPS		250				/* 250 DPS, 500 DPS or 2000 DPS							*/
#define L3G_DATA_RATE_HZ	190				/* 95 Hz, 190 Hz, 380 Hz or 760 Hz						*/

/*--------------------------------------- M A C R O ------------------------------------------------*/
#define L3GD20_ADDRESS_WRITE			0xD6
#define L3GD20_ADDRESS_READ				0xD7

/*---------------------------------- REGISTER ADDRESSES --------------------------------------------*/
#define L3G_WHO_AM_I		0x0F

#define L3G_CTRL_REG1		0x20
#define L3G_CTRL_REG2		0x21
#define L3G_CTRL_REG3		0x22
#define L3G_CTRL_REG4		0x23
#define L3G_CTRL_REG5		0x24
#define L3G_REFERENCE		0x25
#define L3G_OUT_TEMP		0x26
#define L3G_STATUS_REG		0x27

#define L3G_OUT_X_L			0x28
#define L3G_OUT_X_H			0x29
#define L3G_OUT_Y_L			0x2A
#define L3G_OUT_Y_H			0x2B
#define L3G_OUT_Z_L			0x2C
#define L3G_OUT_Z_H			0x2D

#define L3G_FIFO_CTRL_REG	0x2E
#define L3G_FIFO_SRC_REG	0x2F

#define L3G_INT1_CFG		0x30
#define L3G_INT1_SRC		0x31
#define L3G_INT1_THS_XH		0x32
#define L3G_INT1_THS_XL		0x33
#define L3G_INT1_THS_YH		0x34
#define L3G_INT1_THS_YL		0x35
#define L3G_INT1_THS_ZH		0x36
#define L3G_INT1_THS_ZL		0x37
#define L3G_INT1_DURATION	0x38



#define InitDrdy()				DDRE &=~(1<<PE7); PORTE |= (1<<PE7)
#define IsDrdyLow()				((PINE>>PE7)&0x01 ? 0:1)
#define IsDrdyHigh()			((PINE>>PE7)&0x01 ? 1:0)
#define Drdy()					(PINE>>PE7)&0x01 


unsigned char my_gyr_available;	

unsigned char gyr_buffer[6];

static struct{
	long dc_offsetX;
	long dc_offsetY;
	long dc_offsetZ;

	double noiseX;
	double noiseY;
	double noiseZ;

	double rateX;
	double rateY;
	double rateZ;

	double prev_rateX;
	double prev_rateY;
	double prev_rateZ;

	double angleX;
	double angleY;
	double angleZ;

	unsigned long dataCounterX;
	unsigned long dataCounterY;
	unsigned long dataCounterZ;
	
	unsigned int odr;					/* output data range */
	unsigned int dpsRange;				/* Degrees per second range */

}gyrData;




/****************************************************************************************************/
/* Function reads one byte from L3GD20 chip. Reg attribute is used for register address, and *data	*/
/* will contain readed byte if reading is successfully.												*/
/*																									*/
/* @input value:	(reg)			- register address												*/
/*					(*data)			- data for readed byte from register address					*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char GyrReadByte(unsigned char reg, unsigned char *data){
	signed char error=0;
	
	DISABLE_GYRO_INT;

	error += I2cStartWait();							/* send I2C start with deley				*/
	error += I2cWriteWait(L3GD20_ADDRESS_WRITE);		/* send L3GD20 address byte and write bit	*/
	error += I2cWrite(reg);								/* send register address for read			*/
	error += I2cStartWait();							/* send I2C start with deley				*/
	error += I2cWriteWait(L3GD20_ADDRESS_READ);			/* send L3GD20 address byte and read bit	*/
	error += I2cRead(data);								/* read one byte from L3GD20 slave			*/
	I2cStop();											/* send stop to L3GD20						*/
	
	RESTORE_GYRO_INT;

	if(error)											/* error occurred?							*/
		return (1);										/* return error (1) -> communication failed	*/
	return (0);											/* return successfully						*/
}

/****************************************************************************************************/
/* Function reads bytes from L3GD20 chip. Reg_start points to address of the first byte for			*/
/* reading, and number of readed bytes will be stored in attribute buffer.							*/
/*																									*/
/* @input value:	(reg_start)		- first register address										*/
/*					(bytes)			- number of bytes for reading									*/
/*					(*buffer)		- buffer for readed bytes										*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char GyrReadBytes(unsigned char reg_start, unsigned char bytes, unsigned char *buffer){
	unsigned char data, i;
	signed char error=0;
	
	DISABLE_GYRO_INT;

	error += I2cStartWait();						/* send I2C start with deley					*/
	error += I2cWriteWait(L3GD20_ADDRESS_WRITE);	/* send L3GD20 address byte and write bit		*/
	error += I2cWrite(reg_start | (1<<7));			/* write first address byte for miltiple read	*/
	error += I2cStartWait();						/* send I2C start with delay					*/
	error += I2cWriteWait(L3GD20_ADDRESS_READ);		/* send L3GD20 address byte and read bit		*/
	
	for(i=0 ; i<bytes ; i++){						/* read number of bytes							*/
		error += I2cRead(&data);					/* read one byte								*/
		*(buffer+i) = data;							/* store one byte								*/
		if((i+1) == bytes){
			I2cStop();								/* send stop to L3GD20							*/
			break;									/* break for									*/
		}
		else
			error += I2cSendAck();					/* send ACK to L3GD20 slave to read next byte	*/
	}
	
	RESTORE_GYRO_INT;
	
	if(error)										/* error occurred?								*/
		return (1);									/* return error (1) -> communication failed		*/
	return (0);										/* return successfully							*/
}

/****************************************************************************************************/
/* Function write one byte to L3GD20 register address. Return 0 if successfully, 					*/
/*	outherwise return 1.																			*/
/*																									*/
/* @input value:	(reg)			- L3GD20 register address										*/
/*					(byte)			- L3GD20 write byte												*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> failed							*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char GyrWriteByte(unsigned char reg, unsigned char byte){
	signed char error=0;
	
	DISABLE_GYRO_INT;

	error += I2cStartWait();							/* send I2C start with delay				*/
	error += I2cWriteWait(L3GD20_ADDRESS_WRITE);		/* send L3GD20 address byte and write bit	*/
	error += I2cWrite(reg);								/* send register address to L3GD20			*/
	error += I2cWrite(byte);							/* send write byte to L3GD20				*/
	I2cStop();											/* send stop to L3GD20						*/
	
	RESTORE_GYRO_INT;

	if(error)											/* I2C error occurred?						*/
		return (1);										/* return write failed						*/
	return (0);											/* return write successfully				*/
}

/****************************************************************************************************/
/* Function reads 3-axis gyroscope. (L3GD20 axis registers) 										*/
/*																									*/
/* @input value:	(int *x) 		- containter for X-axis angular rate data 						*/
/*					(int *y) 		- containter for Y-axis angular rate data		 				*/
/*					(int *z)		- containter for Z-axis angular rate data						*/
/* @return value:	(int)			- (0) -> successfully, (-1) - reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
int GyrReadAxis(int *x, int *y, int *z){
	unsigned char buffer[6];
	//if(!my_gyr_available)									/* gyr not available					*/
	//	return (-1);										/* return error: communication failed	*/
	if(GyrReadBytes(L3G_OUT_X_L, 6, buffer) != 0){	/* read 6 bytes X,Y and Z registers failed?		*/
		return (-1);								/* return error: I2C communication failed		*/
	}
	/* combine high and low bytes for x, y, z														*/
	*x = ((int)buffer[1]<<8) | buffer[0];	
	*y = ((int)buffer[3]<<8) | buffer[2];
	*z = ((int)buffer[5]<<8) | buffer[4];
	return (0);										/* return successfully							*/
}

/****************************************************************************************************/
/* Function GyrCalibration calculated dc_offset and noise for all 3 axes (L3GD20 dc_offset and		*/ 
/* noise). While calibration sensor should be without movement.						 			    */
/*																									*/
/* @input value:	(unsigned int samples)	- number of samples which should considered for			*/
/*											  calculation											*/
/* @return value:	(int)		- (0)->successfully, (-1)->set register error,						*/
/*--------------------------------------------------------------------------------------------------*/
int GyrCalibration(unsigned int samples){
	int x, y, z;
	unsigned char buf, tempFifoCtrl;
	
	if(GyrReadByte(L3G_FIFO_CTRL_REG, &tempFifoCtrl)!=0)		/* save FIFO_CTRL settings		*/
		return (-1);											/* set register failed			*/
		
/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000						*/
	if(GyrWriteByte(L3G_FIFO_CTRL_REG, 0x00) != 0)	/* Bypass mode								*/
		return (-1);								/* set register failed						*/
	
/*----------------------------- Calculating DC offset of each axle -----------------------------*/
	gyrData.dc_offsetX=0;								
	gyrData.dc_offsetY=0;
	gyrData.dc_offsetZ=0;
	for(int n=0;n<samples;n++){
        if(GyrReadByte(L3G_STATUS_REG, &buf)!=0)		/* check if new data available			*/
			return (-1);						
		while((buf&0x08)==0){							/* X, Y, Z -axis new data available ?	*/
			if(GyrReadByte(L3G_STATUS_REG, &buf)!=0)	/* check if new data available			*/
				return (-1);												
		}

		if(GyrReadAxis(&x, &y, &z)!=0)
			return (-1);				
        gyrData.dc_offsetX+=int(x);
		gyrData.dc_offsetY+=int(y);
		gyrData.dc_offsetZ+=int(z); 
	}
    gyrData.dc_offsetX=gyrData.dc_offsetX/samples;
    gyrData.dc_offsetY=gyrData.dc_offsetY/samples;
    gyrData.dc_offsetZ=gyrData.dc_offsetZ/samples;

/*------------------------------ Calculating noise of each axle --------------------------------*/
	gyrData.noiseX=0;
	gyrData.noiseY=0;
	gyrData.noiseZ=0;
	for(int n=0;n<samples;n++){
		if(GyrReadByte(L3G_STATUS_REG, &buf)!=0)		/* check if new data available			*/
			return (-1);								/*  I2C communication failed 			*/		
		while((buf&0x08)==0){							/* X, Y, Z -axis new data available ?	*/
			if(GyrReadByte(L3G_STATUS_REG, &buf)!=0)	/* check if new data available			*/
				return (-1);							/* I2C communication failed				*/					
		}

		if(GyrReadAxis(&x, &y, &z)!=0)
			return (-1);	

        if((int)x-gyrData.dc_offsetX>gyrData.noiseX)
			gyrData.noiseX=(int)x-gyrData.dc_offsetX;
        else if((int)x-gyrData.dc_offsetX<-gyrData.noiseX)
			gyrData.noiseX=-(int)(x-gyrData.dc_offsetX);
        
		if((int)y-gyrData.dc_offsetY>gyrData.noiseY)
			gyrData.noiseY=(int)y-gyrData.dc_offsetY;
        else if((int)y-gyrData.dc_offsetY<-gyrData.noiseY)
			gyrData.noiseY=-(int)(y-gyrData.dc_offsetY);
        
		if((int)z-gyrData.dc_offsetZ>gyrData.noiseZ)
			gyrData.noiseZ=(int)z-gyrData.dc_offsetZ;
        else if((int)z-gyrData.dc_offsetZ<-gyrData.noiseZ)
			gyrData.noiseZ=-(int)(z-gyrData.dc_offsetZ); 
		
	}
	
	if(gyrData.dpsRange==2000){							/* 2000 dps									*/
		gyrData.noiseX=gyrData.noiseX*70.0F/1000.0F;	/* Calculating data according to range sensitivity  */
		gyrData.noiseY=gyrData.noiseY*70.0F/1000.0F;	/* Table 4. L3GD20 Data sheet				*/
		gyrData.noiseZ=gyrData.noiseZ*70.0F/1000.0F; 
	}
	else if(gyrData.dpsRange==500){						/* 500 dps									*/
		gyrData.noiseX=gyrData.noiseX*17.5F/1000.0F;	/* Calculating data according to range sensitivity  */
		gyrData.noiseY=gyrData.noiseY*17.5F/1000.0F;	/* Table 4. L3GD20 Data sheet				*/ 
		gyrData.noiseZ=gyrData.noiseZ*17.5F/1000.0F; 	
	}
	else{												/* 250 dps									*/	
		gyrData.noiseX=gyrData.noiseX*8.75F/1000.0F;	/* Calculating data according to range sensitivity  */
		gyrData.noiseY=gyrData.noiseY*8.75F/1000.0F;	/* Table 4. L3GD20 Data sheet				*/ 
		gyrData.noiseZ=gyrData.noiseZ*8.75F/1000.0F; 
	}
	
/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000							*/
	if(GyrWriteByte(L3G_FIFO_CTRL_REG, tempFifoCtrl) != 0)	/* restore FIFO_CTRL_REG				*/
		return (-1);										/* set register failed					*/

	return (0);										/* calibration successfully						*/
}
/****************************************************************************************************/
/* Function initialize gyroscope. (L3GD20 initialization) 										    */
/*																									*/
/* @input value:	(unsigned int odr)		- Output data rate setings (95Hz, 190Hz, 380Hz or 760Hz)*/
/*					(unsigned int dpsRange)	- Range (250, 500 or 2000 Degrees Per Second)			*/
/* @return value:	(int)		 - (0)->successfully, (-2)->set register error, (-1)->wrong settings*/
/*--------------------------------------------------------------------------------------------------*/
int GyrInit(unsigned int odr, unsigned int dpsRange, unsigned char fifoEnable){
	int x, y, z;
	unsigned char buf;
	
	InitDrdy();
	DDRF |=(1<<PF0);		//set PF0 to output 
	DDRA |=(1<<PA0);		//set PA0 to output 
	DDRA |=(1<<PA1);		//set PA1 to output 
	PORTA &= ~(1<<PA0);		//set PA0 low
	PORTA &= ~(1<<PA1);		//set PA1 low

	/* CTRL_REG1 (0x20)
	====================================================================
	BIT  Symbol    Description                                   Default
	---  ------    --------------------------------------------- -------
	7-6  DR1/0     Output data rate selection                         00
				   00 = 95Hz, 01 = 190Hz, 10 = 380Hz, 11 = 760Hz
	5-4  BW1/0     Bandwidth selection (cut off freq)                 00
	  3  PD        0 = Power-down mode, 1 = normal/sleep mode          0	
      2  ZEN       Z-axis enable (0 = disabled, 1 = enabled)           1
      1  YEN       Y-axis enable (0 = disabled, 1 = enabled)           1
      0  XEN       X-axis enable (0 = disabled, 1 = enabled)           1							*/
	if(odr==760){	
		if(GyrWriteByte(L3G_CTRL_REG1, 0xCF) != 0){		/* DR1/0 = 11 (760 Hz); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled) */
			return (-2);								/* set register failed						*/
		}
		gyrData.odr=760;
	}
	else if(odr==380){ 
		if(GyrWriteByte(L3G_CTRL_REG1, 0x8F) != 0){		/* DR1/0 = 10 (380 Hz); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled) */
			return (-2);								/* set register failed						*/
		}
		gyrData.odr=380;
	}
	else if(odr==190){ 
		if(GyrWriteByte(L3G_CTRL_REG1, 0x4F) != 0){		/*  DR1/0 = 01 (190 Hz); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled) */
			return (-2);								/* set register failed						*/
		}
		gyrData.odr=190;
	}
	else if (odr==95){
		if(GyrWriteByte(L3G_CTRL_REG1, 0x0F) != 0){		/* DR1/0 = 00 (95 Hz); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled) */
			return (-2);								/* set register failed						*/
		}
		gyrData.odr=95;
	}
	else
		return (-1);

	
/*	CTRL_REG2 (0x21)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    5-4  HPM1/0    High-pass filter mode selection                    00
	    			00 = Normal mode (reset reading HP_RESET_FILTER)
					01 = Reference signal for filtering
					10 = Normal mode
					11 = Autoreset on interrupt event
    3-0  HPCF3..0  High-pass filter cutoff frequency selection      0000							*/
	if(GyrWriteByte(L3G_CTRL_REG2, 0x16) != 0)		/* Reference signal for filtering)				*/
		return (-2);								/* set register failed							*/
	
/*	CTRL_REG3 (0x22)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
      7  I1_Int1   Interrupt enable on INT1 (0=disable,1=enable)       0
      6  I1_Boot   Boot status on INT1 (0=disable,1=enable)            0
      5  H-Lactive Interrupt active config on INT1 (0=high,1=low)      0
      4  PP_OD     Push-Pull/Open-Drain (0=PP, 1=OD)                   0
      3  I2_DRDY   Data ready on DRDY/INT2 (0=disable,1=enable)        0
      2  I2_WTM    FIFO wtrmrk int on DRDY/INT2 (0=dsbl,1=enbl)        0
      1  I2_ORun   FIFO overrun int on DRDY/INT2 (0=dsbl,1=enbl)       0
      0  I2_Empty  FIFI empty int on DRDY/INT2 (0=dsbl,1=enbl)         0						 */
	if(GyrWriteByte(L3G_CTRL_REG3, (fifoEnable?0x04:0x00)) != 0)	/* FIFO watermark interrupt on DRDY/INT2 enable) */
		return (-2);								/* set register failed						 */
	
	/* CTRL_REG4 (0x23)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
      7  BDU       Block Data Update (0=continuous, 1=LSB/MSB)         0
      6  BLE       Big/Little-Endian (0=Data LSB, 1=Data MSB)          0
    5-4  FS1/0     Full scale selection                               00
                                 00 = 250 dps
                                 01 = 500 dps
                                 10 = 2000 dps
                                 11 = 2000 dps
      0  SIM       SPI Mode (0=4-wire, 1=3-wire)                       0 */
	if(dpsRange==2000){	
		if(GyrWriteByte(L3G_CTRL_REG4, 0x20) != 0){		/* FS = 10 (+/- 2000 dps full scale)	*/
			return (-2);								/* set register failed					*/
		}
		gyrData.dpsRange=2000;
	}
	else if(dpsRange==500){ 
		if(GyrWriteByte(L3G_CTRL_REG4, 0x10) != 0){		/* FS = 01 (+/- 500 dps full scale)		*/
			return (-2);								/* set register failed					*/
		}
		gyrData.dpsRange=500;
	}
	else if(dpsRange==250){
		if(GyrWriteByte(L3G_CTRL_REG4, 0x00) != 0){		/* FS = 00 (+/- 250 dps full scale)		*/
			return (-2);								/* set register failed					*/
		}
		gyrData.dpsRange=250;
	}
	else 
		return (-1);


    /* CTRL_REG5 (0x24)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
      7  BOOT      Reboot memory content (0=normal, 1=reboot)          0
      6  FIFO_EN   FIFO enable (0=FIFO disable, 1=enable)              0
      4  HPen      High-pass filter enable (0=disable,1=enable)        0
    3-2  INT1_SEL  INT1 Selection config                              00
    1-0  OUT_SEL   Out selection config                               00 */
	if(GyrWriteByte(L3G_CTRL_REG5, (fifoEnable?0x51:0x11)) != 0)		/* High-pass filter enable, FIFO enable */
		return (-2);									/* set register failed					*/
	
    /* FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000 */
	if(GyrWriteByte(L3G_FIFO_CTRL_REG, (fifoEnable?0x5C:0x00)) != 0)		/* Stream mode,  watermarl level 16	*/
		return (-2);									/* set register failed						*/
	
	
	
	if(GyrCalibration(NUM_SAMPLES)!=0)					/* calculate dc_offset and noise			*/ 
		return (-2);
	
	

	gyrData.prev_rateX=0;								/* set all angles to zero					*/
	gyrData.prev_rateY=0;
	gyrData.prev_rateZ=0;

	gyrData.angleX=0;
	gyrData.angleY=0;
	gyrData.angleZ=0;
	
	EIMSK &= ~(1 <<INT7);						/* disable PCINT7: FIFO Gyro interrupt				*/

	GyrReadByte(L3G_FIFO_SRC_REG, &buf);						/* check if FIFO is empty			*/
	if((buf&0x20)==0){ 
		for(int n=0;n<(buf&0x1F);n++){
			GyrReadBytes(L3G_OUT_X_L, 6, gyr_buffer);			/* read all data from FIFO			*/
		}
	}
	
	EICRB |= (0b11 <<ISC70);					/* Set up FIFO Watermark level interrupt on INT7	*/
												/* The rising edge generates an interrupt request   */
	EIMSK |= (1 <<INT7);						/* enable PCINT7: FIFO Gyro interrupt				*/
	
	my_gyr_available = 1;							/* gyr is available								*/	
	return (0);										/* initialize successfully						*/
}



/****************************************************************************************************/
/* PCINT vector 7, PE7. This interrupt is used in Advanced gyr mode when FIFO level is over			*/
/* watermark level. After reading angular rate data, the angular position of the sensor is			*/ 
/* calculated by integration with respect to time.													*/
/****************************************************************************************************/
ISR(INT7_vect){
	unsigned char buf;
	sei();

	//PORTF |= (1<<PF0);

	GyrReadByte(L3G_FIFO_SRC_REG, &buf);
	
	while((buf&0xC0)>0){								/* if FIFO Full or over watermark			*/
		for(int n=0;n<(buf&0x1F);n++){
			GyrReadBytes(L3G_OUT_X_L, 6, gyr_buffer);	/* read 6 bytes X,Y and Z registers			*/
	
			/* combine high and low bytes for x, y, z												*/
			gyrData.rateX = (double)((int)(gyr_buffer[1]<<8 | (int)gyr_buffer[0])-(int)gyrData.dc_offsetX); 	
			gyrData.rateY = (double)((int)(gyr_buffer[3]<<8 | (int)gyr_buffer[2])-(int)gyrData.dc_offsetY); 	
			gyrData.rateZ = (double)((int)(gyr_buffer[5]<<8 | (int)gyr_buffer[4])-(int)gyrData.dc_offsetZ); 	
			
			switch(gyrData.dpsRange){
				case (2000):
					gyrData.rateX*=70.0F/1000.0F;	/* Calculating data according to range sensitivity  */
					gyrData.rateY*=70.0F/1000.0F;	/* Table 4. L3GD20 Data sheet						*/
					gyrData.rateZ*=70.0F/1000.0F;
					break;
				case (500):
					gyrData.rateX*=17.5F/1000.0F;	/* Calculating data according to range sensitivity  */
					gyrData.rateY*=17.5F/1000.0F;	/* Table 4. L3GD20 Data sheet						*/ 
					gyrData.rateZ*=17.5F/1000.0F; 	
					break;
				case (250):
					gyrData.rateX*=8.75F/1000.0F;	/* Calculating data according to range sensitivity  */
					gyrData.rateY*=8.75F/1000.0F;	/* Table 4. L3GD20 Data sheet						*/ 
					gyrData.rateZ*=8.75F/1000.0F;
					break;
			}
			
			if(gyrData.rateX>=gyrData.noiseX || gyrData.rateX<=-gyrData.noiseX){
				gyrData.angleX+=((double)(gyrData.prev_rateX+gyrData.rateX)/(double)(2*gyrData.odr));	
				gyrData.prev_rateX=gyrData.rateX;
				gyrData.dataCounterX+=1;
			}			

			if(gyrData.rateY>=gyrData.noiseY || gyrData.rateY<=-gyrData.noiseY){
				gyrData.angleY+=((double)(gyrData.prev_rateY+gyrData.rateY)/(double)(2*gyrData.odr));		
				gyrData.prev_rateY=gyrData.rateY;
				gyrData.dataCounterY+=1;
			}
						
			if(gyrData.rateZ>=gyrData.noiseZ || gyrData.rateZ<=-gyrData.noiseZ){
				gyrData.angleZ+=((double)(gyrData.prev_rateZ+gyrData.rateZ)/(double)(2*gyrData.odr));		
				gyrData.prev_rateZ=gyrData.rateZ;
				gyrData.dataCounterZ+=1;
			}
			
												 /* Keep all three angles between 0-359 degrees */
			if (gyrData.angleX < 0)
				gyrData.angleX += 360;
			else if (gyrData.angleX >= 360)
				gyrData.angleX -= 360;
			if (gyrData.angleY < 0)
				gyrData.angleY += 360;
			else if (gyrData.angleY >= 360)
				gyrData.angleY -= 360;
			if (gyrData.angleZ < 0)
				gyrData.angleZ += 360;
			else if (gyrData.angleZ >= 360)
				gyrData.angleZ -= 360;
		}
		GyrReadByte(L3G_FIFO_SRC_REG, &buf);
	}
	
	/*GyrReadByte(L3G_FIFO_SRC_REG, &buf);
	if((buf&0x40)>0)
		PORTA |= (1<<PA0);
	*/
	//GyrReadByte(L3G_WHO_AM_I, &buf);
	//if(buf != 0xD4)								/* device id should be 0xD4					*/
	//	PORTA |= (1<<PA1);
	
	//PORTF &= ~(1<<PF0);
}
/*----------------------------------------------------------------------------------------------*/
/* Function initialize gyroscope L3GD20 in Basic mode (without FIFO and automatic angle	axis	*/
/* calculation). Must be called before using gyro functions: read(*x, *y, *z), readX(), readY() */
/* readZ()																						*/
/* input value:		(none)																		*/
/* return value:	(int)	- (0) -> successfully, (-1) -> initialization failed				*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::initBasic(void){
	if(GyrInit(L3G_DATA_RATE_HZ, L3G_RANGE_DPS, false)!=0)		/* initialiye gyro with default setings	*/	
		return (-1);													/* set register failed	*/

	my_gyr_available = 1;							/* gyr is available	in Basic mode			*/
	return (0);										/* initialize successfully					*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function initialize gyroscope L3GD20 in Basic mode (without FIFO and automatic angle	axis	*/
/* calculation). Must be called before using gyro functions: read(*x, *y, *z), readX(), readY() */
/* readZ()																						*/
/* input value:		(none)																		*/
/* return value:	(int)	- (0) -> successfully, (-1) -> initialization failed				*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::initBasic(unsigned int odr, unsigned int dpsRange){
	if(GyrInit(odr, dpsRange, false)!=0)		/* initialiye gyro with default setings	*/	
		return (-1);											/* set register failed	*/

	my_gyr_available = 1;							/* gyr is available	in Basic mode			*/
	return (0);										/* initialize successfully					*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function initialize gyroscope L3GD20 in Advanced mode (with FIFO buffering, filtering data	*/
/* and automatic angle axis calculations). Gyroscope is initialiyed with default output data 	*/
/* rate and default measuring scale range. (defined in: L3G_RANGE_DPS and L3G_DATA_RATE_HZ)		*/
/* This function must be called befor using gyro functions related with axis angles	data		*/
/*																								*/
/* input value:		(none)																		*/
/* return value:	(int)	- (0) -> successfully, (-1) -> initialization failed				*/
/*----------------------------------------------------------------------------------------------*/
	int EmoroGyrClass::init(void){

	if(GyrInit(L3G_DATA_RATE_HZ, L3G_RANGE_DPS, true)!=0)	/* initialiye gyro with default setings	*/	
		return (-1);													/* set register failed	*/

	my_gyr_available = 2;							/* gyr is available in Advanced mode		*/	
	return (0);										/* initialize successfully					*/
}
/*----------------------------------------------------------------------------------------------*/
/* Function initialize gyroscope L3GD20 in Advanced mode (with FIFO buffering, filtering data   */
/* and automatic angle axis calculations). Gyroscope is initialiyed with provided output data 	*/
/* rate and measuring scale range.																*/
/* This function must be called befor using gyro functions related with axis angles	data		*/
/*																								*/
/* @input value:	(unsigned int rate)		- gyro data rate generation	[Hz]					*/
/*											  possible values: 95, 190, 380 and 760				*/
/*					(unsigned int range)	- full scale selection	[DPS] (degrees per second)	*/
/*											  possible values: 250, 500 and 2000				*/
/* return value:	(int)	- (0) -> successfully, (-1) -> wrong parameters,					*/
/*							- (-2) -> initialization failed										*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::init(unsigned int odr, unsigned int dpsRange){
	int res;
	res=GyrInit(odr, dpsRange, true);					/* initialiye gyro with odr and dpsRange		*/
	
	if(res!=0)											
		return res;						/*(-1) -> wrong parameters, (-2) -> set register failed	*/
	
	my_gyr_available = 2;						/* gyr is available	in Advanced mode			*/	
	return (0);									/* initialize successfully						*/	
}

/*----------------------------------------------------------------------------------------------*/
/* Function stop gyro L3GD20 Advanced mode and save controller time for data and interrupt		*/ 
/* processing. Can be called after Advanced mode initialization.							 	*/
/*																								*/
/* input value:		(none)																		*/
/* return value:	(int)	- (0) -> successfully, (-1) -> Stop failed							*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::stop(void){

	EIMSK &= ~(1 << INT7);							/* disable PCINT7:0							*/

/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000						*/
	if(GyrWriteByte(L3G_FIFO_CTRL_REG, 0x00) != 0)						/* set Bypass mode		*/
		return (-1);													/* set register failed	*/

/*	CTRL_REG5 (0x24)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
      7  BOOT      Reboot memory content (0=normal, 1=reboot)          0
      6  FIFO_EN   FIFO enable (0=FIFO disable, 1=enable)              0
      4  HPen      High-pass filter enable (0=disable,1=enable)        0
    3-2  INT1_SEL  INT1 Selection config                              00
    1-0  OUT_SEL   Out selection config                               00						*/
	if(GyrWriteByte(L3G_CTRL_REG5, 0x00) != 0)			  /* High-pass filter and FIFO disabled	*/
		return (-1);									  /* set register failed				*/

	my_gyr_available = 0;								  /* gyr Advanced mode is disabled		*/
	return (0);											  /* Stop gyr successfully				*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function verifies gyroscope I2C connection. Make sure the device is connected and responds 	*/
/* as expected. Return True if connection is valid (read correct data in Device identification	*/
/* register), false otherwise. Can be used before any gyro initialization.						*/
/*																								*/
/* @input value:	(none)			 															*/
/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully	*/
/*----------------------------------------------------------------------------------------------*/
unsigned char EmoroGyrClass::testConnection(void){
	unsigned char data;
	if(GyrReadByte(L3G_WHO_AM_I, &data) != 0)		/* Read L3GD20 device ID failed?			*/
		return (0);									/* return 0 								*/
	if(data == 0xD4)								/* device id should be 0xD4					*/
		return (1);									/* return connection successfully			*/
	return (0);										/* return connection failed					*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function reads calculated all 3-axes angles.					 								*/
/*																								*/
/* @input value:	(double *x) 	- containter for X-axis angle			 					*/
/*					(double *y) 	- containter for Y-axis angle								*/
/*					(double *z)		- containter for Z-axis angle								*/
/* @return value:	(int)			- (0) -> successfully, (-1) - reading failed				*/
/*----------------------------------------------------------------------------------------------*/
	int EmoroGyrClass::readDegrees(double *x, double *y, double *z){
	unsigned char buf;
	
	if(my_gyr_available != 2)
		return (-1);
	
	if(GyrReadByte(L3G_FIFO_SRC_REG, &buf)!=0){
		return (-2);
	}
	if(buf&0x40){									/* if FIFO is full */
		if(GyrReadByte(L3G_FIFO_CTRL_REG, &buf)!=0)
			return (-2);
	/*-------------------------- Restart FIFO -------------------------*/  
/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000						*/
		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

	}
	*x = gyrData.angleX;	
	*y = gyrData.angleY;
	*z = gyrData.angleZ;
	return (0);						/* return successfully										*/
}
/*----------------------------------------------------------------------------------------------*/
/* Function reads calculated X axis angle.					 									*/
/*																								*/
/* @input value:	(none)													 					*/
/* @return value:	(double)		-  X axis angle												*/
/*----------------------------------------------------------------------------------------------*/
double EmoroGyrClass::readDegreesX(void){
	unsigned char buf;
	
	if(my_gyr_available != 2)
		return (-1);
	
	if(GyrReadByte(L3G_FIFO_SRC_REG, &buf)!=0)
		return (-2);
	
	if(buf&0x40){									/* if FIFO is full */
		if(GyrReadByte(L3G_FIFO_CTRL_REG, &buf)!=0)
			return (-2);
	/*-------------------------- Restart FIFO -------------------------*/  
/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000						*/
		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

	}
	return gyrData.angleX;	
}
/*----------------------------------------------------------------------------------------------*/
/* Function reads calculated Y axis angle.					 									*/
/*																								*/
/* @input value:	(none)													 					*/
/* @return value:	(double)		-  Y axis angle												*/
/*----------------------------------------------------------------------------------------------*/
double EmoroGyrClass::readDegreesY(void){
	unsigned char buf;
	
	if(my_gyr_available != 2)
		return (-1);
	
	if(GyrReadByte(L3G_FIFO_SRC_REG, &buf)!=0){
		return (-2);
	}
		if(buf&0x40){									/* if FIFO is full */
		if(GyrReadByte(L3G_FIFO_CTRL_REG, &buf)!=0)
			return (-2);
	/*-------------------------- Restart FIFO -------------------------*/  
/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000						*/
		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

	}
	return gyrData.angleY;	
}
/*----------------------------------------------------------------------------------------------*/
/* Function reads calculated Z axis angle.					 									*/
/*																								*/
/* @input value:	(none)													 					*/
/* @return value:	(double)		-  Z axis angle												*/
/*----------------------------------------------------------------------------------------------*/
double EmoroGyrClass::readDegreesZ(void){
	unsigned char buf;
	
	if(my_gyr_available != 2)
		return (-1);
	
	if(GyrReadByte(L3G_FIFO_SRC_REG, &buf)!=0){
		return (-2);
	}
	if(buf&0x40){									/* if FIFO is full */
		if(GyrReadByte(L3G_FIFO_CTRL_REG, &buf)!=0)
			return (-2);
	/*-------------------------- Restart FIFO -------------------------*/  
/*	FIFO_CTRL_REG (0x2E)
    ====================================================================
    BIT  Symbol    Description                                   Default
    ---  ------    --------------------------------------------- -------
    7-5  FM2-0   FIFO mode selection								 000
				   000 = Bypass mode
				   001 = FIFO mode
				   010 = Stream mode
				   011 = Stream-to-FIFO mode
				   100 = Bypass-to-Stream mode
	4-0	 WTM4-0  FIFO threshold. Watermark level setting		   00000						*/
		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

		//if(GyrWriteByte(L3G_FIFO_CTRL_REG, buf&0x1F) != 0)				/* set Bypass mode		*/
		//	return (-2);												/* set register failed	*/

	}
	return gyrData.angleZ;	
}

/*----------------------------------------------------------------------------------------------*/
/* Function sets values of all 3-axes angles as current gyro position.							*/
/*																								*/
/* @input value:	(double x) 		- containter for X-axis angle			 					*/
/*					(double y) 		- containter for Y-axis angle								*/
/*					(double z)		- containter for Z-axis angle								*/
/* @return value:	(int)			- (0) -> successfully, (-1) - gyro is not in Advance mode	*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::setDegrees(double x, double y, double z){
	
	if(my_gyr_available != 2)							/* gyr is not in Advanced mode			*/
		return (-1);									/* return error: wrong mode				*/
	
	gyrData.dataCounterX=0;
	gyrData.dataCounterY=0;
	gyrData.dataCounterZ=0;
	gyrData.angleX=x;	
	gyrData.angleY=y;
	gyrData.angleZ=z;
	return (0);									/* return successfully							*/
}
/*----------------------------------------------------------------------------------------------*/
/* Function sets values of all 3-axes angles as 0 degrees position.	(Reset all 3-axes angles)	*/
/*																								*/
/* @input value:	(none)													 					*/
/* @return value:	(int)			- (0) -> successfully, (-1) - gyro is not in Advance mode	*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::resetDegrees(void){
	
	if(my_gyr_available != 2)							/* gyr is not in Advanced				*/
		return (-1);									/* return error: wrong mode				*/
	
	gyrData.dataCounterX=0;
	gyrData.dataCounterY=0;
	gyrData.dataCounterZ=0;
	gyrData.angleX=0;	
	gyrData.angleY=0;
	gyrData.angleZ=0;
	return (0);									/* return successfully							*/
}
/*----------------------------------------------------------------------------------------------*/
/* Function reads 3-axis gyroscope. (L3GD20 axis registers) 									*/
/*																								*/
/* @input value:	(int *x) 		- containter for X-axis angular rate data 					*/
/*					(int *y) 		- containter for Y-axis angular rate data					*/
/*					(int *z)		- containter for Z-axis angular rate data					*/
/* @return value:	(int)			- (0) -> successfully, (-1) - gyr is not in Normal mode		*/
/*									  (-2) - I2C communication failed							*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::read(int *x, int *y, int *z){
	unsigned char buffer[6];
	if(my_gyr_available!=1)								/* gyr is not in Basic mode				*/
		return (-1);									/* return error: wrong mode				*/
	if(GyrReadBytes(L3G_OUT_X_L, 6, buffer) != 0){	 /* read 6 bytes X,Y and Z registers failed?*/
		return (-2);								 /* return error: I2C communication failed	*/
	}
	/* combine high and low bytes for x, y, z													*/
	*x = ((int)buffer[1]<<8) | buffer[0];	
	*y = ((int)buffer[3]<<8) | buffer[2];
	*z = ((int)buffer[5]<<8) | buffer[4];
	return (0);										 /* return successfully						*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns X axis gyroscope reading.													*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(int)	- X-axis gyroscope measurment (16-bit 2's complement format)		*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::readX(void){
	int x;
	unsigned char buffer[2];
	if(my_gyr_available!=1)								/* gyr is not in Basic mode				*/
		return (0);										/* return error: wrong mode				*/
	if(GyrReadBytes(L3G_OUT_X_L, 2, buffer) != 0)		/* read x axis failed?					*/
		return (0);										/* return error: 0 for x axis			*/
	x = ((int)buffer[1]<<8) | buffer[0];				/* sort bites for x axis				*/
	return (x);											/* return x axis						*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns Y axis gyroscope reading.													*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(int)	- Y-axis gyroscope measurment (16-bit 2's complement format)		*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::readY(void){
	int y;
	unsigned char buffer[2];
	if(my_gyr_available!=1)								/* gyr is not in Basic mode				*/
		return (0);										/* return error: wrong mode				*/
	if(GyrReadBytes(L3G_OUT_Y_L, 2, buffer) != 0)	/* read y axis failed?						*/
		return (0);									/* return error: 0 for y axis				*/
	y = ((int)buffer[1]<<8) | buffer[0];			/* sort bites for y axis					*/
	return (y);										/* return y axis							*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns Z axis gyroscope reading.													*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(int)	- Z-axis gyroscope measurment (16-bit 2's complement format)		*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::readZ(void){
	int z;
	unsigned char buffer[2];
	if(my_gyr_available!=1)								/* gyr is not in Basic mode				*/
		return (0);										/* return error: wrong mode				*/
	if(GyrReadBytes(L3G_OUT_Z_L, 2, buffer) != 0)	/* read z axis failed?						*/
		return (0);									/* return error: 0 for z axis				*/
	z = ((int)buffer[1]<<8) | buffer[0];			/* sort bites for z axis					*/
	return (z);										/* return z axis							*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns number of calculated angular data rates of X axis which pass all filtering. */
/* This counter data can be used for filter tuning. While stationary position changing of		*/
/* this counter should as minimum as possible. 					 	  							*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(unsigned long)	- numbers of calculated X-axis angular data values			*/
/*----------------------------------------------------------------------------------------------*/
unsigned long EmoroGyrClass::readCounterX(void){
	unsigned long x;
	if(my_gyr_available!=2)								/* gyr is not in Advanced mode			*/
		return (0);										/* return error: wrong mode				*/
	x = gyrData.dataCounterX;					
	return (x);									/* return counter of X angle calculations		*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns number of calculated angular data rates of Y axis which pass all filtering. */
/* This counter data can be used for filter tuning. While stationary position changing of		*/
/* this counter should as minimum as possible. 					 	  							*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(unsigned long)	- numbers of calculated Y-axis angular data values			*/
/*----------------------------------------------------------------------------------------------*/
unsigned long EmoroGyrClass::readCounterY(void){
	unsigned long y;
	if(my_gyr_available!=2)								/* gyr is not in Advanced mode			*/
		return (0);										/* return error: wrong mode				*/
	y = gyrData.dataCounterY;					
	return (y);									/* return counter of Y angle calculations		*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns number of calculated angular data rates of Z axis which pass all filtering. */
/* This counter data can be used for filter tuning. While stationary position changing of		*/
/* this counter should as minimum as possible. 					 	  							*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(unsigned long)	- numbers of calculated Z-axis angular data values			*/
/*----------------------------------------------------------------------------------------------*/
unsigned long EmoroGyrClass::readCounterZ(void){
	unsigned long z;
	if(my_gyr_available!=2)								/* gyr is not in Advanced mode			*/
		return (0);										/* return error: wrong mode				*/
	z = gyrData.dataCounterZ;					
	return (z);									/* return counter of Z angle calculations		*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns state of DRDY signal from gyro. This signal is used for interrupt generation*/
/* in Advanced mode when FIFO level is over watermark level.									*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(unsigned char)		- (0) -> DRDY/INT2 low level, (1) - DRDY/INT2 high level*/
/*----------------------------------------------------------------------------------------------*/
unsigned char EmoroGyrClass::Dready(void){
	return Drdy();										/* return state of DRDY/INT2 signal		*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function calculates dc_offset drift and noise of all 3 axes. Function can be used only in	*/
/* Advanced mode. While calibration, gyro should be without movement.							*/
/*																								*/
/* @input value:	(unsigned int)		- number of samples which are used for calculation		*/
/* @return value:	(int)				- (0) -> successfully, (-1) - gyr is't in Advanced mode */
/*										  (-2) - calibration failed								*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::Calibration(unsigned int samples){
	
	if(my_gyr_available!=2)								/* gyr is not in Advanced mode			*/
		return (-1);									/* return error: wrong mode				*/
	
	if(GyrCalibration( samples )!=0 )
		return (-2);									/* return error: Calibration faild		*/

	return (0);											/* return successfully					*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns calculated dc_offset for X axis.											*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(int)	- dc_offset for X axis												*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::dc_offsetX(void){
	int dc_offsetX;
	dc_offsetX = gyrData.dc_offsetX;			
	return (dc_offsetX);								/* return calculated dc_offsetX			*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns calculated dc_offset for Y axis.											*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(int)	- dc_offset for Y axis												*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::dc_offsetY(void){
	int dc_offsetY;
	dc_offsetY = gyrData.dc_offsetY;			
	return (dc_offsetY);								/* return calculated dc_offsetY			*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns calculated dc_offset for Z axis.											*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(int)	- dc_offset for Z axis												*/
/*----------------------------------------------------------------------------------------------*/
int EmoroGyrClass::dc_offsetZ(void){
	int dc_offsetZ;
	dc_offsetZ = gyrData.dc_offsetZ;			
	return (dc_offsetZ);								/* return calculated dc_offsetZ			*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns calculated noise for X axis.												*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(double)	- noise for X axis												*/
/*----------------------------------------------------------------------------------------------*/
double EmoroGyrClass::noiseX(void){
	double noiseX;
	noiseX = gyrData.noiseX;					
	return (noiseX);									/* return calculated noiseX				*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns calculated noise for Y axis.												*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(double)	- noise for Y axis												*/
/*----------------------------------------------------------------------------------------------*/
double EmoroGyrClass::noiseY(void){
	double noiseY;
	noiseY = gyrData.noiseY;					
	return (noiseY);									/* return calculated noiseY				*/
}

/*----------------------------------------------------------------------------------------------*/
/* Function returns calculated noise for Z axis.												*/
/*																								*/
/* @input value:	(none) 																		*/
/* @return value:	(double)	- noise for Z axis												*/
/*----------------------------------------------------------------------------------------------*/
double EmoroGyrClass::noiseZ(void){
	double noiseZ;
	noiseZ = gyrData.noiseZ;					
	return (noiseZ);									/* return calculated noiseZ				*/
}

/*************************************** G Y R   C L A S S ******************************************/
EmoroGyrClass Gyr;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
