TWI/I2C Real-time clock library
===============================

A library for the DS1307 and DS3231 real time clocks for ATMega chips. The library comes in two flavors: Arduino library and avr-gcc library. The library auto-detects the chip connected.

Features available for both DS1307 and DS3231:

* Set and get time
* Control the square wave oscillator output (can generate square waves with frequency 1Hz, 1024kHz, 4096kHz and 8192kHz). When in use, a pull-up resistor is required on the output pin.
* Set/get daily alarm

Features available on the DS3231 only:

* Read temperature / force temperature conversion
* Enable 32kHz square wave oscillator output. A pull-up resistor is required on the output pin to use this functionality.

Features available on the DS1307 only:

* Access 56 bytes of SRAM.
* Start/halt the clock.

PS: The alarm function uses SRAM bytes 0 to 2 on the DS1307 (in order to support retaining the alarm value through the backup battery, writing any other values to these 3 bytes will invalidate the alarm. On the DS3231, the chip internal alarm function is used. This alarm value is also retained through the backup battery. 

Arduino library
---------------

Located in the WireRtcLib subdirectory. 

To use the library, copy the entire directory into the libraries subdirectory of your Arduino installation.

After doing this, you will have a WireRtcLib submenu inside File -> Examples. Open the simple example and press PLAY to compile it.

Pinout depends on what chip you are using. SDA goes to pin A4 on the Arduino, SCL to pin A5. The DS1307 is a 5V chip, so make sure to use it with a 5V Arduino. The DS3231 also support 3.3V.

After hooking up your chip of choice, press Upload in the Arduino IDE, then open the serial console (the rightmost button on the toolbar). You should see the current time the chip is set to printed repeatedly.

avr-gcc library
---------------

Located in the library-gcc directory. The library is self-contained, and contains a hardware TWI implementation (in twi.c and twi-lowlevel.c). main.c contains simple test code.
