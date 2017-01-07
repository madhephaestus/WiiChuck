/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Mega analogWrite() test                                                         */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch fades LEDs up and down one at a time on digital pins PWM_0 through PWM_5.         */
/*            This sketch was written for the EMoRo 2560, and will not work on previous boards.             */
/*      The circuit:                                                                                        */
/*            LEDs attached from pins PWM_0 (PWM pin 1) through PWM_5 (PWM pin 6) to ground (PWM pin 11).   */
/*            In serial connection with LED connect 1k ohm resistor or appropriate resistor for used LED.   */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          LED                                           6 pieces                          */
/*                          Resistor 1k ohm                               6 pieces                          */
/*                          Male pin header connector 3x2 RM 2.54mm       1 pieces                          */
/*                          Female header connector 3x2 RM 2.54mm         1 pieces                          */
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
const int lowestPin = PWM_0;
const int highestPin = PWM_5;

void setup() {
  for (int thisPin =lowestPin; thisPin <= highestPin; thisPin++) {    // set pins PWM_0 through PWM_5 as outputs
    pinMode(thisPin, OUTPUT); 
  }
}

void loop() {
  for (int thisPin =lowestPin; thisPin <= highestPin; thisPin++) {    // iterate over the pins:
    for (int brightness = 0; brightness < 255; brightness++) {        // fade the LED on thisPin from off to brightest
      analogWrite(thisPin, brightness);                               // fade LED
      delay(2);                                                       // delay 2 ms
    } 
    
    for (int brightness = 255; brightness >= 0; brightness--) {       // fade the LED on thisPin from brithtest to off
      analogWrite(thisPin, brightness);                               // fade LED
      delay(2);                                                       // delay 2 ms
    } 
    
    delay(100);                                                       // pause between LEDs
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

