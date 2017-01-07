/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Reading a serial ASCII-encoded string.                                          */
/*      Board name: 		    EMoRo 2560 v3.2	                                          					            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch demonstrates the Serial parseInt() function. It looks for an ASCII string of      */
/*            comma-separated values. It parses them into ints, and uses those to fade an RGB LED.          */
/*      Circuit: Common-anode RGB LED wired like so:                                                        */
/*             * Red cathode: digital pin PWM_0                                                             */
/*             * Green cathode: digital pin PWM_1                                                           */
/*             * Blue cathode: digital pin PWM_2                                                            */
/*             * anode: +5V                                                                                 */
/*             * if it is neccesary, use appropriate serial resistor with each diode                        */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          RGB diode                                                                       */
/*                          Pin header connector 2x3 RM 2.54mm                                              */  
/*                          Female header connector 2x3 RM 2.54mm                                           */
/*                          Appropriate serial resistors               3 pieces                             */
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

// define pins for the LEDs:
const int redPin = PWM_0;
const int greenPin = PWM_1;
const int bluePin = PWM_2;

void setup() {
  Serial.begin(9600);                   // initialize serial
  // make the pins outputs:
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT); 
}

void loop() {
  while (Serial.available() > 0) {      // if there's any serial available, read it

    
    int red = Serial.parseInt();        // look for the next valid integer in the incoming serial stream
    int green = Serial.parseInt();      // look for the next valid integer in the incoming serial stream
    int blue = Serial.parseInt();       // look for the next valid integer in the incoming serial stream

    if (Serial.read() == '\n') {        // look for the newline. That's the end of your sentence
      
      // constrain the values to 0 - 255 and invert
      red = 255 - constrain(red, 0, 255);
      green = 255 - constrain(green, 0, 255);
      blue = 255 - constrain(blue, 0, 255);

      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);", or just uncomment this block and comment the block above
      //red = constrain(red, 0, 255);
      //green = constrain(green, 0, 255);
      //blue = constrain(blue, 0, 255);

      // fade the red, green, and blue legs of the LED: 
      analogWrite(redPin, red);
      analogWrite(greenPin, green);
      analogWrite(bluePin, blue);

      // print the three numbers in one string as hexadecimal:
      Serial.print(red, HEX);
      Serial.print(green, HEX);
      Serial.println(blue, HEX);
    }
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/








