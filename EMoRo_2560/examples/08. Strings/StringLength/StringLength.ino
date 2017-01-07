/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String length()                                                                 */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to use length() in a String. Open the Serial Monitor and start sending       */
/*             characters to see the results.                                                               */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/* 
 http://arduino.cc/en/Tutorial/StringLengthTrim
*/

String txtMsg = "";                         // initialize a string variable for incoming text
int lastStringLength = txtMsg.length();     // initialize a variable for storing previous length of the String

void setup() {
  Serial.begin(9600);                       // Open serial communications and wait for port to open

  // send an intro:
  Serial.println("\n\nString  length():");  // print two new line characters (\n\n) and a constant string
  Serial.println();                         // go to new line
}

void loop() { 
  while (Serial.available() > 0) {          // if there is avaialable data to read in the serial buffer
    char inChar = Serial.read();            // add any incoming characters to the String
    txtMsg += inChar;                       // append characters to form a message
  } 

  // print the message and a notice if it's changed:
  if (txtMsg.length() != lastStringLength) {  // if the message changed
    Serial.println(txtMsg);                   // print the message
    Serial.println(txtMsg.length());          // print the message length
   
    if (txtMsg.length() < 140) {              // if the string is longer than 140 characters
      Serial.println("That's a perfectly acceptable text message"); // print OK
    } 
    else {
      Serial.println("That's too long for a text message.");  // declare the message to be too long
    }
    
    lastStringLength = txtMsg.length();       // note the length for next time through the loop
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

