/****************************************************************************************************/
/*	Inovatic-ICT d.o.o																				*/
/*																									*/
/* 	EMoRo Lib:				emoro_mag.c																*/
/* 	Interface name: 		EMoRo 2560 v2.4															*/
/* 	MCU name:				ATMEGA2560-16															*/
/* 	Clock:					16 MHz																	*/
/*	file descriptor:		EMoRo magnetometer HMC5883L driver for EMoRo 2560					    */
/****************************************************************************************************/

#ifndef F_CPU
	#define F_CPU		16000000UL
#endif

/*------------------------------------ I N C L U D E S ---------------------------------------------*/
#include "emoro_mag.h"
#include "emoro_i2c.h"
#include <util\delay.h>
#include <avr\io.h>
#include <avr\eeprom.h>
#include <string.h>
#include <math.h>

/*--------------------------------------- M A C R O- -----------------------------------------------*/

#define SCALE_0_9GA				(1370)  /* 0.9 Ga (1370 counts / Gauss)								*/
#define SCALE_1_3GA				(1090)  /* 1.3 Ga (1090 counts / Gauss)								*/
#define SCALE_1_9GA				(820)   /* 1.9 Ga (820 counts / Gauss)								*/
#define SCALE_2_5GA				(660)   /* 2.5 Ga (660 counts / Gauss)								*/
#define SCALE_4_0GA				(440)   /* 4.0 Ga (440 counts / Gauss)								*/
#define SCALE_4_7GA				(390)   /* 4.7 Ga (390 counts / Gauss)								*/
#define SCALE_5_6GA				(330)   /* 5.6 Ga (330 counts / Gauss)								*/
#define SCALE_8_1GA				(230)   /* 8.1 Ga (230 counts / Gauss)								*/

/* offset data																						*/
#define CAL_OFFSETS_ADDR		(0)

/* sensitivity scaling data																			*/
#define CAL_SENSITIVITY_ADDR	(0 + sizeof(vector3_t))

#define SELF_TEST_DELAY_MSEC	(250)   /* delay in msec during self test							*/
#define READ_DELAY_MSEC			(100)   /* delay in msec between consecutive						*/

#define DATA_OUTPUT_OVERFLOW	((int16_t)0xf000)

#define InitDrdy()				(DDRE &=~(1<<PE5), PORTE |= (1<<PE5))
#define IsDrdyLow()				((PINE>>PE5)&0x01 ? 0:1)
#define IsDrdyHigh()			((PINE>>PE5)&0x01 ? 1:0)

#define HMC5883L_ADDRESS_WRITE	0x3C
#define HMC5883L_ADDRESS_READ	0x3D

/*----------------------------------- HMC8553L registers -------------------------------------------*/
#define HMC5883L_CONFIG_REG_A				0x00		/* configuration register A					*/
#define HMC5883L_CONFIG_REG_B				0x01		/* configuration register B					*/
#define HMC5883L_MODE_REG					0x02		/* mode register							*/
#define HMC5883L_MAG_X_HI					0x03		/* X mag reading - MSB						*/
#define HMC5883L_MAG_X_LO					0x04		/* X mag reading - LSB						*/
#define HMC5883L_MAG_Z_HI					0x05		/* Z mag reading - MSB						*/
#define HMC5883L_MAG_Z_LO					0x06		/* Z mag reading - LSB						*/
#define HMC5883L_MAG_Y_HI					0x07		/* Y mag reading - MSB						*/
#define HMC5883L_MAG_Y_LO					0x08		/* Y mag reading - LSB						*/
#define HMC5883L_STATUS_REG					0x09		/* device status							*/
#define HMC5883L_ID_REG_A					0x0A		/* ID register A							*/
#define HMC5883L_ID_REG_B					0x0B		/* ID register B							*/
#define HMC5883L_ID_REG_C					0x0C		/* ID register C							*/

/* HMC5883L_CONFIG_REG_A (0x00)																		*/
#define MEAS_MODE							0x03		/* measurement mode mask (2 bits)			*/
#define MEAS_MODE_NORM						0x00		/* normal measurement mode					*/
#define MEAS_MODE_POS						0x01		/* positive bias							*/
#define MEAS_MODE_NEG						0x02		/* negative bias							*/

#define DATA_RATE							0x1c		/* data rate mask (3 bits)					*/
#define DATA_RATE_0_75HZ					0x00		/* 0.75 Hz									*/
#define DATA_RATE_1_5HZ						0x04		/* 1.5 Hz									*/
#define DATA_RATE_3HZ						0x08		/* 3 Hz										*/
#define DATA_RATE_7_5HZ						0x0c		/* 7.5 Hz									*/
#define DATA_RATE_15HZ						0x10		/* 15 Hz									*/
#define DATA_RATE_30HZ						0x14		/* 30 Hz									*/
#define DATA_RATE_75HZ						0x18		/* 75 Hz									*/

