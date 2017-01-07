/************************************************************************************************************/
/*      Inovatic-ICT d.o.o						                                                    			            */
/*												                                                                                  */
/*      EMoRo Example:		  Smoothing                                                                       */
/*      Board name: 		    EMoRo 2560 v3.2				                                          		            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Reads repeatedly from an analog input, calculating a running average and printing it to the   */
/*            computer.  Keeps ten readings in an array and continually averages them.                      */
/*      The circuit:                                                                                        */
/*            Attach the center pin of a 10k potentiometer to pin 3 (the right most pin) of ADC_0 connector */
/*            and the outside pins of a potentiometer to pin 1 (GND, the left most pin - marked with a      */
/*            small triangle) and pin 2 (+5V, the center pin) of ADC_0 connector.                           */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Analog signal source (e.g. 10k potentiometer, cable,                            */
/*                                                pin header male 3x1 RM 2.54mm)                            */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
 http://www.arduino.cc/en/Tutorial/Smoothing
*/

// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 10;     // use 10 samples

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the calculated average

int inputPin = ADC_0;           // name the ADC_0 pin

void setup()
{
  Serial.begin(9600);           // initialize serial communication with computer       
  
  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;          
}

void loop() {
  total= total - readings[index];                       // subtract the last reading  
  readings[index] = analogRead(inputPin);               // read from the sensor
  total= total + readings[index];                       // add the reading to the total
  index = index + 1;                                    // advance to the next position in the array

  if (index >= numReadings)                             // if we're at the end of the array...           
    index = 0;                                          // ...wrap around to the beginning
    
  average = total / numReadings;                        // calculate the average
  
  Serial.println(average);                              // send it to the computer as ASCII digits
  
  delay(1);                                             // delay in between reads for stability            
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

