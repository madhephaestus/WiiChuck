/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Ping))) Sensor                                                                  */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch reads a PING))) ultrasonic rangefinder and returns the distance to the closest    */
/*            object in range. To do this, it sends a pulse to the sensor to initiate a reading, then       */
/*            listens for a pulse to return. The length of the returning pulse is proportional to the       */
/*            distance of the object from the sensor.                                                       */
/*      The circuit:                                                                                        */
/*            * +V connection of the PING))) attached to +5V (ADC_0 pin 2)                                  */
/*	          * GND connection of the PING))) attached to ground (ADC_0 pin 1)                              */
/*	          * SIG connection of the PING))) attached to ADC_0  (ADC_0 pin 3)                              */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          PING))) sensor                                                                  */
/*                          Female header connector 3x1 RM 2.54mm                                           */
/*                          Wires                                                                           */
/*                          Male pin header connector 3x1 RM 2.54mm                                         */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/* 
   http://www.arduino.cc/en/Tutorial/Ping
*/   

// this constant won't change.  
const int pingPin = ADC_0;                  // name the pin

void setup() {
  Serial.begin(9600);                       // initialize serial communication
}

void loop()
{
  // initaialize variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);                 // set pin mode as OUTPUT
  digitalWrite(pingPin, LOW);               // ensure a low starting signal
  delayMicroseconds(2);                     // delay 2 us
  digitalWrite(pingPin, HIGH);              // generate a rising edge
  delayMicroseconds(5);                     // delay 5 us
  digitalWrite(pingPin, LOW);               // generate falling edge

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);                  // set pin as input
  duration = pulseIn(pingPin, HIGH);        // measure duration of pulse

  // convert the time into a distance
  inches = microsecondsToInches(duration);  // calculate distance in inches
  cm = microsecondsToCentimeters(duration); // calculate distance in cm
  
  Serial.print(inches);                     // print distance in inches
  Serial.print("in, ");                     // print string
  Serial.print(cm);                         // print distance in cm
  Serial.print("cm");                       // print string 
  Serial.println();                         // go to new line
  
  delay(100);                               // delay 100 ms
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