#define MEAS_AVG							0x60		/* sample average mask (2 bits)				*/
#define MEAS_AVG_1							0x00		/* output = 1 sample (no avg.)				*/
#define MEAS_AVG_2							0x20		/* output = 2 samples averaged				*/
#define MEAS_AVG_4							0x40		/* output = 4 samples averaged				*/
#define MEAS_AVG_8							0x60		/* output = 8 samples averaged				*/

/* HMC5883L_CONFIG_REG_B (0x01)																		*/

#define GAIN_0_9GA							0x00		/* +/- 0.9 Ga (1370 counts/ Gauss)			*/
#define GAIN_1_3GA							0x20		/* +/- 1.3 Ga (1090 counts/ Gauss)			*/
#define GAIN_1_9GA							0x40		/* +/- 1.9 Ga (820 counts / Gauss)			*/
#define GAIN_2_5GA							0x60		/* +/- 2.5 Ga (660 counts / Gauss)			*/
#define GAIN_4_0GA							0x80		/* +/- 4.0 Ga (440 counts / Gauss)			*/
#define GAIN_4_7GA							0xa0		/* +/- 4.7 Ga (390 counts / Gauss)			*/
#define GAIN_5_6GA							0xc0		/* +/- 5.6 Ga (330 counts / Gauss)			*/
#define GAIN_8_1GA							0xe0		/* +/- 8.1 Ga (230 counts / Gauss)			*/

/* HMC5883L_MODE_REG (0x02) 																		*/
#define MODE_CONTIN							0x00		/* continuous conversion mode 				*/
#define MODE_SINGLE							0x01		/* single measurement mode 					*/
#define MODE_IDLE							0x02		/* idle mode 								*/
#define MODE_SLEEP							0x03		/* sleep mode 								*/

/* Self-test Definitions																			*/
#define HMC5883L_TEST_GAIN					GAIN_2_5GA		/* gain value during self-test			*/
#define HMC5883L_TEST_X_MIN					550		/* min X										*/
#define HMC5883L_TEST_X_NORM				766		/* normal X										*/
#define HMC5883L_TEST_X_MAX					850		/* max X										*/
#define HMC5883L_TEST_Y_MIN					550		/* min Y										*/
#define HMC5883L_TEST_Y_NORM				766		/* normal Y										*/
#define HMC5883L_TEST_Y_MAX					850		/* max Y										*/
#define HMC5883L_TEST_Z_MIN					550		/* min Z										*/
#define HMC5883L_TEST_Z_NORM				713		/* normal Z										*/
#define HMC5883L_TEST_Z_MAX					850		/* max Z										*/

/* brief Sensor Self Test Types																		*/
typedef enum {
	SENSOR_TEST_DEFAULT,							/**< Default test type for device				*/
	SENSOR_TEST_BIAS_POS,							/**< Positive bias test							*/
	SENSOR_TEST_BIAS_NEG,							/**< Negative bias test							*/
	SENSOR_TEST_DEFLECTION,							/**< Deflection test (e.g. accelerometer)		*/
	SENSOR_TEST_INTERRUPT							/**< Interrupt test								*/
} sensor_test_t;

/* brief Sensor Self-Test Error Constants															*/
typedef enum {
	SENSOR_TEST_ERR_NONE        = 0x0000,
	SENSOR_TEST_ERR_FUNCTION    = 0x0001,			/**< No such self-test function					*/
	SENSOR_TEST_ERR_READ        = 0x0002,			/**< Read error									*/
	SENSOR_TEST_ERR_WRITE       = 0x0003,			/**< Write error								*/
	SENSOR_TEST_ERR_RANGE       = 0x0004			/**< Results out of range						*/
} sensor_testerr_t;

/* brief scalar 3-dimensional vector																*/
typedef struct{
	float x;
	float y;
	float z;		
}vector3_t;

 /* 3-dimensional vector integer																	*/
typedef struct{
	int x;
	int y;
	int z;
}vector_int_t;

/* brief HMC 5883l axis data format																	*/
typedef struct {
	uint8_t msb;									/**< Most significant 8 bits					*/
	uint8_t lsb;									/**< Least significant 8 bits					*/
} hmc588l_axis_t;

typedef struct {
	vector3_t offsets;
	vector3_t sensitivity;
}cal_data_t;

