/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  AnalogReadSerial  	      				                                              */
/*      Board name: 		    EMoRo 2560 v3.2	    					                                                  */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Reads an analog input on port ADC_0 and prints the result to the serial monitor.              */
/*            Attach the center pin of a 10k potentiometer to pin 3 (the right most pin) of ADC_0 connector */
/*            and the outside pins of a potentiometer to pin 1 (GND, the left most pin - marked with a      */
/*            small triangle) and pin 2 (+5V, the center pin) of ADC_0 connector.                           */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Analog signal source (e.g. Potentiometer, cable, pin header male 3x1 RM 2.54mm  */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(ADC_0);  // read the input on analog pin ADC_0
  Serial.println(sensorValue);          // print out the value you read
  delay(100);                           // delay between reads for stability
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
