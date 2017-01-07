/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Single HC-SR04                                                                  */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch reads a HC-SR04 ultrasonic rangefinder and returns the distance to the closest    */
/*            object in range. It uses special EMoRo 2560 library which continuously make measurements on   */
/*            initialized port in the background and make available reading without special delay. This     */
/*            way of using is especially useful for robotics application.                                   */
/*      The circuit:                                                                                        */
/*            * GND connection of the HC-SR04 attached to ground (GPP_0 pin 1)                              */
/*            * +V connection of the HC-SR04 attached to +5V (GPP_0 pin 4)                                  */
/*	          * Echo connection of the HC-SR04 attached to GPP_0_A  (GPP_0 pin 2)                           */
/*            * Trig connection of the HC-SR04 attached to GPP_0_B  (GPP_0 pin 3)                           */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          HC-SR04 ultrasonic sensor                                                       */
/*                          Female/Female cable with header connectors 4x1 RM 2.54mm                        */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
int sensorPin = GPP_0;    // Select the input pin for the distance sensor. 
                          // Available ports are GPP_0 to GPP_7

void setup() {
  Serial.begin(9600);                         // initialize serial communication
  Serial.println("Example: Single HC-SR04");  // send example name to console
  // initialize background reading of ultrasonic sensor on port defined port
  // it is possible to use max 8 ultrasonic sensors. Available ports are GPP_0 to GPP_7
  Ultrasonic.attach(sensorPin);               // attach sensor 
}

void loop()
{
  // establish variables for the distance result in inches and centimetres:
  double inches;                              // initialize variable
  int  cm;                                    // initialize variable

  // get last distance reading on ultrasonic sensor attached to defined port
  cm=Ultrasonic.read(sensorPin);              // get distance in cm
  inches=(double)cm/2.54;                     // convert the distance from cm to inches

  // print measurements to console
  Serial.print("Measured distance: ");        // print string
  Serial.print(inches);                       // print inches value
  Serial.print("in, ");                       // print string
  Serial.print(cm);                           // print cm value
  Serial.print("cm");                         // print string
  Serial.println();                           // go to new line
  
  delay(100);                                 // delay 100 ms
}


/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

