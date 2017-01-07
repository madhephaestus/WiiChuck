/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String Case changes                                                             */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to change the case of a string                                               */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringCaseChanges
*/

void setup() {
  Serial.begin(9600);                                 // Open serial communications and wait for port to open
  // send an intro:
  Serial.println("\n\nString  case changes:");        // print two new line characters (\n\n) and a constant string
  Serial.println();                                   // go to new line
}

void loop() {
  // toUpperCase() changes all letters to upper case:
  String stringOne = "<html><head><body>";            // store constant string to StringOne variable
  Serial.println(stringOne);                          // print stringOne and go to new line
  stringOne.toUpperCase();                            // change all letters in stringOne to upper case
  Serial.println(stringOne);                          // print stringOne and go to new line
  Serial.println();                                   // go to new line

  // toLowerCase() changes all letters to lower case:  
  String stringTwo = "</BODY></HTML>";                // store constant string to StringTwo variable
  Serial.println(stringTwo);                          // print stringTwo and go to new line
  stringTwo.toLowerCase();                            // change all letters in stringOne to lower case
  Serial.println(stringTwo);                          // print stringTwo and go to new line

  while(true);                                        // do nothing while true - forever
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