static cal_data_t cal_data;

/* brief Sensor Operational Capabilities Descriptors												*/
typedef struct {
	union {
		int bandwidth_Hz;							/**< Engineering unit value (Hz.)				*/
		int range_units;							/**< Engineering unit value (varies)			*/
	};
	unsigned char reserved_val;						/**< Driver-specific reserved value				*/
} sensor_map_t;

/* brief Honeywell HMC5883L Range Table (microtesla, register value)								*/
static const sensor_map_t range_table [] = {
	{{90}, GAIN_0_9GA},								/**< 0.9 Gauss =  90 uTesla						*/
	{{130}, GAIN_1_3GA},							/**< 1.3 Gauss = 130 uTesla						*/
	{{190}, GAIN_1_9GA},							/**< 1.9 Gauss = 190 uTesla						*/
	{{250}, GAIN_2_5GA},							/**< 2.5 Gauss = 250 uTesla						*/
	{{400}, GAIN_4_0GA},							/**< 4.0 Gauss = 400 uTesla						*/
	{{470}, GAIN_4_7GA},							/**< 4.7 Gauss = 470 uTesla						*/
	{{560}, GAIN_5_6GA},							/**< 5.6 Gauss = 560 uTesla						*/
	{{810}, GAIN_8_1GA},							/**< 8.1 Gauss = 810 uTesla						*/
};

enum {index_90uT, index_130uT, index_190uT, index_250uT,
index_400uT, index_470uT, index_560uT, index_810uT};

/* brief Honeywell HMC5883L Data Scaling Table (counts per milligauss)								*/
static const int scale_table [] = {
	SCALE_0_9GA,									/**< 0.9 Gauss range							*/
	SCALE_1_3GA,									/**< 1.3 Gauss range							*/
	SCALE_1_9GA,									/**< 1.9 Gauss range							*/
	SCALE_2_5GA,									/**< 2.5 Gauss range							*/
	SCALE_4_0GA,									/**< 4.0 Gauss range							*/
	SCALE_4_7GA,									/**< 4.7 Gauss range							*/
	SCALE_5_6GA,									/**< 5.6 Gauss range							*/
	SCALE_8_1GA										/**< 8.1 Gauss range							*/
};

/* Current range index for device																	*/
static int mag_range;

/* brief Honeywell HMC5883L Bandwidth Table (hertz, register value)									*/
static const sensor_map_t band_table [] = {
	{{1}, DATA_RATE_0_75HZ},			/* 0.75 Hz  XXX called "1" for now XXX						*/
	{{2}, DATA_RATE_1_5HZ},				/* 1.5 Hz  XXX called "2" for now XXX						*/
	{{3}, DATA_RATE_3HZ},
	{{8}, DATA_RATE_7_5HZ},				/* 7.5 Hz  XXX called "8" for now XXX						*/
	{{15}, DATA_RATE_15HZ},
	{{30}, DATA_RATE_30HZ},
	{{75}, DATA_RATE_75HZ},
};

enum { index_1hz, index_2hz, index_3hz, index_8hz,
index_15hz, index_30hz, index_75hz };

enum{GET_SINGLE, GET_CONTIN};

static unsigned char my_mag_available;

#ifndef MY_PI
#   define MY_PI                     (3.14159265358979323846264338327)
#endif

#define RAD_TO_DEG                  (180 / MY_PI)			/**< radians to degrees					*/
#define DEG_TO_RAD                  (MY_PI / 180)			/**< degrees to radians					*/

#define degrees(a)  (RAD_TO_DEG * (a))

#define GAUSS_TO_MICRO_TESLA        (100)					/**< \f$1 Gauss = 100 \mu T\f$			*/

/** \brief Calculate a vector3_t dot-product														*/
static inline float vector3_dot_product(vector3_t const *v, vector3_t const *w){
	return ((v->x * w->x) + (v->y * w->y) + (v->z * w->z));
}

/** \brief Calculate a vector3_t magnitude															*/
static inline float vector3_magnitude(vector3_t const *v){
	return sqrt(vector3_dot_product(v,v));
}

/** \brief Scale a vector3_t value (scalar multiplication)											*/
static inline void vector3_scale(float s, vector3_t *v){
	v->x *= s; v->y *= s; v->z *= s;
}

