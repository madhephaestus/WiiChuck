/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      DigitalReadSerial                                                               */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            Reads a digital input on pin IO_8, prints the result to the serial monitor.                   */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Push button switch (e.g. Push button, cable, pin header male 3x1 RM 2.54mm      */
/*                                              connect switch to pin 1 and pin 3 of IO_8 connector )       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

int pushButton = IO_8;                        // initialize variable led (give the name "pushButton" to the pin IO_8)
int buttonState;                              // initialize variable "buttinState" to store the state of the button

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);                         // initialize serial communication at 9600 bits per second
  pinMode(pushButton, INPUT_PULLUP);          // make the pushbutton's pin an input with enabled pullup
}

// the loop routine runs over and over again forever:
void loop() {
  buttonState = digitalRead(pushButton);      // read the input pin
  Serial.println(buttonState);                // print out the state of the button
  delay(100);                                 // delay in between reads for stability
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/


