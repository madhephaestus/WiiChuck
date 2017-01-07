#include <Ingenia_SerialServoDrive.h>

// Initialize Serial Port 1 - used by Ingenia ServoDrive
Ingenia_SerialServoDrive servo(&Serial1);

// Intialize the current mode (go to position 0)
int currentMode = 1;

void setup() {
  Serial1.begin(115200);

  // Set Modes of Operation to Position Mode
  servo.setModeOfOperation(Ingenia_SerialServoDrive::DRIVE_MODE_PROFILE_POSITION);

  // Enable the motor
  servo.enableMotor();

  // Send the target position to 0
  servo.setTargetPosition(0);
}

void loop() {
  // If is target reached change the target position and the current mode
  if (servo.statuswordIsTargetReached()) {
    switch (currentMode) {
      case 1:
        servo.setTargetPosition(10000);
        currentMode = 2;
        break;
      case 2:
        servo.setTargetPosition(0);
        currentMode = 1;
        break;
      default:
        break;
    }
  }
}
