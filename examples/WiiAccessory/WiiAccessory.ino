#include <WiiChuck.h>

Accessory nunchuck1;

void setup() {
	Serial.begin(115200);
	nunchuck1.begin();
	if (nunchuck1.type == Unknown) {
		/** If the device isn't auto-detected, set the type explicatly
		 * 	NUNCHUCK,
		 WIICLASSIC,
		 GuitarHeroController,
		 GuitarHeroWorldTourDrums,
		 DrumController,
		 DrawsomeTablet,
		 Turntable
		 */
		nunchuck1.type = NUNCHUCK;
	}
}

void loop() {
	Serial.println("-------------------------------------------");
	nunchuck1.readData();    // Read inputs and update maps
	nunchuck1.printInputs(); // Print all inputs
	for (int i = 0; i < WII_VALUES_ARRAY_SIZE; i++) {
		Serial.println(
				"Controller Val " + String(i) + " = "
						+ String((uint8_t) nunchuck1.values[i]));
	}

}
