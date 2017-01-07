// Arduino RBD Timer Library v1.3.0 Example - A three second interval timer that prints to serial.
// https://github.com/alextaujenis/RBD_Timer
// Copyright 2016 Alex Taujenis
// MIT License

#include <RBD_Timer.h> // https://github.com/alextaujenis/RBD_Timer

RBD::Timer timer;

void setup() {
  Serial.begin(115200);
  timer.setTimeout(3000);
  timer.restart();
}

void loop() {
  if(timer.onRestart()) {
    Serial.println("Three seconds passed");
  }
}