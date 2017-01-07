/* 
 * Arduino Learning Board Project
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * This is the main "Parent" library that gets included into each
 * sketch that requires one of the "child" libraries
 *
 * In each sketch, first DEFINE the components of the library that are 
 * going to be used for that sketch:
 *
 * #define USE_ALB_LCD_I2C // LCD Library
 *
 * THEN include the main ArduinoLearningBoard library.
 * Based on the one or more defines above, the appropriate
 * modules will be added to the project
 *
 * #include "ArduinoLearningBoard.h"
 *
 * List of child libraries that may be included:
 *   #define USE_ALB_I2C_Scan
 *   #define USE_ALB_LCD_I2C
 *   #define USE_ALB_DHT11
 *   #define USE_ALB_DS1302
 *   #define USE_ALB_Servo
 *   #define USE_ALB_Stepper
 *   #define USE_ALB_RFID
 *   #define USE_ALB_MPU6050
 *   #define USE_ALB_SevenSegment
 *
 */
 
/*
	ArduinoLearningBoard.h - Arduino Learning Board Library
	Created by Jeff Shapiro, July 2016
	http://ArduinoLearningBoard.com
*/
#ifndef ArduinoLearningBoard_h
#define ArduinoLearningBoard_h

#include "Arduino.h"

#if defined(USE_ALB_I2C_Scan)
	#include "ALB_I2C_Scan.h"
#endif

#if defined(USE_ALB_LCD_I2C)
	#include "ALB_LCD_I2C.h"
#endif

#if defined(USE_ALB_DHT11)
	#include "ALB_DHT11.h"
#endif

#if defined(USE_ALB_DS1302)
	#include "ALB_DS1302RTC.h"
#endif

#if defined(USE_ALB_Servo)
	#include "ALB_Servo.h"
#endif

#if defined(USE_ALB_Stepper)
	#include "ALB_AccelStepper.h"
#endif

#if defined(USE_ALB_RFID)
	#include "ALB_RFID.h"
#endif

#if defined(USE_ALB_MPU6050)
	#include "ALB_I2Cdev.h"
	#include "ALB_MPU6050_6Axis_MotionApps20.h"
	#include "ALB_Wire.h"
#endif

#if defined(USE_ALB_SevenSegment)
	#include "ALB_SevSeg.h"
#endif

#endif