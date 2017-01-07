/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Sharp_GP2Y0A41SK                               				                          */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example read analogue signal from sensor Sharp GP2Y0A41SK attached to port ADC_0.        */
/*            GP2Y0A41SK is infrared distance sensor with measurement range 4-30cm. Analogue                */
/*            output/distance characteristic can be found in data sheet.                                    */
/*      The circuit:                                                                                        */
/*            * pin 2 (ground) of sensor GP2Y0A41SK connect to pin 1 (ground) ADC_0 port                    */
/*            * pin 3 (Vcc) of sensor GP2Y0A41SK connect to pin 2 (+5V) ADC_0 port                          */
/*            * pin 1 (Vo) of sensor GP2Y0A41SK connect to pin 3 (analogue in) ADC_0 port                   */
/*      Sharp GP2Y0A41SK:                                                                                   */
/*            http://www.inovatic-ict.com/Other/gp2y0a41sk_e.pdf                                            */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GP2Y0A41SK with wires and pin header connectors 3x1 RM 2.54mm                   */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

int sensorPin = ADC_0;                    // select the input pin for the distance sensor
int sensorValue = 0;                      // variable to store the value coming from the sensor

void setup(){				        
  Serial.begin(9600);					            // initialize serial communication
  Serial.println("Example: Sharp_GP2Y0A41SK"); // send example name to console
}

void loop(){					
  double voltageValue;
  sensorValue = analogRead(sensorPin);    // read the value from the sensor:

  // convert ADC value to real voltage. ADC value is in range 0 to 1023, 
  // corresponding to voltage in range 0V - 5V
  voltageValue=sensorValue*5.0/1023.0;    // voltage calculation
     
  // print sensor value
  Serial.print("Sensor value = ");        // print string
  Serial.print(voltageValue);             // print voltage value
  Serial.println(" V");                   // print string
  
  delay(100);                             // wait for 100ms (0.1 sec)
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

