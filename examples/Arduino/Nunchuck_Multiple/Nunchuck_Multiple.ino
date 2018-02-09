#include <WiiChuck.h>

Nunchuck nunchuck1;
Nunchuck nunchuck2;

void setup() {
  Serial.begin(115200);

  // Multiplexer located at address 0x70  
  nunchuck1.addMultiplexer(0x70, 0); // Nunchuk connected to multiplexer port 0
  nunchuck2.addMultiplexer(0x70, 1); // "" port 1

  nunchuck1.begin();
  nunchuck2.begin();
}

void loop() {
  Serial.println("-------------------------------------------");
  nunchuck1.readData();   // Read inputs and update maps
  nunchuck2.readData();

  nunchuck1.printInputs(); // Print all inputs
  nunchuck2.printInputs();
}
