/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Bluetooth.connection                  	   	                                    */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows usage of function Bluetooth.connection(); which test if the Bluetooth       */
/*            connection is established with pairing device and sends hello message to connected device.    */
/*            Initial Bluetooth name is: "EMoRo 2560", initial password is: "0000".                         */
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

  Serial.begin(9600);                                             // initialize serial communication
  Serial.println("Example: Bluetooth.connection");                // send example name to console

  if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE)                   // if Bluetooth is available
    Serial.println("Bluetooth Available");                        // print constant string
  else
    Serial.println("Bluetooth Not Available");                    // print constant string

  if(ReadEmoroHardware() & LCD_AVAILABLE){                        // if LCD is available
    Lcd.locate(0, 0);                                             // set LCD cursor position (row, column)
    Lcd.print("BT.connection();");                                // print constant string
    Lcd.locate(1, 0);                                             // set LCD cursor position (row, column)
    if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE)                 // if Bluetooth is available
      Lcd.print("BT Available");					                        // print constant string
    else
      Lcd.print("BT Not Available");                              // print constant string
  }
  delay(1500);                                                    // wait for 1500ms (1.5 sec)
}

void loop(){				
  static unsigned char last_bt_state = 0;                         // initialize variable - store last Bluetooth state

  if(Bluetooth.connection() != last_bt_state){                    // if bluetooth connection has changed the state
    last_bt_state = Bluetooth.connection();                       // store current Bluetooth connection status as last Bluetooth status
    if(last_bt_state)					                                    // print current connection state to Serial monitor 
      Serial.println("Bluetooth connection with paired device UP");// print constant string
    else
      Serial.println("Bluetooth connection Down");                // print constant string
		
    if(ReadEmoroHardware() & LCD_AVAILABLE){                      // if LCD available
      Lcd.locate(1, 0);                                           // set LCD cursor position (row, column)
      if(last_bt_state)                                           // if connection is established
        Lcd.print("UP              ");                            // print constant string
      else                        
        Lcd.print("Down            ");                            // print constant string
    }
  }

  if(Bluetooth.connection()){                                     // if bluetooth connection has established with pairing device
    Serial1.println("Hello from Bluetooth");                      // send message to paired bluetooth device
  }
  delay(500);                                                     // wait for 500ms (0.5 sec) 
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

