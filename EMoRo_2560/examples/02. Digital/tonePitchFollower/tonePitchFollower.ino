/************************************************************************************************************/
/*      Inovatic-ICT d.o.o		                                                    							            */
/*												                                                                                  */
/*      EMoRo Example:		  Pitch follower                                                                  */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Plays a pitch that changes based on a changing analog input                                   */
/*            The pitch will be played on the builtin buzzer.                                               */  
/*      Circuit:                                                                                            */
/*            Photoresistor on analog ADC_0 (pin 3) to +5V (pin 2)                                          */
/*            4.7K resistor on analog ADC_0 (pin 3) to ground (pin 1)                                       */
/*            Photoresisor and resisor are connected to form a voltage divider.                             */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Resistor 4k7                                                                    */
/*                          Photoresistor                                                                   */
/*                          Connecting wires                                                                */
/*                          Pin header male 3x1 RM 2.54mm                                                   */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
 http://arduino.cc/en/Tutorial/Tone2
*/
 
void setup() {
  Serial.begin(9600);                                 // initialize serial communications (for debugging only)
}

void loop() {
  int sensorReading = analogRead(ADC_0);              // read the sensor:
  Serial.println(sensorReading);                      // print the sensor reading so you know its range
  if (sensorReading<400)
    sensorReading=400;
    
  // map the analog input range (in this case, 400 - 1024 from the photoresistor)
  // to the output pitch range (300 - 1500Hz)
  // change the minimum and maximum input numbers below
  // depending on the range your sensor's giving:
  int thisPitch = map(sensorReading, 400, 1023, 300, 1500);
  
  
  tone(BUZ_BUILTIN, thisPitch);                       // play the pitch
  delay(10);                                          // delay in between reads for stability
}   

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

