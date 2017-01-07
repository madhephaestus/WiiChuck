/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Comparing Strings                                                               */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to compare strings using the comparison operators                            */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringComparisonOperators
*/

String stringOne, stringTwo;

void setup() {
  Serial.begin(9600);                                         // Open serial communications and wait for port to open
  
  stringOne = String("this");                                 // store constant string to StringOne variable
  stringTwo = String("that");                                 // store constant string to StringTwo variable
  // send an intro:
  Serial.println("\n\nComparing Strings:");                   // print two new line characters (\n\n) and a constant string
  Serial.println();                                           // go to new line

}

void loop() {
  // if two strings are equal:
  if (stringOne == "this") {                                  // if stringOne equals "this"
    Serial.println("StringOne == \"this\"");                  // print string
  }
  // if two strings are not equal:
  if (stringOne != stringTwo) {                               // if stringOne is different than stringTwo
    Serial.println(stringOne + " =! " + stringTwo);           // print stringOne, then string " =! ", and finally stringTwo
  }

  // two strings not equal (case sensitivity matters):
  stringOne = "This";                                         // store constant string to StringOne variable
  stringTwo = "this";                                         // store constant string to StringTwo variable
  if (stringOne != stringTwo) {                               // if stringOne is different than stringTwo
    Serial.println(stringOne + " =! " + stringTwo);           // print stringOne, then string " =! ", and finally stringTwo
  }
  // you can also use equals() to see if two strings are the same:
  if (stringOne.equals(stringTwo)) {                          // if stringOne equals stringTwo
    Serial.println(stringOne + " equals " + stringTwo);       // print stringOne, then string " equals ", and finally stringTwo
  } 
  else {                                                      // if stringOne is not equal to stringTwo
    Serial.println(stringOne + " does not equal " + stringTwo);// print stringOne, then string " does not equal ", and finally stringTwo
  }

  // or perhaps you want to ignore case:
  if (stringOne.equalsIgnoreCase(stringTwo)) {                // if stringOne equals stringTwo (ignoring case)
    Serial.println(stringOne + " equals (ignoring case) " + stringTwo);// print stringOne, then string " equals (ignoring case) ", and finally stringTwo
  } 
  else {                                                      // if stringOne does not equal stringTwo (ignoring case)
    Serial.println(stringOne + " does not equal (ignoring case) " + stringTwo); // print stringOne, then string " does not equal (ignoring case) ", and finally stringTwo
  }

  // a numeric string compared to the number it represents:
  stringOne = "1";                                            // store constant string to StringOne variable
  int numberOne = 1;                                          // store number to integer variable
  if (stringOne.toInt() == numberOne) {                       // check if string representation of a number in stringOne variable equals to integer stored in numberOne variable
    Serial.println(stringOne + " = " + numberOne);            // print stringOne, then string " = ", and finally numberOne
  }



  // two numeric strings compared:
  stringOne = "2";                                            // store constant string to StringOne variable
  stringTwo = "1";                                            // store constant string to StringTwo variable
  if (stringOne >= stringTwo) {                               // check if string representation of a number in stringOne variable is greater or equal to integer stored in numberOne variable
    Serial.println(stringOne + " >= " + stringTwo);           // print stringOne, then string " >= ", and finally numberOne
  }

  // comparison operators can be used to compare strings for alphabetic sorting too:
  stringOne = String("Brown");                                // store constant string to StringOne variable
  if (stringOne < "Charles") {                                // if StringOne is before "Charles" in alphabetical order
    Serial.println(stringOne + " < Charles");                 // print stringOne, then string " < Charles"
  }

  if (stringOne > "Adams") {                                  // if StringOne is after "Adams" in alphabetical order
    Serial.println(stringOne + " > Adams");                   // print stringOne, then string " > Adams"
  }

  if (stringOne <= "Browne") {                                // if StringOne is before of equals "Browne" in alphabetical order
    Serial.println(stringOne + " <= Browne");                 // print stringOne, then string " <= Browne"
  }

  if (stringOne >= "Brow") {                                  // if StringOne is after of equals "Brow" in alphabetical order
    Serial.println(stringOne + " >= Brow");                   // print stringOne, then string " >= Brow"
  }

  // the compareTo() operator also allows you to compare strings
  // it evaluates on the first character that's different.
  // if the first character of the string you're comparing to
  // comes first in alphanumeric order, then compareTo() is greater than 0:
  stringOne = "Cucumber";                                     // store constant string to stringOne variable
  stringTwo = "Cucuracha";                                    // store constant string to stringTwo variable
  if (stringOne.compareTo(stringTwo) < 0 ) {                  // compare stringTwo to stringOne
    Serial.println(stringOne + " comes before " + stringTwo); // print result
  } 
  else {
    Serial.println(stringOne + " comes after " + stringTwo);  // print result
  }

  delay(10);                                                  // delay 10 s


  // compareTo() is handy when you've got strings with numbers in them too:
  while (true) {
    stringOne = "Sensor: ";                                   // store constant string to stringOne variable
    stringTwo= "Sensor: ";                                    // store constant string to stringTwo variable

    stringOne += analogRead(ADC_0);                           // read ADC_0 analog input and append measurment to stringOne
    stringTwo += analogRead(ADC_1);                           // read ADC_1 analog input and append measurment to stringOne

    if (stringOne.compareTo(stringTwo) < 0 ) {                // compare stringTwo to stringOne - by comparing strings wa actually compare analog measurments!
      Serial.println(stringOne + " comes before " + stringTwo); // print result
    } 
    else {
      Serial.println(stringOne + " comes after " + stringTwo); // print result

    }
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

