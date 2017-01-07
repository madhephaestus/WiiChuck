/*
 Demonstration sketch for Adafruit LCD backpack
 using MCP23008 I2C expander and DHT Temperature/Humidity Sensor
 Uses the 5 volt Trinket mini microcontroller with the
 Trinket set at 16 MHz due to timing reading the DHT sensor
 
 This sketch prints the temperature and humidity to the LCD
 
 The circuit:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * Display i2c backpack CLK to Trinket GPIO #2 
 * Display i2c backpack DAT to Trinket GPIO #0 
 * DHT Temperature Sensor to Trinket GPIO #1 (with 1K ohm pullup to 5V)

// Connect DHT pin 1 (on the left) of the sensor to +5V
// Connect DHT pin 2 of the sensor to whatever your DHTPIN is
// Connect DHT pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
*/

// include the library code
#include <TinyWireM.h>      // Wire/I2C library for Trinket and Gemma
#include <TinyLiquidCrystal.h>
#include <TinyDHT.h>        // lightweit DHT sensor library
#include <avr/power.h>      // needed to up clock to 16 MHz on 5v Trinket

// Uncomment whatever type sensor you are using!
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22     // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define TEMPTYPE 1        // Use Fahrenheit (0 for celsius)

// Trinket GPIO #1 would be better but LED prevents digital talk with DHT sensor
#define DHTPIN 1          // Sensor connected to GPIO #3

DHT dht(DHTPIN, DHTTYPE);  // Define Temp Sensor

// Connect display via  i2c, default address #0 (A0-A2 not jumpered)
TinyLiquidCrystal lcd(0);

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1); // 5V Trinket: run at 16 MHz
  dht.begin();  // Initialize DHT Teperature Sensor
  
  lcd.begin(16, 2);  // set up the LCD's number of rows and columns: 
  lcd.setBacklight(HIGH); // Set backlight
}

void loop() {
  int8_t h = dht.readHumidity();               // Read humidity
  int16_t t = dht.readTemperature(TEMPTYPE);   // read temperature

  lcd.setCursor(0, 0); 
  if ( t == BAD_TEMP || h == BAD_HUM ) { // if error conditions (see TinyDHT.h)
     lcd.print("Bad read on DHT");       //   print error message
  } else {
     lcd.print("Humidity: ");            // write to LCD
     lcd.setCursor(10,0); 
     lcd.print(h);
     lcd.setCursor(12, 0); 
     lcd.print(" % ");
     lcd.setCursor(0, 1); 
     lcd.print("Temp:"); 
     lcd.setCursor(7, 1); 
     lcd.print(t);
     lcd.setCursor(10, 1); 
     lcd.print("*F");
  }
  delay(2000);  // Read temp every second (2000 ms) (DHT sensor max rate)
}
