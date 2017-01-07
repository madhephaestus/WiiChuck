/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  DS18S20                               				                                  */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example read temperature sensor DS18S20 attached to port IO_0 and print temperature to   */
/*            console. It is possible to attach temperature sensor DS18S20 to next ports: IO_0 – IO_16,     */
/*            ADC_0 – ADC_7, SERVO_0 – SERVO_7, GPP_0_A – GPP_7_B, PWM_0 – PWM_5, EX_IO_0 – EX_IO_17. Before */
/*            any reading sensor reading should be initialized with DS18S20.attach(<pin name>);             */
/*      The circuit:                                                                                        */
/*            * pin 1 (ground) of sensor DS18S20 (TO-92) connect to pin 1 (ground) of IO_0                  */
/*            * pin 3 (Vcc) of sensor DS18S20 (TO-92) connect to pin 2 (+5V) of IO_0                        */
/*            * pin 2 (DQ)  of sensor DS18S20 (TO-92) connect to pin 3 (IO_0) of IO_0                       */
/*      DS18S20:                                                                                            */
/*            http://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18S20.html   */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Temperature sensor DS18S20 (TO-92)                                              */
/*                          Pin header connectors 3x1 RM 2.54mm                                             */
/*                          Wires                                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

int sensorPin = IO_0;                     // select the input pin for the temperature sensor

void setup(){				        
  Serial.begin(9600);					            // initialize serial communication
  Serial.println("Example: DS18S20");     // send example name to console

  int res = DS18S20.attach(sensorPin);    // initialize sensor DS18S20 reading on port IO_0
  if(res<0)                               // if sensor is not available
    Serial.println("There is no DS18S20 sensor on port IO_0.");
  }

void loop(){					 
  float temp_c = DS18S20.read(sensorPin); // read temperature sensor attached to port IO_0. Return value is in Celsius
  float temp_f = temp_c * 9/5 + 32;       // convert Fahrenheit from Celsius
  
  // print temperature in Celsius
  Serial.print("T = ");                   // print string
  Serial.print(temp_c);                   // print temperature value
  Serial.println(" Celsius");             // print string

  // print temperature in Fahrenheit  
  Serial.print("T = ");                   // print string
  Serial.print(temp_f);                   // print temperature value
  Serial.println(" Fahrenheit");          // print string

  delay(1500);                            // wait for 1500ms (1.5 sec)
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