/****************************************************************************************************/
/* Calculate direction, inclination, and field strength.											*/
/*																									*/
/* @input value:	(field)			-	Ma	gnetometer raw vector field sample input				*/
/*					(theta)			-	Calculated direction angle (degrees) output					*/
/*					(delta)			-	Calculated inclination angle (degrees) output				*/
/*					(strength)		-	Calculated raw vector field magnitude output				*/
/* @return value:	(unsigned char)	-	(0) -> successfully, (1) -> reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char MagFieldDirection(vector3_t *field, float *theta, float *delta, float *strength){
	/* Calculate the geomagnetic field direction vector (unit vector) and
	 * strength (field magnitude).
	 */
	float const magnitude = vector3_magnitude(field);

	if (0 == magnitude) {
		return 1;
	}

	vector3_scale((1 / magnitude), field);

	*strength = magnitude;

	/* Calculate the direction angle (degrees) assuming no mounting tilt.
	 * "Wraparound" negative results to a positive heading.  The angle is
	 * calculated relative to +Y axis, so atan2() arguments are (x,y)
	 * instead of (y,x).  Result is inverted (* -1) so positive values
	 * reflect clockwise rotation.
	 */
	*theta = degrees(-1 * atan2(field->y, field->x));
	
	if (*theta < 0) {
		*theta += 360;
	}

	/* Calculate the inclination angle degrees (assuming no mounting tilt).
	 * Downward angle is indicated by a positive inclination value.
	 */
	*delta = degrees(-1 * asin(field->z));
	

	return 0;
}

/****************************************************************************************************/
/* Apply stored sensitivity scaling factors to sensor reading.										*/
/* This function applies stored sensitivity scaling to the "input" vector of magnetometer values.	*/
/*																									*/
/* @input value:	(input) - Contains the measured magnetic field values							*/
/* @return value:	(none)																			*/
/*--------------------------------------------------------------------------------------------------*/
static void MagApplySensitivity(vector3_t *input){
	/* Apply sensitivity scaling to the uncalibrated input field vector.							*/
	input->x *= cal_data.sensitivity.x;
	input->y *= cal_data.sensitivity.y;
	input->z *= cal_data.sensitivity.z;
}

/****************************************************************************************************/
/* Apply stored offset values to sensor reading.													*/
/*																									*/
/* @input value:	(input) - Contains the measured magnetic field values, adjusted for sensitivity.*/
/* @return value:	(none)																			*/
/*--------------------------------------------------------------------------------------------------*/
static void MagApplyOffset(vector3_t *input){
	/* Apply calibrated offsets to the uncalibrated input field vector.								*/
	input->x -= cal_data.offsets.x;
	input->y -= cal_data.offsets.y;
	input->z -= cal_data.offsets.z;
}

/****************************************************************************************************/
/* Function reads bytes from HMC5883L chip. Reg_start points to address of the first byte for		*/
/* reading, and number of readed bytes will be stored in attribute buffer.							*/
/*																									*/
/* @input value:	(reg_start)		- first register address										*/
/*					(bytes)			- number of bytes for reading									*/
/*					(*buffer)		- buffer for readed bytes										*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> reading failed					*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned char MagReadBytes(unsigned char reg_start,unsigned char bytes,unsigned char *buffer){
	unsigned char data, i;
	signed char error=0;
	
	DISABLE_GYRO_INT;

	error += I2cStartWait();						/* send I2C start with delay					*/
	error += I2cWriteWait(HMC5883L_ADDRESS_WRITE);	/* send HMC5883L address byte and write bit		*/
	error += I2cWrite(reg_start | (1<<7));			/* write first address byte for multiple read	*/
	error += I2cStartWait();						/* send I2C start with delay					*/
	error += I2cWriteWait(HMC5883L_ADDRESS_READ);	/* send HMC5883L address byte and read bit		*/
	
	for(i=0 ; i<bytes ; i++){						/* read number of bytes							*/
		error += I2cRead(&data);					/* read one byte								*/
		*(buffer+i) = data;							/* store one byte								*/
		if((i+1) == bytes){
			I2cStop();								/* send stop to HMC5883L						*/
			break;									/* break for									*/
		}
		else
			error += I2cSendAck();					/* send ACK to HMC5883L slave to read next byte	*/
	}

	RESTORE_GYRO_INT;

	if(error)										/* error occurred?								*/
		return (1);									/* return error (1) -> communication failed		*/
	return (0);										/* return successfully							*/
}

