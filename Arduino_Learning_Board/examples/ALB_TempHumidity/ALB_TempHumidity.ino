/*
* Arduino Learning Board Project - Temperature/Humidity Sensor Example
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified July 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*/

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_DHT11 to include the DHT11 functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)

#define USE_ALB_DHT11

// NOW include the main ArduinoLearningBoard library
// Based on the defines above, the appropriate code will be added to the project
#include "ArduinoLearningBoard.h"

ALB_DHT11 dht11(2); // Initialize the ALB_DHT11 class with Pin 2 as the Digital Input pin

void setup()
{
	Serial.begin(9600); // Start Serial port so we can see the results
	
	dht11.begin(); // Get the DHT11 class to start reading
}

void loop()
{
	// Wait a few seconds between measurements.
	delay(2000);

	// Reading temperature or humidity takes about 250 milliseconds
	float h = dht11.readHumidity();
	// Read temperature as Celsius (the default)
	float c = dht11.readTemperatureC();
	// Read temperature as Fahrenheit (isFahrenheit = true)
	float f = dht11.readTemperatureF();

	// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(c) || isnan(f))
	{
		Serial.println("Failed to read from DHT sensor!");
		return;
	}

	// Compute heat index in Fahrenheit
	float hif = dht11.computeHeatIndexF(f, h);
	// Compute heat index in Celsius
	float hic = dht11.computeHeatIndexC(c, h);

	Serial.print("Humidity: ");
	Serial.print(h);
	Serial.print(" %\t");
	Serial.print("Temperature: ");
	Serial.print(c);
	Serial.print(" *C ");
	Serial.print(f);
	Serial.print(" *F\t");
	Serial.print("Heat index: ");
	Serial.print(hic);
	Serial.print(" *C ");
	Serial.print(hif);
	Serial.println(" *F");
}
