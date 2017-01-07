/*
 * Arduino Learning Board Project - Stepper Motor Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 *
 */
 
// - -> GND
// + -> 5V
// IN1 -> D8
// IN2 -> D9
// IN3 -> D10
// IN4 -> D11

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_Stepper to include the Stepper Motor functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)
#define USE_ALB_Stepper

// NOW include the main ArduinoLearningBoard library
// Based on the defines above, the appropriate modules will be added to the project
#include "ArduinoLearningBoard.h"

#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  8     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  10    // IN3 on the ULN2003 driver 1
#define motorPin4  11    // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  Serial.begin(9600);
  
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(300.0);
  stepper1.setSpeed(1000);
  stepper1.moveTo(5000);

  Serial.println("Moving Stepper to Position 5000");
}

void loop() {
  
  // Serial.println(stepper1.currentPosition());
  	
  //Change direction when the stepper reaches the target position
  if (stepper1.distanceToGo() == 0) {
  	Serial.print("Stepper Arrived at Destination. MoveTo(");
  	Serial.print(-stepper1.currentPosition());
  	Serial.println(")");
    stepper1.moveTo(-stepper1.currentPosition());
  }
  stepper1.run();
}