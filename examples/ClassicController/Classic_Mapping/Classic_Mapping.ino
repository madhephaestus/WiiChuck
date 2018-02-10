#include <WiiChuck.h>

Classic classic;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting WiiChuck Classic Controller Demo");

  classic.begin();

  classic.addMap(new Classic::joyXLeft(5,200,128,10)); // Servo pin, max servo value, zero center value, min servo value 
  classic.addMap(new Classic::joyXLeft(6,200,128,10));
  classic.printMaps();
}

void loop() {
  classic.readData();    // Read inputs and update maps
  classic.printInputs(); // Print all inputs

  delay(50);
}
