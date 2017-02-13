#include <WiiChuck.h>
#include <Wire.h>
#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif


Nunchuck generic(SDA, SCL);
void setup() {

	Serial.begin(115200);
	Serial.println("Starting WiiChuck Generic Controller Demo");

  generic.begin();

  ControllerType c = generic.identifyController();

  switch(c){
    case NUNCHUCK:
      Serial.println("\tCorrect Nunchuck."); break;
    case WIICLASSIC:
      Serial.println("\tWRONG Found Wii Clasic."); break;
    case GuitarHeroController:
      Serial.println("\tWRONG FFound Guitar Hero Guitar."); break;
    case GuitarHeroWorldTourDrums:
      Serial.println("\tWRONG FFound Guitar Hero Drums."); break;
    case DrumController:
      Serial.println("\tWRONG FFound Taiko Drums."); break;
    case DrawsomeTablet:
      Serial.println("\tWRONG FFound Drawsome Tablet."); break;
    case Turntable:
      Serial.println("\tWRONG FFound DJ Hero Turntable."); break;
    case Unknown:
      Serial.println("\tWRONG FUnknown Controller "); break;
  }

  
}

void loop() {

	generic.readData();
	generic.printInputs(Serial);
  
	delay(50);
}