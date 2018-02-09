
#include <WiiChuck.h>
#include <Wire.h>

// This works with the Guitar Hero World Tour (Wii) Drums

Drums drumkit;

void setup() {


  Serial.begin(115200);
  Serial.println("Starting WiiChuck Generic Controller Demo");

  drumkit.begin();

  drumkit.addMap(new Drums::greenDrum(D5,200,10,10,1000)); // Servo max zero min cooldown
                                                           // cooldown: hold value for x ms.
                                                           
  drumkit.addMap(new Drums::redDrum(D6,200,10,10,500)); // Servo max zero min cooldown
                                                           // cooldown: hold value for x ms.                                                           
  drumkit.printMaps();  
}


void loop() {

  drumkit.readData();   // Read inputs and update maps
  drumkit.printInputs(); // Print all inputs
  
  delay(50);
}
