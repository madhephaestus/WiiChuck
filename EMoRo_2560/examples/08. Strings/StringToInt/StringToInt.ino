/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String to Integer conversion                                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*              Reads a serial input string until it sees a newline, then converts the string to a number   */
/*              if the characters are digits.                                                               */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    

String inString = "";    // string to hold input

void setup() {
  Serial.begin(9600);                             // Open serial communications and wait for port to open
  
  // send an intro:
  Serial.println("\n\nString toInt():");          // print two new line characters (\n\n) and a constant string
  Serial.println();                               // go to new line
}

void loop() {
  // Read serial input:
  while (Serial.available() > 0) {                // while there is data available on the serial port
    int inChar = Serial.read();                   // read incoming character
    if (isDigit(inChar)) {                        // if recieved character is a digit
      // convert the incoming byte to a char 
      // and add it to the string:
      inString += (char)inChar;                   // add character to string
    }
    // if you get a newline, print the string,
    // then the string's value:
    if (inChar == '\n') {                         // detect new line 
      Serial.print("Value:");                     // print constant string
      Serial.println(inString.toInt());           // print value stored in string
      Serial.print("String: ");                   // print constant string
      Serial.println(inString);                   // print inString
      
      inString = "";                              // clear the string for new input
    }
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

