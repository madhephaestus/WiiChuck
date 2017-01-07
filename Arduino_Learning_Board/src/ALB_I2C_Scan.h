/* 
 * Arduino Learning Board Project
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * I2C_Scan - I2C Bus Scanner Utility to find and report any devices
 *            on the I2C Bus
 *
 * (Most of the included libraries were "collected" from other places
 *  and combined into one master library to simplify using the
 *  Arduino Learning Board sample programs)
 *
 */

#ifndef ALB_I2C_Scan_h
#define ALB_I2C_Scan_h

#include "Arduino.h"
#include "ALB_Wire.h"


class ALB_I2C_Scan
{
	public:
		ALB_I2C_Scan(); // This defines a constructor for the class ALB_I2CScan
		~ALB_I2C_Scan(); // This defines a destructor for the class ALB_I2CScan
		void scan();
	private:
};

#endif