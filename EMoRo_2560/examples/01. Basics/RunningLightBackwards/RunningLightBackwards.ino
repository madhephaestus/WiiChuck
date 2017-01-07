/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Running Light Backwards                                                                 */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            Turns LED IO_15 on for 100 milliseconds, then turns it off for 100 milliseconds.              */
/*            After LED IO_15 repeat the same procedure for next LEDs down to IO_0.                         */
/*            After IO_0 LED start again wiht IO_15 LED.                                                    */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

// Pins IO_0 to IO_15 have a LED connected on EMoRo 2560 board.
int led = IO_0;                   // initialialize variable "led" - give a name to the first LED
int i;                            // initialize variable "i" - used for counter

// the setup routine runs once when you press reset:
void setup() {                
  for(i=0;i<=IO_15;i++)           // initialize the digital pins (IO_0 to IO_15) as an output pins
    pinMode(led+i, OUTPUT);     
}

// the loop routine runs over and over again forever:
void loop() {
  for(i=15;i>=IO_0;i--){          // swipe trough pins IO_15 down to IO_0 
    digitalWrite(led+i, HIGH);    // turn the LED on (voltage level - 5V - HIGH)
    delay(100);                   // wait for 100 milliseconds
    digitalWrite(led+i, LOW);     // turn the LED on (voltage level - 0V - LOW)
    delay(100);                   // wait for 100 milliseconds
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/
