/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Appending to Strings using the += operator and concat()                         */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to append different data types to strings                                    */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringAppendOperator
 */
 
String stringOne, stringTwo;                    // initialize two string variables

void setup() {
  Serial.begin(9600);                           // open serial communications and wait for port to open
  stringOne = String("Sensor ");                // store constant string to StringOne variable
  stringTwo = String("value");                  // store constant string to StringTwo variable
  // send an intro:
  Serial.println("\n\nAppending to a string:"); // print two new line characters (\n\n) and a constant string
  Serial.println();                             // go to new line
}

void loop() {
  Serial.println(stringOne);                    // prints  "Sensor "

  // adding a string to a string:
  stringOne += stringTwo;                       // add stringTwo to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value"

  // adding a constant string to a string:
  stringOne += " for input ";                   // add string " for input " to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value for input"

  // adding a constant character to a string:
  stringOne += 'A';                             // add character 'A' to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value for input A"

  // adding a constant string to a string:
  stringOne += "DC_";                           // add string "DC_" to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value for input ADC_"
  
  // adding a constant integer to a string:
  stringOne += 0;                               // add integer number 0 to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value for input ADC_0"

  // adding a constant string to a string:
  stringOne += ": ";                            // add string ": " to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value for input ADC_0:"

  // adding a variable integer to a string:
  stringOne += analogRead(ADC_0);               // add integer value read form analogRead function to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "Sensor value for input ADC_0: 456" or whatever analogRead(ADC_0) gave

  // change the string values:
  Serial.println("\n\nChanging the Strings values"); // print two new line characters (\n\n) and a constant string
  stringOne = "A long integer: ";               // store constant string to StringOne variable
  stringTwo = "The millis(): ";                 // store constant string to StringTwo variable

  // adding a constant long integer to a string:
  stringOne += 123456789;                       // add a constant long integer to stringOne and store the result to stringOne
  Serial.println(stringOne);                    // prints "A long integer: 123456789"

  // using concat() to add a long variable to a string:
  stringTwo.concat(millis());                   // add a long variable value got from millis function to stringTwo
  Serial.println(stringTwo);                    // prints "The millis(): 43534" or whatever the value of the millis() is

  while(true);                                  // do nothing while true - forever
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/


