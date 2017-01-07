#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>

NXPMotionSense imu;

void setup() {
  float offsets[9];
  float softIron[9];
  float fieldStrength;
  int i, j;

  Serial.begin(9600);
  while (!Serial) ; // wait for serial port open
  imu.begin();
  Serial.println("Calibration Data");
  Serial.println();
  imu.getCalibration(offsets, softIron, &fieldStrength);
  Serial.println("Magnetic (Hard Iron) Offsets");
  for (i=0; i<3; i++) {
    printnum(offsets[i + 6], 2);
    Serial.println(" uT");
  }
  Serial.println();
  Serial.println("Magnetic Soft Iron Mapping");
  for (i=0; i<3; i++) {
    for (j=0; j<3; j++) {
      printnum(softIron[i*3+j], 4);
    }
    Serial.println();
  }
  Serial.println();
  Serial.println("Expected Magnetic Field Strength");
  printnum(fieldStrength, 2);
  Serial.println(" uT");
  Serial.println();
  Serial.println("Accelerometer Offsets");
  for (i=0; i<3; i++) {
    printnum(offsets[i], 3);
    Serial.println(" g");
  }
  Serial.println();
  Serial.println("Gyroscope Offsets");
  for (i=0; i<3; i++) {
    printnum(offsets[i + 3], 3);
    Serial.println(" degrees/sec");
  }
  Serial.println();
}

void printnum(float num, int digits) {
      Serial.print("  ");
      if (num >= 0.0f) Serial.print(' ');
      Serial.print(num, digits);
}

void loop() {
}
