#include "Volume3.h"
#define speakerPin 9

uint16_t volume = 0;
uint8_t vDir = 1;
uint16_t frequency = 100;
uint8_t fDir = 1;

void setup() {
  // Nothing needed here!
}

void loop() {
  randomSeed(analogRead(A0));
  
  if(volume < 1023 && vDir == 1){
    volume+=1;
  }
  else{
    vDir = 0;
  }
  if(volume > 1 && vDir == 0){
    volume-=1;
  }
  else{
    vDir = 1;
  }

  if(frequency < random(50,500) && fDir == 1){
    frequency+=random(0,5);
  }
  if(frequency > random(0,500) && fDir == 0){
    frequency-=random(0,5);
  }
  fDir = random(0,2);
  
  vol.tone(speakerPin,frequency,volume); 
  delay(4);
}
