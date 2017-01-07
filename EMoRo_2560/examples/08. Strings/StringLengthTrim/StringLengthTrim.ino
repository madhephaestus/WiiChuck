/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String length() and trim()                                                      */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to use length() and trim() in a String                                       */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringLengthTrim
*/

void setup() {
  Serial.begin(9600);                                         // Open serial communications and wait for port to open

  // send an intro:
  Serial.println("\n\nString  length() and trim():");         // print two new line characters (\n\n) and a constant string
  Serial.println();                                           // go to new line
}

void loop() {
  // here's a String with empty spaces at the end (called white space):
  String stringOne = "Hello!       ";                         // store constant string to StringOne variable
  Serial.print(stringOne);                                    // print stringOne
  Serial.print("<--- end of string. Length: ");               // print constant string
  Serial.println(stringOne.length());                         // print stringOne length and append newline
  
  stringOne.trim();                                           // trim the white space off the string
  Serial.print(stringOne);                                    // print stringOne
  Serial.print("<--- end of trimmed string. Length: ");       // print constant string
  Serial.println(stringOne.length());                         // print stringOne length and append newline

  while(true);                                                // do nothing while true
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

