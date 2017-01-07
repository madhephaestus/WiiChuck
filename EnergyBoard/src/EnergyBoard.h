/* *******************************************************************************
 *
 *  Copyright (C) 2014-2015 LVX Elektronik A.S. (info@lvx.com.tr)
 *  Can not be copied and/or distributed without the express permission of LVX
 *
 *	LVX Energy Board is a 3 phase energy monitoring development kit
 *
 *	The library is compatable with Arduino and the Open Hardware Community.
 *
 *	Library				: LVX Energy Board (MAX78630)
 *	Code Developer		: Mehmet Gunce Akkoyun (gunce.akkoyun@lvx.com.tr)
 *	GitHub				: https://github.com/LVXElektronik/EnergyBoard
 *	Revision			: 3.0.1
 *	Relase				: 27.10.2015
 *
 *********************************************************************************/

#ifndef __EnergyBoard__
#define __EnergyBoard__

#include <Arduino.h>

// EnergyBoard Setting
#define BoudRate            38400						// Serial Boud Rate (Default: 38400)
#define Voltage_Gain        0.0000793945940258412		// Voltage Gain Default
#define Current_Gain        0.00000596046186144157		// Current Gain Default
#define Power_Gain          0.00396962860118015		// Power Gain Default
#define Power_Factor_Gain   0.00000023841946872415		// Power Factor Gain Default
#define Frequency_Gain      0.0000152587672179127		// Frequency Gain Default
#define Min_Read_Voltage    12							// Minumum Voltage Read Value (V)
#define Min_Read_Current    1							// Minumum Current Read Value (A)
#define Min_Read_Power      100						// Miumum Power Read Value (W)

class EnergyBoard
{
public:
	EnergyBoard(int Gain_);							// Start EnergyBoard Startup Parameters
    float Voltage_RMS(char phase);					// Read RMS Voltage @ Selected Phase
	float Voltage_RMS_Average();					// Read RMS Voltage Average
    float Voltage_Instantaneous(char phase);		// Read Instantaneous Voltage @ Selected Phase
    float Voltage_Fundamental(char phase);			// Read Fundamental Voltage @ Selected Phase
    float Voltage_Harmonic(char phase);				// Read Harmonic Voltage @ Selected Phase
    float Voltage_RMS_Alarm_Min();					// Read RMS Voltage Min Alarm Limit
    float Voltage_RMS_Alarm_Max();					// Read RMS Voltage Max Alarm Limit
    float Current_RMS(char phase);					// Read RMS Current at selected phase
	float Current_RMS_Average();					// Read RMS Current Avarage
	float Current_Peak(char phase);					// Read Peak Current at selected phase
	float Current_Instantaneous(char phase);		// Read Instantaneous Current at selected phase
	float Current_Fundamental(char phase);			// Read Fundamental Current at selected phase
	float Current_Harmonic(char phase);				// Read Harmonic Current at selected phase
	float Current_RMS_Alarm_Max();					// Read RMS Current Max Limit
	float Power_Active(char phase);					// Read Active Power at selected phase
	float Power_Active_Average();					// Read Active Power Avarage
	float Power_Reactive(char phase);				// Read ReActive Power at selected phase
	float Power_Reactive_Average();					// Read ReActive Power Avarage
	float Power_Apparent(char phase);				// Read Apparent Power at selected phase
	float Power_Apparent_Average();					// Read ApparentPower Power Avarage
	float Power_Fundamental(char phase);			// Read Fundamental Power at selected phase
	float Power_Harmonic(char phase);				// Read Harmonic Power at selected phase
	float Power_Fundamental_VA(char phase);			// Read Fundamental Volt Amper at selected phase
    float Power_Factor(char phase);					// Read Power Factor at selected phase
	float Power_Factor_Average();					// Read Power Factor Avarage
    float Frequency(void);							// Read Frequency of system
    float IC_Temperature(void);						// Chip Temperature
    
private:
	bool ClearBuffer(void);							// Clear Serial Buffer Function (internal)
    bool SendCommand(int CHR1, int CHR2);			// Send Command
    bool Gain;										// Gain Variable
};

#endif /* defined(__EnergyBoard__) */