/****************************************************************************************************/
/* Function write one byte to HMC8553L register address. Return 0 if successfully, 					*/
/*	otherwise return 1.																				*/
/*																									*/
/* @input value:	(reg)			- HMC8553L register address										*/
/*					(byte)			- HMC8553L write byte											*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> failed							*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned char MagWriteByte(unsigned char reg, unsigned char byte){
	signed char error=0;

	DISABLE_GYRO_INT;

	error += I2cStartWait();						/* send I2C start with delay					*/
	error += I2cWriteWait(HMC5883L_ADDRESS_WRITE);	/* send HMC5883L address byte and write bit		*/
	error += I2cWrite(reg);							/* send register address to HMC8553L			*/
	error += I2cWrite(byte);						/* send write byte to HMC8553L					*/
	I2cStop();										/* send stop to HMC8553L						*/
	
	RESTORE_GYRO_INT;
	
	if(error)										/* I2C error occurred?							*/
		return (1);									/* return write failed							*/
	return (0);										/* return write successfully					*/
}

/****************************************************************************************************/
/* Set the sample bandwidth for the magnetometer.													*/
/*																									*/
/* @input value:	(bw)			- The index of a driver-specific bandwidth table entry.			*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> failed (I2C error)				*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned char MagSetBandwidth(int bw){
	unsigned char reg_val;
	MagReadBytes(HMC5883L_CONFIG_REG_A, 1, &reg_val);
	reg_val &= ~(DATA_RATE); /* clear data rate select bits											*/	
	MagWriteByte(HMC5883L_CONFIG_REG_A, reg_val | band_table[bw].reserved_val);
	return 0;
}

/****************************************************************************************************/
/*  Set the range for the magnetometer.																*/
/*																									*/
/* @input value:	(range)			- The index of a driver-specific range table entry.				*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> failed (I2C error)				*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned char MagSetRange(int range){
	MagWriteByte(HMC5883L_CONFIG_REG_B, range_table[range].reserved_val);
	mag_range = range;								/* Store the index for use with data scaling.	*/
	return 0;										/* return successfully							*/
}

/****************************************************************************************************/
/* This function obtains magnetometer data for all three axes of the Honeywell device.  The data is	*/
/* read from six device registers using a multi-byte bus transfer.									*/
/*																									*/
/* @input value:	(*data)			- The address of a vector storing sensor axis data				*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> failed (I2C error)				*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned char MagGetData(vector3_t *data, unsigned char mode){
	int axis_data[3];								/* input data values:  X,Y,Z					*/
	hmc588l_axis_t input[3];
	
	if(mode == GET_SINGLE){
		MagWriteByte(HMC5883L_MODE_REG, MODE_SINGLE);
	
		do{
			/* Wait for the data ready signal.
			*
			* Instead of polling DRDY, as is done below, the device can be
			* placed in continuous measurement mode and this signal can be
			* configured to trigger an asynchronous interrupt.
			 */
		}while (IsDrdyHigh());
		MagWriteByte(HMC5883L_MODE_REG, MODE_CONTIN);
	}
	if(MagReadBytes(HMC5883L_MAG_X_HI, sizeof(input), (unsigned char*)input) != 0){
		return 1;									/* communication error							*/
	}
	
	/* Note: Device data register order = x, z, y !!												*/
	axis_data[0] = ((int16_t)((input[0].msb << 8) + input[0].lsb));
	axis_data[2] = ((int16_t)((input[1].msb << 8) + input[1].lsb));
	axis_data[1] = ((int16_t)((input[2].msb << 8) + input[2].lsb));
	
	if((axis_data[0] == DATA_OUTPUT_OVERFLOW) ||
	(axis_data[1] == DATA_OUTPUT_OVERFLOW) ||
	(axis_data[2] == DATA_OUTPUT_OVERFLOW)){
		return 1;
	}
	 /** Convert signed integer values to a real data type for calculations.
	 * Use device orientation configuration to assign axes.
	 */
	data->x = axis_data[0]; 
	data->y = axis_data[1];  
	data->z = axis_data[2];

	return (0);										/* return successfully							*/
}

