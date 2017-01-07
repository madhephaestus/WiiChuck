/*********************************************************************
This is an Arduino library for our Monochrome SHARP Memory Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1393

These displays use SPI to communicate, 3 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include "Adafruit_SharpMem.h"

/**************************************************************************
    Sharp Memory Display Connector
    -----------------------------------------------------------------------
    Pin   Function        Notes
    ===   ==============  ===============================
      1   VIN             3.3-5.0V (into LDO supply)
      2   3V3             3.3V out
      3   GND
      4   SCLK            Serial Clock
      5   MOSI            Serial Data Input
      6   CS              Serial Chip Select
      9   EXTMODE         COM Inversion Select (Low = SW clock/serial)
      7   EXTCOMIN        External COM Inversion Signal
      8   DISP            Display On(High)/Off(Low)

 **************************************************************************/

#define SHARPMEM_BIT_WRITECMD   (0x80)
#define SHARPMEM_BIT_VCOM       (0x40)
#define SHARPMEM_BIT_CLEAR      (0x20)
#define TOGGLE_VCOM             do { _sharpmem_vcom = _sharpmem_vcom ? 0x00 : SHARPMEM_BIT_VCOM; } while(0);

byte sharpmem_buffer[(SHARPMEM_LCDWIDTH * SHARPMEM_LCDHEIGHT) / 8];

/* ************* */
/* CONSTRUCTORS  */
/* ************* */
Adafruit_SharpMem::Adafruit_SharpMem(uint8_t clk, uint8_t mosi, uint8_t ss) :
Adafruit_GFX(SHARPMEM_LCDWIDTH, SHARPMEM_LCDHEIGHT) {
  _clk = clk;
  _mosi = mosi;
  _ss = ss;

/*  
// Spark core firmware does not do hardware config in class inilization so move to init()
// Set pin state before direction to make sure they start this way (no glitching)
  digitalWrite(_ss, HIGH);  
  digitalWrite(_clk, LOW);  
  digitalWrite(_mosi, HIGH);  
  
  pinMode(_ss, OUTPUT);
  pinMode(_clk, OUTPUT);
  pinMode(_mosi, OUTPUT);
  
  clkport     = portOutputRegister(digitalPinToPort(_clk));
  clkpinmask  = digitalPinToBitMask(_clk);
  dataport    = portOutputRegister(digitalPinToPort(_mosi));
  datapinmask = digitalPinToBitMask(_mosi);
*/  
  // Set the vcom bit to a defined state
  _sharpmem_vcom = SHARPMEM_BIT_VCOM;

}

void Adafruit_SharpMem::begin() {
  setRotation(2);
}

void Adafruit_SharpMem::init() {
  // Set pin state before direction to make sure they start this way (no glitching)
  digitalWrite(_ss, HIGH);  
  digitalWrite(_clk, LOW);  
  digitalWrite(_mosi, HIGH);  
  
  pinMode(_ss, OUTPUT);
  pinMode(_clk, OUTPUT);
  pinMode(_mosi, OUTPUT);
}

/* *************** */
/* PRIVATE METHODS */
/* *************** */

 
/**************************************************************************/
/*!
    @brief  Sends a single byte in pseudo-SPI.
*/
/**************************************************************************/
void Adafruit_SharpMem::sendbyte(uint8_t data) 
{
  uint8_t i = 0;

  // LCD expects LSB first
  for (i=0; i<8; i++) 
  { 
    // Make sure clock starts low
    //digitalWrite(_clk, LOW);
    //*clkport &= ~clkpinmask;
    PIN_MAP[_clk].gpio_peripheral->BRR = PIN_MAP[_clk].gpio_pin;
    if (data & 0x80) 
      //digitalWrite(_mosi, HIGH);
      //*dataport |=  datapinmask;
      PIN_MAP[_mosi].gpio_peripheral->BSRR = PIN_MAP[_mosi].gpio_pin;
    else 
      //digitalWrite(_mosi, LOW);
      //*dataport &= ~datapinmask;
      PIN_MAP[_mosi].gpio_peripheral->BRR = PIN_MAP[_mosi].gpio_pin;

    // Clock is active high
    //digitalWrite(_clk, HIGH);
    //*clkport |=  clkpinmask;
    PIN_MAP[_clk].gpio_peripheral->BSRR = PIN_MAP[_clk].gpio_pin;
    data <<= 1; 
  }
  // Make sure clock ends low
  //digitalWrite(_clk, LOW);
  //*clkport &= ~clkpinmask;
  PIN_MAP[_clk].gpio_peripheral->BRR = PIN_MAP[_clk].gpio_pin;
}

