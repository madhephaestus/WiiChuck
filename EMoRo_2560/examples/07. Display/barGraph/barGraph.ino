/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  LED bar graph                                                                   */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Turns on a series of LEDs based on the value of an analog sensor. This is a simple way to     */
/*            make a bar graph display. Though this graph uses 16 LEDs, you can use any number by changing  */
/*            the LED count and the pins in the array.                                                      */
/*            This method can be used to control any series of digital outputs that depends on an           */
/*            analog input.                                                                                 */
/*      Circuit:                                                                                            */
/*            Potentiometer attached to analog input ADC_0.                                                 */
/*            Attach the center pin of a 10k potentiometer to pin 3 (the right most pin) of ADC_0 connector */
/*            and the outside pins of a potentiometer to pin 1 (GND, the left most pin - marked with a      */
/*            small triangle) and pin 2 (+5V, the center pin) of ADC_0 connector.                           */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Analog signal source (e.g. 10k potentiometer, cable,                            */
/*                                                pin header male 3x1 RM 2.54mm)                            */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
  http://www.arduino.cc/en/Tutorial/BarGraph
*/


// these constants won't change:
const int analogPin = ADC_0;                                // the pin that the potentiometer is attached to
const int ledCount = 16;                                    // the number of LEDs in the bar graph

// an array of pin numbers to which LEDs are attached
int ledPins[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};


void setup() {
  // loop over the pin array and set them all to output:
  for (int thisLed = 0; thisLed < ledCount; thisLed++) {    // iterate through LED pins
    pinMode(ledPins[thisLed], OUTPUT);                      // set each pin as OUTPUT
    digitalWrite(ledPins[thisLed], LOW);                    // turn off all LEDs
  }
}

void loop() {
  int sensorReading = analogRead(analogPin);                // read the potentiometer
  int ledLevel = map(sensorReading, 0, 1023, 0, ledCount);  // map the result to a range from 0 to the number of LEDs

  for (int thisLed = 0; thisLed < ledCount; thisLed++) {    // loop over the LED array
    if (thisLed < ledLevel) {                               // if the array element's index is less than ledLevel
      digitalWrite(ledPins[thisLed], HIGH);                 // turn the pin for this element on
    }
    else {                                                  // else
      digitalWrite(ledPins[thisLed], LOW);                  // turn off all pins higher than the ledLevel
    }
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