/****************************************************************************************************/
/* This function sets up and executes an built-in self test function within the HMC5883L device.	*/
/*																									*/
/* @input value:	(test_code) -	Address of a numeric code of which test to perform. This		*/
/*									location will contain the specific test result code when the 	*/
/*									function returns.												*/
/* @return value:	(unsigned char)	- (0) -> successfully, (1) -> failed (I2C error)				*/
/*--------------------------------------------------------------------------------------------------*/
static unsigned char MagSelfTest(int *test_code, vector_int_t *axis){
	unsigned char meas_mode, status=0;
	vector3_t data;
	struct {
		unsigned char config_reg_a;
		unsigned char config_reg_b;
		unsigned char mode_reg;
	}
	reg_set;
	
	if(MagReadBytes(HMC5883L_CONFIG_REG_A, sizeof(reg_set), (unsigned char*)&reg_set) != 0){
		*test_code = SENSOR_TEST_ERR_READ;
		return 1;									/* self test failed								*/
	}
	MagWriteByte(HMC5883L_CONFIG_REG_B, HMC5883L_TEST_GAIN);
	
	/* Set test mode																				*/
	switch (*test_code) {							/* which test was specified						*/
		case SENSOR_TEST_DEFAULT:
		case SENSOR_TEST_BIAS_POS:
		meas_mode = MEAS_MODE_POS;					/* positive bias measurement mode				*/
		break;

		case SENSOR_TEST_BIAS_NEG:
		meas_mode = MEAS_MODE_NEG;					/* negative bias measurement mode				*/
		break;

		default:
		/* bad test code specified																	*/
		MagWriteByte(HMC5883L_CONFIG_REG_B, reg_set.config_reg_b);
		*test_code = SENSOR_TEST_ERR_FUNCTION;
		return 1;									/* return error									*/
	}
	
	MagWriteByte(HMC5883L_CONFIG_REG_A, DATA_RATE_15HZ | MEAS_AVG_1 | meas_mode);
	
	_delay_ms(SELF_TEST_DELAY_MSEC);
	
	MagWriteByte(HMC5883L_MODE_REG, MODE_SINGLE);
	
	if(MagGetData(&data, GET_SINGLE) != 0){
		*test_code = SENSOR_TEST_ERR_READ;
		status = 1;									/* return error									*/
	}
	else{
		
		axis->x = (int)data.x;
		axis->y = (int)data.y;
		axis->z = (int)data.z;
		
		/* Check range of readings																	*/
		if ((HMC5883L_TEST_X_MIN > data.x) ||
		(data.x > HMC5883L_TEST_X_MAX) ||
		(HMC5883L_TEST_Y_MIN > data.y) ||
		(data.y > HMC5883L_TEST_Y_MAX) ||
		(HMC5883L_TEST_Z_MIN > data.z) ||
		(data.z > HMC5883L_TEST_Z_MAX)) {
			*test_code = SENSOR_TEST_ERR_RANGE;
			status = 1;								/* value out of range							*/
		}
	}
	
	/* Restore registers																			*/
	if(MagWriteByte(HMC5883L_CONFIG_REG_A, reg_set.config_reg_a) != 0){
		*test_code = SENSOR_TEST_ERR_WRITE;
		return 1;
	}
	if(MagWriteByte(HMC5883L_CONFIG_REG_B, reg_set.config_reg_b) != 0){
		*test_code = SENSOR_TEST_ERR_WRITE;
		return 1;
	}
	if(MagWriteByte(HMC5883L_MODE_REG, reg_set.mode_reg) != 0){
		*test_code = SENSOR_TEST_ERR_WRITE;
		return 1;
	}
	if(!status){
		*test_code = SENSOR_TEST_ERR_NONE;
	}
	return status;
}

/****************************************************************************************************/
/* Function initialize magnetometer HMC5883L, and must be called before using another 				*/
/* magnetometer function.																			*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(int)	- (0) -> successfully, (-1) -> initialization failed (I2C error)		*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroMagClass::init(void){
	InitDrdy();
		
	if(MagWriteByte(HMC5883L_MODE_REG, MODE_CONTIN) != 0)
		return (-1);				/* return error -> communication failed							*/
	
	MagWriteByte(HMC5883L_CONFIG_REG_A, DATA_RATE_75HZ | MEAS_MODE_NORM);
		
	MagSetRange(index_130uT);
	MagSetBandwidth(index_75hz);
	
	eeprom_read_block(&cal_data.sensitivity, (void*)CAL_SENSITIVITY_ADDR, sizeof(vector3_t));
	eeprom_read_block(&cal_data.offsets, (void*)CAL_OFFSETS_ADDR , sizeof(vector3_t));
	my_mag_available = 1;					/* mag is available										*/	
	return (0);								/* return successfully									*/
}

/****************************************************************************************************/
/* Function verify mag sensor I2C connection. Make sure the device is connected and  				*/
/* responds as expected. Return True if connection is valid, false otherwise.						*/
/*																									*/
/* @input value:	(none)			 																*/
/* @return value:	(unsigned char)	- (0) -> connection failed, (1) -> connection successfully		*/
/*--------------------------------------------------------------------------------------------------*/
unsigned char EmoroMagClass::testConnection(void){
	unsigned char buffer[3];
	if(MagReadBytes(HMC5883L_ID_REG_A, 3, buffer) != 0)	/* Read mag device ID failed?				*/
		return (0);										/* return 0 								*/
	if(buffer[0] == 0x48 && buffer[1] == 0x34 && buffer[2] == 0x33)
		return (1);										/* return connection successfully			*/
	return (0);											/* return connection failed					*/
}

