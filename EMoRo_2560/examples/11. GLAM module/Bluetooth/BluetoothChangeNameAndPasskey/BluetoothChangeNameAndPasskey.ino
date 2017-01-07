/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Bluetooth.changeNameAndPasskey                                                  */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This sketch shows usage of function Bluetooth.changeNameAndPasskey(<new name>, <new passkey>);*/
/*            which change Bluetooth name and passkey. New settings will be used for Bluetooth connection   */
/*            of EMoRo 2560 controller with pairing device.                                                 */
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

char new_name[16]="Arduino BT";                                   // initialize variable - new name of Bluetooth device
char new_passkey[16]="1234";                                      // initialize variable - new passkey

void setup(){                      
  int res;                                                        // initialize variable

  Serial.begin(9600);                                             // initialize serial communication
  Serial.println("Example: Bluetooth.changeNameAndPasskey");      // send example name to console

  if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE)                   // if Bluetooth available
    Serial.println("Bluetooth Available");                        // print constant string
  else
    Serial.println("Bluetooth Not Available");                    // print constant string

  res = Bluetooth.changeNameAndPasskey(new_name, new_passkey);    // change Bluetooth name to new_name and passkey to new_passkey

  if(res == 0){                                                   // if name and passkey changed successfully 
    // print status on Serial monitor:
    Serial.print("Name successfully changed to: \"");             // print constant string
    Serial.print(new_name);                                       // print Bluetooth's new name
    Serial.println("\"");                                         // print constant string
    Serial.print("Passkey successfully changed to: \"");          // print constant string
    Serial.print(new_passkey);                                    // print Bluetooth's new name
    Serial.println("\"");                                         // print constant string
  }
  else
    Serial.println("Failed");                                     // print constant string

  // print status on LCD:
  if(ReadEmoroHardware() & LCD_AVAILABLE){                        // if LCD is available
    Lcd.locate(0, 0);                                             // set LCD cursor position (row, column)
    Lcd.print("NameAndPasskey()");                                // print constant string
    Lcd.locate(1, 0);                                             // set LCD cursor position (row, column)
    if(res == 0)                                                  // if name and passkey changed successfully 
      Lcd.print("Successfully");                                  // print constant string
    else
      Lcd.print("Failed");                                        // print constant string
  }
}

void loop(){          
    
  if(Bluetooth.connection()){                                     // if Bluetooth connection is established with partner device
    Serial1.println("Hello from Bluetooth");                      // send a message via bluetooth
  }
  delay(500);                                                     // wait for 500ms
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

