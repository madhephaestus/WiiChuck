#include "Volume3.h"
#define speakerPin 9

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  chirpFade();
  delay(random(100,5000));
}

void chirpFade() {
  uint16_t f = 3900;

  uint8_t times = random(1,3);
  float master = 1.0;
  uint16_t v = 0;
  uint8_t vDir = 1;
  float vb = 0;

  while (times > 0) {
    while (vb < 1.0) {
      if (v < 1023 && vDir == 1) {
        v += 16;
      }
      else {
        vDir = 0;
      }

      if (v > 0 && vDir == 0) {
        v -= 16;
      }
      else {
        vDir = 1;
      }

      vol.tone(speakerPin, f, v * constrain(vb, 0.0, 1.0)*master);
      delayMicroseconds(50);
      vb += 0.003;
    }
    while (vb > 0.0) {
      if (v < 1023 && vDir == 1) {
        v += 16;
      }
      else {
        vDir = 0;
      }

      if (v > 0 && vDir == 0) {
        v -= 16;
      }
      else {
        vDir = 1;
      }

      vol.tone(speakerPin, f, v * constrain(vb, 0.0, 1.0)*master);
      delayMicroseconds(50);
      vb -= 0.001;
    }
    times--;
    master -= 0.75;
  }
  vol.noTone();
}
