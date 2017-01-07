/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ReadEmoroHardware();	        			                                            */
/*      Board name: 		    EMoRo 2560 v3.2	                                          					            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows how can be find out available options of GLAM module. According to attached */
/*            GLAM module, function ReadEmoroHardware(); will return available hardware options. Available  */
/*            hardware options of GLAM module are: Gyroscope L3GD20TR, Accelerometer BMA150, Compass        */
/*            HMC5883L, Bluetooth adapter, four switches keyboard (SW_1 to SW_4) and LCD 2x16 characters.   */
/*            Before executing setup(); and loop(); functions, all sketches will call function InitEmoro(); */
/*            (note: this is defined in .\cores\emoro_core\main.cpp) and this function will make the first  */
/*            initialization of GLAM module. While executing this initialization it is possible to reset    */
/*            Bluetooth settings by holding SW_1 while restarting the controller. Also it is possible to    */
/*            enter compass calibration by holding SW_2 while restarting the controller and following the   */
/*            instructions on LCD or Serial monitor. Any time by calling ReadEmoroHardware(); this function */
/*            will return available GLAM options which were available during booting the controller.        */
/*            Each time when EMoRo 2560 controller is turned on, input voltage will be checked. If DC Power */
/*            supply will be in range from Vin>=5V and Vin < 6.20V, users can hear SOS Morse code from      */
/*            buzzer and see battery low warning on LCD and Serial monitor console.                         */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module                                                                     */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(void){				    

  Serial.begin(9600);				                          // initialize serial communication at 9600 bps
  Serial.println("Example: ReadEmoroHardware();");    // send example name to console 

  // send available hardware
  Serial.print("EMoRo HW = ");			    
  // 0 bit - LCD , 1 bit - switches SW_1 to SW_4, 2 bit - Bluetooth, 3 bit Accelerometer,
  // 4 bit - gyroscope, 5 bit - magnetometer/compass
  // send available hardware in binary format
  Serial.println(ReadEmoroHardware(), BIN);           // print available hardware bit mask

  // test if LCD is available
  if(ReadEmoroHardware() & LCD_AVAILABLE){            // if LCD is available
    Serial.println("\nLCD is available.");            // print LCD is available
    // print available hardware in binary format on LCD
    Lcd.locate(0, 0);                                 // set LCD cursor position, the first row, first position
    Lcd.print("EMoRo HW =");                          // print text from current cursor position on LCD 
    Lcd.locate(1, 0);                                 // set cursor position, the second row, first position
    Lcd.print(ReadEmoroHardware(), BIN);              // print text from current cursor position on LCD
    delay(1500);                                      // wait for 1.5 sec
    Lcd.clear();                                      // clear LCD
    Lcd.locate(0, 0);                                 // set LCD cursor position, the first row, first position
    Lcd.print("Available:");                          // print text from current cursor position on LCD 
    Lcd.locate(1, 0);                                 // set cursor position, the second row, first position
    Lcd.print("LCD             ");                    // print text from current cursor position on LCD
    delay(1500);                                      // wait for 1.5 sec
  }
  else{                                               // if LCD isn't available
    Serial.println("\nLCD isn't available.");         // print LCD isn't available
  }

  // test if switches (SW_1, SW_2, SW_3, SW_4) are available 
  if(ReadEmoroHardware() & SW_AVAILABLE){             // if switches are available
    Serial.println("\nSwitches SW_1 to SW_4 are available."); // print switches are available
    if(ReadEmoroHardware() & LCD_AVAILABLE){          // if LCD is available
      Lcd.locate(1, 0);                               // set LCD cursor position, the second row, first position
      Lcd.print("SW_1 to SW_4    ");                  // print text from current cursor position on LCD
      delay(1500);                                    // wait for 1.5 sec
    }
  }
  else{                                               // if switches aren't available
    Serial.println("\nSwitches SW_1 to SW_4 are not available."); // print switches aren't available
  }

  // test if bluetooth is available
  if(ReadEmoroHardware() & BLUETOOTH_AVAILABLE){      // if bluetooth is available
    Serial.println("\nBluetooth is available.");      // print bluetooth is available
    if(ReadEmoroHardware() & LCD_AVAILABLE){          // if LCD is available
      Lcd.locate(1, 0);                               // set LCD cursor position, the second row, first position
      Lcd.print("Bluetooth       ");                  // print text from current cursor position on LCD
      delay(1500);                                    // wait for 1.5 sec
    }
  }
  else{                                               // if bluetooth isn't available
    Serial.println("\nBluetooth isn't available");    // print bluetooth isn't available
  }
  
  // test if accelerometer is available 
  if(ReadEmoroHardware() & ACC_AVAILABLE){            // if accelerometer is available
    Serial.println("\nAccelerometer is available.");  // print accelerometer is available
    if(ReadEmoroHardware() & LCD_AVAILABLE){          // if LCD is available
      Lcd.locate(1, 0);                               // set LCD cursor position, the second row, first position
      Lcd.print("Accelerometer  ");                   // print text from current cursor position on LCD
      delay(1500);                                    // wait for 1.5 sec 
    }
  }
  else{                                               // if accelerometer isn't available
    Serial.println("\nAccelerometer isn't available");// print accelerometer isn't available
  }

  // test if gyroscope is available 
  if(ReadEmoroHardware() & GYR_AVAILABLE){            // if gyroscope is available
    Serial.println("\nGyroscope is available.");      // print gyroscope is available
    if(ReadEmoroHardware() & LCD_AVAILABLE){          // if LCD is available
      Lcd.locate(1, 0);                               // set LCD cursor position, the second row, first position
      Lcd.print("Gyroscope       ");                  // print text from current cursor position on LCD
      delay(1500);                                    // wait for 1.5 sec
    }
  }
  else{                                               // if gyroscope isn't available
    Serial.println("\nGyroscope isn't available");    // print gyroscope isn't available
  }

  // test if magnetometer / compass is available 
  if(ReadEmoroHardware() & MAG_AVAILABLE){            // if magnetometer / compass is available
    Serial.println("\nMagnetometer/Compass is available."); // print magnetometer / compass is available
    if(ReadEmoroHardware() & LCD_AVAILABLE){          // if LCD is available
      Lcd.locate(1, 0);                               // set LCD cursor position, the second row, first position
      Lcd.print("Compass         ");                  // print text from current cursor position on LCD
      delay(1500);                                    // wait for 1.5 sec
    }
  } 
  else{                                               // if magnetometer / compass isn't available
    Serial.println("\nMagnetometer/Compass isn't available"); // print magnetometer/compass isn't available
  }
  
}

void loop(){				   
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

