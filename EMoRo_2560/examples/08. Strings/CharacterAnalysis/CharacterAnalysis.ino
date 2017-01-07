/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Character analysis operators                                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*             Examples using the character analysis operators. Send any byte and the sketch will           */
/*             tell you about it.                                                                           */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    

void setup() {
  Serial.begin(9600);                           // Open serial communications and wait for port to open:
 
  // send an intro:
  Serial.println("Send any byte and I'll tell you everything I can about it");
  Serial.println();                             // go to new line
}

void loop() {
  // get any incoming bytes:
  if (Serial.available() > 0) {                 // if there is data available in the serial buffer
    int thisChar = Serial.read();               // read the incoming data

    // say what was sent:
    Serial.print("You sent me: \'");            // print string
    Serial.write(thisChar);                     // print recieved character
    Serial.print("\'  ASCII Value: ");          // print string
    Serial.println(thisChar);                   // print ASCII value of recieved character

    // analyze what was sent:
    if(isAlphaNumeric(thisChar)) {              // if recieved data is alphanumeric
      Serial.println("it's alphanumeric");      // print string
    }   
    if(isAlpha(thisChar)) {                     // if recieved data is a letter
      Serial.println("it's alphabetic");        // print string
    }
    if(isAscii(thisChar)) {                     // if recieved data is in the ASCII values range
      Serial.println("it's ASCII");             // print string
    }
    if(isWhitespace(thisChar)) {                // if recieved data is a whitespace
      Serial.println("it's whitespace");        // print string
    }
    if(isControl(thisChar)) {                   // if recieved data is 
      Serial.println("it's a control character");//print string
    }
    if(isDigit(thisChar)) {                     // if recieved data is
      Serial.println("it's a numeric digit");   // print string
    }
    if(isGraph(thisChar)) {                     // if recieved data is
      Serial.println("it's a printable character that's not whitespace"); // print string
    }
    if(isLowerCase(thisChar)) {                 // if recieved data is
      Serial.println("it's lower case");        // print string
    }
    if(isPrintable(thisChar)) {                 // if recieved data is
      Serial.println("it's printable");         // print string
    }
    if(isPunct(thisChar)) {                     // if recieved data is
      Serial.println("it's punctuation");       // print string
    }
    if(isSpace(thisChar)) {                     // if recieved data is
      Serial.println("it's a space character"); // print string
    }
    if(isUpperCase(thisChar)) {                 // if recieved data is
      Serial.println("it's upper case");        // print string
    }
    if (isHexadecimalDigit(thisChar)) {         // if recieved data is
      Serial.println("it's a valid hexadecimaldigit (i.e. 0 - 9, a - F, or A - F)");// print string
    }

    // add some space and ask for another byte:
    Serial.println();                           // go to new line
    Serial.println("Give me another byte!");    // print string
    Serial.println();                           // go to new line
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

