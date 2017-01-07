/*
 Joystick Mouse Control
 
 Controls a PC mouse from a joystick on an Adafruit Pro Trinket.
 Uses the joystick pushbutton to click the left mouse button
 
 Hardware:
 * 2-axis joystick connected to pins A0 and A1 with pushbutton on D10
 * Pushbutton enable/disable the mouse entirely on D9 (optional)
 
 The mouse movement is always relative. 
 
 The sketch assumes that the joystick resting values are around the 
 middle of the range, but that they vary within a threshold.
 
 WARNING:  When you use the new mouse, the Arduino takes
 over your mouse!  Make sure you have control before you use the project.
 This sketch includes a pushbutton to toggle the mouse on and off.
 
Based on software on arduino.cc by Tom Igoe placed in the public domain

Version 1.0  Initial version for Adafruit Pro Trinket by Mike Barela
 
 */
#include <ProTrinketMouse.h>  // include mouse library for Pro Trinket (3V or 5V)

// set pin numbers for switch, joystick axes, and LED
const int switchPin   =  9;   // switch to turn on and off mouse control
const int mouseButton = 10;   // input pin for the mouse pushButton
const int xAxis  = 1;         // joystick X axis to A1 
const int yAxis  = 0;         // joystick Y axis to A0
const int ledPin = 13;        // Mouse control LED 

// parameters for reading the joystick
int range = 12;               // output range of X or Y movement (zero to range)
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range/4;      // resting threshold
int center = range/2;         // resting position value
const float powerValue = 1.4; // for exponential behavior, 1 < value < 2

boolean mouseIsActive = false;      // whether or not to control the mouse
int lastSwitchState = LOW;          // previous switch state
boolean mouseButtonPressed = false; // whether or not mouse button pressed
int  lastReading = 1;       // last joystick/mouse button reading
long debounceTime = 0;      // last time the mouse button was toggled
long debounce = 50;         // debounce time, increase if the mouse clicks rapidly

void setup() {
  pinMode(switchPin,   INPUT_PULLUP);   // the switch pin
  pinMode(mouseButton, INPUT_PULLUP);   // mouse button on joystick
  pinMode(ledPin, OUTPUT);              // the LED pin  
  TrinketMouse.begin();                 // initialize the mouse library
}

void loop() {
  int switchState;  // State of the mouse enable/disable button
  int buttonState;  // State of the mouse left button switch on joystick
  int xReading, yReading; // readings of the joystick movements
  int buttonReading;      // reading of the joystick (left mouse) button
  
  switchState = digitalRead(switchPin);  // read the mouse disable switch
  // if it's changed and it's high, toggle the mouse state
  if (switchState != lastSwitchState) {
    if (switchState == HIGH) {
      mouseIsActive = !mouseIsActive;
//      digitalWrite(ledPin, mouseIsActive);   // toggle LED to indicate mouse state
    } 
  }
  lastSwitchState = switchState;    // save switch state for next comparison

  // read and scale the two joystick readings, one for each axis
  xReading = readAxis(xAxis);
  yReading = readAxis(yAxis);

  // This code gives the mouse a nonlinear acceleration 
  //   These 8 lines may be commented out to have linear acceleration
  if(xReading > 0)
     xReading =  (int)pow(powerValue,xReading);
  else if(xReading < 0)
     xReading = -(int)pow(powerValue,-xReading);
     
  if(yReading > 0)
     yReading =  (int)pow(powerValue,yReading);
  else if(yReading < 0)
     yReading = -(int)pow(powerValue,-yReading);  // end nonlinear acceleration code

  // Read the joystick button as the left mouse button.  Debounce per 
  //   Ladyada code at https://learn.adafruit.com/tilt-sensor/using-a-tilt-sensor
  buttonReading = digitalRead(mouseButton);  // read the mouse left button (push joystick)
  if(buttonReading != lastReading) {         // switch changed
     debounceTime = millis();                // reset debounce timer
  }
  if((millis() - debounceTime) > debounce) {
     buttonState = buttonReading;
     if(buttonState == LOW) {
        mouseButtonPressed = true;
     }
     else {
        mouseButtonPressed = false;
     }
  } 
  lastReading = buttonReading;
  digitalWrite(ledPin, mouseButtonPressed);  // toggle LED to indicate button state
  
  // if the mouse control state is active, move the mouse:
  if (mouseIsActive) {
      if (mouseButtonPressed) {  // if joystick pressed down, indicate that too
         TrinketMouse.move(xReading, yReading, 0, MOUSEBTN_LEFT_MASK);
      }
      else {
         TrinketMouse.move(xReading, yReading, 0, 0);  // move, no mouse button press
      }
  }  
  delay(responseDelay);  // wait between mouse readings
}

// Reads a joystick axis (0 or 1 for x or y) and scales the 
//  analog input range to a range from 0 to <range>
int readAxis(int thisAxis) { 
  int reading = analogRead(thisAxis);  // read the analog input

  // map the reading from the analog input range to the output range
  reading = map(reading, 0, 1023, 0, range);

  // if the output reading is outside from the rest position threshold, use it
  int distance = center - reading;

  if (abs(distance) < threshold) { // if distance not to threshold, no move
    distance = 0;                  // prevents tiny jitters due to readings
  } 
  return distance;   // return the distance for this axis
}

