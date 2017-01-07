TWIDISPLAY
==========

Firmware, Arduino library and avr-gcc library for Akafugu 7-segment TWI display. See [this](http://www.akafugu.jp/posts/products/twi-7-segment/) page for more details.

Arduino Library
---------------

The Arduino Library is located in the TWIDisplay subdirectory.

To use this library, you will first need the Arduino IDE. Get it at [http://arduino.cc/](http://arduino.cc/).

In order to use the library, you must copy the entire TWIDisplay subdirectory into the libraries subdirectory of your Arduino installation. Be sure to restart the Arduino IDE after doing this.
For details on library installation, have a look [http://www.akafugu.jp/posts/resources/arduino-ide-akafugu/](here).

You should now have a TWIDisplay submenu inside File -> Examples. Open the simple example and press PLAY to compile it.

To try out the display with an Arduino board (such as the Arduino UNO), hook the four wires as follows:

SDA - Pin A4  
SCL - Pin A5  
GND - GND  
VCC - 5V  

After everything is hooked up, connect the USB port of the Arduino to your computer, select the correct port in the Tools->Serial Port menu, and press the Upload button. Once programming finished, you should see the display count from 0 to 9999.

avr-gcc Library
---------------

The avr-gcc library is meant for more advanced users who wish to use the display directly in low-level C code. The library is contained in the library-gcc subdirectory.

To compile the avr-gcc library, you first have to install avr-gcc. For Windows, use [WinAVR](http://winavr.sourceforge.net/), for OS X, use [CrossPack](http://www.obdev.at/products/crosspack/index.html).

In the library-gcc directory, you will find a Makefile. Type make, and you should have an example program that demonstrates the different features of the display.

To flash the binary file to your microcontroller, you will first have to create a file user.mk in your home directory (if you are on Windows, we recommend you install [Cygwin](www.cygwin.com) and run make from the Cygwin command line).

If you are using an ISP programmer, user.mk should contain the following:

AVRDUDE_PROGRAMMER = [the name of the ISP programmer you are using]  
AVRDUDE_PORT = [the COM port the programmer is hooked up to]  
AVRDUDE_SPEED = [the max speed the programmer supports]  

If you are using an Arduino, or a chip or board with an Arduino (or optiboot) bootloader, try the following:

AVRDUDE_PROGRAMMER = arduino  
AVRDUDE_PORT = [the COM port of the Arduino]  
AVRDUDE_SPEED = 115200  

(Try AVRDUDE_PROGRAMMER to stk500v1 if arduino doesn't work.) This allows you to program in low-level avr-gcc without having an ISP programmer. Depending on the OS you are running and the version of avrdude you have installed, it is sometimes neccesary to manually reset the device just before programming it.

To start programming, type make flash.

Dot Net Micro Library
---

There is a library for dot net micro in the directory TWIDisplay-dotnetmicro.
The library is designed for microcontroller boards that run the dot net micro
framework.

It has been tested with Netduino and FEZ Panda. (The included project file is
for Netduino, to build for FEZ Panda some changes may be required).

To run on Netduino, pull-up resistors on SDA and SCL are required. On FEZ Panda
the internal pull-ups can be used.

Firmware
--------

The firmware for the Akafugu 7-segment TWI display is located in the firmware subdirectory.

The display comes pre-flashed with firmware, so it is not neccesary to flash it unless you want to upgrade to a newer version. To flash it you will need a ISP programmer. The makefile functions the same way as the makefile in the avr-gcc library.


