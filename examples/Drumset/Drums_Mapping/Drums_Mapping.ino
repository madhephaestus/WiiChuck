#include <WiiChuck.h>

// This works with the Guitar Hero World Tour (Wii) Drums

Drums drumkit;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting WiiChuck Drumset Controller Demo");

  drumkit.begin();

  drumkit.addMap(new Drums::greenDrum(5,200,10,10,1000)); // Servo pin, max value, zero value, min value, cooldown
  drumkit.addMap(new Drums::redDrum(6,200,10,10,500));    //  Cooldown: hold value for x milliseconds
  drumkit.printMaps();  
}

void loop() {
  drumkit.readData();    // Read inputs and update maps
  drumkit.printInputs(); // Print all inputs

  delay(50);
}
