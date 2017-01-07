/************************************************************************************************************/
/*      Inovatic-ICT d.o.o	                                                    								            */
/*												                                                                                  */
/*      EMoRo Example:		  Multiple tone player                                                            */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            Plays multiple tones on speakers connected to multiple pins                                   */  
/*      Circuit:                                                                                            */
/*            3 8-ohm speaker on digital pins IO_0, IO_1, and IO_2 (the first wire of a speaker connect to  */
/*            pin 1 (leftmost pin marked with a triangle) and  the second wire of a speaker to pin 3).      */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          8-ohm speaker                   3 pieces                                        */
/*                          Connecting wires                                                                */
/*                          Pin header male 3x1 RM 2.54mm   3 pieces                                        */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
 http://arduino.cc/en/Tutorial/Tone4
*/

void setup() {

}

void loop() {
  
  tone(IO_1, 440, 200);           // play a note on pin IO_1 for 200 ms
  delay(200);                     // delay 200 ms
  noTone(IO_1);                   // turn off tone function for pin IO_1
  
  tone(IO_2, 494, 500);           // play a note on pin IO_2 for 500 ms
  delay(500);                     // delay 500 ms
  noTone(IO_2);                   // turn off tone function for pin IO_2
  
  tone(IO_0, 523, 300);           // play a note on pin IO_0 for 500 ms
  delay(300);                     // delay 300 ms
  noTone(IO_0);                   // turn off tone function for pin IO_0
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

