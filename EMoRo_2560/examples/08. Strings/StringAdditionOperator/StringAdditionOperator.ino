/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Adding Strings together                                                         */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to add strings together. You can also add several different data types to    */
/*             string, as shown here.                                                                       */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringAdditionOperator
*/

// declare three strings:
String stringOne, stringTwo, stringThree;

void setup() {
  Serial.begin(9600);                       // initialize serial and wait for port to open
 
  stringOne = String("stringThree = ");     // store constant string to StringOne variable
  stringTwo = String("this string");        // store constant string to StringTwo variable
  stringThree = String ();                  // store NULL in StringThree variable
  // send an intro:
  Serial.println("\n\nAdding strings together (concatenation):"); // print two new line characters (\n\n) and a constant string
  Serial.println();                         // go to new line
}

void loop() {
  // adding a constant integer to a string:
  stringThree =  stringOne + 123;           // add a integer to a string
  Serial.println(stringThree);              // prints "stringThree = 123"

  // adding a constant long interger to a string:
  stringThree = stringOne + 123456789;      // add a long integer to a string
  Serial.println(stringThree);              // prints "stringThree = 123"

  // adding a constant character to a string:
  stringThree =  stringOne + 'A';           // add character to a string
  Serial.println(stringThree);              // prints "stringThree = A"

  // adding a constant string to a string:
  stringThree =  stringOne +  "abc";        // add string to a string
  Serial.println(stringThree);              // prints "stringThree = abc"

  stringThree = stringOne + stringTwo;      // add two strings
  Serial.println(stringThree);              // prints stringOne and StringTwo concatenated - "stringThree = this string"

  // adding a variable integer to a string:
  int sensorValue = analogRead(ADC_1);      // read analog value of ADC_1 analog input and store it to a local variable
  stringOne = "Sensor value: ";             // store constant string to StringOne variable
  stringThree = stringOne  + sensorValue;   // print stringOne followed by sensor value
  Serial.println(stringThree);              // prints "Sensor Value: 600" or whatever value analogRead(ADC_1) gave

  // adding a variable long integer to a string:
  stringOne="millis() value: ";             // store constant string to StringOne variable
  stringThree = stringOne + millis();       // add current time to stringOne and store it in stringThree
  Serial.println(stringThree);              // prints "millis() value: 345345" or whatever value millis() gave

  while(true);                              // do nothing while true - forever
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
