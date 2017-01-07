/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  SingleServo						                                                          */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch controls the servo motor attached to defined servo port end sends coresponding    */
/*            description to console port. EmoroServo.attach(<port>); initialize servo on desired port.     */
/*            After initialization with EmoroServo.attach(<port>); pulse width is automatically set to      */
/*            1500us. It is possible to use max. 8 servo motors on ports SERVO_0 to SERVO_7. After          */
/*            initialization, servo pulse is controlled by EmoroServo.write(<port>, <width>); Background    */
/*            engine will repeat that pulse width every 20ms automatically until it is not changed by new   */
/*            EmoroServo.write(<port>, <width>); or terminated with EmoroServo.detach(<port>);              */
/*            EmoroServo.read(<port>); will return current pulse width in us (micro seconds).               */
/*      The circuit:                                                                                        */
/*            Connect servo motor with JR cable to SERVO_0 (white triangle (pin 1) is ground (black wire))  */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Servo motor with JR cable                                                       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

int servoPort = SERVO_0;                      // select the pin for the servo motor
int waitState = 2000;                         // time in milliseconds between actions

void setup(void){			             
  Serial.begin(9600);                         // initialize serial communication:
  Serial.println("Example: SingleServo");     // send example name to console 
  EmoroServo.attach(servoPort);               // initialize servo motor on selected port

  Serial.print("Pulse width: ");              // print string
  Serial.print(EmoroServo.read(servoPort));   // print current servo pulse width
  Serial.println(" us\n");                    // print string

}

void loop(void){

  // Set servo pulse to 1000us. For position servo motors this will set the left position,
  // for servo motors with continuous rotation this will set continuous rotation in CCW direction
  EmoroServo.write(servoPort, 1000);

  // print action to console
  Serial.println("Set servo pulse to 1000us");
  Serial.println("Standard servo: set left position");
  Serial.println("Rotation servo: rotation in CCW direction\n");

  // wait for specified time in milliseconds
  delay(waitState);

  // Set servo pulse to 1500us. For position servo motors this will set the middle position,
  // for servo motors with continuous rotation this will stop rotation
  EmoroServo.write(servoPort, 1500);

  // print action to console
  Serial.println("Set servo pulse to 1500us");
  Serial.println("Standard servo: set middle position");
  Serial.println("Rotation servo: stop\n");

  // wait for specified time in milliseconds
  delay(waitState);

  // Set servo pulse to 2000us. For position servo motors this will set the right position,
  // for servo motors with continuous rotation this will set continuous rotation in CW direction
  EmoroServo.write(servoPort, 2000);

  // print action to console
  Serial.println("Set servo pulse to 2000us");
  Serial.println("Standard servo: set right position");
  Serial.println("Rotation servo: rotation in CW direction\n");

  // wait for specified time in milliseconds
  delay(waitState);

  // Set servo pulse to 1500us. For position servo motors this will set the middle position,
  // for servo motors with continuous rotation this will stop rotation
  EmoroServo.write(servoPort, 1500);

  // print action to console
  Serial.println("Set servo pulse to 1500us");
  Serial.println("Standard servo: set middle position");
  Serial.println("Rotation servo: stop\n");

  // wait for specified time in milliseconds
  delay(waitState);

}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