/****************************************************************************************************/
/* This function reads direction inclination and strength from compass device.						*/
/*																									*/
/* @input value:	(*direction)																	*/
/*					(*inclination)																	*/
/*					(*strength)																		*/
/* @return value:	(int)			- (0) -> successfully, (-1) -> failed (I2C error)				*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroMagClass::read(int *direction, int *inclination, int *strength){
	/* Get magnetic field measurements & test for data overflow.									*/
	vector3_t mag_data;
	unsigned char result;
	float theta_f;				/* direction angle (degrees)										*/
	float delta_f;				/* inclination angle (degrees)										*/
	float strength_f;			/* magnetic field intensity											*/
	
	if(!my_mag_available)							/* mag not available							*/
		return (-1);								/* return error: communication failed			*/
	
	result = MagGetData(&mag_data, GET_CONTIN);
	
	if(result == 0){
		MagApplySensitivity(&mag_data);			/* Apply sensitivity adjustment to data				*/
		MagApplyOffset(&mag_data);				/* Apply measurement offsets to data				*/
		
		/* Calculate direction, inclination, and field strength.									*/
		result = MagFieldDirection(&mag_data, &theta_f, &delta_f, &strength_f);
		
		if (result == 0) {
			strength_f *= ((float)GAUSS_TO_MICRO_TESLA /scale_table[mag_range]);

			*direction   = (int)theta_f;
			*inclination = (int)delta_f;
			*strength    = (int)strength_f;
		}
		else
			return (-1);
	}
	else
		return (-1);
	return 0;									/* communication successfully						*/
}

/****************************************************************************************************/
/* This function reads direction from compass device.												*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(int)	- direction																*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroMagClass::readDirection(void){
	/* Get magnetic field measurements & test for data overflow.									*/
	vector3_t mag_data;
	unsigned char result;
	float theta_f;				/* direction angle (degrees)										*/
	float delta_f;				/* inclination angle (degrees)										*/
	float strength_f;			/* magnetic field intensity											*/
	
	if(!my_mag_available)							/* mag not available							*/
		return (0);									/* return error: communication failed			*/

	result = MagGetData(&mag_data, GET_CONTIN);
	
	if(result == 0){
		MagApplySensitivity(&mag_data);			/* Apply sensitivity adjustment to data				*/
		MagApplyOffset(&mag_data);				/* Apply measurement offsets to data				*/
		
		/* Calculate direction, inclination, and field strength.									*/
		result = MagFieldDirection(&mag_data, &theta_f, &delta_f, &strength_f);
		if (result == 0)
			return (int)theta_f;
	}
	return 0;
}

/****************************************************************************************************/
/* This function reads inclination from compass device.												*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(int)	- inclination															*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroMagClass::readInclination(void){
	/* Get magnetic field measurements & test for data overflow.									*/
	vector3_t mag_data;
	unsigned char result;
	float theta_f;				/* direction angle (degrees)										*/
	float delta_f;				/* inclination angle (degrees)										*/
	float strength_f;			/* magnetic field intensity											*/
	
	if(!my_mag_available)							/* mag not available							*/
		return (0);									/* return error: communication failed			*/

	result = MagGetData(&mag_data, GET_CONTIN);
	
	if(result == 0){
		MagApplySensitivity(&mag_data);			/* Apply sensitivity adjustment to data				*/
		MagApplyOffset(&mag_data);				/* Apply measurement offsets to data				*/
		
		/* Calculate direction, inclination, and field strength.									*/
		result = MagFieldDirection(&mag_data, &theta_f, &delta_f, &strength_f);
		if (result == 0) 
			return (int)delta_f;
	}
	return 0;
}

/****************************************************************************************************/
/* This function reads strength from compass device.												*/
/*																									*/
/* @input value:	(none)																			*/
/* @return value:	(int)	- strength																*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroMagClass::readStrength(void){
	/* Get magnetic field measurements & test for data overflow.									*/
	vector3_t mag_data;
	unsigned char result;
	float theta_f;				/* direction angle (degrees)										*/
	float delta_f;				/* inclination angle (degrees)										*/
	float strength_f;			/* magnetic field intensity											*/
	
	if(!my_mag_available)							/* mag not available							*/
		return (0);									/* return error: communication failed			*/

	result = MagGetData(&mag_data, GET_CONTIN);
	
	if(result == 0){
		MagApplySensitivity(&mag_data);			/* Apply sensitivity adjustment to data				*/
		MagApplyOffset(&mag_data);				/* Apply measurement offsets to data				*/
		
		/* Calculate direction, inclination, and field strength.									*/
		result = MagFieldDirection(&mag_data, &theta_f, &delta_f, &strength_f);
		
		if (result == 0) {
			strength_f *= ((float)GAUSS_TO_MICRO_TESLA /scale_table[mag_range]);
			return (int)strength_f;
		}
	}
	return 0;
}

