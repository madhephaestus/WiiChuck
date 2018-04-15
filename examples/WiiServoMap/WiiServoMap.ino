#include <WiiChuck.h>
#include <Servo.h>

Accessory nunchuck1;
Servo one;
Servo two;
void setup() {
	delay(1000);
	Serial.begin(115200);
	Serial.println("Loading controller...");
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
	one.attach(33,1000,2000);
	two.attach(35,1000,2000);

}

void loop() {
	//Serial.println("-------------------------------------------");
	nunchuck1.readData();    // Read inputs and update maps
	int Sewrvo1Val = map(nunchuck1.values[0],0,255,0,180);
	int Sewrvo2Val = map(nunchuck1.values[1],0,255,0,180);
	one.write(Sewrvo1Val);
	two.write(Sewrvo2Val);
	Serial.println("Set "+String(Sewrvo1Val)+" "+String(Sewrvo2Val));

}
