This Arduino Library will let you PWM color-control an HL1606-based RGB LED strip. 
Its capable of 3, 4 or even 5 bit color per LED (9, 12, 15 bit RGB) depending on the length of the strip


RAM usage: 3 times the number of LEDs (in bytes) plus maybe 24 more after that
CPU usage: settable from 0-100 percent

Timer 2 is used to run the PWM in the background

Pins 11 and 13 (hardware SPI must be used), an additional pin is required for the latch

If you don't need PWM support, see https://github.com/adafruit/HL1606-LED-Strip for basic LED strip control

--

To download, click *DOWNLOADS* in the top right

--

To install, see our tutorial here
http://www.ladyada.net/library/arduino/libraries.html