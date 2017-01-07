/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Calibration                                                                     */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Demonstrates one technique for calibrating sensor input. The sensor readings during the       */
/*            first ten seconds of the sketch execution define the minimum and maximum of expected values   */
/*            attached to the sensor pin.                                                                   */
/*            The sensor minimum and maximum initial values may seem backwards. Initially, you set the      */
/*            minimum high and listen for anything lower, saving it as the new minimum. Likewise, you set   */ 
/*            the maximum low and listen for anything higher as the new maximum.                            */
/*      The circuit:                                                                                        */
/*            Attach the center pin of a 10k potentiometer to pin 3 (the right most pin) of ADC_0 connector */
/*            and the outside pins of a potentiometer to pin 1 (GND, the left most pin - marked with a      */
/*            small triangle) and pin 2 (+5V, the center pin) of ADC_0 connector.                           */
/*            LED attached from pin PWM_0 (PWM pin 1) to ground (PWM pin 11). In serial connection with     */
/*            LED connect 1k ohm resistor or appropriate resistor to used LED.                              */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          LED                                                                             */
/*                          Resistor 1k ohm                                                                 */
/*                          Male pin header connector 3x2 RM 2.54mm                                         */
/*                          Female header connector 3x2 RM 2.54mm                                           */
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

// These constants won't change:
const int sensorPin = ADC_0;                // pin that the sensor is attached to
const int ledPin = PWM_0;                   // pin that the LED is attached to

// variables:
int sensorValue = 0;                        // the sensor value
int sensorMin = 1023;                       // minimum sensor value
int sensorMax = 0;                          // maximum sensor value


void setup() {
  pinMode(IO_0, OUTPUT);                    // set IO_0 as OUTPUT
  digitalWrite(IO_0, HIGH);                 // turn on LED to signal the start of the calibration period
 
  while (millis() < 10000) {                // calibrate during the first ten seconds
    sensorValue = analogRead(sensorPin);    // read sensorPin and store sensorValue

    // record the maximum sensor value:
    if (sensorValue > sensorMax) {          // if the current reading is greater than the maximum
      sensorMax = sensorValue;              // set the currrent reading to be the new maximum
    }

    // record the minimum sensor value
    if (sensorValue < sensorMin) {          // if the current reading is lower than the minimum
      sensorMin = sensorValue;              // set the currrent reading to be the new minimum
    }
  }

  digitalWrite(IO_0, LOW);                  // signal the end of the calibration period - turn LED off
}

void loop() {
  sensorValue = analogRead(sensorPin);                            // read the sensor
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);   // apply the calibration to the sensor reading - map the min-max range to 0-255
  sensorValue = constrain(sensorValue, 0, 255);                   // in case the sensor value is outside the range seen during calibration constrain value to 0-255
  analogWrite(ledPin, sensorValue);                               // fade the LED using the calibrated value
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

