/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  LcdSimple             						                                              */
/*      Board name: 		    EMoRo 2560 v3.2  		  				                                                  */
/*      MCU name:		        ATMEGA2560-16	   				                                                        */
/*      Clock:			        16 MHz					                                          	                    */
/*      File description:                                                                                   */
/*            This sketch shows simple usage of Lcd.print(<something to print>); LCD in GLAM module has two */
/*            rows, 16 characters in each row. Lcd.locate(<row>,<column position>); is used for defining    */
/*            starting position of Lcd.print(<something>);                                                  */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with LCD option                                                     */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
char my_name[16]="John";                    // initialize variable - hold my name

void setup(){				       

  Serial.begin(9600);                       // initialize serial communication:
  Serial.println("Example: LcdSimple");     // send example name to console

  if(ReadEmoroHardware() & LCD_AVAILABLE){  // if LCD is available
    Serial.println("Lcd Available");        // send message to Serial monitor console 

    Lcd.locate(0, 0);                       // set LCD cursor position (row, column)
    Lcd.print("My name:");                  // print constant string
    // set cursor position on the second row, forth character (the first character position is defined as 0, 
    // the second character position is defined as 1 ...)
    Lcd.locate(1, 3);                       // set LCD cursor position (row, column)
    Lcd.print(my_name);                     // print my_name
  }
  else
    Serial.println("Lcd Not Available");    // print my_name

}

void loop(){					
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

