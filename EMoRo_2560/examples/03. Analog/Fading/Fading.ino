/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Fading                                                                          */
/*      Board name: 		    EMoRo 2560 v3.2                                         						            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example shows how to fade an LED using the analogWrite() function.                       */
/*      The circuit:                                                                                        */
/*            LED attached from pin PWM_0 (PWM connector pin 1) to ground (PWM connector pin 11). In        */
/*            serial connection with LED connect 1k ohm resistor or appropriate resistor to used LED.       */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          LED                                                                             */
/*                          Resistor 1k ohm                                                                 */
/*                          Male pin header connector 3x2 RM 2.54mm                                         */
/*                          Female header connector 3x2 RM 2.54mm                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
 http://arduino.cc/en/Tutorial/Fading
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

int ledPin = PWM_0;                                             // LED connected to digital pin PWM_0

void setup()  { 
  // nothing happens in setup 
} 

void loop()  { 
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) {    // fade in from min to max in increments of 5
    analogWrite(ledPin, fadeValue);                             // sets the value (range from 0 to 255)  
    delay(30);                                                  // wait for 30 milliseconds to see the dimming effect
  } 

  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) {    // fade out from max to min in increments of 5
    analogWrite(ledPin, fadeValue);                             // sets the value (range from 0 to 255)     
    delay(30);                                                  // wait for 30 milliseconds to see the dimming effect                           
  } 
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/


