/************************************************************************************************************/
/*      Inovatic-ICT d.o.o						                                                                      */
/*												                                                                                  */
/*      EMoRo Example:		  Switch statement                                                                */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Demonstrates the use of a switch statement.  The switch statement allows you to choose from   */
/*            among a set of discrete values of a variable.  It's like a series of if statements.           */
/*            To see this sketch in action, put the board and sensor in a well-lit room, open the serial    */
/*            monitor, and and move your hand gradually down over the sensor.                               */
/*      The circuit:                                                                                        */
/*           * photoresistor from analog in ADC_0 (pin 3) to +5V (pin 2)                                    */
/*           * 10K resistor from analog in ADC_0 (pin 3) to ground (pin 1)                                  */
/*           The photoresistor and 10k resistor are conected to form a voltage divider.                     */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          LDR (photoresistor)                                                             */
/*                          Resistor 10k ohm                                                                */
/*                          Pin header male 3x1 RM 2.54mm                                                   */
/*                          Wires                                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
http://www.arduino.cc/en/Tutorial/SwitchCase
*/

// these constants won't change - they are the lowest and highest readings you get from your sensor:
const int sensorMin = 0;                                          // sensor minimum, discovered through experiment
const int sensorMax = 1023;                                        // sensor maximum, discovered through experiment

void setup() {
  Serial.begin(9600);                                             // initialize serial communication
}

void loop() {
  int sensorReading = analogRead(ADC_0);                          // read the sensor and store the value to a local variable
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);     // map the sensor range to a range of four options - 0,1,2 or 3

  // do something different depending on the range value:
  switch (range) {                                                // begin switch case
  case 0:                                                         // your hand is on the sensor
    Serial.println("dark");                                       // send status via serial communication
    break;                                                        // break from switch statement
  case 1:                                                         // your hand is close to the sensor
    Serial.println("dim");                                        // send status via serial communication
    break;                                                        // break from switch statement
  case 2:                                                         // your hand is a few inches from the sensor
    Serial.println("medium");                                     // send status via serial communication
    break;                                                        // break from switch statement
  case 3:                                                         // your hand is nowhere near the sensor
    Serial.println("bright");                                     // send status via serial communication
    break;                                                        // break from switch statement
  }                               
  delay(10);                                                      // delay in between reads for stability
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

