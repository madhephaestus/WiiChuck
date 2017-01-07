/*************************************************** 
  This is an example for the HDC100x Humidity & Temp Sensor

  Designed specifically to work with the HDC1000 sensor from Adafruit
  ----> https://www.adafruit.com/products/2635

  These sensors use I2C to communicate, 2 pins are required to  
  interface
 ****************************************************/

#include <Wire.h>
#include "Adafruit_HDC1000.h"

// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)

Adafruit_HDC1000 hdc = Adafruit_HDC1000();

void setup() {
  Serial.begin(9600);
  Serial.println("HDC100x test");

  if (!hdc.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}


void loop() {
  Serial.print("Temp: "); Serial.print(hdc.readTemperature());
  Serial.print("\t\tHum: "); Serial.println(hdc.readHumidity());
  delay(500);
}
