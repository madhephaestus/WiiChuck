/************************************************************************************************************/
/*      Inovatic-ICT d.o.o                                                                                  */
/*                                                                                                          */
/*      EMoRo Example:      Row-Column Scanning an 8x8 LED matrix with X-Y input                            */
/*      Board name:         EMoRo 2560 v3.2                                                                 */
/*      MCU name:           ATMEGA2560-16                                                                   */
/*      Clock:              16 MHz                                                                          */
/*      File description:                                                                                   */
/*            This example controls an 8x8 LED matrix using two analog inputs                               */
/*            This example works for the Lumex  LDM-24488NI Matrix. For the pin connections See             */
/*            http://sigma.octopart.com/140413/datasheet/Lumex-LDM-24488NI.pdf                              */
/*                                                                                                          */
/*            For other LED cathode column matrixes, you should only need to change the pin numbers in      */
/*            the row[] and column[] arrays                                                                 */
/*            rows are the anodes, cols are the cathodes                                                    */
/*      Circuit:                                                                                            */
/*            LED matrix row 1-8 (pins: 9, 14, 8, 12, 1, 7, 2, 5) connect to IO_0 - IO_7 (pin 3 of each     */
/*            port). LED matrix column 1-8 (pins: 13, 3 , 4, 10, 6, 11, 15, 16 ) connect to IO_8 - IO_15    */
/*            (pin 3 of each port). It is recommended to connect 270 Ohm resistors between column pins and  */
/*            IO_8 - IO_15 ports (in serial connection with each LED).                                      */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          LEM Matrix LDM-24488NI                                                          */
/*                          Pin header male 3x1 RM 2.54mm     16 pieces                                     */
/*                          Wires                                                                           */  
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/*
 http://www.arduino.cc/en/Tutorial/RowColumnScanning
 
 see also http://www.tigoe.net/pcomp/code/category/arduinowiring/514 for more
 */


const int row[8] = {                                    // 2-dimensional array of row pin numbers
  IO_0, IO_1, IO_2, IO_3, IO_4, IO_5, IO_6, IO_7 
};

const int col[8] = {                                    // 2-dimensional array of column pin numbers
 IO_8, IO_9, IO_10, IO_11, IO_12, IO_13, IO_14, IO_15  
};

int pixels[8][8];                                       // 2-dimensional array of pixels

// cursor position:
int x = 5;
int y = 5;

void setup() {
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {       // iterate throug pins
    pinMode(col[thisPin], OUTPUT);                      // initialize the output pins
    pinMode(row[thisPin], OUTPUT);                      // initialize the output pins
    // take the col pins (i.e. the cathodes) low to ensure that
    // the LEDS are off: 
    digitalWrite(col[thisPin], LOW);    
  }

  // initialize the pixel matrix:
  for (int x = 0; x < 8; x++) {                         // iterate throug pixels
    for (int y = 0; y < 8; y++) {                       // iterate throug pixels
      pixels[x][y] = LOW;                               // set all pixels inactive
    }
  }
}

void loop() {         
  readSensors();                                        // read inputs
  refreshScreen();                                      // draw the screen
}

void readSensors() {                                    // read sensors routine
  pixels[x][y] = LOW;                                   // turn off the last position
  x = 7 - map(analogRead(ADC_0), 0, 1023, 0, 7);        // read the sensors for X and Y values
  y = map(analogRead(ADC_1), 0, 1023, 0, 7);            // read the sensors for X and Y values
  // set the new pixel position high so that the LED will turn on
  // in the next screen refresh:
  pixels[x][y] = HIGH;

}

void refreshScreen() {
  for (int thisRow = 0; thisRow < 8; thisRow++) {       // iterate over the rows (anodes)
    digitalWrite(row[thisRow], LOW);                    // take the row pin (anode) low
    for (int thisCol = 0; thisCol < 8; thisCol++) {     // iterate over the cols (cathodes)
      int thisPixel = pixels[thisRow][thisCol];         // get the state of the current pixel
      // when the row is HIGH and the col is LOW,
      // the LED where they meet turns on:
      digitalWrite(col[thisCol], thisPixel);
      if (thisPixel == LOW) {                           // turn the pixel off
        digitalWrite(col[thisCol], LOW);                
      }
    }
    digitalWrite(row[thisRow], HIGH);                   // take the row pin high to turn off the whole row
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

