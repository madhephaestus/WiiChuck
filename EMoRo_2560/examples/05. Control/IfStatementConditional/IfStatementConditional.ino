/************************************************************************************************************/
/*      Inovatic-ICT d.o.o			                                                    						            */
/*												                                                                                  */
/*      EMoRo Example:		  Conditionals - If statement                                                     */
/*      Board name: 		    EMoRo 2560 v3.2	                                          					            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example demonstrates the use of if() statements. It reads the state of a potentiometer   */
/*            (an analog input) and turns on an LED only if the LED goes above a certain threshold level.   */
/*            It prints the analog value regardless of the level.                                           */
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
http://arduino.cc/en/Tutorial/IfStatement
*/
 
// These constants won't change:
const int analogPin = ADC_0;                    // pin that the sensor is attached to
const int ledPin = IO_0;                        // pin that the LED is attached to
const int threshold = 400;                      // an arbitrary threshold level that's in the range of the analog input

void setup() {
  pinMode(ledPin, OUTPUT);                      // initialize the LED pin as an output
  Serial.begin(9600);                           // initialize serial communications
}

void loop() {
  int analogValue = analogRead(analogPin);       // read the value of the potentiometer and store to a local variable
  
  if (analogValue > threshold) {                 // if the analog value is higher than treshold
    digitalWrite(ledPin, HIGH);                  // turn on the LED
  } else {                                       // else
    digitalWrite(ledPin, LOW);                   // turn off the LED
  }

  Serial.println(analogValue);                   // print the analog value
  delay(1);                                      // delay in between reads for stability
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

