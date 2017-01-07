#include <A4990MotorShield.h>

/*
 * This example uses the A4990MotorShield library to drive each motor with the
 * Pololu A4990 Dual Motor Driver Shield for Arduino forward, then backward. 
 * The yellow user LED is on when a motor is set to a positive speed and off when
 * a motor is set to a negative speed.
 */

#define LED_PIN 13

A4990MotorShield motors;

/* 
 * For safety, it is good practice to monitor motor driver faults and handle
 * them in an appropriate way. If a fault is detected, both motor speeds are set
 * to zero and it is reported on the serial port.
 */
void stopIfFault()
{
  if (motors.getFault())
  {
    motors.setSpeeds(0,0);
    Serial.println("Fault");
    while(1);
  }
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Pololu A4990 Dual Motor Driver Shield for Arduino");
  
  // uncomment one or both of the following lines if your motors' directions need to be flipped
  //motors.flipM1(true);
  //motors.flipM2(true);
}

void loop()
{
  // run M1 motor with positive speed

  digitalWrite(LED_PIN, HIGH);
  
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setM1Speed(speed);
    stopIfFault();
    delay(2);
  }

  for (int speed = 400; speed >= 0; speed--)
  {
    motors.setM1Speed(speed);
    stopIfFault();
    delay(2);
  }
  
  // run M1 motor with negative speed
  
  digitalWrite(LED_PIN, LOW);
  
  for (int speed = 0; speed >= -400; speed--)
  {
    motors.setM1Speed(speed);
    stopIfFault();
    delay(2);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setM1Speed(speed);
    stopIfFault();
    delay(2);
  }

  // run M2 motor with positive speed
  
  digitalWrite(LED_PIN, HIGH);
  
  for (int speed = 0; speed <= 400; speed++)
  {
    motors.setM2Speed(speed);
    stopIfFault();
    delay(2);
  }

  for (int speed = 400; speed >= 0; speed--)
  {
    motors.setM2Speed(speed);
    stopIfFault();
    delay(2);
  }
  
  // run M2 motor with negative speed
  
  digitalWrite(LED_PIN, LOW);
  
  for (int speed = 0; speed >= -400; speed--)
  {
    motors.setM2Speed(speed);
    stopIfFault();
    delay(2);
  }
  
  for (int speed = -400; speed <= 0; speed++)
  {
    motors.setM2Speed(speed);
    stopIfFault();
    delay(2);
  }
  
  delay(500);
}