/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Come back                                                                       */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This sketch shows basic use of magnetometer (digital compass) to obtain robot's direction and */
/*            point the robot to North. After initialization press switch 1 on GLAM module. The robot will  */
/*            rotate until facing the North direction.                                                      */
/*            It is necessary to make calibration of magnetometer before the first using of it.             */
/*            Calibration data are stored in EEPROM of EMoRo 2560 controller. Calibration can be run by     */
/*            resetting EMoRo 2560 controller and holding SW_2 while restarting the controller. After that  */
/*            instructions on LCD or console port should be followed.                                       */
/*            (Calibration has 3 steps, the first one is to lay down controller on horizontal surface, the  */
/*            second step is to rotate EMoRo 2560 controller for 180 degrees, and the last step is to flip  */
/*            controller upside down)                                                                       */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with HMC5883L Magnetometer                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

static int Lfwd = 1000;                                           // Servo values for each motor and each direction
static int Rfwd = 2000;                                           // Servo values for each motor and each direction
static int Lbck = 2000;                                           // Servo values for each motor and each direction
static int Rbck = 1000;                                           // Servo values for each motor and each direction
static int Lstp = 1500;                                           // Servo values for each motor and each direction
static int Rstp = 1500;                                           // Servo values for each motor and each direction

int robotdir;                                                     // initialize variable - store robot direction

void setup() {

  InitEmoro();                                                    // Initializes all available inputs and outputs on EMoRo 2560.
  EmoroServo.attach(SERVO_0);                                     // Initiates servo on SERVO_1 port.
  EmoroServo.attach(SERVO_1);                                     // Initiates servo on SERVO_2 port.

  Serial.begin(9600);                                             // initialize serial communication      
  Serial.println("Example: Turn to North");                       // send example name to console  
  
  if(ReadEmoroHardware() & MAG_AVAILABLE)                         // if magnetometer is available
    Serial.println("Magnetometer / Compass is Available");        // print constant string    
  else{
    Serial.println("Magnetometer / Compass is not Available");    // print constant string   
    Serial.println("Please attach GLAM module and restart");      // print constant string 
    while(1);                                                     // stop example execution
  }
  
}

void loop() {
  if (ReadSwitch(SW_1) == 1) {                                    // if switch 1 is pressed

    // rotate about central axis - CCW:
    EmoroServo.write(SERVO_0, Lbck);                              // go backwards with left servo
    EmoroServo.write(SERVO_1, Rfwd);                              // go forward with right servo

    robotdir=Mag.readDirection();                                 // read robot direction in degrees

    // rotate until Nort facing direction is reached (with +-2 degrees tolerance)
    while(robotdir>2 && robotdir<358){                            // while robot is not facing North
    robotdir=Mag.readDirection();                                 // read robot direction in degrees
    delay(10);                                                    // delay 10 ms
    }

    EmoroServo.write(SERVO_0, Lstp);                              // stop left servo
    EmoroServo.write(SERVO_1, Rstp);                              // stop right servo
  
  } else {                                                        // if switch 1 is not pressed
    EmoroServo.write(SERVO_0, Lstp);                              // stop left servo
    EmoroServo.write(SERVO_1, Rstp);                              // stop right servo
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
