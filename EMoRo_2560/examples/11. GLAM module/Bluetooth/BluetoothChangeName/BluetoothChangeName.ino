/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Bluetooth.changeName  	          	        		                                */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows usage of function Bluetooth.changeName(<new name>); which change Bluetooth  */
/*            name. New name will be visible for Bluetooth connection of EMoRo 2560 controller while        */
/*            pairing device will search for available Bluetooth devices.                                   */
/*            Any time it is possible to restore initial settings of Bluetooth name and password by         */
/*            resetting EMoRo 2560 controller and holding SW_1 while restarting the controller.             */
/*            Initial Bluetooth name is: "EMoRo 2560", initial password is: "0000".                         */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with Bluetooth option                                               */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

char new_name[16]="Arduino BT";                               // initialize variable - new name of Bluetooth device


void setup(){			                                            // initialize variable
  int res;

  Serial.begin(9600);                                         // initialize serial communication
  Serial.println("Example: Bluetooth.changeName");            // send example name to console

  if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE)               // if Bluetooth available
    Serial.println("Bluetooth Available");                    // print constant string
  else
    Serial.println("Bluetooth Not Available");                // print constant string

  res =	Bluetooth.changeName(new_name);                       // change Bluetooth name to new_name

  if(res == 0){                                               // if name changed successfully 
    Serial.print("Name successfully changed to: \"");         // print constant string
    Serial.print(new_name);                                   // print the Bluetooth's new name
    Serial.println("\"");                                     // print constant string
  }
  else
    Serial.println("Failed");                                 // print constant string

  if(ReadEmoroHardware() & LCD_AVAILABLE){                    // if LCD available print status 
    Lcd.locate(0, 0);                                         // set LCD cursor position (row, column)
    Lcd.print("changeName();");                               // print the Bluetooth's new name
    Lcd.locate(1, 0);                                         // set LCD cursor position (row, column)
    if(res == 0)                                              // if name changed successfully
      Lcd.print("Successfully");                              // print constant string
    else
      Lcd.print("Failed");                                    // print constant string
  }
}

void loop(){					
   
  if(Bluetooth.connection()){                                 // if Bluetooth connection is established with partner device
    Serial1.println("Hello from Bluetooth");                  // send a message via bluetooth
  }
  delay(500);                                                 // wait for 500ms
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

