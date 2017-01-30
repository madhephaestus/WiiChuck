#include <WiiChuck.h>

#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif
WiiChuck myChuck(SDA, SCL);

void setup() {

	Serial.begin(115200);
	Serial.println("\n\nStarting WiiChuck Demo");
	//myChuck.type=OFFICIALWII;
	myChuck.type = THIRDPARTYWII;
	//myChuck.type = WIICLASSIC;
	
	
	myChuck. addControlMap(D3, 17,50,
					160,-63,0,100,
					JOY_X);
	myChuck. addControlMap(D4, 160,50,
						17,-50,0,100,
						JOY_Y);
	myChuck.addButtonMap(D6, 17,
					160,
					ZBUTTON);
	myChuck.begin();
	
	
}
void loop() {

	myChuck.readData();
	delay(40);
}