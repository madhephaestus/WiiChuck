/************************************************************************************************************/
/*      Inovatic-ICT d.o.o	                                                    								            */
/*												                                                                                  */
/*      EMoRo Example:		  Debounce                          			                                        */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Each time the input pin goes from HIGH to LOW (e.g. because of a push-button press), the      */
/*            output pin is toggled from LOW to HIGH or HIGH to LOW.  There's a minimum delay between       */
/*            toggles to debounce the circuit (i.e. to ignore noise).                                       */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Push button switch (e.g. Push button, cable, pin header male 3x1 RM 2.54mm      */
/*                                              connect switch to pin 1 and pin 3 of IO_8 connector )       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// Constants won't change - they're used here to set pin numbers:
const int buttonPin = IO_8;               // initialize constant "buttonPin" - the number of the pushbutton pin
const int ledPin = IO_0;                  // initialize constant "ledPin" - the number of the LED pin

// Variables will change:
int ledState = LOW;                       // initialize variable "ledState" - store the current state of the output pin
int buttonState;                          // initialize variable "buttonState" - store the current reading from the input pin
int lastButtonState = HIGH;               // initialize variable "lastButtonState" - store the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int
long lastDebounceTime = 0;                // initialize variable "lastDebounceTime" - store the last time the output pin was toggled
long debounceDelay = 50;                  // initialize variable "debounceDelay" - store the debounce time; increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);       // make the pushbutton's pin an input with enabled pullup
  pinMode(ledPin, OUTPUT);                // initialize the LED pin as digital output pin

  digitalWrite(ledPin, ledState);         // set initial LED state
}

void loop() {

  int reading = digitalRead(buttonPin);   // read the state of the switch into a local variable:

  // check to see if you just pressed the button 
  // (i.e. the input went from HIGH to LOW),  and you've waited 
  // long enough since the last press to ignore any noise:  

  if (reading != lastButtonState) {       // if the switch changed, due to noise or pressing
    lastDebounceTime = millis();          // reset the debouncing timer
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {  // if the amount of time that passed is greater than the set debounceDelay then

    if (reading != buttonState) {         // if the button state has changed
      buttonState = reading;              // store new button state

      if (buttonState == LOW) {           // only toggle the LED if the new button state is LOW - if the button is pressed
        ledState = !ledState;
      }
    }
  }
  
  digitalWrite(ledPin, ledState);         // set the LED state

  // save the reading  
  // next time through the loop it will be the lastButtonState:
  lastButtonState = reading;
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
