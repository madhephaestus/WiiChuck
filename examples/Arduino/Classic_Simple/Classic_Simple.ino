#include <WiiChuck.h>

Classic classic;
 
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