/****************************************************************************************************/
/* This routine must be called 3 times total, with the "step" parameter indicating what stage of	*/
/* the calibration is being performed.																*/
/* Calibration step 1:	Lay board flat!																*/
/* Calibration step 2:  Rotate 180 degrees!															*/
/* Calibration step 3:	Flip board!																	*/
/*																									*/
/* @input value:	(step)			-  The calibration stage number (1 to 3).						*/
/* @return value:	(int)			- (0)  -> successfully 											*/
/*									- (-1) -> failed (I2C error)									*/
/*									- (-2) -> step out of range										*/
/*--------------------------------------------------------------------------------------------------*/
int EmoroMagClass::calibrate(unsigned char step){
	static vector3_t step_data[3];				 /* sensor readings during calibration				*/
	vector_int_t axis;
	vector3_t read_back;
	vector3_t dummy_data; 
	int test_code;

	if(!my_mag_available)							/* mag not available							*/
		return (-1);								/* return error: communication failed			*/

	/* Validate the supported calibration types and step number.									*/
	if ((step < 1) || (step > 3)) {		
		return (-2);							/* calibration step out of range					*/
	}
	
	/* During the first pass, use self-test to determine sensitivity scaling						*/
	if (step == 1){
		test_code = SENSOR_TEST_BIAS_POS;
	
		if( (MagSelfTest(&test_code, &axis) != 0) || test_code != SENSOR_TEST_ERR_NONE)
			return (-1);						/* return error										*/
	
		/* Calculate & store sensitivity adjustment values											*/
		cal_data.sensitivity.x = (float)HMC5883L_TEST_X_NORM / axis.x;
		cal_data.sensitivity.y = (float)HMC5883L_TEST_Y_NORM / axis.y;
		cal_data.sensitivity.z = (float)HMC5883L_TEST_Z_NORM / axis.z;
	
		eeprom_write_block(&cal_data.sensitivity, (void*)CAL_SENSITIVITY_ADDR, sizeof(vector3_t));
		eeprom_read_block( &read_back, (void*)CAL_SENSITIVITY_ADDR, sizeof(vector3_t));

		if (memcmp(&cal_data.sensitivity, &read_back, sizeof(vector3_t))){
			return (-1);
		}
	}
	
	/* Read sensor data and test for data overflow.
	 *   Note: Sensor must be read twice - the first reading may
	 *         contain stale data from previous orientation.
	 */
	if (MagGetData(&dummy_data, GET_SINGLE) != 0) {
		return (-1);
	}
	_delay_ms(READ_DELAY_MSEC);
	
	if (MagGetData(&(step_data[step - 1]), GET_SINGLE) != 0) {
		return (-1);
	}
	
	/* Apply sensitivity scaling to the uncalibrated input field vector.							*/
	MagApplySensitivity(&(step_data [step - 1]));
	
	switch(step) {	
	/* There's nothing more to do on the first two passes.											*/
	case 1:
	case 2:
		break;
	
	/* Calculate & store the offsets on the final pass.												*/
	case 3:
		cal_data.offsets.x = (step_data[0].x + step_data[1].x) / 2;
		cal_data.offsets.y = (step_data[0].y + step_data[1].y) / 2;
		cal_data.offsets.z = (step_data[1].z + step_data[2].z) / 2;

		eeprom_write_block(&cal_data.offsets, (void*)CAL_OFFSETS_ADDR, sizeof(cal_data.offsets));

		/* Read back data and confirm it was written correctly */
		eeprom_read_block(&read_back, (void*)CAL_OFFSETS_ADDR , sizeof(vector3_t));

		if (memcmp(&cal_data.offsets, &read_back, sizeof(vector3_t))) {
			return (-1);
		}
		break;

	default:
		return (-2);								/* bad step value								*/
	}
	return (0);										/* return successfully							*/
}

/****************************************** M A G   C L A S S ***************************************/
EmoroMagClass Mag;

/****************************************************************************************************/
/*											end of file												*/
/****************************************************************************************************/
