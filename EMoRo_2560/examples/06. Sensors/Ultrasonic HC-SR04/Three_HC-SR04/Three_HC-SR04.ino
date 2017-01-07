/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Three HC-SR04                                                                   */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch reads three HC-SR04 ultrasonic rangefinders and returns the distances to the      */
/*            closest object in the range of each sensor. This example uses special EMoRo 2560 library      */
/*            which continuously make measurements on initialized ports in the background and make          */
/*            available reading without special delay. This way of using is especially useful for robotics  */
/*            application.                                                                                  */
/*      The circuit:                                                                                        */
/*            * GND connection of the HC-SR04 attached to ground (GPP_x pin 1)                              */
/*            * +V connection of the HC-SR04 attached to +5V (GPP_x pin 4)                                  */
/*	          * Echo connection of the HC-SR04 attached to GPP_x_A  (GPP_x pin 2)                           */
/*            * Trig connection of the HC-SR04 attached to GPP_x_B  (GPP_x pin 3)                           */
/*            Used ports are GPP_0, GPP_4 and GPP_7                                                         */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          HC-SR04 ultrasonic sensor                                       3 pieces        */
/*                          Female/Female cable with header connectors 4x1 RM 2.54mm        3 pieces        */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
//initialize public variables
int loop_count;
int max_loop;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // send example name to console
  Serial.println("Example: Three HC-SR04");
  // initialize background reading of ultrasonic sensor on port GPP_0, GPP_4 and GPP_7 
  // it is possible to use max 8 ultrasonic sensors. Available ports are GPP_0 to GPP_8
  Ultrasonic.attach(GPP_0);       
  Ultrasonic.attach(GPP_4);
  Ultrasonic.attach(GPP_7);       
  //reset loop count
  loop_count=0;
  // after reading 100 times measured data of all three ultrasonic sensors, example will detach two 
  // sensors (on port GPP_4 and GPP_7) and continues with using only one ultrasonic sensor attached
  // on port GPP_0   
  max_loop=100;
}

void loop()
{
  // establish variables for the distance result in inches and centimetres:
  double inches_0, inches_4, inches_7;
  int  cm_0, cm_4, cm_7;

  // get last distance reading on ultrasonic sensor attached to GPP_0
  cm_0=Ultrasonic.read(GPP_0);
  // convert the distance from cm to inches
  inches_0=(double)cm_0/2.54;

  // get last distance reading on ultrasonic sensor attached to GPP_4
  cm_4=Ultrasonic.read(GPP_4);
  // convert the distance from cm to inches
  inches_4=(double)cm_4/2.54;

  // get last distance reading on ultrasonic sensor attached to GPP_7
  cm_7=Ultrasonic.read(GPP_7);
  // convert the distance from cm to inches
  inches_7=(double)cm_7/2.54;


  // print measurements to console
  Serial.print("Measured distance on sensor attached to GPP_0: ");
  Serial.print(inches_0);
  Serial.print("in, ");
  Serial.print(cm_0);
  Serial.print("cm");
  Serial.println();

  if(cm_4>=0){
    // print measurements to console
    Serial.print("Measured distance on sensor attached to GPP_4: ");
    Serial.print(inches_4);
    Serial.print("in, ");
    Serial.print(cm_4);
    Serial.print("cm");
    Serial.println();
  }
  else
    Serial.println("Ultrasonic sensor on port GPP_4 is detached.");

  if(cm_7>=0){
    // print measurements to console
    Serial.print("Measured distance on sensor attached to GPP_7: ");
    Serial.print(inches_7);
    Serial.print("in, ");
    Serial.print(cm_7);
    Serial.print("cm");
    Serial.println();
  }
  else
    Serial.println("Ultrasonic sensor on port GPP_7 is detached.");

  delay(100);

  if(loop_count<max_loop){
    //count finished loop
    loop_count++;  
  }
  else if(loop_count==max_loop){
    loop_count++;
    // Ultrasonic.detach(<port name>); will stops the measurements on specified port in the background
    // This will stops waste time on something useless, and also it will increase sample time on ultrasonic
    // sensors which are still in use
    Serial.println("Detach ultrasonic sensor on port GPP_4.");
    Ultrasonic.detach(GPP_4);       
    Serial.println("Detach ultrasonic sensor on port GPP_7.");
    Ultrasonic.detach(GPP_7); 
  }
}
/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

