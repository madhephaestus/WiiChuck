/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Button                          			                                          */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Turns on and off a light emitting diode(LED) connected to digital pin IO_0, when pressing     */
/*            a pushbutton attached to pin IO_8.                                                            */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Push button switch (e.g. Push button, cable, pin header male 3x1 RM 2.54mm      */
/*                                              connect switch to pin 1 and pin 3 of IO_8 connector )       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = IO_8;               // initialize variable - number of pushbutton pin
const int ledPin =  IO_0;                 // initialize variable - number of  LED pin

// variables will change:
int buttonState = 0;                      // initialize variable - reading the pushbutton status

void setup() {
  pinMode(ledPin, OUTPUT);                // initialize the LED pin as an output:
  pinMode(buttonPin, INPUT_PULLUP);       // initialize the pushbutton pin as an input:
}

void loop() {
  buttonState = digitalRead(buttonPin);   // read the state of the pushbutton value

  if (buttonState == LOW) {               // if pushbutton is pressed
    digitalWrite(ledPin, HIGH);           // turn LED on
  }
  else {
    digitalWrite(ledPin, LOW);            // turn LED off
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
