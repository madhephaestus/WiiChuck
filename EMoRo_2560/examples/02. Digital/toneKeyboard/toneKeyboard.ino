/************************************************************************************************************/
/*      Inovatic-ICT d.o.o				                                                    					            */
/*												                                                                                  */
/*      EMoRo Example:		  tone Keyboard                                                                   */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Plays a pitch on the builtin buzzer. The pitch changes based on a changing analog input       */  
/*      Circuit:                                                                                            */
/*            3 force-sensing resistors from +5V (pin 2) to analog in (pin 3) ADC_0, ADC_1 and ADC_2        */
/*            3 10K resistors from analog in (pin 3) ADC_0, ADC_1 and ADC_2 to ground (pin 1)               */
/*            Force-sensing resistors and resistors are connected to form a voltage divider.                */
/*            You can use photoresistors insted of force-sencing resistors.                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Resistor 10k ohm                3 pieces                                        */
/*                          Force-sensing resistors         3 pieces                                        */
/*                          Connecting wires                                                                */
/*                          Pin header male 3x1 RM 2.54mm   3 pieces                                        */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

#include "pitches.h"            // include wxternal file "pitches.h"

// constants wont change:
const int threshold = 30;       // minimum reading of the sensors that generates a note

// initialize array "notes"
int notes[] = {
  NOTE_A5, NOTE_B5,NOTE_C5 };   // notes to play, corresponding to the 3 sensors

void setup() {

}

void loop() {
  for (int thisSensor = 0; thisSensor < 3; thisSensor++) {    // swipe through sensors, using a locally declared variable thisSensor
    int sensorReading = analogRead(thisSensor);               // get a sensor reading
    if (sensorReading > threshold) {                          // if the sensor is pressed hard enough
      tone(BUZ_BUILTIN, notes[thisSensor], 20);               // play the note corresponding to this sensor
      delay(20);                                              // delay 20ms - minimum tone duration
    } 
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

