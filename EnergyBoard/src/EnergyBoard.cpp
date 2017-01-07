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

#include "Arduino.h"
#include "EnergyBoard.h"

#define EnergyBoard_Serial Serial3				// Define EnergyBoard Serial

EnergyBoard::EnergyBoard(int Gain_) {
    
    EnergyBoard_Serial.begin(BoudRate);

    ClearBuffer();
    EnergyBoard_Serial.write(0xAA);			// Header (0xAA)
    EnergyBoard_Serial.write(0x04);			// Total Sended Byte (0x04)
    EnergyBoard_Serial.write(0xC4);			// Setting Command (0xC4)
    EnergyBoard_Serial.write(0x8E);			// CheckSum (0x8E)

	delay(10);
	
	ClearBuffer();
	EnergyBoard_Serial.write(0xAA);			// Header (0xAA)
	EnergyBoard_Serial.write(0x06);			// Total Sended Byte (0x06)
	EnergyBoard_Serial.write(0xCA);			// Setting Command (0xCA)
	EnergyBoard_Serial.write(0x65);			// Setting Command (0x65)
	EnergyBoard_Serial.write(0xFF);			// Setting Command (0xFF)
	EnergyBoard_Serial.write(0x22);			// CheckSum (0x22)
	EnergyBoard_Serial.end();
    
    if (Gain_ != 0) Gain = true;
}

// Voltage Functions
float EnergyBoard::Voltage_RMS(char Phase) {
	
	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x90,0x00); // RMS Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
			break;
		case 'S':
			SendCommand(0x93,0x00); // RMS Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
			break;
		case 'T':
			SendCommand(0x96,0x00); // RMS Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
			break;
		default:
			return 0; // No Phase Selected Error
	}

	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
        // Close UART Connection
        EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
        // Control Recieved Data
        if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
            
            // Acknowledge with data.
            if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
            
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
        }
        else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}
float EnergyBoard::Voltage_RMS_Average() {
	
	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x99,0x00); // RMS Voltage Avarage = 0xAA,0x07,0xA3,0x99,0x00,0xE3,0x39

	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}
float EnergyBoard::Voltage_Instantaneous(char Phase) {
	
	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x87,0x00); // Instantaneous Voltage Phase R = 0xAA,0x07,0xA3,0x87,0x00,0xE3,0x39
			break;
		case 'S':
			SendCommand(0x8A,0x00); // Instantaneous Voltage Phase S = 0xAA,0x07,0xA3,0x8A,0x00,0xE3,0x36
			break;
		case 'T':
			SendCommand(0x8D,0x00); // Instantaneous Voltage Phase T = 0xAA,0x07,0xA3,0x8D,0x00,0xE3,0x33
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}
float EnergyBoard::Voltage_Fundamental(char Phase) {
	
	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x9C,0x00); // Fundamental Voltage Phase R = 0xAA,0x07,0xA3,0x90,0x00,0xE3,0x39
			break;
		case 'S':
			SendCommand(0x9F,0x00); // Fundamental Voltage Phase S = 0xAA,0x07,0xA3,0x93,0x00,0xE3,0x36
			break;
		case 'T':
			SendCommand(0xA2,0x00); // Fundamental Voltage Phase T = 0xAA,0x07,0xA3,0x96,0x00,0xE3,0x33
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}
float EnergyBoard::Voltage_Harmonic(char Phase) {
	
	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0xA5,0x00); // Harmonic Voltage Phase R = 0xAA,0x07,0xA3,0xA5,0x00,0xE3,0x39
			break;
		case 'S':
			SendCommand(0xA8,0x00); // Harmonic Voltage Phase S = 0xAA,0x07,0xA3,0xA8,0x00,0xE3,0x36
			break;
		case 'T':
			SendCommand(0xAB,0x00); // Harmonic Voltage Phase T = 0xAA,0x07,0xA3,0xAB,0x00,0xE3,0x33
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}
float EnergyBoard::Voltage_RMS_Alarm_Min() {

	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0xB1,0x00); // RMS Voltage MIN Limit = 0xAA,0x07,0xA3,0xB1,0x00,0xE3,0x39
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}
float EnergyBoard::Voltage_RMS_Alarm_Max() {

	float Voltage_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0xB4,0x00); // RMS Voltage MAX Limit = 0xAA,0x07,0xA3,0xB4,0x00,0xE3,0x39
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Voltage_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Voltage_ = Voltage_ * Voltage_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Voltage_ > Min_Read_Voltage) return Voltage_;
	return -3; // No Data Error
}

// Current Function
float EnergyBoard::Current_RMS(char Phase) {

	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0xD5,0x00); // RMS Current Phase R = 0xAA,0x07,0xA3,0xD5,0x00,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0xD8,0x00); // RMS Current Phase S = 0xAA,0x07,0xA3,0xD8,0x00,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0xDB,0x00); // RMS Current Phase T = 0xAA,0x07,0xA3,0xDB,0x00,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}
