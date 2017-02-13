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

  // map wiichuck joyX to servo 3.
  generic.addAnalogMap(
     UNUSED,0,0,        // MSB (Unused)
     UNUSED,0,0,        // CSB (Unused)
     BYTE0,BIT0,BIT7,   // LSB (7:0 bits of data in byte 0 of controller data)
     0,127,255,          // min, middle, max
     0,255,127,         // servo min, zero, max PWM values
     D5);                // Channel

  // map wiichuck joyY to servo 4
  generic.addAnalogMap(
     UNUSED,0,0,        // MSB (Unused)
     UNUSED,0,0,        // CSB (Unused)
     BYTE1,BIT0,BIT7,   // LSB (7:0 bits of data in byte 1 of controller data)
     0,127,255,          // min, middle, max
     0,255,127,         // servo min, max, zero PWM values
     D6);                // Channel

  generic.addDigitalMap(
     BYTE5,BIT1,true,   // Byte and BIT of button. TRUE for asserted=0
     0,255,127,         // servo min, max, zero PWM values
     D7);

  
     
  generic.printMaps(Serial);

}

void loop() {

	generic.readData();
  //generic.printInputs(Serial);
  
  
	delay(50);
}
