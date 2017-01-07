/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Accelerometer	     				                                                        */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows how to read data from BMA150 accelerometer. Each time when EMoRo 2560       */
/*            controller is turned on, if accelerometer is available, initialization of accelerometer will  */
/*            be automatically called. Accelerometer will be set it up in measuring range -2.000g to 1.966g.*/
/*            ( g is acceleration of free fall = 9.81 m/sec^2 ) Returning value from accelerometer are in   */
/*            form of the second complement: -512 to +511 (corresponding to -2.000g  to 1.996g).            */
/*            Accelerometer reads data in three directions. X direction is to right, -X to left, Y          */
/*            direction is to backward, -Y to forward, Z direction is up above, -Z to the ground. Be aware  */
/*            that there is a reading of acceleration of 9.81m/sec^2 in -Y direction while accelerometer is */
/*            in steady position. This reading is acceleration due to gravity and it is the same everywhere */
/*            on the earth. If X and Y directions are not exactly perpendicular with free fall direction,   */
/*            it is possible to have some reading of acceleration in X and Y direction while accelerometer  */
/*            is in steady position. This reading is because of X and Y vector components of acceleration   */
/*            of free fall.                                                                                 */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with BMA150 Accelerometer                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

void setup(void){				       

  Serial.begin(9600);                                         // initialize serial communication
  Serial.println("Example: Accelerometer read");              // send example name to console
  
  // test availability of accelerometer
  if(ReadEmoroHardware() & ACC_AVAILABLE){                    // if accelerometer is available
    Serial.println("Accelerometer available.");		            // print constant string
    if(ReadEmoroHardware() & LCD_AVAILABLE){		              // if LCD available
      Lcd.locate(0, 0);                                       // set LCD cursor position (row, column)
      Lcd.print("Acc read ");                                 // print example name on LCD
      Lcd.locate(1, 0);                                       // set LCD cursor position (row, column)
      Lcd.print("Acc Available");                             // print info on LCD  
    }
  }
  else{
    Serial.println("Accelerometer not available.");           // print constant string
    if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD available
      Lcd.locate(0, 0);                                       // set LCD cursor position (row, column)
      Lcd.print("Accelerometer");                             // print example name on LCD
      Lcd.locate(1, 0);                                       // set LCD cursor position (row, column)
      Lcd.print("Acc Not Availab.");                          // print info on LCD
    }
  }
  
  delay(1500);                                                // wait for 1.5 second
}

void loop(void){					
  // the second complements of each direction, -512 to +511 (corresponding to -2.000g  to 1.996g)
  int x, y, z, res;                                           // initialize variables
  // acceleration in m/s^2 in each direction
  float x_phy, y_phy, z_phy;                                  // initialize variables
  // string form of acceleration in m/s^2
  char x_phyStr[10], y_phyStr[10], z_phyStr[10];              // initialize variables

  // read the second complements of acceleration in each direction (x, y, z)
  // x -> right, -x -> left
  // y -> backward, -y -> forward
  // -z -> to the eart, z -> to up above 
  res = Acc.read(&x, &y, &z);                                 // read the second complements of acceleration in each direction (x, y, z)
   
   
  if(res == 0){                                               // if reading successfull
    // convert to physical acceleration values (m/s^2)
    x_phy=x*2.0*9.81/512;
    y_phy=y*2.0*9.81/512;
    z_phy=z*2.0*9.81/512;

    char buf[32];					                                    // initialize variables
    sprintf(buf, "X =%4d, Y =%4d, Z =%4d", x, y, z);          // form a result string
    Serial.println(buf);                                      // print buf string
    Serial.print("X =");                                      // print constant string
    Serial.print(x_phy);                                      // print x_phy value
    Serial.print(" m/s^2, Y =");                              // print constant string
    Serial.print(y_phy);                                      // print y_phy value
    Serial.print(" m/s^2, Z =");                              // print constant string
    Serial.println(z_phy);                                    // print z_phy value
  
    if(ReadEmoroHardware() & LCD_AVAILABLE){                  // if LCD is available
        
      // it is also possible to read acceleration of only one direction:
      x=Acc.readX();                                          // read x axis acceleration
      y=Acc.readY();                                          // read y axis acceleration
      z=Acc.readZ();                                          // read z axis acceleration
      // convert to physical acceleration values (m/s^2)
      x_phy=x*2.0*9.81/512;
      y_phy=y*2.0*9.81/512;
      z_phy=z*2.0*9.81/512;

      // sprintf can't handle float, because of that convert floats to strings
      dtostrf(x_phy, 3, 2, x_phyStr);
      dtostrf(y_phy, 3, 2, y_phyStr);
      dtostrf(z_phy, 3, 2, z_phyStr);

      Lcd.locate(0, 0);                                       // set LCD cursor position (row, column)
      Lcd.print(" X    Y    Z");                              // print coordinate names
      Lcd.locate(1, 0);                                       // set LCD cursor position (row, column)
      sprintf(buf, "%s %s %s", x_phyStr, y_phyStr, z_phyStr); // print acceleration in m/s^2 for each direction (vector)
      Lcd.print(buf);                                         // print buf string
    }
  }
  
  delay(300);                                                 // wait for 300 ms (0.3 sec)
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

