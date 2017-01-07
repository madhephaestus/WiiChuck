/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String substring()                                                              */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to use substring in a String                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringSubstring
*/

void setup() {
  Serial.begin(9600);                               // Open serial communications and wait for port to open

  // send an intro:
  Serial.println("\n\nString  substring():");       // print two new line characters (\n\n) and a constant string
  Serial.println();                                 // go to new line
}

void loop() {
  // Set up a String:
  String stringOne = "Content-Type: text/html";     // store constant string to StringOne variable
  Serial.println(stringOne);                        // print stringOne

  if (stringOne.substring(19) == "html") {          // substring(index) looks for the substring from the index position to the end
    Serial.println("It's an html file");            // print result
  } 
  
  if (stringOne.substring(14,18) == "text") {       // you can also look for a substring in the middle of a string
    Serial.println("It's a text-based file");       // print result
  } 
  
  while(true);                                      // do nothing while true
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

