/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Blink                                                                           */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            Turns LED IO_0 on for one second, then off for one second, repeatedly.                        */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// Pin IO_0 has a LED connected on EMoRo 2560 board
int led = IO_0;                     // initialize variable led (give the name "led" to the pin IO_0)

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led, OUTPUT);             // initialize the digital pin as an output  
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);          // turn the LED on (voltage level - 5V - HIGH)
  delay(1000);                      // wait for a second
  digitalWrite(led, LOW);           // turn the LED off (voltage level - 0V- LOW)
  delay(1000);                      // wait for a second
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
