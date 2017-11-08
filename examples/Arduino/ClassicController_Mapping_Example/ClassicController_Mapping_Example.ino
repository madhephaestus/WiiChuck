
#include <WiiChuck.h>
#include <Wire.h>

// This works with the Guitar Hero World Tour (Wii) Drums

Classic classic;

void setup() {


  Serial.begin(115200);
  Serial.println("Starting WiiChuck Generic Controller Demo");

  classic.begin();

  classic.addMap(new Classic::joyXLeft(D5,200,128,10)); // Servo max zero min cooldown
                                                           // cooldown: hold value for x ms.
                                                           
  classic.addMap(new Classic::joyXLeft(D6,200,128,10)); // Servo max zero min cooldown
                                                           // cooldown: hold value for x ms.                                                           
  classic.printMaps();
}


void loop() {

	classic.readData();   // Read inputs and update maps
	classic.printInputs(); // Print all inputs
  
  delay(50);
}
