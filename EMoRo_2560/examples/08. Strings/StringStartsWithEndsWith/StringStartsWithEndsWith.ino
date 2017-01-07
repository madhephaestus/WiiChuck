/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  String startWith() and endsWith()                                               */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples of how to use startsWith() and endsWith() in a String                               */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://arduino.cc/en/Tutorial/StringStartsWithEndsWith
*/

void setup() {
  Serial.begin(9600);                                           // open serial communications and wait for port to open

  // send an intro:
  Serial.println("\n\nString startsWith() and endsWith():");    // print two new line characters (\n\n) and a constant string
  Serial.println();                                             // go to new line
}

void loop() {
  // startsWith() checks to see if a String starts with a particular substring:
  String stringOne = "HTTP/1.1 200 OK";                         // store constant string to StringOne variable
  Serial.println(stringOne);                                    // print stringOne
  if (stringOne.startsWith("HTTP/1.1")) {                       // check start substring
    Serial.println("Server's using http version 1.1");          // print result
  } 

  // you can also look for startsWith() at an offset position in the string:
  stringOne = "HTTP/1.1 200 OK";                                // store constant string to StringOne variable
  if (stringOne.startsWith("200 OK", 9)) {                      // check start substring
    Serial.println("Got an OK from the server");                // print result
  } 

  // endsWith() checks to see if a String ends with a particular character:
  String sensorReading = "sensor = ";                           // store constant string to sensorReading variable
  sensorReading += analogRead(ADC_0);                           // append analog input reading to sensorReading string
  Serial.print (sensorReading);                                 // print sensorReading string
  if (sensorReading.endsWith(0)) {                              // if the string ends with zero
    Serial.println(". This reading is divisible by ten");       // print result
  } 
  else {
    Serial.println(". This reading is not divisible by ten");   // print result

  }

  while(true);                                                  // do nothing while true
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

