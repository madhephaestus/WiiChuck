/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ReadPowerSupply();						                                                  */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows usage of (float)ReadPowerSupply(); function. This function will measure     */
/*            input voltage on Power supply connector. If controller is supplied only from USB connection   */
/*            (without power supply on DC connector) this value will be 2-4.5V depending if Power switch    */
/*            is On or Off. ( In that situation, without main power supply, it is not possible to use       */
/*            power supply on SERVO_0 to SERVO_7 ports.)                                                    */
/*            Each time when EMoRo 2560 controller is turned on, input voltage will be checked. If DC Power */
/*            supply will be in range from Vin>=5V and Vin < 6.20V, users can hear SOS Morse code from      */
/*            buzzer and see battery low warning on LCD and Serial monitor console.                         */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){				   
  Serial.begin(9600);                               // initialize serial communication
  Serial.println("Example: ReadPowerSupply();");    // send example name to console
}

void loop(){		      		   
  // send measured input voltage 
  Serial.print("Power Supply = ");		              // print string
  Serial.print(ReadPowerSupply());		              // print read supply voltage
  Serial.println("V");                              // print voltage

  // test if LCD is available and print measured input
  // voltage on LCD
  if(ReadEmoroHardware() & LCD_AVAILABLE){          // if LCD is available
    Lcd.locate(0, 0);                               // set LCD cursor position, the first row, first position
    Lcd.print("U = ");                              // print text from current cursor position on LCD
    Lcd.print(ReadPowerSupply());                   // print text from current cursor position on LCD
    Lcd.print("V ");                                // print text from current cursor position on LCD
  }
  delay(500);					                              // wait for 500 ms
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

