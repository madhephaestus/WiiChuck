#include "volume2.h"
Volume vol;

void setup() {
}

void loop() {
  vol.tone(440, SQUARE, 255); // 100% Volume
  vol.delay(1000);
  vol.tone(440, SQUARE, 192); // 75% Volume
  vol.delay(1000);
  vol.tone(440, SQUARE, 128); // 50% Volume
  vol.delay(1000);
  vol.tone(440, SQUARE, 64); // 25% Volume
  vol.delay(1000);
  vol.tone(440, SQUARE, 32); // 12.5% Volume
  vol.delay(1000);
}
