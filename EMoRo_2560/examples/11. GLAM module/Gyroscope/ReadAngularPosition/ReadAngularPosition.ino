/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ReadAngularPosition 			                                                      */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Gyroscope L3GD20 which is used in GLAM module can be used on EMoRo 2560 controller in two     */
/*            modes, Basic and Advanced. In Advance mode it is possible to get automatically calculated     */
/*            angular position. Gyroscope L3GD20 in advanced mode will use FIFO buffering, filtering data,  */
/*            automatic calibration, automatic angle axis calculations in the background process. Because   */
/*            of that, it is very easy to get angular position. Before reading angular position it is       */
/*            important to initialize gyroscope with function Gyr.init(); while gyroscope is in steady      */
/*            position without moving. To stop using gyroscope in Advance mode (stop background process of  */
/*            automatically angular position calculations) use Gyr.stop();                                  */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with Gyroscope option                                               */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
void setup(){         
  int res;
  
  Serial.begin(9600);                                                 // initialize serial communication
  Serial.println("Example: ReadAngularPosition");                     // send example name to console

  if(ReadEmoroHardware() & GYR_AVAILABLE){                            // if Gyroscope is available
    Serial.println("Gyroscope is available.");                        // print constant string
    res=Gyr.init();                                                   // initialize gyroscope in Advanced mode
    if(res==0)                                                        // if gyroscope initialized OK
      Serial.println("Gyroscope is in Advanced mode.");               // print constant string
    else
      Serial.println("Gyroscope isn't in Advanced mode.");            // print constant string
  }
  else  
    Serial.println("Gyroscope isn't available.");                     // print constant string

  if(ReadEmoroHardware() & LCD_AVAILABLE){                            // if LCD available
    Lcd.locate(0, 0);                                                 // set LCD cursor position (row, column)
    Lcd.print("Gyroscope");                                           // print constant string
    Lcd.locate(1, 0);                                                 // set LCD cursor position (row, column)
    if(ReadEmoroHardware() & GYR_AVAILABLE)                           // if gyroscope is available
      Lcd.print("Available");                                         // print constant string
    else
      Lcd.print("Not available");                                     // print constant string
  }

  delay(1500);                                                        // wait for 1500ms (1.5 sec)
}

void loop(){              
  int res;                                                            // initialize variable - hold result
  double x_deg, y_deg, z_deg;                                         // initialize variable - angular position for each axes
  char buf[64];                                                       // initialize bariable - sprintf buffer

  res = Gyr.readDegrees(&x_deg, &y_deg, &z_deg);                      // read 2's complement angular velocity of all axes x, y and z at once

  if(res == 0){                                                       // if successfully read
    // print current positions:
    sprintf(buf, "Current position: X =%3d, Y =%3d, Z =%3d", (int)x_deg, (int)y_deg, (int)z_deg);
    Serial.println(buf);                                              // print buffer string
  }
  else
    Serial.println("Can't read angular position.");                   // print constant string
    
  // read again current position, but each axle separately 
  x_deg=Gyr.readDegreesX();                                           // read position along X axis
  y_deg=Gyr.readDegreesY();                                           // read position along Y axis
  z_deg=Gyr.readDegreesZ();                                           // read position along Z axis
    
  if( (ReadEmoroHardware() & LCD_AVAILABLE) && (res==0) ){            // if LCD is available and Gyro initialized OK
      Lcd.locate(0, 0);                                               // set LCD cursor position (row, column)
      Lcd.print(" X   Y   Z");                                        // print coordinate names 
      Lcd.locate(1, 0);                                               // set LCD cursor position (row, column)
      sprintf(buf, "%3d %3d %3d", (int)x_deg, (int)y_deg, (int)z_deg);// print angular velocity for each direction
      Lcd.print(buf);                                                 // print buffer string
  }
  else if(ReadEmoroHardware() & LCD_AVAILABLE){                       // if LCD is available
      Lcd.locate(1, 0);                                               // set LCD cursor position (row, column)
      Lcd.print("Can't read      ");                                  // print constant string
  }

  // if SW_1 to SW_4 available
  if(ReadEmoroHardware() & SW_AVAILABLE){                             // if switches are available
    if(ReadSwitch(SW_1)){                                             // if SW_1 is activated, set current gyro position as 0, 0, 0
      Serial.println("Reset current position. X=0, Y=0, Z=0.");       // print constant string
      Gyr.resetDegrees();                                             // reset Gyro axes reading
      // also it is possible to use: 
      // Gyr.setDegrees(0, 0, 0) 
    }
    else if(ReadSwitch(SW_2)){                                        // release gyro advanced mode resource
      Gyr.stop();                                                     // stop background angular position calculations
      Serial.println("Terminate Gyro Advanced mode.");                // print constant string
    }
  }
    
  delay(300);                                                         // wait for 300ms
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

