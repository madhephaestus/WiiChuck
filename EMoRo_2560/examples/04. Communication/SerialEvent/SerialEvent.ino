/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Serial Event example                                                            */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            When new serial data arrives, this sketch adds it to a String. When a newline is received,    */
/*            the loop prints the string and clears it.                                                     */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/* 
 http://www.arduino.cc/en/Tutorial/SerialEvent
*/

String inputString = "";          // a string to hold incoming data
boolean stringComplete = false;   // whether the string is complete

void setup() {
  
  Serial.begin(9600);             // initialize serial:
  inputString.reserve(200);       // reserve 200 bytes for the inputString
}

void loop() {
  if (stringComplete) {           // if the string is complete
    Serial.println(inputString);  // print the string
    inputString = "";             // clear the string
    stringComplete = false;       // set stringComplete to false for next string
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {            // if data is available
    char inChar = (char)Serial.read();    // get the new byte
    inputString += inChar;                // add it to the inputString
    
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;              // set stringComplete to true
    } 
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/



