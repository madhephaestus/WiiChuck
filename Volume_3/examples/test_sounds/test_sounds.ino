#include "Volume3.h"

#define speakerPin 9

void setup() {
  // put your setup code here, to run once:
  wolfWhistle();
  delay(1000);
  R2D2();
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void gameboy(){
  vol.tone(speakerPin,1025,1023); // pa
  delay(70);
  uint16_t v = 1000;
  while(v > 0){
    vol.tone(speakerPin, 2090, v); // ting!
    delay(10);
    v-=10;
  }
}

void wolfWhistle() {
  int f = 122; // starting frequency
  int v = 0;   // starting volume
  while (f < 4000) {  // slide up to 4000Hz
    vol.tone(speakerPin, f, v);
    v = 1023 * (f / 4000.00);
    f += 25;
    delay(1);
  }
  vol.noTone();
  delay(100); // wait a moment
  f = 122; // starting frequency
  v = 0;   // starting volume
  while (f < 3000) { // slide up to 3000Hz
    vol.tone(speakerPin, f, v);
    v = 1023 * (f / 4000.00); 
    f += 25;
    delay(2);
  }
  while (f > 125) { // slide down to 125Hz
    vol.tone(speakerPin, f, v);
    v = 1023 * (f / 4000.00);
    f -= 25;
    delay(2);
  }
  vol.noTone(); // end tone production
}

void R2D2() {
  int beeps[] = {1933, 2156, 1863, 1505, 1816, 1933, 1729, 2291};
  int buzzVols[] = {144, 180, 216, 252, 252, 252, 252, 216, 180, 144};

  int i = 9;
  while (i >= 0) {
    vol.tone(speakerPin, 1050, buzzVols[i]*4);
    delayMicroseconds(20*64);
    vol.tone(speakerPin, 1050, buzzVols[i] / 8*4);
    delayMicroseconds(40*64);
    i--;
  }

  delay(35);

  i = 0;
  while (i < 8) {
    int v = 0;
    while (v < 250) { // 12.5 mS fade up time
      vol.tone(speakerPin, beeps[i], v*4);
      v += 10;
      delayMicroseconds(2*64);
    }
    delay(20);
    v = 250;
    while (v > 0) { // 12.5 mS fade down time
      vol.tone(speakerPin, beeps[i], v*4);
      v -= 10;
      delayMicroseconds(5*64);
    }
    vol.noTone();
    delay(25);
    i++;
  }

  int f = 2466;
  while (f < 2825) {
    vol.tone(speakerPin, f, 1023);
    f += 3;
    delay(1);
  }  
  f = 2825;
  int v = 255;
  while (f > 2000) {
    vol.tone(speakerPin, f, v*4);
    f -= 6;
    v -= 1;
    delay(1);
  }
  vol.noTone();
  delay(35);

  i = 10;
  while (i > 0) {
    vol.tone(speakerPin, 1050, buzzVols[i]*4);
    delayMicroseconds(20*64);
    vol.tone(speakerPin, 1050, buzzVols[i] / 8*4);
    delayMicroseconds(40*64);
    i--;
  }
  vol.noTone();
}
