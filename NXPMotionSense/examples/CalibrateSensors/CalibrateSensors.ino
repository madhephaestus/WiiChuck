#include <NXPMotionSense.h>
#include <Wire.h>
#include <EEPROM.h>
#include <util/crc16.h>

NXPMotionSense imu;

const int ledPin = 13;
int ledState = LOW;
int ledFastblinks = 0;
elapsedMillis ledMillis = 0;
int loopcount = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) ; // wait for serial port open
  imu.begin();
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int ax, ay, az;
  int gx, gy, gz;
  int mx, my, mz;

  // get and print uncalibrated data
  if (imu.available()) {
    imu.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
    Serial.print("Raw:");
    Serial.print(ax);
    Serial.print(',');
    Serial.print(ay);
    Serial.print(',');
    Serial.print(az);
    Serial.print(',');
    Serial.print(gx);
    Serial.print(',');
    Serial.print(gy);
    Serial.print(',');
    Serial.print(gz);
    Serial.print(',');
    Serial.print(mx);
    Serial.print(',');
    Serial.print(my);
    Serial.print(',');
    Serial.print(mz);
    Serial.println();
    loopcount = loopcount + 1;
  }

  // check for incoming calibration
  receiveCalibration();

  // occasionally print calibration
  if (loopcount == 50 || loopcount > 100) {
    Serial.print("Cal1:");
    float caloffsets[9], calmag;
    imu.getCalibration(caloffsets, NULL, &calmag);
    for (int i=0; i<9; i++) {
      Serial.print(caloffsets[i], 3);
      Serial.print(',');
    }
    Serial.println(calmag, 3);
    loopcount = loopcount + 1;
  }
  if (loopcount >= 100) {
    Serial.print("Cal2:");
    float calsoftiron[9];
    imu.getCalibration(NULL, calsoftiron, NULL);
    for (int i=0; i<9; i++) {
      Serial.print(calsoftiron[i], 4);
      if (i < 8) Serial.print(',');
    }
    Serial.println();
    loopcount = 0;
  }

  // blink LED, slow normally, fast when calibration written
  if (ledMillis >= 1000) {
    if (ledFastblinks > 0) {
      ledFastblinks = ledFastblinks - 1;
      ledMillis -= 125;
    } else {
      ledMillis -= 1000;
    }
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}

byte caldata[68]; // buffer to receive magnetic calibration data
byte calcount=0;

void receiveCalibration() {
  uint16_t crc;
  byte b, i;

  if (Serial.available()) {
    b = Serial.read();
    if (calcount == 0 && b != 117) {
      // first byte must be 117
      return;
    }
    if (calcount == 1 && b != 84) {
      // second byte must be 84
      calcount = 0;
      return;
    }
    // store this byte
    caldata[calcount++] = b;
    if (calcount < 68) {
      // full calibration message is 68 bytes
      return;
    }
    // verify the crc16 check
    crc = 0xFFFF;
    for (i=0; i < 68; i++) {
      crc = _crc16_update(crc, caldata[i]);
    }
    if (crc == 0) {
      // data looks good, use it
      imu.writeCalibration(caldata);
      calcount = 0;
      loopcount = 10000;
      ledFastblinks = 16; // Toggle LED faster the next 16 times
      return;
    }
    // look for the 117,84 in the data, before discarding
    for (i=2; i < 67; i++) {
      if (caldata[i] == 117 && caldata[i+1] == 84) {
        // found possible start within data
        calcount = 68 - i;
        memmove(caldata, caldata + i, calcount);
        return;
      }
    }
    // look for 117 in last byte
    if (caldata[67] == 117) {
      caldata[0] = 117;
      calcount = 1;
    } else {
      calcount = 0;
    }
  }
}



