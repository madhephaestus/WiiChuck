/* Analog12bitReadSerial
 Copyright (c) 2016 Alorim Technology.  All right reserved.
 by Matt Weber (linkedin.com/in/mattweber0) of
 Alorium Technology (info@aloriumtech.com)
 
 Uses the XLR8 ADC to read analog input pin 0, do both 10bit (0-1023, about
  4.9mV per step) and 12bit (0-4095, about 1.2mv per step) analog to digital
  conversions, and print the result values to Serial.
 Hardware wiring is the same as Arduino's Examples-Basics-AnalogReadSerial
   Attach the center pin of a potentiometer to pin A0, and the 
   outside pins to +5V and ground.
*/
  
#include <XLR8ADC.h>

void setup() {
  Serial.begin(9600);
  // Enable the XLR8 ADC. Starts in 12 bit mode
  XLR8ADC.begin();
}

void loop() {
  XLR8ADC.set12bitMode();
  // reading the input on analog pin 0 uses the ordinary analogRead()
  int sensorValue12 = analogRead(A0);
  XLR8ADC.set10bitMode();
  // read again after setting 10 bit mode
  int sensorValue10 = analogRead(A0);
  // print out the values read:
  Serial.print(sensorValue12);
  Serial.print("    ");
  Serial.print(sensorValue10);  
  delay(1);        // delay in between reads so Serial doesn't get overwhelmed
}