float EnergyBoard::Current_RMS_Average() {

	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0xDE,0x00); // RMS Current Avarage = 0xAA,0x07,0xA3,0xDE,0x00,0xE3,0xF4
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}
float EnergyBoard::Current_Peak(char Phase) {

	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0xCC,0x00); // Peak Current Phase R = 0xAA,0x07,0xA3,0xCC,0x00,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0xCF,0x00); // Peak Current Phase S = 0xAA,0x07,0xA3,0xCF,0x00,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0xD2,0x00); // Peak Current Phase T = 0xAA,0x07,0xA3,0xD2,0x00,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}
float EnergyBoard::Current_Instantaneous(char Phase) {
	
	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0xBA,0x00); // Instantaneous Current Phase R = 0xAA,0x07,0xA3,0xBA,0x00,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0xBD,0x00); // Instantaneous Current Phase S = 0xAA,0x07,0xA3,0xBD,0x00,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0xC0,0x00); // Instantaneous Current Phase T = 0xAA,0x07,0xA3,0xC0,0x00,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}
float EnergyBoard::Current_Fundamental(char Phase) {

	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0xE1,0x00); // Fundamental Current Phase R = 0xAA,0x07,0xA3,0xE1,0x00,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0xE4,0x00); // Fundamental Current Phase S = 0xAA,0x07,0xA3,0xE4,0x00,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0xE7,0x00); // Fundamental Current Phase T = 0xAA,0x07,0xA3,0xE7,0x00,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}
float EnergyBoard::Current_Harmonic(char Phase) {

	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0xEA,0x00); // Harmonic Current Phase R = 0xAA,0x07,0xA3,0xEA,0x00,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0xED,0x00); // Harmonic Current Phase S = 0xAA,0x07,0xA3,0xED,0x00,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0xF0,0x00); // Harmonic Current Phase T = 0xAA,0x07,0xA3,0xF0,0x00,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}
float EnergyBoard::Current_RMS_Alarm_Max() {

	float Current_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0xF3,0x00); // RMS Current Max Limit = 0xAA,0x07,0xA3,0xF3,0x00,0xE3,0xF4
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) Current_ = (Data3 * 65536 + Data2 * 256 + Data1);
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Current_ = Current_ * Current_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Current_ > Min_Read_Current) return Current_;
	return -3; // No Data Error
}

// Power Functions
float EnergyBoard::Power_Active(char Phase) {

	float Power_ = 0;
		
	EnergyBoard_Serial.begin(BoudRate);
		
	switch (Phase) {
		case 'R':
			SendCommand(0x1D,0x01); // Active Power Phase R = 0xAA,0x07,0xA3,0x1D,0x01,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0x20,0x01); // Active Power Phase S = 0xAA,0x07,0xA3,0x20,0x01,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0x23,0x01); // Active Power Phase T = 0xAA,0x07,0xA3,0x23,0x01,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
		
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {

		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();

		// Close UART Connection
		EnergyBoard_Serial.end();

		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
			
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {

			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error

			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}

	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Active_Average() {
	
	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x38,0x01); // Active Power Avarage = 0xAA,0x07,0xA3,0x38,0x01,0xE3,0xF4
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Reactive(char Phase) {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x26,0x01); // ReActive Power Phase R = 0xAA,0x07,0xA3,0x26,0x01,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0x29,0x01); // ReActive Power Phase S = 0xAA,0x07,0xA3,0x29,0x01,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0x2C,0x01); // ReActive Power Phase T = 0xAA,0x07,0xA3,0x2C,0x01,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Reactive_Average() {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x3B,0x01); // ReActive Power Avarage = 0xAA,0x07,0xA3,0x38,0x01,0xE3,0xF4
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Apparent(char Phase) {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x2F,0x01); // Apparent Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0x32,0x01); // Apparent Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0x35,0x01); // Apparent Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Apparent_Average() {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x3E,0x01); // Apparent Power Avarage = 0xAA,0x07,0xA3,0x3E,0x01,0xE3,0xF4
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Fundamental(char Phase) {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x4A,0x01); // Fundamental Power Phase R = 0xAA,0x07,0xA3,0x2F,0x01,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0x4D,0x01); // Fundamental Power Phase S = 0xAA,0x07,0xA3,0x32,0x01,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0x50,0x01); // Fundamental Power Phase T = 0xAA,0x07,0xA3,0x35,0x01,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Harmonic(char Phase) {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x53,0x01); // Harmonic Power Phase R = 0xAA,0x07,0xA3,0x53,0x01,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0x56,0x01); // Harmonic Power Phase S = 0xAA,0x07,0xA3,0x56,0x01,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0x59,0x01); // Harmonic Power Phase T = 0xAA,0x07,0xA3,0x59,0x01,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Fundamental_VA(char Phase) {

	float Power_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x5C,0x01); // Fundamental VA Phase R = 0xAA,0x07,0xA3,0x53,0x01,0xE3,0xF4
			break;
		case 'S':
			SendCommand(0x5F,0x01); // Fundamental VA Phase S = 0xAA,0x07,0xA3,0x56,0x01,0xE3,0xF1
			break;
		case 'T':
			SendCommand(0x62,0x01); // Fundamental VA Phase T = 0xAA,0x07,0xA3,0x59,0x01,0xE3,0xEE
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_ = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_ = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_ = Power_ * Power_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	if (Power_ > Min_Read_Power) return Power_;
	return -3; // No Data Error
}
float EnergyBoard::Power_Factor(char Phase) {

	float Power_Factor = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	switch (Phase) {
		case 'R':
			SendCommand(0x65,0x01); // PowerFactor Phase R = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
			break;
		case 'S':
			SendCommand(0x68,0x01); // PowerFactor Phase S = 0xAA,0x07,0xA3,0x68,0x01,0xE3,0x60
			break;
		case 'T':
			SendCommand(0x6B,0x01); // PowerFactor Phase T = 0xAA,0x07,0xA3,0x6B,0x01,0xE3,0x5D
			break;
		default:
			return 0; // No Phase Selected Error
	}
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_Factor = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_Factor = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_Factor = Power_Factor * Power_Factor_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	return Power_Factor;
}
float EnergyBoard::Power_Factor_Average() {

	float Power_Factor = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x6E,0x01); // PowerFactor Avarage = 0xAA,0x07,0xA3,0x65,0x01,0xE3,0x63
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				if (Data3 < 128) {
					Power_Factor = (Data3 * 65536 + Data2 * 256 + Data1);
				}
				else {
					Power_Factor = -((255 - Data3) * 65536 + (255 - Data2) * 256 + (255 - Data1));
				}
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Power_Factor = Power_Factor * Power_Factor_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	return Power_Factor;
}

