#include <Dynamixel_Servo.h>

#define HALF_DUPLEX_DIRECTION_PIN 4

/*--------------------------------------------------------------------*/
void setup(void)
{
  servo_init(&Serial1, HALF_DUPLEX_DIRECTION_PIN, SERVO_DEFAULT_BAUD);
  Serial.begin(9600);
}

/*--------------------------------------------------------------------*/
void loop(void)
{
  int timeout = 50; //milliseconds
  servo_error_t error;
  
  /* using high-level interface */
  error = servo_set(SERVO_DEFAULT_ID, SERVO_REGISTER_GOAL_ANGLE, 0, timeout);
  if(error) Serial.print(servo_errors_to_string(error));
  delay(2000);
  
  error = servo_set(SERVO_DEFAULT_ID, SERVO_REGISTER_GOAL_ANGLE, 6.283, timeout);
  if(error) Serial.print(servo_errors_to_string(error));
  delay(2000);



  /* using low-level interface */
  /*  
  //int raw = servo_radians_to_raw(6.283);
  int raw = 4095;
  
  error = servo_set_raw_word(SERVO_DEFAULT_ID, SERVO_REGISTER_GOAL_ANGLE, 0, timeout);
  if(error) Serial.print(servo_errors_to_string(error));
  delay(2000);
    
  error = servo_set_raw_word(SERVO_DEFAULT_ID, SERVO_REGISTER_GOAL_ANGLE, raw, timeout);
  if(error) Serial.print(servo_errors_to_string(error));
  delay(2000);
  */
}
