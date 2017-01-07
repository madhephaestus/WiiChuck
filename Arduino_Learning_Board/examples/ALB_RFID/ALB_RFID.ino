/*
* Arduino Learning Board Project - RFID Sensor
*
* ALB_RFID - This sketch demonstrates how to use the RFID kit to detect
* either a RFID card or keychain's unique code when placed near the detector
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

/*
 Read a card using a mfrc522 reader on your SPI interface
 Pin layout should be as follows
 
 1. VCC  -> 3.3V
 2. RST  -> pin 9
 3. GND  -> GND
 4. IRQ  -> Skip (No Connection)
 5. MISO -> pin 12
 6. MOSI -> pin 11
 7. SCK  -> pin 13
 8. SDA  -> pin 10
*/

/*
*/

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_RFID to include the RFID functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)
#define USE_ALB_RFID

// NOW include the main ArduinoLearningBoard library (quotes now, <Arduino...> when it's "live")
// Based on the defines above, the appropriate code will be added to the project
#include "ArduinoLearningBoard.h"
  
#define SS_PIN 10 // Chip Select Pin
#define RST_PIN 9 // Reset Pin

RFID rfid(SS_PIN, RST_PIN);
int serNum[5];

void setup()
{
	Serial.begin(9600);
	SPI.begin();
	rfid.init();
}

void loop()
{
	if(rfid.isCard())
	{
		if(rfid.readCardSerial())
		{
			Serial.print(rfid.serNum[0], HEX);
			Serial.print(" ");
			Serial.print(rfid.serNum[1], HEX);
			Serial.print(" ");
			Serial.print(rfid.serNum[2], HEX);
			Serial.print(" ");
			Serial.print(rfid.serNum[3], HEX);
			Serial.print(" ");
			Serial.print(rfid.serNum[4], HEX);
			Serial.println("");
			delay(1000);
		}
	}
	rfid.halt();
}