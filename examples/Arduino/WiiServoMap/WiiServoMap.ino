#include <WiiChuck.h>

#if defined(ARDUINO_ARCH_ESP8266)
#define SDA D2
#define SCL D1
#endif
WiiChuck myChuck(SDA, SCL);

void setup() {

	Serial.begin(115200);
	Serial.println("Starting WiiChuck Demo");
	//myChuck.type=OFFICIALWII;
	myChuck.type = THIRDPARTYWII;
	myChuck. addControlMap(3, 17,50,
				160,-100,0,100,
				JOY_X);
	myChuck.addButtonMap(4, 17,
					160,
					ZBUTTON);
	//myChuck.type = WIICLASSIC;
	myChuck.begin();
	
	
}
void loop() {

	myChuck.readData();
	delay(20);
}