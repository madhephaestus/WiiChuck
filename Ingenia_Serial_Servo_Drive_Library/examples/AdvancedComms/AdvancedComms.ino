#include <Ingenia_SerialServoDrive.h>

// Initialize Serial Port 1 - used by Ingenia ServoDrive
Ingenia_SerialServoDrive servo(&Serial1);

// Please, notice that this example does not verify if all commands have been executed correctly 
// in the driver to simplify the understanding of the code.

void setup() {
  Serial1.begin(115200);

  // Set Modes of Operation to Position Mode
  servo.write(0x6060, 0x0, 1L);

  // Enable the motor
  // Send "Shutdown command" to enter in Ready to "Switch on"
  servo.write(0x6040, 0x0, 6L);
  // Send "Switch on command" to enter in "Switch on"
  servo.write(0x6040, 0x0, 7L);
  // Send "Enable operation command" to enter in "Operation Enable"
  servo.write(0x6040, 0x0, 15L);
}

void loop() {  
  // Set Target position to 10000
  servo.write(0x607A, 0x0, 1000L);
  // Set New set-point bit
  servo.write(0x6040, 0x0, 31L);
  // Reset New set-point bit
  servo.write(0x6040, 0x0, 15L);
  delay(500);
  
  // Set Target position to 0
  servo.write(0x607A, 0x0, 0L);
  // Set New set-point bit
  servo.write(0x6040, 0x0, 31L);
  // Reset New set-point bit
  servo.write(0x6040, 0x0, 15L);
  delay(500);
}
