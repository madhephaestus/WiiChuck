/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  For Loop Iteration                                                              */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Demonstrates the use of a for() loop. Lights multiple LEDs in sequence, then in reverse.      */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/* 
 http://www.arduino.cc/en/Tutorial/ForLoop
*/

int timer = 100;                                              // The higher the number, the slower the timing.

void setup() {
  // use a for loop to initialize each pin as an output:
  for (int thisPin = IO_0; thisPin <= IO_15; thisPin++)  {
    pinMode(thisPin, OUTPUT);                                 // initialize each pin as OUTPUT
    digitalWrite(thisPin, LOW);                               // turn off each LED
  }
}

void loop() {
  // loop from the lowest pin to the highest:
  for (int thisPin = IO_0; thisPin <= IO_15; thisPin++) { 
    digitalWrite(thisPin, HIGH);                              // turn the LED on
    delay(timer);                                             // wait for <timer> ms      
    digitalWrite(thisPin, LOW);                               // turn the LED off
  }

  // loop from the highest pin to the lowest:
  for (int thisPin = IO_15; thisPin >= IO_0; thisPin--) { 
    digitalWrite(thisPin, HIGH);                              // turn the LED on
    delay(timer);                                             // wait for <timer> ms
    digitalWrite(thisPin, LOW);                               // turn the LED off
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

