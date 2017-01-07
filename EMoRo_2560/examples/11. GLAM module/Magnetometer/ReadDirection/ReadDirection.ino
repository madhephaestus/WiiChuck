/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ReadDirection                                 	      			                    */
/*      Board name: 		    EMoRo 2560 v3.2       						                                              */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows basic use of magnetometer (digital compass). It shows direction in degrees  */
/*            according to North Earth pole. 0 degrees -> North; 90 -> East; 180 -> South; and 270 -> West  */
/*            GLAM module use magnetometer HMC5883L. Magnetic field of Earth is different on different      */
/*            places, so it is necessary to make calibration of magnetometer before the first using of it.  */
/*            Calibration data are stored in EEPROM of EMoRo 2560 controller. Calibration can be run by     */
/*            resetting EMoRo 2560 controller and holding SW_2 while restarting the controller. After that  */
/*            instructions on LCD or console port should be followed.                                       */
/*            (Calibration has 3 steps, the first one is to lay down controller on horizontal surface, the  */
/*            second step is to rotate EMoRo 2560 controller for 180 degrees, and the last step is to flip  */
/*            controller upside down)                                                                       */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with HMC5883L Magnetometer                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){	                              

  Serial.begin(9600);		                                          // initialize serial communication			
  Serial.println("Example: ReadDirection");                       // send example name to console  
  
  if(ReadEmoroHardware() & MAG_AVAILABLE)				                  // if magnetometer is available
    Serial.println("Magnetometer / Compass is Available");        // print constant string		
  else
    Serial.println("Magnetometer / Compass is not Available");    // print constant string   

  if(ReadEmoroHardware() & LCD_AVAILABLE){		                    // if LCD is available
    Lcd.locate(0, 0);                                             // set LCD location to the first row, first column
    Lcd.print("Read Direction");				                          // print constant string
    if(Mag.testConnection() == 0){			                          // if magnetometer is available
      Lcd.locate(1, 0);					                                  // set LCD cursor position (row, column)
      Lcd.print("Available");			                                // print constant string
    }
  }
  
  delay(1500);                                                    // wait for 1500ms (1.5 sec)
}

void loop(){				

  if(ReadEmoroHardware() & MAG_AVAILABLE){			                  // if magnetometer is available     
    char buf[32];                                                 // initialize char array
    // get direction in degrees; 
    // 0 degrees -> North; 90 -> East; 180 -> South; and 270 -> West
    int direction = Mag.readDirection();		                      // read magnetometer direction in degrees

    // print direction on Serial monitor console:
    sprintf(buf, "Dir =%4d", direction);                          // form string
    Serial.println(buf);				                                  // print buf string

    // print direction on LCD if LCD available
    if(ReadEmoroHardware() & LCD_AVAILABLE){	                    // if LCD is available
      Lcd.locate(1, 0);				                                    // set LCD cursor position (row, column)
      Lcd.print(buf);					                                    // print buf string on lcd
    }
  }
  
  delay(300);			                                                // delay for 300ms (0.3 sec)		        
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

