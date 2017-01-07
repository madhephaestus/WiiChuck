// Arduino RBD Servo Library v1.0.1 Example - Move a servo with a 1-2ms pulse and 20ms interval to 90 degrees.
// https://github.com/alextaujenis/RBD_Servo
// Copyright 2015 Alex Taujenis
// MIT License

#include <RBD_Servo.h> // https://github.com/alextaujenis/RBD_Servo

RBD::Servo servo(2, 1000, 2000); //pin 2, 1ms - 2ms pulse

void setup() {
  servo.moveToDegrees(90);
}

void loop() {
  servo.update();
}
