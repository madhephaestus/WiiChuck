/**************************************************************************/
/*!
    @file     ProTrinketMouseAccel.ino
    @author   Mike Barela for Adafruit Industries
    @license  BSD (see license.txt)

    This is an example of using the Adafruit Pro Trinket with the Adafruit MMA8451 
    Accelerometer breakout board to make a mouse using motion gestures

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release 1/25/2015  Mike Barela
*/
/**************************************************************************/

#include <Wire.h>               // I2C Library
#include <Adafruit_MMA8451.h>   // Accelerometer library
#include <Adafruit_Sensor.h>    // Adafruit sensor library
#include <ProTrinketMouse.h>    // Pro Trinket V-USB mouse emulator
#define  DEBUG  0               // Set to 1 for serial console debugging, 0 otherwise

const uint8_t LEFTBUTTON  = 8;  // Left Mouse Button on this Pro Trinket Pin
const uint8_t RIGHTBUTTON = 9;  // Right Mouse Button on Pro Trinket

Adafruit_MMA8451 mma = Adafruit_MMA8451();

//Change these values if accelerometer reading are different:
//How far the accerometer is tilted before
//starting to move the mouse:
const int MovementThreshold = 18;

//The average zero acceleration values read
//from the accelerometer for each axis:
const int ZeroXValue = 0;
const int ZeroYValue = 0;
//const int ZeroZValue = 0;

//The maximum (positive) acceleration values read
//from the accelerometer for each axis:
const int MaxXValue = 4096;
const int MaxYValue = 4096;
//const int MaxZValue = 4096;

//The minimum (negative) acceleration values read
//from the accelerometer for each axis:
const int MinXValue = -4096;
const int MinYValue = -4096;
//const int MinZValue = -4096;

//The sign of the mouse movement relative to the acceleration.
//If your cursor is going in the opposite direction you think it
//should go, change the sign for the appropriate axis.
const int XSign = 1;
const int YSign = 1;
//const int ZSign = 1;

//The maximum speed in each axis (x and y)
//that the cursor should move. Set this to a higher or lower
//number if the cursor does not move fast enough or is too fast.
const int MaxMouseMovement = 50;  

//This reduces the 'twitchiness' of the cursor by calling
//a delay function at the end of the main loop.
//There are better way to do this without delaying the whole
//microcontroller, but that is left for another tutorial or project.
const int MouseDelay = 12;
  
void setup(void) {
#if DEBUG  
  Serial.begin(9600);
  Serial.println("Pro Trinket Accelerometer Mouse");
#endif  
  if (! mma.begin()) {  // If the accelerometer cannot be found, flash LED
    pinMode(13, OUTPUT);
    while (1) {         // Flash the Pin 13 LED quickly to indicate an error
      digitalWrite(13, HIGH);
      delay(350);
      digitalWrite(13, LOW);
      delay(350);
    }
  }
  mma.setRange(MMA8451_RANGE_2_G);  // 2G Mode is best for hand gestures
  mma.read();                       // get an initial read 
  
  TrinketMouse.begin();               // Initialize mouse library
  pinMode(LEFTBUTTON,  INPUT_PULLUP); // Left and right mouse button pins initialized
  pinMode(RIGHTBUTTON, INPUT_PULLUP); //   with internal pullup resistors (bring Low with button)
}

void loop() {
  
  mma.read();  //   // Read the 'raw' data in 14-bit counts
#if DEBUG
  Serial.print("X:\t"); Serial.print(mma.x); 
  Serial.print("\tY:\t"); Serial.print(mma.y); 
  Serial.print("\tZ:\t"); Serial.println(mma.z); 
#endif

  processAccelerometer(mma.x,mma.y, mma.z);  // Work with the read data
  
  delay(MouseDelay);  // wait until next reading - was 500 in Adafruit example
}

//Function to process the acclerometer data
//and send mouse movement information via USB
void processAccelerometer(int16_t XReading, int16_t YReading, int16_t ZReading)
{
  //Initialize values for the mouse cursor movement.
  int16_t MouseXMovement = 0;
  int16_t MouseYMovement = 0;
  
  //Calculate mouse movement
  //If the analog X reading is ouside of the zero threshold...
  if( MovementThreshold < abs( XReading - ZeroXValue ) )
  {
    //...calculate X mouse movement based on how far the X acceleration is from its zero value.
    MouseXMovement = XSign * ( ( ( (float)( 2 * MaxMouseMovement ) / ( MaxXValue - MinXValue ) ) * ( XReading - MinXValue ) ) - MaxMouseMovement );
    //it could use some improvement, like making it trigonometric.
  }
  else
  {
    //Within the zero threshold, the cursor does not move in the X.
    MouseXMovement = 0;
  }

  //If the analog Y reading is ouside of the zero threshold... 
  if( MovementThreshold < abs( YReading - ZeroYValue ) )
  {
    //...calculate Y mouse movement based on how far the Y acceleration is from its zero value.
    MouseYMovement = YSign * ( ( ( (float)( 2 * MaxMouseMovement ) / ( MaxYValue - MinYValue ) ) * ( YReading - MinYValue ) ) - MaxMouseMovement );
    //it could use some improvement, like making it trigonometric.
  }
  else
  {
    //Within the zero threshold, the cursor does not move in the Y.
    MouseYMovement = 0;
  }
 
  if(digitalRead(LEFTBUTTON) == LOW) {             // If left button pressed
#if DEBUG
    Serial.println("Left Mouse Button");
#endif
    TrinketMouse.move(0,0,0,MOUSEBTN_LEFT_MASK);   //  tell PC
  }
  else if (digitalRead(RIGHTBUTTON) == LOW) {      // If right button pressed
#if DEBUG
    Serial.println("Right Mouse Button");
#endif
    TrinketMouse.move(0,0,0,MOUSEBTN_RIGHT_MASK);  //  tell PC 
  }
  else {
    TrinketMouse.move(MouseXMovement, MouseYMovement, 0, 0);  // otherwise just move mouse
  }

}

