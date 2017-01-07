/************************************************************************************************************/
/*      Inovatic-ICT d.o.o	                                                    								            */
/*												                                                                                  */
/*      EMoRo Example:		  Analog input, analog output, serial output                                      */
/*      Board name: 		    EMoRo 2560 v2.4						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Reads an analog input pin, maps the result to a range from 0 to 255 and uses the result to    */
/*            set the pulsewidth modulation (PWM) of an output pin. Also prints the results to the          */
/*            serial monitor.                                                                               */  
/*      Circuit:                                                                                            */
/*            Attach the center pin of a 10k potentiometer to pin 3 (the right most pin) of ADC_0 connector */
/*            and the outside pins of a potentiometer to pin 1 (GND, the left most pin - marked with a      */
/*            small triangle) and pin 2 (+5V, the center pin) of ADC_0 connector.                           */
/*            PWM Controlled LED is Status LED on EMoRo2560 controller.                                     */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Analog signal source (e.g. 10k potentiometer, cable,                            */
/*                                                pin header male 3x1 RM 2.54mm)                            */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

/************************************************************************************************************/
/*                         Available PWM outputs for analogWrite() function:                                */
/*----------------------------------------------------------------------------------------------------------*/
/*   PORT        USING TIMER      PIN       FREQ          NOTE                                              */
/*----------------------------------------------------------------------------------------------------------*/
/*   PWM_0           T0           PG5       980 Hz        USED FOR SYSTEM TIMER                             */
/*   PWM_1           T0           PB7       980 Hz        USED FOR SYSTEM TIMER                             */
/*   PWM_2           T3           PE2       490 Hz        NOT USED                                          */
/*   PWM_3           T3           PE4       490 Hz        NOT USED                                          */
/*   PWM_4           T1           PB5       490 Hz        USED FOR EMoRo SERVO                              */
/*   PWM_5           T1           PB6       490 Hz        USED FOR EMoRo SERVO                              */
/*   LED_BUILTIN     T2           PB4       490 Hz        USED FOR ARDUINO TONE LIB                         */
/*   SERVO_3         T5           PL3       490 Hz        USED FOR ARDUINO SERVO LIB                        */
/*   SERVO_4         T5           PL4       490 Hz        USED FOR ARDUINO SERVO LIB                        */
/*   SERVO_5         T5           PL5       490 Hz        USED FOR ARDUINO SERVO LIB                        */
/*   EX_IO_7         T3           PE5       490 Hz        NOT USED                                          */
/*   GPP_1_B         T2           PH6       490 Hz        USED FOR ARDUINO TONE LIB                         */
/*   GPP_2_B         T4           PH5       490 Hz        USED FOR EMoRo ULTRASONIC                         */
/*   GPP_3_B         T4           PH4       490 Hz        USED FOR EMoRo ULTRASONIC                         */
/*   GPP_4_B         T4           PH3       490 Hz        USED FOR EMoRo ULTRASONIC                         */
/************************************************************************************************************/

// These constants won't change.  They're used to give names to the pins used:
const int analogInPin = ADC_0;                    // analog input pin that the potentiometer is attached to
const int analogOutPin = LED_BUILTIN;             // analog output pin that the LED is attached to

int sensorValue = 0;                              // value read from the potentiometer
int outputValue = 0;                              // value output to the PWM (analog out)

void setup() {
  Serial.begin(9600);                             // initialize serial communications at 9600 bps
}

void loop() {
  sensorValue = analogRead(analogInPin);          // read the analog in value:       
  outputValue = map(sensorValue, 0, 1023, 0, 255);// map it to the range of the analog out  
  analogWrite(analogOutPin, outputValue);         // change the analog out value

  // print the results to the serial monitor:
  Serial.print("sensor = " );                     // print string "sensor ="      
  Serial.print(sensorValue);                      // print sensor value (0 - 1023)
  Serial.print("\t output = ");                   // append tabulator and string "output"
  Serial.println(outputValue);                    // print the calculated output value and append newline

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);                     
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

