#include <Ingenia_SerialServoDrive.h>

// Initialize Serial Port 1 - used by Ingenia ServoDrive
Ingenia_SerialServoDrive servo(&Serial1);

void setup() {
  Serial1.begin(115200);

  // Set Modes of Operation to Position Mode
  servo.setModeOfOperation(Ingenia_SerialServoDrive::DRIVE_MODE_PROFILE_POSITION);

  // Enable the motor
  servo.enableMotor();
}

void loop() {
  servo.setTargetPosition(1000);
  delay(500);

  servo.setTargetPosition(0);
  delay(500);
}
