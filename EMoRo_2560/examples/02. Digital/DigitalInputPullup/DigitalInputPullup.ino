/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Input Pullup Serial                          			                              */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a digital input on       */
/*            pin IO_8 and prints the results to the serial monitor. Unlike pinMode(INPUT), there is no     */
/*            pull-down resistor necessary. An internal 20k-50k ohm resistor is pulled to 5V. This          */
/*            configuration causes the input to read HIGH when the switch is open, and LOW when             */
/*            it is closed. The IO_0 LED will be turned on while the pushbutton on IO_8 is pressed.         */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Push button switch (e.g. Push button, cable, pin header male 3x1 RM 2.54mm      */
/*                                              connect switch to pin 1 and pin 3 of IO_8 connector )       */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){
  Serial.begin(9600);                   // initialize serial communication at 9600 bits per second
  pinMode(IO_8, INPUT_PULLUP);          // configure pin IO_8 as an input and enable the internal pull-up resistor
  pinMode(IO_0, OUTPUT);                // configure IO_0 as output

}

void loop(){
  int sensorVal = digitalRead(IO_8);    // read the pushbutton value into a local variable
  Serial.println(sensorVal);            // print out the value of the pushbutton
  
  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on led IO_0 when the 
  // button's pressed, and off when it's not:
  
  if (sensorVal == LOW) {               // if pushbutton is pressed
    digitalWrite(IO_0, HIGH);           // turn the LED on
  } 
  else {                                // else
    digitalWrite(IO_0, LOW);            // turn the LED off
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

