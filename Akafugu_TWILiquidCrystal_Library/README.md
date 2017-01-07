TWILCD
==========

Firmware and Arduino library for Akafugu TWI/I2C LCD controller. See [this](http://www.akafugu.jp/posts/products/twilcd/) page for more details.

Arduino Library
---------------

The Arduino Library is located in the TWILiquidCrystal subdirectory.

To use this library, you will first need the Arduino IDE. Get it at [http://arduino.cc/](http://arduino.cc/).

In order to use the library, you must copy the entire TWILiquidCrystal subdirectory into the libraries subdirectory of your Arduino installation. Be sure to restart the Arduino IDE after doing this.
For details on library installation, have a look [http://www.akafugu.jp/posts/resources/arduino-ide-akafugu/](here).

You should now have a TWILiquidCrystal submenu inside File -> Examples. Open the simple example and press PLAY to compile it.

To try out the display with an Arduino board (such as the Arduino UNO), hook the four wires as follows:

SDA - Pin A4  
SCL - Pin A5  
GND - GND  
VCC - 5V  

After everything is hooked up, connect the USB port of the Arduino to your computer, select the correct port in the Tools->Serial Port menu, and press the Upload button. Once programming finished, you should see the message "Hello World!" and a second counter at the bottom.

Firmware
--------

The firmware for the Akafugu TWI/I2C LCD controller is located in the firmware subdirectory.

The controller comes pre-flashed with firmware, so it is not neccesary to flash it unless you want to upgrade to a newer version. To flash it you will need a ISP programmer.

