#include "EnergyBoard.h"

EnergyBoard* Energy = 0;

void setup() {
Serial.begin(9600);
Energy = new EnergyBoard(1);
}

void loop() {

Serial.print("RMS Voltage : "); Serial.print(Energy->Voltage_RMS('R')); Serial.println(" V");
Serial.print("RMS Voltage Avarage : "); Serial.print(Energy->Voltage_RMS_Average()); Serial.println(" V");
Serial.print("Instantaneous Voltage : "); Serial.print(Energy->Voltage_Instantaneous('R')); Serial.println(" V");
Serial.print("Fundamental Voltage : "); Serial.print(Energy->Voltage_Fundamental('R')); Serial.println(" V");
Serial.print("Harmonic Voltage : "); Serial.print(Energy->Voltage_Harmonic('R')); Serial.println(" V");
Serial.print("RMS Voltage MIN Limit : "); Serial.print(Energy->Voltage_RMS_Alarm_Min()); Serial.println(" V");
Serial.print("RMS Voltage MAX Limit : "); Serial.print(Energy->Voltage_RMS_Alarm_Max()); Serial.println(" V");

Serial.print("RMS Current : "); Serial.print(Energy->Current_RMS('R')); Serial.println(" A");
Serial.print("RMS Current Avarege: "); Serial.print(Energy->Current_RMS_Average()); Serial.println(" A");
Serial.print("PEAK Current : "); Serial.print(Energy->Current_Peak('R')); Serial.println(" A");
Serial.print("Instantaneous Current : "); Serial.print(Energy->Current_Instantaneous('R')); Serial.println(" A");
Serial.print("Fundamental Current : "); Serial.print(Energy->Current_Fundamental('R')); Serial.println(" A");
Serial.print("Harmonic Current : "); Serial.print(Energy->Current_Harmonic('R')); Serial.println(" A");
Serial.print("RMS Current MAX Limit : "); Serial.print(Energy->Current_RMS_Alarm_Max()); Serial.println(" A");

Serial.print("Active Power : "); Serial.print(Energy->Power_Active('R')); Serial.println(" W");
Serial.print("Active Power Avarage : "); Serial.print(Energy->Power_Active_Average()); Serial.println(" W");
Serial.print("ReActive Power : "); Serial.print(Energy->Power_Reactive('R')); Serial.println(" VAR");
Serial.print("ReActive Power Avarage : "); Serial.print(Energy->Power_Reactive_Average()); Serial.println(" VAR");
Serial.print("Apparent Power : "); Serial.print(Energy->Power_Apparent('R')); Serial.println(" VA");
Serial.print("Apparent Power Avarage : "); Serial.print(Energy->Power_Apparent_Average()); Serial.println(" VA");
Serial.print("Fundamental Power : "); Serial.print(Energy->Power_Fundamental('R')); Serial.println(" W");
Serial.print("Harmonic Power : "); Serial.print(Energy->Power_Harmonic('R')); Serial.println(" W");
Serial.print("Fundamental Volt Amper : "); Serial.print(Energy->Power_Fundamental_VA('R')); Serial.println(" VA");
Serial.print("Power Factor : "); Serial.print(Energy->Power_Factor('R')); Serial.println(" ");
Serial.print("Power Factor Avarage : "); Serial.print(Energy->Power_Factor_Average()); Serial.println(" ");

Serial.print("Frequency : "); Serial.print(Energy->Frequency()); Serial.println(" Hz");
Serial.print("IC Temperature : "); Serial.print(Energy->IC_Temperature()); Serial.println(" C");
Serial.println("------------------------------------------");

delay(5000);
}
