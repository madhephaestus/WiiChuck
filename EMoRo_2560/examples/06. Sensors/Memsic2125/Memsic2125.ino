/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Memsic2125                                                                      */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Read the Memsic 2125 two-axis accelerometer.  Converts the pulses output by the 2125 into     */
/*            milli-g's (1/1000 of earth's gravity) and prints them over the serial connection to the       */
/*            computer.                                                                                     */
/*      The circuit:                                                                                        */
/*           * X output of accelerometer to GPP_0_A (GPP_0 pin 2)                                           */
/*	         * Y output of accelerometer to GPP_0_B (GPP_0 pin 3)                                           */
/*	         * +V of accelerometer to +5V (GPP_0 pin 4)                                                     */
/*	         * GND of accelerometer to ground (GPP_0 pin 1)                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Memsic 2125 sensor                                                              */
/*                          Female header connector 4x1 RM 2.54mm                                           */
/*                          Wires                                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
  http://www.arduino.cc/en/Tutorial/Memsic2125
*/   
 
// these constants won't change:
const int xPin = GPP_0_A;		                  // X output of the accelerometer
const int yPin = GPP_0_B;		                  // Y output of the accelerometer

void setup() {
  Serial.begin(9600);                         // initialize serial communications
  pinMode(xPin, INPUT);                       // initialize the pins connected to the accelerometer as inputs
  pinMode(yPin, INPUT);                       // initialize the pins connected to the accelerometer as inputs
}

void loop() {
  int pulseX, pulseY;                         // variables to read the pulse widths
  int accelerationX, accelerationY;           // variables to contain the resulting accelerations
  
  // read pulse from x- and y-axes:
  pulseX = pulseIn(xPin,HIGH);                // measures the pulse duration on xPin
  pulseY = pulseIn(yPin,HIGH);                // measures the pulse duration on yPin
  
  // convert the pulse width into acceleration
  // accelerationX and accelerationY are in milli-g's: 
  // earth's gravity is 1000 milli-g's, or 1g.
  accelerationX = ((pulseX / 10) - 500) * 8;  // x axis conversion
  accelerationY = ((pulseY / 10) - 500) * 8;  // y axis conversion

  Serial.print(accelerationX);                // print the acceleration
  Serial.print("\t");                         // print a tab character
  Serial.print(accelerationY);                // print the acceleration
  Serial.println();                           // add new line

  delay(100);                                 // delay 100 ms
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

