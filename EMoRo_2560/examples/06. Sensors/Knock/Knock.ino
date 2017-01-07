/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Knock Sensor                                                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch reads a piezo element to detect a knocking sound. It reads an analog pin and      */
/*            compares the result to a set threshold. If the result is greater than the threshold, it       */
/*            writes "knock" to the serial port, and toggles the LED on pin IO_0.                           */            
/*      The circuit:                                                                                        */
/*           * + connection of the piezo attached to analog in ADC_0 (pin 3)                                */
/*	         * - connection of the piezo attached to ground in ADC_0 (pin 1)                                */ 
/*	         * 1-megohm resistor attached from analog in ADC_0 (pin 3) to ground (pin 1)                    */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Piezoelectric sensor                                                            */
/*                          Pin header connector 3x1 RM 2.54mm                                              */
/*                          Wires                                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
   http://www.arduino.cc/en/Tutorial/Knock
*/

// these constants won't change:
const int ledPin = IO_0;                // led indicator on digital port IO_0
const int knockSensor = ADC_0;          // the piezo is connected to analog in ADC_0
const int threshold = 100;              // threshold value to decide when the detected sound is a knock or not


// these variables will change:
int sensorReading = 0;                  // variable to store the value read from the sensor pin
int ledState = LOW;                     // variable used to store the last LED status, to toggle the light

void setup() {
 pinMode(ledPin, OUTPUT);               // declare the ledPin as as OUTPUT
 digitalWrite(ledPin, LOW);             // turn off LED
 Serial.begin(9600);                    // use the serial port
}

void loop() {
  sensorReading = analogRead(knockSensor);      // read the sensor and store it in the variable sensorReading
  
  if (sensorReading >= threshold) {             // if the sensor reading is greater than the threshold
    ledState = !ledState;                       // toggle the status of the ledPin 
    digitalWrite(ledPin, ledState);             // update the LED pin itself:       
    Serial.println("Knock!");                   // send the string "Knock!" back to the computer, followed by newline
  }
  delay(100);                                   // delay to avoid overloading the serial port buffer
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

