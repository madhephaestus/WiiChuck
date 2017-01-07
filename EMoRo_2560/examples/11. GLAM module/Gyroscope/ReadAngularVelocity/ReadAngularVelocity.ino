/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  ReadAngularVelocity 			                                                      */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Gyroscope L3GD20 which is used in GLAM module can be used on EMoRo 2560 controller in two     */
/*            modes, Basic and Advanced. In Basic mode is available only raw angular velocity reading of    */
/*            all three axes. Reading values are 2's complement 16 bits  values from -32768 to              */
/*            +32767. Those values corresponding to angular velocity from -250 to 250 dps (degrees per      */
/*            seconds). It is not necessary to initialize gyroscope in Basic mode because it is already     */
/*            done before setup(); function by calling Gyr.initBasic(); in function InitEmoro();            */
/*            (see: .\cores\emoro_core\main.cpp)                                                            */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with Gyroscope option                                               */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(){					

  Serial.begin(9600);                                         // initialize serial communication
  Serial.println("Example: ReadAngularVelocity");             // send example name to console
  
  if(ReadEmoroHardware() & GYR_AVAILABLE)                     // if Gyro is available
    Serial.println("Gyroscope is available.");                // print constant string
  else  
    Serial.println("Gyroscope isn't available.");             // print constant string

  if(ReadEmoroHardware() & LCD_AVAILABLE){                    // if LCD available
    Lcd.locate(0, 0);                                         // set LCD cursor position (row, column)
    Lcd.print("Gyroscope");                                   // print constant string
    Lcd.locate(1, 0);                                         // set LCD cursor position (row, column)
    if(ReadEmoroHardware() & GYR_AVAILABLE)                   // if Gyro is available
      Lcd.print("Available");                                 // print constant string
    else
      Lcd.print("Not available");                             // print constant string
  }

  delay(1500);                                                // wait for 1500ms (1.5 sec)
}

void loop(){				        
  int x, y, z, res;                                           // angular velocity as 2's complement for each axes
  float x_phy, y_phy, z_phy;                                  // angular velocity in dps for each axes
  char x_phyStr[10], y_phyStr[10], z_phyStr[10];              // string form of angular velocity
  char buf[64];                                               //sprintf buffer

  res = Gyr.read(&x, &y, &z);                                 // read 2's complement angular velocity of all axes x, y and z at once

  if(res == 0){                                               // if successfully read
    // print as the second complement in the range from -32768 to
    // +32767, corresponding to angular velocity from -250 to 250 dps
    sprintf(buf, "2'S complement values: X =%6d, Y =%6d, Z =%6d", x, y, z); // form string
    Serial.println(buf);                                      // print buffer string
  }

  // read again 2's complement angular velocity, but each axle separately 
  x=Gyr.readX();
  y=Gyr.readY();
  z=Gyr.readZ();

  // convert to degrees per seconds
  x_phy=x*250.0/32767;
  y_phy=y*250.0/32767;
  z_phy=z*250.0/32767;
    
  // sprintf can't handle float, because of that convert floats to strings
  dtostrf(x_phy, 3, 2, x_phyStr);
  dtostrf(y_phy, 3, 2, y_phyStr);
  dtostrf(z_phy, 3, 2, z_phyStr);

  sprintf(buf, "Velocity in degrees per second: X =%s, Y =%s, Z =%s", x_phyStr, y_phyStr, z_phyStr);// form string
  Serial.println(buf);                                        // print buffer string
        
      
  if(ReadEmoroHardware() & LCD_AVAILABLE){                    // if LCD is available
      Lcd.locate(0, 0);                                       // set LCD cursor position (row, column)
      Lcd.print(" X    Y    Z");                              // print coordinate names
      Lcd.locate(1, 0);                                       // set LCD cursor position (row, column)
      sprintf(buf, "%s %s %s", x_phyStr, y_phyStr, z_phyStr); // print angular velocity for each direction
      Lcd.print(buf);                                         // print buffer string
  }
  
  delay(300);                                                 // wait for 300ms
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

