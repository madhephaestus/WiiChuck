/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String charAt() and setCharAt()                                                 */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to get and set characters of a String                                        */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringCharacters
*/

void setup() {
  Serial.begin(9600);                                                     // Open serial communications and wait for port to open
  Serial.println("\n\nString  charAt() and setCharAt():");                // print two new line characters (\n\n) and a constant string
  Serial.println();                                                       // go to new line
}

void loop() {
  // make a string to report a sensor reading:
  String reportString = "SensorReading: 456";                             // store constant string to reportString variable
  Serial.println(reportString);                                           // print string

  // the reading's most significant digit is at position 15 in the reportString:
  char mostSignificantDigit = reportString.charAt(15);                    // get character at position 15 in string - character number 16
  Serial.print("Most significant digit of the sensor reading is: ");      // print string
  Serial.println(mostSignificantDigit);                                   // print character from string

  Serial.println();                                                       // go to new line

  // you can alo set the character of a string. Change the : to a = character
  reportString.setCharAt(13, '=');                                        // change character at position 13 in reportString to '='
  Serial.println(reportString);                                           // print string
  
  while(true);                                                            // do nothing while true - forever
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

