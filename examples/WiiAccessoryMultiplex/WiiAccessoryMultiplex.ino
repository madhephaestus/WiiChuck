#include <WiiChuck.h>

Accessory nunchuck1;
Accessory nunchuck2;

void setup() {
	Serial.begin(115200);
	// Multiplexer located at address 0x70
	nunchuck1.addMultiplexer(0x70, 0); // Nunchuk connected to multiplexer port 0
	nunchuck2.addMultiplexer(0x70, 1); // "" port 1
	nunchuck1.begin();
	nunchuck2.begin();
	/** If the device isn't auto-detected, set the type explicatly
	 * 	NUNCHUCK,
	 WIICLASSIC,
	 GuitarHeroController,
	 GuitarHeroWorldTourDrums,
	 DrumController,
	 DrawsomeTablet,
	 Turntable
	 */
	if (nunchuck1.type == Unknown) {
		nunchuck1.type = NUNCHUCK;
	}
	if (nunchuck2.type == Unknown) {
		nunchuck2.type = NUNCHUCK;
	}
}

void loop() {
	Serial.println("-------------------------------------------");
	nunchuck1.readData();    // Read inputs and update maps
	nunchuck2.readData();    // Read inputs and update maps

	nunchuck1.printInputs(); // Print all inputs
	for (int i = 0; i < WII_VALUES_ARRAY_SIZE; i++) {
		Serial.println(
				"Controller Val " + String(i) + " = "
						+ String((uint8_t) nunchuck1.values[i]));
	}
	nunchuck2.printInputs(); // Print all inputs
	for (int i = 0; i < WII_VALUES_ARRAY_SIZE; i++) {
		Serial.println(
				"Controller Val " + String(i) + " = "
						+ String((uint8_t) nunchuck2.values[i]));
	}

}
