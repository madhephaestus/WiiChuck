/************************************************************************************************************/
/*      Inovatic-ICT d.o.o								                                                    	            */
/*												                                                                                  */
/*      EMoRo Example:		  Mega multple serial test                                                        */
/*      Board name: 		    EMoRo 2560 v3.2	                                          					            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Receives from the main serial port, sends to the others. Receives from serial port 2, sends   */
/*            to the main serial (Serial 0). This example works only on the Arduino Mega and EMoRo 2560     */
/*      The circuit:                                                                                        */
/*            Any serial device attached to Serial port 2                                                   */
/*            Serial monitor open on Serial port 0                                                          */
/*            In the case of connecting GPP_6_B and GPP_7_B every character send through serial 0 will be   */
/*            sent back as an echo from Serial port 2 back to serial port 0.                                */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          Wires                                                                           */
/*                          Female header connector 4x1 RM 2.54mm       2 pieces                            */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

/* note Serial connection on the EMoRo 2560:                                                                */
/* Serial_0 Tx    Not Available               (Used for USB CDC)                                            */
/* Serial_0 Rx    Not Available               (Used for USB CDC)                                            */
/* Serial_1 Tx    PD3              EX_IO_3    (Used for External board)                                     */
/* Serial_1 Rx    PD2              EX_IO_2    (Used for External board)                                     */
/* Serial_2 Tx    PH1              GPP_6_B    (Used for GPP_6)                                              */
/* Serial_2 Rx    PH0              GPP_7_B    (Used for GPP_7)                                              */
/* Serial_3 Tx    PJ1              GPP_2_A    (Used for GPP_2)                                              */
/* Serial_3 Rx    PJ0              GPP_1_A    (Used fom GPP_1)                                              */

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);                       // initialize Serial port
  Serial2.begin(9600);                      // initialize Serial port 2
}

void loop() {
  // read from port 2, send to port 0:
  if (Serial2.available()) {                // if data is available on Serial port 2
    int inByte = Serial2.read();            // read available data 
    Serial.write(inByte);                   // send recieved data on Serial port
  }
  
  // read from port 0, send to port 2:
  if (Serial.available()) {                 // if data is available on Serial port
    int inByte = Serial.read();             // read available data
    Serial2.write(inByte);                  // send recieved data on Serial port
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

