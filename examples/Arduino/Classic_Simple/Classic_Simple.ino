#include <WiiChuck.h>

#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif

Classic classic(SDA, SCL);
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  classic.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  classic.readData(); 
  int joyLeftY = classic.getJoyYLeft();
  Serial.println(joyLeftY, DEC);
}
