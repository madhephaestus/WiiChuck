#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP183.h>

// For hardware SPI:
// Connect SCK to SPI Clock, SDO to SPI MISO, and SDI to SPI MOSI
// See  http://arduino.cc/en/Reference/SPI for your Arduino's SPI pins!
// On UNO, Clock is #13, SDO/MISO is #12 and SDI/MOSI is #11

// You can also use software SPI and define your own pins!
#define BMP183_CLK  13
#define BMP183_SDO  12  // AKA MISO
#define BMP183_SDI  11  // AKA MOSI

// You'll also need a chip-select pin, use any pin!
#define BMP183_CS   10

// initialize with hardware SPI
//Adafruit_BMP183 bmp = Adafruit_BMP183(BMP183_CS);
// or initialize with software SPI and use any 4 pins
Adafruit_BMP183 bmp = Adafruit_BMP183(BMP183_CLK, BMP183_SDO, BMP183_SDI, BMP183_CS);

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(9600);
  Serial.println("BMP183 Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP183 ... check your connections */
    Serial.print("Ooops, no BMP183 detected ... Check your wiring!");
    while(1);
  }
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{
    /* Display atmospheric pressue in Pascals */
    Serial.print("Pressure:    ");
    Serial.print(bmp.getPressure());
    Serial.print(" Pascals / ");
    Serial.print(bmp.getPressure() / 100);
    Serial.println(" millibar (hPa)");

    /* First we get the current temperature from the BMP085 */
    float temperature;
    temperature = bmp.getTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
    
    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted. If you don't have these values, a 'generic' value of    *
     * 1013.25 mbar can be used (defined as SENSORS_PRESSURE_SEALEVELHPA  *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     

    /* Then convert the atmospheric pressure, SLP and temp to altitude    */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA; // should be ~1000
    Serial.print("Sea level pressure: "); 
    Serial.print(SENSORS_PRESSURE_SEALEVELHPA);
    Serial.println(" millibar/hPa");
    
    Serial.print("Altitude:    "); 
    Serial.print(bmp.getAltitude(seaLevelPressure)); 
    Serial.println(" m");
    Serial.println("");

    delay(1000);
}