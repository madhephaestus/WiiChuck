/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ASCII table                                                                     */
/*      Board name: 		    EMoRo 2560 v3.2	                                          					            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Prints out byte values in all possible formats:                                               */
/*             * as raw binary values                                                                       */
/*             * as ASCII-encoded decimal, hex, octal, and binary values                                    */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup() { 
  Serial.begin(9600);                                // initialize serial and wait for port to open: 
  Serial.println("ASCII Table ~ Character Map");     // prints title with ending line break
} 

// first visible ASCIIcharacter '!' is number 33:
int thisByte = 33;                                    // set variable thisByte to value 33 which represents "!"
// you can also write ASCII characters in single quotes.
// for example. '!' is the same as 33, so you could also use this:
//int thisByte = '!';  

void loop() { 
  // prints value unaltered, i.e. the raw binary version of the 
  // byte. The serial monitor interprets all bytes as 
  // ASCII, so 33, the first number,  will show up as '!' 
  Serial.write(thisByte);                             // send ASCII character over serial communication    

  Serial.print(", dec: ");                            // send string ", dec: " over serial communication
  
  // prints value as string as an ASCII-encoded decimal (base 10).
  // Decimal is the  default format for Serial.print() and Serial.println(),
  // so no modifier is needed:
  Serial.print(thisByte);                             // send the number thisByte as string over serial communication
  // But you can declare the modifier for decimal if you want to.
  //this also works if you uncomment it:

  // Serial.print(thisByte, DEC);  


  Serial.print(", hex: ");                            // send string ", hex: " over serial communication
  Serial.print(thisByte, HEX);                        // prints value as string in hexadecimal (base 16)

  Serial.print(", oct: ");                            // send string ", oct: " over serial communication
  Serial.print(thisByte, OCT);                        // prints value as string in octal (base 8)

  Serial.print(", bin: ");                            // send string ", bin: " over serial communication
  Serial.println(thisByte, BIN);                      // prints value as string in binary (base 2), also prints ending line break 

   
  if(thisByte == 126) {                               // if printed last visible character '~' or 126, stop; you could also use if (thisByte == '~') {
    while(true) { 
      continue; 
    } 
  } 
 
  thisByte++;                                         // go on to the next character
} 

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

