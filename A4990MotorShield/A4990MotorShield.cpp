#include "A4990MotorShield.h"
const unsigned char A4990MotorShield::_M1DIR = 7;
const unsigned char A4990MotorShield::_M2DIR = 8;
const unsigned char A4990MotorShield::_M1PWM = 9;
const unsigned char A4990MotorShield::_M2PWM = 10;
const unsigned char A4990MotorShield::_FAULT = 6;
boolean A4990MotorShield::_flipM1 = false;
boolean A4990MotorShield::_flipM2 = false;

void A4990MotorShield::initPinsAndMaybeTimer()
{
  // Initialize the pin states used by the motor driver shield
  // digitalWrite is called before and after setting pinMode.
  // It called before pinMode to handle the case where the board
  // is using an ATmega AVR to avoid ever driving the pin high, 
  // even for a short time.
  // It is called after pinMode to handle the case where the board
  // is based on the Atmel SAM3X8E ARM Cortex-M3 CPU, like the Arduino
  // Due. This is necessary because when pinMode is called for the Due
  // it sets the output to high (or 3.3V) regardless of previous
  // digitalWrite calls.
  digitalWrite(_M1PWM, LOW);
  pinMode(_M1PWM, OUTPUT);
  digitalWrite(_M1PWM, LOW);
  digitalWrite(_M2PWM, LOW);
  pinMode(_M2PWM, OUTPUT);
  digitalWrite(_M2PWM, LOW);
  digitalWrite(_M1DIR, LOW);
  pinMode(_M1DIR, OUTPUT);
  digitalWrite(_M1DIR, LOW);
  digitalWrite(_M2DIR, LOW);
  pinMode(_M2DIR, OUTPUT);
  digitalWrite(_M2DIR, LOW);
  pinMode(_FAULT, INPUT_PULLUP);
#ifdef A4990MOTORSHIELD_USE_20KHZ_PWM
  // timer 1 configuration
  // prescaler: clockI/O / 1
  // outputs enabled
  // phase-correct PWM
  // top of 400
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
  TCCR1A = 0b10100000;
  TCCR1B = 0b00010001;
  ICR1 = 400;
#endif
}

// speed should be a number between -400 and 400
void A4990MotorShield::setM1Speed(int speed)
{
  init(); // initialize if necessary
    
  boolean reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed; // make speed a positive quantity
    reverse = 1;    // preserve the direction
  }
  if (speed > 400)  // max 
    speed = 400;
    
#ifdef A4990MOTORSHIELD_USE_20KHZ_PWM
  OCR1A = speed;
#else
  analogWrite(_M1PWM, speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
#endif 

  if (reverse ^ _flipM1) // flip if speed was negative or _flipM1 setting is active, but not both
    digitalWrite(_M1DIR, HIGH);
  else
    digitalWrite(_M1DIR, LOW);
}

// speed should be a number between -400 and 400
void A4990MotorShield::setM2Speed(int speed)
{
  init(); // initialize if necessary
    
  boolean reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 400)  // max PWM duty cycle
    speed = 400;
    
#ifdef A4990MOTORSHIELD_USE_20KHZ_PWM
  OCR1B = speed;
#else
  analogWrite(_M2PWM, speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
#endif

  if (reverse ^ _flipM2) // flip if speed was negative or _flipM2 setting is active, but not both
    digitalWrite(_M2DIR, HIGH);
  else
    digitalWrite(_M2DIR, LOW);
}

// set speed for both motors
// speed should be a number between -400 and 400
void A4990MotorShield::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

void A4990MotorShield::flipM1(boolean flip)
{
  A4990MotorShield::_flipM1 = flip;
}

void A4990MotorShield::flipM2(boolean flip)
{
  A4990MotorShield::_flipM2 = flip;
}

boolean A4990MotorShield::getFault()
{
  init(); // initialize if necessary
  return digitalRead(_FAULT) == LOW;
}