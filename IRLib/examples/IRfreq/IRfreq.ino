/* Example program for from IRLib – an Arduino library for infrared encoding and decoding
 * Version 1.5   June 2014
 * Copyright 2014 by Chris Young http://tech.cyborg5.com
 * Based on Arduino firmware for use with AnalysIR IR signal analysis
 * software for Windows PCs. Many thanks to the people 
 * at http://analysir.com for their assistance In developing this program.
 */
/*
 * IRLib: IRfreq - dump details of IR codes with IRrecvPCI and
 * measures frequency using IRfrequency. You must connect an IR learner 
 * such as TSMP58000 to a hardware interrupt pin.
 */

#include <IRLib.h>

#define FREQUENCY_INTERRUPT 0

IRfrequency My_Freq(FREQUENCY_INTERRUPT);

void setup()
{
  Serial.begin(9600);
  delay(2000);while(!Serial);//delay for Leonardo
  Serial.print(F("Interrupt="));Serial.print(FREQUENCY_INTERRUPT,DEC);
  Serial.print(F(" Pin=")); Serial.println(My_Freq.getPinNum(),DEC);
  if(My_Freq.getPinNum()==255)
    Serial.println(F("Invalid interrupt number."));
  My_Freq.enableFreqDetect();//starts interrupt routine to compute frequency
}

void loop() {
  if (My_Freq.HaveData()) {
    delay(500);  //it couldn't hurt to collect a little more just in case
    My_Freq.disableFreqDetect();
    My_Freq.DumpResults(false);//Change to "true" for more detail
    My_Freq.enableFreqDetect();//Zero out previous results and restart YSR
  }
}

