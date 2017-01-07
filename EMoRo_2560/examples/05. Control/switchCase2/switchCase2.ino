/************************************************************************************************************/
/*      Inovatic-ICT d.o.o	                                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Switch statement  with serial input                                             */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Demonstrates the use of a switch statement.  The switch statement allows you to choose from   */
/*            among a set of discrete values of a variable.  It's like a series of if statements.           */
/*            To see this sketch in action, open the Serial monitor and send any character. The characters  */
/*            a, b, c, d, and e, will turn on LEDs.  Any other character will turn the LEDs off.            */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    
/*
 http://www.arduino.cc/en/Tutorial/SwitchCase2
*/

void setup() {
  Serial.begin(9600);                                       // initialize serial communication
  for (int thisPin = IO_0; thisPin <= IO_4; thisPin++) {    // iterate through all pins
    pinMode(thisPin, OUTPUT);                               // initialize each pin as OUTPUT
    digitalWrite(thisPin, LOW);                             // turn each LED off
  } 
}

void loop() {
  if (Serial.available() > 0) {                             // if data is recieved over serial communication
    int inByte = Serial.read();                             // read the recieved data
    
    // do something different depending on the character received.  
    // The switch statement expects single number values for each case;
    // in this exmaple, though, you're using single quotes to tell
    // the controller to get the ASCII value for the character.  For 
    // example 'a' = 97, 'b' = 98, and so forth:

    switch (inByte) {                                       // begin switch statement
    case 'a':                                               // in case 'a' is recieved
      digitalWrite(IO_0, HIGH);                             // turn on IO_0 LED
      break;                                                // break switch statement
    case 'b':                                               // in case 'b' is recieved
      digitalWrite(IO_1, HIGH);                             // turn on IO_1 LED
      break;                                                // break switch statement
    case 'c':                                               // in case 'c' is recieved
      digitalWrite(IO_2, HIGH);                             // turn on IO_2 LED
      break;                                                // break switch statement
    case 'd':                                               // in case 'd' is recieved
      digitalWrite(IO_3, HIGH);                             // turn on IO_3 LED
      break;                                                // break switch statement
    case 'e':                                               // in case 'e' is recieved
      digitalWrite(IO_4, HIGH);                             // turn on IO_4 LED
      break;                                                // break switch statement
    default:                                                // in case other data is recieved
      // turn all the LEDs off:
      for (int thisPin = IO_0; thisPin <= IO_4; thisPin++) {
        digitalWrite(thisPin, LOW);                         // turn each LED off
      }
    } 
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/


