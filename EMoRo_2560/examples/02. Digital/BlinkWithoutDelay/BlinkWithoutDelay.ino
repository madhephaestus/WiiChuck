/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Blink without Delay                   			                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Turns on and off a light emitting diode (LED) connected to a digital pin IO_0, without using  */
/*            the delay() function.  This means that other code can run at the same time without being      */
/*            interrupted by the LED code.                                                                  */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// Constants won't change - used here to set pin numbers:
const int ledPin =  IO_0;       // initialize constant "ledPin" - the number of the LED pin

// Variables will change:
int ledState = HIGH;            // initialize variable "ledState" - used to set the LED on or off
long previousMillis = 0;        // initialize variable "previousMillis" - store last time LED was updated

// the following variable is of type long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // initialize variable "interval" - interval at which to blink (milliseconds)

void setup() {
  pinMode(ledPin, OUTPUT);      // set the digital pin as output
}

void loop()
{
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long currentMillis = millis();             //store current ime
 
  if(currentMillis - previousMillis > interval) {     // if the amount of time that pased is grater then the set interval
    previousMillis = currentMillis;                   // save the last time you blinked the LED as current time

    if (ledState == LOW)                              // if the LED is on turn it off and vice-versa:
      ledState = HIGH;
    else
      ledState = LOW;

    digitalWrite(ledPin, ledState);                   // set the LED to the value of the variable ledState
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
