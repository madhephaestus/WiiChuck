/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ReadDirIncStr                                 	      			                    */
/*      Board name: 		    EMoRo 2560 v3.2       						                                              */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows available information from magnetometer (digital compass). GLAM module use  */
/*            magnetometer HMC5883L. This sensor measure magnetic field in x, y and z direction. According  */ 
/*            to that reading, direction, inclination and magnetic strength is calculated. Those data are   */
/*            available and in this sketch is shown how can those data be retrieved.                        */
/*            Magnetic field of Earth is different on different places, so it is necessary to make          */
/*            calibration of magnetometer before the first using of it. Calibration data are stored in      */
/*            EEPROM of EMoRo 2560 controller. Calibration can be run by resetting EMoRo 2560 controller    */
/*            and holding SW_2 while restarting the controller. After that instructions on LCD or console   */
/*            port should be followed. (Calibration has 3 steps, the first one is to lay down controller on */
/*            horizontal surface, the second step is to rotate EMoRo 2560 controller for 180 degrees, and   */
/*            the last step is to flip controller upside down)                                              */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with HMC5883L Magnetometer                                          */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){	                                

  Serial.begin(9600);				                                                // initialize serial communication	
  Serial.println("Example: Read Direction Incclination and Strength");      // send example name to console
  
  if(ReadEmoroHardware() & MAG_AVAILABLE)			                              // if magnetometer is available	
    Serial.println("Magnetometer / Compass is Available");                  // print constant string		
  else
    Serial.println("Magnetometer / Compass is not Available");              // print constant string

  
  if(ReadEmoroHardware() & LCD_AVAILABLE){		                              // if LCD is available 
    Lcd.locate(0, 0);                                                       // set LCD location to the first row, first column
    Lcd.print("ReadDirIncStr");	                                            // print constant string			
    if(Mag.testConnection() == 0){			                                    // if magnetomoter is available
      Lcd.locate(1, 0);					                                            // set LCD location to the second row, first column
      Lcd.print("Available");			                                          // print constant string
    }
  }
  
  delay(1500);                                                              // wait for 1500ms (1.5 sec)

  if(ReadEmoroHardware() & LCD_AVAILABLE){                                  // if LCD is available
    Lcd.clear();                                                            // clear LCD
    Lcd.locate(0, 0);                                                       // set LCD location to the first row, first column
    Lcd.print("Dir. Incl. Stre.");                                          // print constant string - header line
  }  
}

void loop(){				
  int dir, incl, stren, res;                                                // initialize variables

  if(ReadEmoroHardware() & MAG_AVAILABLE){			                            // if magnetometer is available        
    // get all three data at once
    // direction in degrees, inclination in degrees and magnetic strength in uT (micro Tesla); 
    // Direction: 0 degrees -> North; 
    //                   90 -> East
    //                  180 -> South
    //                  270 -> West
    res = Mag.read(&dir, &incl, &stren);	                                  // read magnetometer data

    // print direction (in degrees), inclination (in degrees) and strength (in uT) on Serial monitor console
    char buf[64];                                                           // initialize char array
    sprintf(buf, "Direction =%4d, Inclination =%4d, Strength =%4d", dir, incl, stren); // form string
    Serial.println(buf);				                                            // print constant string

    // example how to retrieve only one parameter:
    dir=Mag.readDirection();                                                // get direction in degrees
    incl=Mag.readInclination();                                             // get inclination in degrees
    stren=Mag.readStrength();                                               // get magnetic strength in uT (micro Tesla)
    
    // print data on LCD if LCD available:
    sprintf(buf, "%3d   %3d  %3d", dir, incl, stren);                       // form string
    if(ReadEmoroHardware() & LCD_AVAILABLE){	                              // if LCD is available
      Lcd.locate(1, 0);				                                              // set LCD location to the first row, first column
      Lcd.print(buf);					                                              // print buf string
    }
  }
  
  delay(300);		                                                            // delay for 300ms (0.3 sec)			        
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

