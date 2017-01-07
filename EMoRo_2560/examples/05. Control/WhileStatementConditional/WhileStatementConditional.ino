/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Conditionals - while statement                                                  */
/*      Board name: 		    EMoRo 2560 v3.2	                                                                */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example demonstrates the use of  while() statements. While the pushbutton is pressed,    */
/*            the sketch runs the calibration routine. The  sensor readings during the while loop define    */
/*            the minimum and maximum of expected values from the photo resistor.                           */
/*      The circuit:                                                                                        */
/*           * photo resistor connected from +5V (pin 2) to analog in pin ADC_0 (pin 3)                     */
/*           * 10K resistor connected from ground (pin 1) to analog in pin ADC_0 (pin 3)                    */
/*           * pushbutton attached from pin IO_8 (pin 3) to GND (pin 1)                                     */
/*           * LED attached from pin PWM_0 (PWM connector pin 1) to GND (PWM connector pin 11)              */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          LED                                                                             */
/*                          Resistor 1k ohm                                                                 */
/*                          Male pin header connector 3x2 RM 2.54mm                                         */
/*                          Female header connector 3x2 RM 2.54mm                                           */
/*                          Push button switch (e.g. Push button, cable, pin header male 3x1 RM 2.54mm      */
/*                                              connect switch to pin 1 and pin 3 of IO_8 connector )       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/WhileLoop
 */

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
const int sensorPin = ADC_0;         // pin that the sensor is attached to
const int ledPin = PWM_0;            // pin that the LED is attached to
const int indicatorLedPin = IO_0;    // pin that the built-in LED is attached to
const int buttonPin = IO_8;          // pin that the button is attached to

// These variables will change:
int sensorMin = 1023;                // minimum sensor value
int sensorMax = 0;                   // maximum sensor value
int sensorValue = 0;                 // the sensor value

// Calibration running flag:
int calibrationFlag =0;
void setup() {
  // set the LED pins as outputs and the switch pin as input with enabled pullups:
  pinMode(indicatorLedPin, OUTPUT);
  pinMode (ledPin, OUTPUT);
  pinMode (buttonPin, INPUT_PULLUP);
  digitalWrite(indicatorLedPin, HIGH);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  while (digitalRead(buttonPin) == LOW) {                           // while the button is pressed, take calibration readings:
    calibrate();                                                    // call calibration routine
  }
  calibrationFlag=0;                                                //calibration finished
  
  digitalWrite(indicatorLedPin, LOW);                               // signal the end of the calibration period
  sensorValue = analogRead(sensorPin);                              // read the sensor
  sensorValue = map(sensorValue, sensorMin, sensorMax, 0, 255);     // apply the calibration to the sensor reading
  sensorValue = constrain(sensorValue, 0, 255);                     // constrain in case the sensor value is outside the range seen during calibration

  analogWrite(ledPin, sensorValue);                                 // fade the LED using the calibrated value
}

void calibrate() {                                // calibration routine
  if(calibrationFlag==0){                         // if the calibration routine is run for the first time the flag will be 0
    calibrationFlag=1;                            // set calibration flag
    sensorMin = 1023;                             // reset minimum sensor value
    sensorMax = 0;                                // reset maximum sensor value
    sensorValue = 0;                              // reset the sensor value
  }
  
  digitalWrite(indicatorLedPin, HIGH);            // turn on the indicator LED to indicate that calibration is happening
  sensorValue = analogRead(sensorPin);            // read the sensor

  // record the maximum sensor value
  if (sensorValue > sensorMax) {                  // if current reading is greater than stored maximum value
    sensorMax = sensorValue;                      // set the current reading as maximum
  }

  // record the minimum sensor value
  if (sensorValue < sensorMin) {                  // if current reading is smaller than stored minimum value
    sensorMin = sensorValue;                      // set the current reading as minimum
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/


