## Adafruit Unified L3GD20 Driver ##

This unified sensor driver is for use with the Adafruit Triple-Axis Gyro sensor and the Adafruit 10DOF Breakout, and has been designed specifically for these boards:

  ----> https://www.adafruit.com/products/1032
  ----> http://www.adafruit.com/products/1604

Check out the links above for our tutorials and wiring diagrams 

The updated 'Unified' sensor driver (based on Adafruit's Sensor API) use I2C to communicate.  If you need to use SPI on the L3GD20, please look at the original (non unified) driver that is still available here: https://github.com/adafruit/Adafruit_L3GD20

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

## What is the Adafruit Unified Sensor Library? ##

The Adafruit Unified Sensor Library ([Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)) provides a common interface and data type for any supported sensor.  It defines some basic information about the sensor (sensor limits, etc.), and returns standard SI units of a specific type and scale for each supported sensor type.

It provides a simple abstraction layer between your application and the actual sensor HW, allowing you to drop in any comparable sensor with only one or two lines of code to change in your project (essentially the constructor since the functions to read sensor data and get information about the sensor are defined in the base Adafruit_Sensor class).

This is imporant useful for two reasons:

1.) You can use the data right away because it's already converted to SI units that you understand and can compare, rather than meaningless values like 0..1023.

2.) Because SI units are standardised in the sensor library, you can also do quick sanity checks when working with new sensors, or drop in any comparable sensor if you need better sensitivity or if a lower cost unit becomes available, etc. 

Light sensors will always report units in lux, gyroscopes will always report units in rad/s, etc. ... freeing you up to focus on the data, rather than digging through the datasheet to understand what the sensor's raw numbers really mean.

## About this Driver ##

Written by Kevin Townsend for Adafruit Industries.  
BSD license, all text above must be included in any redistribution

To download. click the **Download ZIP** in the right-hand column, rename the uncompressed folder Adafruit\_L3GD20\_U. Check that the Adafruit\_L3GD20\_U folder contains Adafruit\_L3GD20\U.cpp and Adafruit\_L3GD20\_U.h

Place the Adafruit\_L3GD20\_U library folder your (arduinosketchfolder)/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.
