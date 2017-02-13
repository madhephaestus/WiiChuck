#include <WiiChuck.h>
#include <Wire.h>
#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif


Accessory generic(SDA, SCL);

void setup() {

	Serial.begin(115200);
	Serial.println("Starting WiiChuck Generic Controller Demo");

  generic.begin();

  ControllerType c = generic.identifyController();

  switch(c){
    case NUNCHUCK:
      Serial.println("\tFound Nunchuck."); break;
    case WIICLASSIC:
      Serial.println("\tFound Wii Clasic."); break;
    case GuitarHeroController:
      Serial.println("\tFound Guitar Hero Guitar."); break;
    case GuitarHeroWorldTourDrums:
      Serial.println("\tFound Guitar Hero Drums."); break;
    case DrumController:
      Serial.println("\tFound Taiko Drums."); break;
    case DrawsomeTablet:
      Serial.println("\tFound Drawsome Tablet."); break;
    case Turntable:
      Serial.println("\tFound DJ Hero Turntable."); break;
    case Unknown:
      Serial.println("\tUnknown Controller "); break;
  }

  
}

void loop() {

	generic.readData();
  generic.printInputs(Serial);
  
	delay(50);
}
