/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Come back                                                                       */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This sketch shows basic use of line sensors to follow a black line on a white surface.        */
/*            After initialization press switch 1 on GLAM module. The robot will start to follow the black  */
/*            line until both sensors detect a black line. The robot will then stop. The process can be     */
/*            restarted by pressing the switch 1 again.                                                     */
/*                                                                                                          */
/*      The circuit:                                                                                        */
/*            * Attach two infrared sensors to EMoRo 2560 controller                                        */
/*              Left sensor is attached to IO_0 and right sensor is attached to IO_1                        */
/*              Attach brown wire of sensor to leftmost pin of IO connector (marked with a small triangle)  */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                                                                                                          */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

static int Lfwd = 1000;                                           // Servo values for each motor and each direction
static int Rfwd = 2000;                                           // Servo values for each motor and each direction
static int Lbck = 2000;                                           // Servo values for each motor and each direction
static int Rbck = 1000;                                           // Servo values for each motor and each direction
static int Lstp = 1500;                                           // Servo values for each motor and each direction
static int Rstp = 1500;                                           // Servo values for each motor and each direction

int follow=0;                                                     // initialize variable - follow on/off

void setup() {

  InitEmoro();                                                    // Initializes all available inputs and outputs on EMoRo 2560.
  EmoroServo.attach(SERVO_0);                                     // Initiates servo on SERVO_1 port.
  EmoroServo.attach(SERVO_1);                                     // Initiates servo on SERVO_2 port.

  Serial.begin(9600);                                             // initialize serial communication
  Serial.println("Example: Line follow");                         // send example name to console

  pinMode(IO_0, INPUT_PULLUP);                                    // Initiates input mode with pullup - infrared sensor on IO_1 port.
  pinMode(IO_1, INPUT_PULLUP);                                    // Initiates input mode with pullup - infrared sensor on IO_2 port.

}

void loop() {
  while(ReadSwitch(SW_1) == 0 && follow == 0){                    // wait for switch 1 press if not already following
    delay(10);                                                    // delay 10 ms
  }

  follow=1;                                                       // switch 1 was pressed - enter following mode

    if (digitalRead(IO_0) == 1 && digitalRead(IO_1) == 1) {       // Both sensros are on the white, go forward.
      EmoroServo.write(SERVO_0, Lfwd);                            // left servo forward 
      EmoroServo.write(SERVO_1, Rfwd);                            // right servo forward
    }
    else if (digitalRead(IO_0) == 0 && digitalRead(IO_1) == 0) {  // Both sensors are on the black, stop.
      EmoroServo.write(SERVO_0, Lstp);                            // left servo stop
      EmoroServo.write(SERVO_1, Rstp);                            // right servo stop
      follow=0;                                                   // Goal reached - exit following mode
    }
    else if (digitalRead(IO_0) == 0) {                            // Only left sensor is on the black, turn right.
      EmoroServo.write(SERVO_0, Lfwd);                            // left servo forward
      EmoroServo.write(SERVO_1, Rbck);                            // right servo backwards
      delay(20);                                                  // delay 20 ms
    }
    else if (digitalRead(IO_1) == 0) {                            // Only right sensor is on the black, turn left.
      EmoroServo.write(SERVO_0, Lbck);                            // left servo backwards
      EmoroServo.write(SERVO_1, Rfwd);                            // right servo forwawrd
      delay(20);                                                  // delay 20 ms
    }

}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
