/*
 * Copyright (c) 2015 by 
 Arturo Guadalupi <a.guadalupi@arduino.cc>
 Angelo Scialabba <a.scialabba@arduino.cc>
 Claudio Indellicati <c.indellicati@arduino.cc> <bitron.it@gmail.com>
 
 * Audio library for Arduino Zero.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "AudioZero.h"
#include <SD.h>
#include <SPI.h>


/*Global variables*/
bool __StartFlag;
volatile uint32_t __SampleIndex;
uint32_t __HeadIndex;
uint32_t __NumberOfSamples; // Number of samples to read in block
uint8_t *__WavSamples;

int __Volume;

void AudioZeroClass::begin(uint32_t sampleRate) {
	
	__StartFlag = false;
	__SampleIndex = 0;					//in order to start from the beginning
	__NumberOfSamples = 1024;	//samples to read to have a buffer
	
	/*Allocate the buffer where the samples are stored*/
	__WavSamples = (uint8_t *) malloc(__NumberOfSamples * sizeof(uint8_t));
	
	/*Modules configuration */
  	dacConfigure();
	tcConfigure(sampleRate);
}

void AudioZeroClass::end() {
	tcDisable();
	tcReset();
	analogWrite(A0, 0);	
}

/*void AudioZeroClass::prepare(int volume){
//Not Implemented yet
}*/

void AudioZeroClass::play(File myFile) {
while (myFile.available()) {
	 if (!__StartFlag)
    {
      myFile.read(__WavSamples, __NumberOfSamples);
      __HeadIndex = 0;
	  
	  /*once the buffer is filled for the first time the counter can be started*/
      tcStartCounter();
      __StartFlag = true;
    }
    else
    {
      uint32_t current__SampleIndex = __SampleIndex;
      
      if (current__SampleIndex > __HeadIndex) {
        myFile.read(&__WavSamples[__HeadIndex], current__SampleIndex - __HeadIndex);
        __HeadIndex = current__SampleIndex;        
      }
      else if (current__SampleIndex < __HeadIndex) {
        myFile.read(&__WavSamples[__HeadIndex], __NumberOfSamples-1 - __HeadIndex);
        myFile.read(__WavSamples, current__SampleIndex);
        __HeadIndex = current__SampleIndex;
      }
    }
}
	myFile.close();
}


/**
 * Configures the DAC in event triggered mode.
 *
 * Configures the DAC to use the module's default configuration, with output
 * channel mode configured for event triggered conversions.
 */
void AudioZeroClass::dacConfigure(void){
	analogWriteResolution(10);
	analogWrite(A0, 0);
}

/**
 * Configures the TC to generate output events at the sample frequency.
 *
 * Configures the TC in Frequency Generation mode, with an event output once
 * each time the audio sample frequency period expires.
 */
 void AudioZeroClass::tcConfigure(uint32_t sampleRate)
{
	// Enable GCLK for TCC2 and TC5 (timer counter input clock)
	GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
	while (GCLK->STATUS.bit.SYNCBUSY);

	tcReset();

	// Set Timer counter Mode to 16 bits
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;

	// Set TC5 mode as match frequency
	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;

	TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE;

	TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate - 1);
	while (tcIsSyncing());
	
	// Configure interrupt request
	NVIC_DisableIRQ(TC5_IRQn);
	NVIC_ClearPendingIRQ(TC5_IRQn);
	NVIC_SetPriority(TC5_IRQn, 0);
	NVIC_EnableIRQ(TC5_IRQn);

	// Enable the TC5 interrupt request
	TC5->COUNT16.INTENSET.bit.MC0 = 1;
	while (tcIsSyncing());
}	


bool AudioZeroClass::tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}

void AudioZeroClass::tcStartCounter()
{
  // Enable TC

  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}

void AudioZeroClass::tcReset()
{
  // Reset TCx
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}

void AudioZeroClass::tcDisable()
{
  // Disable TC5
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}

AudioZeroClass AudioZero;

#ifdef __cplusplus
extern "C" {
#endif

void Audio_Handler (void)
{
  if (__SampleIndex < __NumberOfSamples - 1)
  {
    analogWrite(A0, __WavSamples[__SampleIndex++]);

    // Clear the interrupt
    TC5->COUNT16.INTFLAG.bit.MC0 = 1;
  }
  else
  {
    __SampleIndex = 0;
    TC5->COUNT16.INTFLAG.bit.MC0 = 1;
	}
}

void TC5_Handler (void) __attribute__ ((weak, alias("Audio_Handler")));

#ifdef __cplusplus
}
#endif