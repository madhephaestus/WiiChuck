#include <WiiChuck.h>
#include <Wire.h>

// This works with the Guitar Hero World Tour (Wii) Drums

Nunchuck nunchuck;

void setup() {

  Serial.begin(115200);
  Serial.println("Starting WiiChuck Generic Controller Demo");

  nunchuck.begin();

  nunchuck.addMap(new Nunchuck::joyX(5,200,128,10)); // Servo pin, max servo value, zero center value, min servo value 
  nunchuck.addMap(new Nunchuck::joyY(6,200,128,10)); // Servo pin, max servo value, zero center value, min servo value                                                          
  nunchuck.addMap(new Nunchuck::buttonZ(7,200,128,10)); // Servo pin, max servo value, zero center value, min servo value 
  nunchuck.addMap(new Nunchuck::buttonC(8,200,128,10)); // Servo pin, max servo value, zero center value, min servo value 

  nunchuck.printMaps(Serial);  
}


void loop() {
  nunchuck.readData();   // Read inputs and update maps
  nunchuck.printInputs(); // Print all inputs
  int value = nunchuck.getJoyX();
  Serial.print(" Raw value x: ");
  Serial.print(value);
  Serial.print(" y: ");
  Serial.print(nunchuck.getJoyY());
  Serial.print("\r\n ");
  nunchuck.printMaps();  
  delay(50);
}
