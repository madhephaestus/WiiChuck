/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String constructors                                                             */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how  to create strings from other data types                                     */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringConstructors
*/

void setup() {  
  Serial.begin(9600);                             // open serial communications and wait for port to open
  
  // send an intro:
  Serial.println("\n\nString Constructors:");     // print two new line characters (\n\n) and a constant string
  Serial.println();                               // go to new line
}

void loop() {
  // using a constant String:
  String stringOne = "Hello String";              // store constant string to StringOne variable
  Serial.println(stringOne);                      // prints "Hello String"

  // converting a constant char into a String:
  stringOne =  String('a');                       // store constant string to StringOne variable
  Serial.println(stringOne);                      // prints "a"

  // converting a constant string into a String object:
  String stringTwo =  String("This is a string"); // store constant string to StringTwo variable
  Serial.println(stringTwo);                      // prints "This is a string"

  // concatenating two strings:
  stringOne =  String(stringTwo + " with more");  // concatenate stringTwo and constant string and store the result in stringOne
  Serial.println(stringOne);                      // prints "This is a string with more"

  // using a constant integer:
  stringOne =  String(13);                        // store a integer number to stringOne
  Serial.println(stringOne);                      // prints "13"

  // using an int and a base:
  stringOne =  String(analogRead(ADC_1), DEC);    // store a integer number (result of ADC reading) to stringOne
  Serial.println(stringOne);                      // prints "453" or whatever the value of analogRead(ADC_1) is

  // using an int and a base (hexadecimal):
  stringOne =  String(45, HEX);                   // store a hex number to stringOne
  Serial.println(stringOne);                      // prints "2d", which is the hexadecimal version of decimal 45

  // using an int and a base (binary)
  stringOne =  String(255, BIN);                  // store a bin number to stringOne
  Serial.println(stringOne);                      // prints "11111111" which is the binary value of 255

  // using a long and a base:
  stringOne =  String(millis(), DEC);             // store a long integer number (result of millis function) to stringOne
  Serial.println(stringOne);                      // prints "123456" or whatever the value of millis() is

  while(true);                                    // do nothing while true
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

