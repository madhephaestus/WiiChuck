/************************************************************************************************************/
/*      Inovatic-ICT d.o.o			                                                     						            */
/*												                                                                                  */
/*      EMoRo Example:		  ReadSwitches                 			                                              */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows how to read status of switches SW_1 – SW_4 on GLAM module.                  */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with switches SW_1 to SW_4                                          */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){					

  Serial.begin(9600);		                                        // initialize serial communication			
  Serial.println("Example: ReadSwitches");	                    // send example name to console    
  
  if(ReadEmoroHardware() & SW_AVAILABLE)		                    // if switches are available
    Serial.println("Switches SW_1 to SW_4 available");		      // print constant string     
  else
    Serial.println("Switches SW_1 to SW_4 are not available");	// print constant string

  // if LCD available print info:
  if(ReadEmoroHardware() & LCD_AVAILABLE){		                  // if LCD ia available
    Lcd.locate(0, 0);                                           // set LCD cursor to the first row, the first character
    Lcd.print("ReadSwitches");                                  // print constant string
    Lcd.locate(1, 0);                                           // set LCD cursor to the second row, the first character
    if(ReadEmoroHardware() & SW_AVAILABLE)                      // if switches are available
      Lcd.print("SW_1-4 available");                            // print constant string
    else
      Lcd.print("SW not available");                            // print constant string
  }
  
  delay(1500);                                                  // wait for 1500ms (1.5 sec)
}

void loop(){					
 
  if(ReadEmoroHardware() & SW_AVAILABLE){		                    // if switches are available
    
    if(ReadSwitch(SW_1)==1){                                    // if Switch SW_1 is activated
      Serial.println("Switch SW_1 is activated");               // print constant string
      
      if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD is available
        Lcd.locate(1, 0);                                       // set LCD cursor to the second row, the first character
        Lcd.print("SW_1 activated");                            // print constant string
      }
    }
    else if(ReadSwitch(SW_2)==1){                               // if Switch SW_2 is activated
      Serial.println("Switch SW_2 is activated");               // print constant string
      
      if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD is available
        Lcd.locate(1, 0);                                       // set LCD cursor to the second row, the first character
        Lcd.print("SW_2 activated");                            // print constant string
      }
    }
    else if(ReadSwitch(SW_3)==1){                               // if Switch SW_3 is activated
      Serial.println("Switch SW_3 is activated");               // print constant string
      
      if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD is available
        Lcd.locate(1, 0);                                       // set LCD cursor to the second row, the first character
        Lcd.print("SW_3 activated");                            // print constant string
      }
    }
    else if(ReadSwitch(SW_4)==1){                               // if Switch SW_4 is activated
      Serial.println("Switch SW_4 is activated");               // print constant string
      
      if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD is available
        Lcd.locate(1, 0);                                       // set LCD cursor to the second row, the first character
        Lcd.print("SW_4 activated");                            // print constant string
      }
    }
    else{
      Serial.println("All Switches are deactivated");           // print constant string

      if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD is available
        Lcd.locate(1, 0);                                       // set LCD cursor to the second row, the first character
        Lcd.print("Waiting...      ");                          // print constant string
      }
    }
  }  
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