// Other Functions
float EnergyBoard::Frequency(void) {

	float Frequency_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x80,0x01); // Frequency = 0xAA,0x07,0xA3,0x80,0x01,0xE3,0x48
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				Frequency_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Frequency_ = Frequency_ * Frequency_Gain;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	return Frequency_;
}
float EnergyBoard::IC_Temperature(void) {

	float Temperature_ = 0;
	
	EnergyBoard_Serial.begin(BoudRate);
	
	SendCommand(0x74,0x01); // Temperature
	
	// Read Data From UART
	if (EnergyBoard_Serial.available()) {
		
		byte Header					= EnergyBoard_Serial.read();
		byte Recieved_Byte_Count	= EnergyBoard_Serial.read();
		byte Data1					= EnergyBoard_Serial.read();
		byte Data2					= EnergyBoard_Serial.read();
		byte Data3					= EnergyBoard_Serial.read();
		byte CheckSum				= EnergyBoard_Serial.read();
		
		// Close UART Connection
		EnergyBoard_Serial.end();
		
		// Calculate Checksum
		byte ChkS = 0x100 - ((Header + Recieved_Byte_Count + Data1 + Data2 + Data3) % 256);
		
		// Control Recieved Data
		if (int(Recieved_Byte_Count) == 6 and CheckSum == ChkS) {
			
			// Acknowledge with data.
			if (Header == 0xAA) {
				Temperature_ = (Data3 * 65536 + Data2 * 256 + Data1);
			}
			else {return -1;} // Header Error
			
			// Gain Correction
			if (Gain) Temperature_ = Temperature_ / 1000;
		}
		else {return -2;} // Recieved Data or Checksum Error
	}
	else {EnergyBoard_Serial.end();}
	
	return Temperature_;
}

// Private Functions
bool EnergyBoard::ClearBuffer(void) {

    delay(1); // UART State Change Delay
	EnergyBoard_Serial.flush(); while(EnergyBoard_Serial.available() > 0) EnergyBoard_Serial.read();
	delay(5);

	return true;
}
bool EnergyBoard::SendCommand(int CHR1, int CHR2) {
	
/*
	MAX78630 Serial Comminication Read Values Structure
	---------------------------------------------------
	Read Request : [1]-[2]-[3]-[4]-[5]-[6]-[7]
	---------------------------------------------------
	1. byte is the IC address byte (0xAA)
	2. byte is the total sended byte (0x07)
	3. byte is the package payload type (0xA3)
	4. byte is the request (RMS) byte (2)
	5. byte is the request (RMS) byte (1)
	6. byte is the requested byte count (0xE3)
	7. byte is the CRC correction byte (CHK)
*/
	
	ClearBuffer();

    int ChkS = 0x100 - ((0xAA + 0x07 + 0xA3 + CHR1 + CHR2 + 0xE3) % 256); // Calculate checksum
    
    EnergyBoard_Serial.write(0xAA);
    EnergyBoard_Serial.write(0x07);
    EnergyBoard_Serial.write(0xA3);
    EnergyBoard_Serial.write(CHR1);
    EnergyBoard_Serial.write(CHR2);
    EnergyBoard_Serial.write(0xE3);
    EnergyBoard_Serial.write(ChkS);
    delay(20);
    
    return true;
}