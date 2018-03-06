#include <WiiChuck.h>

Classic classic;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(23, INPUT);           // set pin to input
  //classic.enableEncryption(true);
  classic.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  classic.readData(); 
  classic. printInputs();
  //int joyLeftY = classic.getJoyYLeft();
  //Serial.println(joyLeftY, DEC);
}
