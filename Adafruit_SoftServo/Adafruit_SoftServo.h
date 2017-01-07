// This is an ultra simple software servo driver. For best
// results, use with a timer0 interrupt to refresh() all
// your servos once every 20 milliseconds!
// Written by Limor Fried for Adafruit Industries, BSD license

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
 

class Adafruit_SoftServo {
 public:
  Adafruit_SoftServo(void);
  void attach(uint8_t pin);
  void detach();
  boolean attached();
  void write(uint8_t a);
  void refresh(void);
 private:
  boolean isAttached;
  uint8_t servoPin, angle;
  uint16_t micros;
};
