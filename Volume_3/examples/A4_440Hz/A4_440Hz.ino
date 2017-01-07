#include "Volume3.h"
#define speakerPin 9

uint16_t frequency = 440;

void setup() {
  // Nothing neeeded here!
}

void loop() {  
  for(uint16_t volume = 0; volume < 1023; volume++){
    vol.tone(speakerPin,frequency,volume);
    delay(1);
  }
  for(uint16_t volume = 1023; volume > 0; volume--){
    vol.tone(speakerPin,frequency,volume);
    delay(1);
  }
}
