/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Fade                                                                            */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This example shows how to fade STATUS LED  using the analogWrite() function.                  */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
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

int led = LED_BUILTIN;                        // initialize variable "led" - fade built in STATUS_LED
int brightness = 0;                           // initialize variable "brightness" - stores how bright the LED is
int fadeAmount = 5;                           // initialize variable "fadeAmount" - fadeing amount steps

// the setup routine runs once when you press reset:
void setup()  { 
  pinMode(led, OUTPUT);                       // declare pin LED_BUILTIN to be an output
} 

// the loop routine runs over and over again forever:
void loop()  { 
  analogWrite(led, brightness);               // set the brightness of pin LED_BUILTIN
  brightness = brightness + fadeAmount;       // change the brightness for next time through the loop

  if (brightness <= 0 || brightness >= 255) { // if minimum or maximum brightness is reached
    fadeAmount = -fadeAmount ;                // reverse the direction of the fading
  }     
    
  delay(30);                                  // wait for 30 milliseconds (to see the dimming effect)                          
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
