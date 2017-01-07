// Adafruit Trinket analog meter clock
// Date and time functions using a DS1307 RTC connected via I2C and the TinyWireM lib

// Download these libraries from Adafruit's Github repository and install in Arduino Libraries
#include <TinyWireM.h>
#include <TinyRTClib.h>

//For debug, uncomment serial code, use a FTDI Friend with its RX pin connected to Pin 3 
//   You will need a terminal program (such as freeware PuTTY for Windows) set to the
//   USB port of the FTDI friend at 9600 baud.  Uncomment out Serial commands to see what's up
//#include <SendOnlySoftwareSerial.h>  // See http://forum.arduino.cc/index.php?topic=112013.0
 
#define HOUR_PIN     1   // Hour display via PWM on Trinket GPIO #1
#define MINUTE_PIN   4   // Minute display via PWM on Trinket GPIO #4 (via Timer 1 calls)
 
//SendOnlySoftwareSerial Serial(3);  // Serial transmission on Trinket Pin 3
RTC_DS1307 rtc;                      // Set up real time clock

void setup () {
  pinMode(HOUR_PIN, OUTPUT);    // define PWM meter pins as outputs
  pinMode(MINUTE_PIN, OUTPUT);
  PWM4_init();                  // Set timer 1 to work PWM on Trinket Pin 4
  
  TinyWireM.begin();            // Begin I2C 
  rtc.begin();                  // Begin DS1307 real time clock
  //Serial.begin(9600);           // Begin Serial Monitor at 9600 baud
  if (! rtc.isrunning()) {      // Uncomment lines below first use of clock to set time
    //Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop () {
    uint8_t hourvalue, minutevalue;
    uint8_t hourvoltage, minutevoltage;
    
    DateTime now = rtc.now();           // Get the RTC info
    hourvalue = now.hour();             // Get the hour
    if(hourvalue > 12) hourvalue -= 12; // This clock is 12 hour, is 13-24, convert to 1-12
    minutevalue = now.minute();         // Get the minutes
// if you have calibration issues, you can change the last two values (zero higher, 255 lower)
// to have the needle move less if your scale is not pasted on 100% straight.
    hourvoltage = map(hourvalue, 0, 12, 0, 255);     // Convert hour to PWM duty cycle
    minutevoltage = map(minutevalue, 0, 60, 0, 255); // Convert minutes to PWM duty cycle
/*    
    // Uncomment out this and other serial code to check that your clock is working. 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" - ");
    Serial.print(hourvoltage, DEC);
    Serial.print(' ');
    Serial.print(minutevoltage, DEC);
    Serial.println(); 
*/  
    analogWrite(HOUR_PIN, hourvoltage);
    analogWrite4(minutevoltage);
    
    // code to put the processor to sleep might be preferable - we will delay
    delay(5000);  // check time every 5 seconds.  You can change this.
}

void PWM4_init() {
  // Set up PWM on Trinket GPIO #4 (PB4, pin 3) using Timer 1
  TCCR1 = _BV (CS10);           // no prescaler
  GTCCR = _BV (COM1B1) | _BV (PWM1B);  //  clear OC1B on compare
  OCR1B = 127;                  // duty cycle initialize to 50%
  OCR1C = 255;                  // frequency
}

// Function to allow analogWrite on Trinket GPIO #4 
void analogWrite4(uint8_t duty_value) {  
  OCR1B = duty_value;  // duty may be 0 to 255 (0 to 100%)
}
