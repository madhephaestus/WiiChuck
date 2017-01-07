/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Bluetooth.readNameAndPasskey                                                    */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This sketch shows usage of functions Bluetooth.readName(); and Bluetooth.readPasskey();.      */
/*            Those functions will read current Bluetooth name and passkey which can be used for pairing    */
/*            EMoRo 2560 controller with other Bluetooth device.                                            */
/*            Any time it is possible to restore initial settings of Bluetooth name and password by         */
/*            resetting EMoRo 2560 controller and holding SW_1 while restarting the controller.             */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with Bluetooth option                                               */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){					
  char bt_name[16];                                               // initialize variable - hold the Bluetooh's name
  char bt_passkey[16];                                            // initialize variable - hold the Bluetooh's passkey
  
  Serial.begin(9600);                                             // initialize serial communication
  Serial.println("Example: Bluetooth.readNameAndPasskey");        // send example name to console

  if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE){                  // if Bluetooth available print Bluetooth name and passkey
    strcpy(bt_name, Bluetooth.readName());                        // get bluetooth name

    Serial.println("Bluetooth is available.");                    // print constant string
    Serial.print("Bluetooth name: \"");                           // print constant string
    Serial.print(bt_name);                                        // print Bluetooth's name
    Serial.println("\"");                                         // print constant string

    strcpy(bt_passkey, Bluetooth.readPasskey());                  // get bluetooth passkey
    
    Serial.print("Bluetooth passkey: \"");                        // print constant string
    Serial.print(bt_passkey);                                     // print Bluetooth's name
    Serial.println("\"");                                         // print constant string

  }
  else
    Serial.println("Bluetooth is not available");                 // print constant string

  if(ReadEmoroHardware() & LCD_AVAILABLE){                        // if LCD is available
    Lcd.locate(0, 0);                                             // set LCD cursor position (row, column)
    if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE){                // if Bluetooth is available
      Lcd.print("U: ");                                           // print constant string
      Lcd.print(bt_name);                                         // print Bluetooth's name
    }
    else    
      Lcd.print("Not Available");                                 // print constant string
      
    Lcd.locate(1, 0);                                             // set LCD cursor position (row, column)
    if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE){                // if Bluetooth is available
      Lcd.print("P: ");                                           // print constant string
      Lcd.print(bt_passkey);                                      // print Bluetooth's passkey
    }
    else
      Lcd.print("Not Available");                                 // print constant string
  }
}

void loop(){					
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

