/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Arrays                                                                          */
/*      Board name: 		    EMoRo 2560 v3.2				                                                          */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Demonstrates the use of  an array to hold pin numbers in order to iterate over the pins in    */
/*            a sequence. Lights multiple LEDs in sequence, then in reverse. Unlike the For Loop tutorial,  */
/*            where the pins have to be contiguous, here the pins can be in any random order.               */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
 http://www.arduino.cc/en/Tutorial/Array
 */

int timer = 150;                                                // the higher the number, the slower the timing

// an array of pin numbers to which LEDs are attached
int ledPins[] = { 
  IO_0, IO_8, 
  IO_1, IO_9, 
  IO_2, IO_10,
  IO_3, IO_11,
  IO_4, IO_12,
  IO_5, IO_13,
  IO_6, IO_14,
  IO_7, IO_15,
};     
int pinCount = 16;                                              // the number of pins (i.e. the length of the array)

void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {
    pinMode(ledPins[thisPin], OUTPUT);                          // initialize each pin as OUTPUT
    digitalWrite(ledPins[thisPin], LOW);                        // turn off LEDs at each pin
  }
}

void loop() {
  // loop from the lowest pin to the highest:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) { 
    digitalWrite(ledPins[thisPin], HIGH);                       // turn the LED on
    delay(timer);                                               // delay for <timer> ms
    digitalWrite(ledPins[thisPin], LOW);                        // turn the LED off

  }

  // loop from the highest pin to the lowest:
  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) {   
    digitalWrite(ledPins[thisPin], HIGH);                       // turn the LED on
    delay(timer);                                               // delay for <timer> ms
    digitalWrite(ledPins[thisPin], LOW);                        // turn the LED off
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

