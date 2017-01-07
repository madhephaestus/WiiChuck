/*************************************************** 
  This is an implementation of Conway's Game of Life.

  Designed specifically to work with the Adafruit Trellis 
  ----> https://www.adafruit.com/products/1616
  ----> https://www.adafruit.com/products/1611

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Tony Sherwood for Adafruit Industries.  
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include "Adafruit_Trellis.h"

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();
Adafruit_Trellis matrix2 = Adafruit_Trellis();
Adafruit_Trellis matrix3 = Adafruit_Trellis();

Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0, &matrix1, &matrix2, &matrix3);

#define NUMTRELLIS 4

#define numKeys (NUMTRELLIS * 16)

/*
  In order for this layout to work, you will have to have soldered your 4 boards
  together in the same order that I did.  Below is the top view of the circuit, with
  each board's address, and lines indicating which boards are connected to each other.
  If you soldered your boards together differently, you will need to edit the chessboard
  array below until the LED wipe when you start up your sketch turns on all of the LEDs
  in order.

             [0x73]--[0x72]
                       |
  [ARDUINO]--[0x70]--[0x71]
 
*/

int chessboard[8][8] = {
  {60, 56, 52, 48, 44, 40, 36, 32}, 
  {61, 57, 53, 49, 45, 41, 37, 33},
  {62, 58, 54, 50, 46, 42, 38, 34},
  {63, 59, 55, 51, 47, 43, 39, 35},
  {12, 8, 4, 0, 28, 24, 20, 16},
  {13, 9, 5, 1, 29, 25, 21, 17},
  {14, 10, 6, 2, 30, 26, 22, 18},
  {15, 11, 7, 3, 31, 27, 23, 19}
};

int nextFrame[64];


// Connect Trellis Vin to 5V and Ground to ground.
// Connect the INT wire to pin #5 (can change later
#define INTPIN 5
// Connect I2C SDA pin to your Arduino SDA line
// Connect I2C SCL pin to your Arduino SCL line
// All Trellises share the SDA, SCL and INT pin! 
// Even 8 tiles use only 3 wires max


void setup() {
  Serial.begin(9600);
  //Serial.println("Trellis Game of Life");

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  trellis.begin(0x72, 0x71, 0x70, 0x73);
  
  for (uint8_t i=0; i<8; i++) {
    for (uint8_t j=0; j<8; j++) {
      trellis.setLED(chessboard[i][j]);
      trellis.writeDisplay();
      delay(50);
    }
  }  

  
  for (uint8_t i=0; i<numKeys; i++) {
    trellis.clrLED(i);
  }
  trellis.writeDisplay();
  
  // Set up a glider
  makeOscillator();
  trellis.writeDisplay();
  delay(1000);
}

void toggle(int placeVal) {
 if (trellis.isLED(placeVal))
    trellis.clrLED(placeVal);
  else
    trellis.setLED(placeVal);
}

/* Directions are ordered clockwise starting with 0 = NORTH */
int getNeighbor(int placeVal, int neighbor) {
  int px = 0;
  int py = 0;
  int x = 0;
  int y = 0;
  
  getPosition(placeVal, &px, &py);
  switch (neighbor) {
    case 0:
      x = px;
      y = py - 1;
      break;
    case 1:
      x = px + 1;
      y = py - 1;
      break;
    case 2:
      x = px + 1;
      y = py;
      break;
    case 3:
      x = px + 1;
      y = py + 1;
      break;
    case 4:
      x = px;
      y = py + 1;
      break;
    case 5:
      x = px - 1;
      y = py + 1;
      break;
    case 6:
      x = px - 1;
      y = py;
      break;
    case 7:
      x = px - 1;
      y = py - 1;
      break;
    default:
      x = 0;
      y = 0;
  }
  if (x < 0) x = 7;
  if (x > 7) x = 0;
  if (y < 0) y = 7;
  if (y > 7) y = 0;
  
  return chessboard[x][y];
}

int getPosition(int pv, int *tx, int *ty) {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      if (chessboard[i][j] == pv) {
        *tx = i;
        *ty = j;
        return 1;  
      }
    }
  }
  return -1;
}

void makeGlider() {
  trellis.setLED(chessboard[2][3]);
  trellis.setLED(chessboard[1][2]);
  trellis.setLED(chessboard[3][1]);
  trellis.setLED(chessboard[3][2]);
  trellis.setLED(chessboard[3][3]);
  trellis.writeDisplay();
}

void makeOscillator() {
  trellis.setLED(chessboard[6][0]);
  trellis.setLED(chessboard[6][6]);
  trellis.setLED(chessboard[6][7]);
  trellis.setLED(chessboard[5][0]);
  trellis.setLED(chessboard[5][1]);
  trellis.setLED(chessboard[5][2]);
}


void liveOrDie(int placeVal) {  
  // Calculate whether to live or die the next round
  int neighbors = 0;
  for (int d=0; d<=7; d++) {
    if (trellis.isLED(getNeighbor(placeVal, d))) {
      neighbors++;
    }
  }
  
  if (neighbors == 3 && !trellis.isLED(placeVal)) {
    nextFrame[placeVal] = 1;
  }else if ((neighbors == 2 || neighbors == 3) && trellis.isLED(placeVal)) {
    nextFrame[placeVal] = 1;
  } else {
    nextFrame[placeVal] = 0;
  }
}

void loop() {
  delay(500); // 100ms delay is required, dont remove me!
 
  // Clear out the next frame
  for(int c=0; c<64; c++) {
    nextFrame[c] = 0;
  }
  
  //compute the next step
  for (uint8_t i=0; i<numKeys; i++) {
    liveOrDie(i);
  } 
  
  if (trellis.readSwitches()) {
    // go through every button
    for (uint8_t i=0; i<numKeys; i++) {
      // if it was pressed, add it to the list!
      if (trellis.justPressed(i)) {
        nextFrame[i] = 1;
      }
    }
  }  
  
  // Update the map
  for (uint8_t i=0; i<numKeys; i++) {
    if(nextFrame[i] == 1) {
      trellis.setLED(i);
    } else {
      trellis.clrLED(i);
    }
  } 
  
  // tell the trellis to set the LEDs we requested
  trellis.writeDisplay(); 

}

