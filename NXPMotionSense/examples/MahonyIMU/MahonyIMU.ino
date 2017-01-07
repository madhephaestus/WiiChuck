// Inertial Monitoring Unit (IMU) using Mahony filter.
//
// To view this data, use the Arduino Serial Monitor to watch the
// scrolling angles, or run the OrientationVisualiser example in Processing.

#include <NXPMotionSense.h>
#include <MahonyAHRS.h>
#include <Wire.h>
#include <EEPROM.h>

NXPMotionSense imu;
Mahony filter;

void setup() {
  Serial.begin(9600);
  imu.begin();
  filter.begin(100); // 100 measurements per second
}

void loop() {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  if (imu.available()) {
    // Read the motion sensors
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);

    // Update the Mahony filter
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
  }
}

