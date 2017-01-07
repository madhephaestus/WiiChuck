/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Replacing a character ob substring of a string                                  */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*             Examples of how to replace characters or substrings of a string                              */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://www.arduino.cc/en/Tutorial/StringReplace
 */


void setup() {
  Serial.begin(9600);                             // open serial communications and wait for port to open

  // send an intro:
  Serial.println("\n\nString  replace:\n");       // print two new line characters (\n\n) and a constant string
  Serial.println();                               // go to new line
}

void loop() { 
  String stringOne = "<html><head><body>";        // store constant string to StringOne variable
  Serial.println(stringOne);                      // print stringOne
  
  // replace() changes all instances of one substring with another:
  String stringTwo = stringOne;                   // copy stringOne into stringTwo
  stringTwo.replace("<", "</");                   // replace "<" with "</" in stringTwo
  Serial.println("Original string: " + stringOne);// print the original string
  Serial.println("Modified string: " + stringTwo);// print the modified string

  // you can also use replace() on single characters:
  String normalString = "bookkeeper";             // store constant string to StringOne variable
  Serial.println("normal: " + normalString);      // print string
  String leetString = normalString;               // initialize a string variable and copy content from normalString
  leetString.replace('o', '0');                   // replace all "o" with "0"
  leetString.replace('e', '3');                   // replace all "e" with "3" 
  Serial.println("l33tspeak: " + leetString);     // print new string

  while (true);                                   // do nothing while true
}
