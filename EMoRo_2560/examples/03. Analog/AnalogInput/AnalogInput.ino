/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Analog Input                                                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Demonstrates analog input by reading an analog sensor on analog pin ADC_0 and turning on and  */
/*            off a light emitting diode(LED) connected to digital pin IO_0. The amount of time the LED     */
/*            will be on and off depends on the value obtained by analogRead().                             */ 
/*      Circuit:                                                                                            */
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

int sensorPin = ADC_0;                    // select the input pin for the potentiometer
int ledPin = IO_0;                        // select the pin for the LED
int sensorValue = 0;                      // variable to store the value coming from the sensor

void setup() {
  pinMode(ledPin, OUTPUT);                // declare the ledPin as an OUTPUT
}

void loop() {
  sensorValue = analogRead(sensorPin);    // read the value from the sensor
  digitalWrite(ledPin, HIGH);             // turn the ledPin on
  delay(sensorValue);                     // stop the program for <sensorValue> milliseconds   
  digitalWrite(ledPin, LOW);              // turn the ledPin off
  delay(sensorValue);                     // stop the program for for <sensorValue> milliseconds
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

