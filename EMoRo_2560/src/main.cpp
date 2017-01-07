#include <Arduino.h>

#include "pitches.h"
#define NO_START_TONES 

int main(void)
{
	init();									/* arduino basic initialization		*/

	InitEmoro();							/* emoro basic initialization		*/

#ifndef NO_START_TONES
	tone(BUZ_BUILTIN, NOTE_C5, 90);	delay(95);
	tone(BUZ_BUILTIN, NOTE_E5, 90);	delay(95);
	tone(BUZ_BUILTIN, NOTE_G5, 90);	delay(95);
	tone(BUZ_BUILTIN, NOTE_C6, 90);	delay(95);
	noTone(BUZ_BUILTIN);
#endif
	
#if defined(USBCON)
	USBDevice.attach();
#endif
	
	setup();
    
	for (;;) {
		loop();
		if (serialEventRun) serialEventRun();
	}
        
	return 0;
}

