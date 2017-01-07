#XLR8Servo
## Accelerated Servo Library for Arduino

Arduino's Servo library causes your servos to jitter.  
Alorium Technology's XLR8Servo Library doesn't jitter.  
With Arduino's Servo library you lose the analogWrite() (PWM) functionality on pins 9 and 10.  
With Alorium Technology's XLR8Servo Library you can still use analogWrite() (PWM) functionality on pins 9 and 10.  
Arduino's Servo library uses the microcontroller's only 16bit timer.  
Alorium Technology's XLR8Servo Library leaves the 16bit timer available for other uses.

This library allows an Alorim Technology XLR8 board (a drop-in replacement for Arduino Uno found at www.aloriumtech.com) to control RC (hobby) servo motors.  
It's a drop in replacement for the standard Arduino servo library, but it uses Alorium Technology's XLR8 acceleration hardware to drive the servos so the timing is more precise, doesn't jitter, doesn't depend on interrupt handling, and doesn't cause PWM functionality to be lost.  
If you have an XLR8 board, this library is better in every way.

Using this library is the same as using Arduino's servo library which is very nicely described at http://arduino.cc/en/Reference/Servo
