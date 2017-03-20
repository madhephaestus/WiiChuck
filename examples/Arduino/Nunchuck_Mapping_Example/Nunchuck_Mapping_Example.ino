
#include <WiiChuck.h>
#include <Wire.h>

#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif
// This works with the Guitar Hero World Tour (Wii) Drums

Nunchuck nunchuck(SDA, SCL);

void setup() {

  Serial.begin(115200);
  Serial.println("Starting WiiChuck Generic Controller Demo");

  nunchuck.begin();

  nunchuck.addMap(new Nunchuck::joyX(D5,200,128,10)); // Servo max zero min cooldown
                                                           // cooldown: hold value for x ms.
                                                           
  nunchuck.addMap(new Nunchuck::joyY(D6,200,128,10)); // Servo max zero min cooldown
                                                           // cooldown: hold value for x ms.                                                           
  nunchuck.printMaps(Serial);  
}


void loop() {
	nunchuck.readData();   // Read inputs and update maps
	nunchuck.printInputs(Serial); // Print all inputs
  
	delay(50);
}
