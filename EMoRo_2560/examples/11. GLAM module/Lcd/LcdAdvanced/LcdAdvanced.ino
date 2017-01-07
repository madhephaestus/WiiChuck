/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  LcdAdvanced             						                                            */
/*      Board name: 		    EMoRo 2560 v3.2  		  				                                                  */
/*      MCU name:		        ATMEGA2560-16	   				                                                        */
/*      Clock:			        16 MHz					                                          	                    */
/*      File description:                                                                                   */
/*            This sketch shows advanced usage of functions related to LCD; LCD in GLAM module has two      */
/*            rows, 16 characters in each row.                                                              */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with LCD option                                                     */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
char my_name[16]="John";                                // initialize variable - hold my name

void setup(){				       

  Serial.begin(9600);                                   // initialize serial communication
  Serial.println("Example: LcdAdvanced");               // send example name to console

  if(ReadEmoroHardware() & LCD_AVAILABLE){              // if LCD is available
    Serial.println("Lcd Available");                    // send message to Serial monitor console
    
    Lcd.locate(0, 0);                                   // set cursor position on the first row, first character
    Lcd.print("LCD Advanced");                          // print predefined string

    delay(1500);                                        // wait for 1500ms (1.5sec)
    Lcd.clear();                                        // clear content of LCD
  }
  else
    Serial.println("Lcd Not Available");                // send message to Serial monitor console
}

void loop(){				
  int n;                                                // initialize variable - counter

  Lcd.locate(0,0);                                      // set LCD cursor position (row, column)
  
  Lcd.print("for n=0 to 10");                           // print constant string
  
  for(n=0;n<=10;n++){                                   // iterate n from 0 to 10
    Lcd.locate(1,0);                                    // set LCD cursor position (row, column)
    Lcd.print(n);                                       // print integer n
    delay(300);                                         // wait for 300ms
  }
  
  delay(1500);                                          // wait for 1500ms

  
  Lcd.clear();                                          // clear LCD
  Lcd.backlightOff();                                   // turn LCD backlight off
  delay(1500);                                          // wait for 1500ms
  Lcd.backlightOn();                                    // turn LCD backlight on

  // set LCD cursor position as the first row, the firs character place
  Lcd.locate(0,0);                                      // set LCD cursor position (row, column)
  
  Lcd.print("ASCII code ");                             // print constant string
  for(n=65;n<=90;n++){                                  // iterate n from 65 to 90
    Lcd.locate(0,12);                                   // set LCD cursor position as the first row, 13th character place
    Lcd.print(n);                                       // print integer n
    Lcd.locate(1,0);                                    // set LCD cursor position as the second row, 1st character place
    Lcd.print("is: ");                                  // print predefined string 
    Lcd.write(n);                                       // print ASCII character defined as ASCII code n
    delay(300);                                         // wait for 300ms
  }
  
  delay(1500);                                          // wait for 1500ms

  Lcd.clear();                                          // clear LCD
  Lcd.locate(0,0);                                      // set LCD cursor position on the first row, the firs character place
  Lcd.printChar('P');                                   // print single character
  
  char buf[16];                                         // initialize char array
  strcpy(buf, "I = ");                                  // copy constant string to character array
  Lcd.print(buf);                                       // print string variable
  Lcd.print(3.1415);                                    // print float (only two decimal places will appear)
  
  
  Lcd.locate(1,0);                                      // set LCD cursor position on the second row, 1st character place
  Lcd.printString("PI = ");                             // printString, the same as print

  float x=3.1415;                                       // initialize float variable 
  Lcd.print(x);                                         // print float from variable (only two decimal places will appear)
  
  delay(1500);                                          // wait for 1500ms
  
  Lcd.clear();                                          // clear LCD
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

