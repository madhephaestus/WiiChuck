# Ingenia Serial Servo Drive Library
##### Ingenia servo drives master library for Arduino

This library will abstract users from the servo drive communication protocol layer. This will allow the Arduino to interface with the Ingenia Servo Drives through serial communications and easyly prototipe a simple master for the ingenia servo drive with Arduino.

## Features  
-   `enableMotor` and `disableMotor`  functions that allows the user to enable and disable the motor using a single line function.
-   Get and Set function for the position, velocity and torque actual or target values.
-   Easily read the drive controlword status.
-   Change the mode of operation with the `setModeOfOperation` function.
-   `read` and `write` functions to allow modify the drive registers with a simple call.

## Examples
**AdvancedComms**
: This example interact with the servodrive enabling the motor and moving through two different positions using the registers with the `read` and `write` functions.

**BasicComms**
: This example moves the motor in Profile Position Mode thrugh two different positions using the functions: `enableMotor`, `setModeOfOperation` and `setTargetPosition`.

**StatusWordTargetReach**
: This example moves the motor in Profile Position Mode thrugh two different positions when the target is reached, using the functions: `statuswordIsTargetReached`, `enableMotor`, `setModeOfOperation` and `setTargetPosition`.

## Installation  
To install the library into your Arduino IDE you can use the Library Manager (available from IDE version 1.6.2). Open the IDE and click to the "Sketch" menu and then Include Library > Manage Libraries. Then you should find the library called: Ingenia Serial Servo Driver Library.

Also, you can download from the top of this page and unzip it.
The new folder will have a name like Ingenia_SerialServoDrive-1.0.0.
Rename it to just Ingenia_SerialServoDrive.   Then, following the instructions from the [Arduino libraries guide](http://arduino.cc/en/Guide/Libraries), except using the renamed folder rather the zip:  

In the Arduino IDE, navigate to __Sketch > Import Library__.  At the top of the drop
down list, select the option to __Add Library__.  Navigate to the folder's location and open it. 
Return to the __Sketch > Import Library__ menu.
You should now see the library at the bottom of the drop-down menu.
It is ready to be used in your sketch.