void Adafruit_SharpMem::sendbyteLSB(uint8_t data) 
{
  uint8_t i = 0;

  // LCD expects LSB first
  for (i=0; i<8; i++) 
  { 
    // Make sure clock starts low
    //digitalWrite(_clk, LOW);
    //*clkport &= ~clkpinmask;
    PIN_MAP[_clk].gpio_peripheral->BRR = PIN_MAP[_clk].gpio_pin;
    if (data & 0x01) 
      //digitalWrite(_mosi, HIGH);
      //*dataport |=  datapinmask;
      PIN_MAP[_mosi].gpio_peripheral->BSRR = PIN_MAP[_mosi].gpio_pin;
    else 
      //digitalWrite(_mosi, LOW);
      *dataport &= ~datapinmask;
      PIN_MAP[_mosi].gpio_peripheral->BRR = PIN_MAP[_mosi].gpio_pin;
    // Clock is active high
    //digitalWrite(_clk, HIGH);
    //*clkport |=  clkpinmask;
	// DELAY HERE??  Data needs to be setup 380ns before clk
    PIN_MAP[_clk].gpio_peripheral->BSRR = PIN_MAP[_clk].gpio_pin;
    data >>= 1; 
  }
  // Make sure clock ends low
  //digitalWrite(_clk, LOW);
  //*clkport &= ~clkpinmask;
  PIN_MAP[_clk].gpio_peripheral->BRR = PIN_MAP[_clk].gpio_pin;
}
/* ************** */
/* PUBLIC METHODS */
/* ************** */

/**************************************************************************/
/*! 
    @brief Draws a single pixel in image buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)
*/
/**************************************************************************/
void Adafruit_SharpMem::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
  if ((x >= SHARPMEM_LCDWIDTH) || (y >= SHARPMEM_LCDHEIGHT))
    return;

  if (color)
    sharpmem_buffer[(y*SHARPMEM_LCDWIDTH + x) /8] |= (1 << x % 8);
  else
    sharpmem_buffer[(y*SHARPMEM_LCDWIDTH + x) /8] &= ~(1 << x % 8);
}

/**************************************************************************/
/*! 
    @brief Gets the value (1 or 0) of the specified pixel from the buffer

    @param[in]  x
                The x position (0 based)
    @param[in]  y
                The y position (0 based)

    @return     1 if the pixel is enabled, 0 if disabled
*/
/**************************************************************************/
uint8_t Adafruit_SharpMem::getPixel(uint16_t x, uint16_t y)
{
  if ((x >=SHARPMEM_LCDWIDTH) || (y >=SHARPMEM_LCDHEIGHT)) return 0;
  return sharpmem_buffer[(y*SHARPMEM_LCDWIDTH + x) /8] & (1 << x % 8) ? 1 : 0;
}

/**************************************************************************/
/*! 
    @brief Clears the screen
*/
/**************************************************************************/
void Adafruit_SharpMem::clearDisplay() 
{
  memset(sharpmem_buffer, 0xff, (SHARPMEM_LCDWIDTH * SHARPMEM_LCDHEIGHT) / 8);
  // Send the clear screen command rather than doing a HW refresh (quicker)
  digitalWrite(_ss, HIGH);
  delayMicroseconds(6);		//SS needs to be high 6us before data - Spark is FAST!
  sendbyte(_sharpmem_vcom | SHARPMEM_BIT_CLEAR);
  sendbyteLSB(0x00);
  TOGGLE_VCOM;
  delayMicroseconds(2);		//SS needs to be low 2us after data before going high - Spark is FAST!
  digitalWrite(_ss, LOW);
}

/**************************************************************************/
/*! 
    @brief Renders the contents of the pixel buffer on the LCD
*/
/**************************************************************************/
void Adafruit_SharpMem::refresh(void) 
{
  uint16_t i, totalbytes, currentline, oldline;  
  totalbytes = (SHARPMEM_LCDWIDTH * SHARPMEM_LCDHEIGHT) / 8;

  // Send the write command
  digitalWrite(_ss, HIGH);
  delayMicroseconds(6);		//SS needs to be high 6us before data - Spark is FAST!
  sendbyte(SHARPMEM_BIT_WRITECMD | _sharpmem_vcom);
  TOGGLE_VCOM;

  // Send the address for line 1
  oldline = currentline = 1;
  sendbyteLSB(currentline);

  // Send image buffer
  for (i=0; i<totalbytes; i++)
  {
    sendbyteLSB(sharpmem_buffer[i]);
    currentline = ((i+1)/(SHARPMEM_LCDWIDTH/8)) + 1;
    if(currentline != oldline)
    {
      // Send end of line and address bytes
      sendbyteLSB(0x00);
      if (currentline <= SHARPMEM_LCDHEIGHT)
      {
        sendbyteLSB(currentline);
      }
      oldline = currentline;
    }
  }

  // Send another trailing 8 bits for the last line
  sendbyteLSB(0x00);
  delayMicroseconds(2);		//SS needs to be low 2us after data before going high - Spark is FAST!
  digitalWrite(_ss, LOW);
}
