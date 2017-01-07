/************************************************************************************************************/
/*      Inovatic-ICT d.o.o	                                                     								            */
/*												                                                                                  */
/*      EMoRo Example:		  State change detection (edge detection)                                         */
/*      Board name: 		    EMoRo 2560 v3.2  					                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Often, you don't need to know the state of a digital input all the time, but you just need    */
/*            to know when the input changes from one state to another. For example, you want to know when  */
/*            a button goes from OFF to ON.  This is called state change detection, or edge detection.      */
/*            This example shows how to detect when a button or button changes from OFF to ON               */
/*            and ON to OFF.                                                                                */  
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Push button switch (e.g. Push button, cable, pin header male 3x1 RM 2.54mm      */
/*                                              connect switch to pin 1 and pin 3 of IO_8 connector )       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// Constants won't change:
const int  buttonPin = IO_8;    // initialize constant "buttonPin" - the pin that the pushbutton is attached to
const int ledPin = IO_0;        // initialize constant "ledPin" - the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;      // initialize variable "buttonPushCounter" - counter for the number of button presses
int buttonState = HIGH;         // initialize variable "buttonState" - current state of the button
int lastButtonState = HIGH;     // initialize variable "lastButtonState" - previous state of the button

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);       // initialize the button pin as a input
  pinMode(ledPin, OUTPUT);                // initialize the LED as an output
  digitalWrite(ledPin, HIGH);             // turn the LED on
  Serial.begin(9600);                     // initialize serial communication
}


void loop() {
 
  buttonState = digitalRead(buttonPin);                 // read the pushbutton input pin:

  if (buttonState != lastButtonState) {                 // compare the buttonState to its previous state
    if (buttonState == LOW) {                           // if the current state is LOW then the button went from off to on:
      buttonPushCounter++;                              // if the state has changed, increment the counter
      Serial.println("on");                             // print "on" via serial communication and append newline
      Serial.print("number of button pushes:  ");       // print "number of button pushes:  " string via serial communication
      Serial.println(buttonPushCounter);                // print the total number of button changes via serial communication and append newline
    } 
    else {                                              // if the current state is HIGH then the button went from on to off
      Serial.println("off");                            // print "off" via serial communication and append newline
    }
  }
   
  lastButtonState = buttonState;                        // save the current state as the last state, for next time through the loop

  // turns on the LED every four button pushes by 
  // checking the modulo of the button push counter.
  // the modulo function gives you the remainder of 
  // the division of two numbers:
  if (buttonPushCounter % 4 == 0) {                     // if the pushButton has been pressed for 4,8,12,16 etc. times
    digitalWrite(ledPin, HIGH);                         // turn the LED on
  } else {                                              // else
   digitalWrite(ledPin, LOW);                           // turn the LED off
  }
  
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
