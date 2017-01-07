/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String indexOf() and lastIndexOf() functions                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to evaluate, look for, and replace characters in a String                    */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringIndexOf
*/

void setup() {
  Serial.begin(9600);                                                         // open serial communications and wait for port to open
 
  // send an intro:
  Serial.println("\n\nString indexOf() and lastIndexOf()  functions:");       // print two new line characters (\n\n) and a constant string
  Serial.println();                                                           // go to new line
}

void loop() {
  // indexOf() returns the position (i.e. index) of a particular character
  // in a string. For example, if you were parsing HTML tags, you could use it:
  String stringOne = "<HTML><HEAD><BODY>";                                    // store constant string to StringOne variable
  int firstClosingBracket = stringOne.indexOf('>');                           // finds the first occurence of ">" and return its position
  Serial.println("The index of > in the string " + stringOne + " is " + firstClosingBracket); // print result

  stringOne = "<HTML><HEAD><BODY>";                                           // store constant string to StringOne variable
  int secondOpeningBracket = firstClosingBracket + 1;                         // secondOpeningBracket can be at least one index after the first
  int secondClosingBracket = stringOne.indexOf('>', secondOpeningBracket );   // search for ">" after the first occurence
  Serial.println("The index of  the second > in the string " + stringOne + " is " + secondClosingBracket);  // print result

  // you can also use indexOf() to search for Strings:
  stringOne = "<HTML><HEAD><BODY>";                                           // store constant string to StringOne variable
  int bodyTag = stringOne.indexOf("<BODY>");                                  // finds the first occurence of ">" and return its position
  Serial.println("The index of the body tag in the string " + stringOne + " is " + bodyTag); // print result

  stringOne = "<UL><LI>item<LI>item<LI>item</UL>";                            // store constant string to StringOne variable
  int firstListItem = stringOne.indexOf("<LI>");                              // finds the first occurence of "<LI>" and return its position
  int secondListItem = stringOne.indexOf("item", firstListItem + 1 );         // search for "item" after the first occurence of "<LI>"
  Serial.println("The index of the second list item in the string " + stringOne + " is " + secondClosingBracket); // print result

  // lastIndexOf() gives you the last occurrence of a character or string:
  int lastOpeningBracket = stringOne.lastIndexOf('<');                        // finds the last occurence of "<" and return its position
  Serial.println("The index of the last < in the string " + stringOne + " is " + lastOpeningBracket); // print result

  int lastListItem  = stringOne.lastIndexOf("<LI>");                          // finds the last occurence of "<LI>" and return its position
  Serial.println("The index of the last list item in the string " + stringOne + " is " + lastListItem); // print result

  // lastIndexOf() can also search for a string:
  stringOne = "<p>Lorem ipsum dolor sit amet</p><p>Ipsem</p><p>Quod</p>";     // store constant string to StringOne variable
  int lastParagraph = stringOne.lastIndexOf("<p");                            // finds the last occurence of "<p" and return its position
  int secondLastGraf = stringOne.lastIndexOf("<p", lastParagraph - 1);        // finds the penultimate occurence of "<p" and return its position
  Serial.println("The index of the second last paragraph tag " + stringOne + " is " + secondLastGraf);  // print result

  while(true);                                                                // do nothing while true
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

