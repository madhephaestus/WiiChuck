/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      ThreeServos                                                                     */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This sketch controls three servo motors attached to defined servo ports end sends             */
/*            corresponding description to console port. EmoroServo.attach(<port>); initialize servo on     */
/*            desired port. After initialization with EmoroServo.attach(<port>); pulse width is             */
/*            automatically set to 1500us. It is possible to use max. 8 servo motors on ports SERVO_0 to    */
/*            SERVO_7. After initialization, servo pulse is controlled by EmoroServo.write(<port>, <width>);*/
/*            Background engine will repeat that pulse width every 20ms automatically until it is not       */
/*            changed by new EmoroServo.write(<port>, <width>); or terminated with                          */
/*            EmoroServo.detach(<port>); EmoroServo.read(<port>); will return the current pulse width in us */
/*            (micro seconds).                                                                              */
/*                                                                                                          */
/*      The circuit:                                                                                        */
/*            Connect servo motor with JR cable to SERVO_0 (white triangle (pin 1) is ground (black wire))  */
/*            Connect servo motor with JR cable to SERVO_1 (white triangle (pin 1) is ground (black wire))  */
/*            Connect servo motor with JR cable to SERVO_3 (white triangle (pin 1) is ground (black wire))  */
/*      Note:                                                                                               */
/*            EMoRo 2560 controller has two independent power supplies. One 5V 2A for controller and        */
/*            everything else except servo ports. Servo ports (SERVO_0 to SERVO_7) have its own power       */
/*            supply which is capable to deliver 5V max 3A for all 8 servo motors. Each power supply has    */
/*            short circuit and over current protection.                                                    */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Servo motor with JR cable   3 pieces                                            */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

int servoPort[3] = {SERVO_0, SERVO_1, SERVO_3};   // select ports for servo motors
int waitState = 2000;                             // time in milliseconds between actions
int repeatCounter;                                // repeat counter, after 10 execution two servo motors will be detached

void setup(void){ 
  Serial.begin(9600);                             // initialize serial communication:
  Serial.println("Example: SingleServo");         // send example name to console

  for(int n=0; n<3; n++){ 
    EmoroServo.attach(servoPort[n]);              // initialize servo motor on selected port

    //print current servo pulse width
    Serial.print("Pulse width (SERVO_");          // print string
    Serial.print(servoPort[n]-SERVO_0);           // print servo number
    Serial.print("):");                           // print string
    Serial.print(EmoroServo.read(servoPort[n]));  // print servo pulse width
    Serial.println(" us");                        // print string
  }
  Serial.println("\n");                           // append newline

  repeatCounter=0;                                // reset repeat counter
}

void loop(){              
  int n, k, res;                                  // initialize variables
  int pulseWidth[4]={1000, 1500, 2000, 1500};     // define used pulse widths

  for (n=0; n<4; n++){                            // iterate throug pulse widths
    for (k=0; k<3; k++){                          // iterate throug servos
      
      // Set servo pulse according to data from array: 
      res=EmoroServo.write(servoPort[k], pulseWidth[n]);    // set servo pulse width
      if(res==0){                                           // if servo is attached
        // print action to console
        Serial.print("Set servo pulse (SERVO_");            // print string
        Serial.print(servoPort[k]-SERVO_0);                 // print servo number
        Serial.print("):");                                 // print string
        Serial.print(EmoroServo.read(servoPort[k]));        // print servo pulse width
        Serial.println(" us");                              // print string
      }
      else if(res<0){                                       // if servo is not attached
        // servo motor on specified port isn't initialized
        // there is no PWM signal on specified port
        // print info to console
        Serial.print("SERVO_");                             // print string
        Serial.print(servoPort[k]-SERVO_0);                 // print servo number
        Serial.println(" is't initialized!");               // print string
      }
    }
    
    repeatCounter++;                                        // increment repeat counter
    Serial.print("Counter: ");                              // print string
    Serial.println(repeatCounter);                          // print conter value
    Serial.println("\n");                                   // append newline

    delay(waitState);                                       // wait for specified time in milliseconds
 
    if(repeatCounter==10){                                  // after 10 repeats detach the second and the third servo motor
      EmoroServo.detach(servoPort[1]);                      // detach Servo 1
      EmoroServo.detach(servoPort[2]);                      // detach Servo 3
    }

  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

