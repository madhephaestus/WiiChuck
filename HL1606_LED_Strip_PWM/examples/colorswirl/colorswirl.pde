// Example code for fast-PWM control of HL1606 LEDs
// public domain, enjoy!

#include "HL1606stripPWM.h"
// This library uses Timer 2 for PWM counting and 3 * numLEDs bytes of RAM
// and Arduino pins 11 & 13 (Mega, 51 & 52)

// First value passed is how many LEDs are in your HL1606 strand
// fewer LEDs means less work
// This library uses hardware SPI so pins 11 and 13 are used 
// for Data and Clock output (for the Mega, pins 51 and 52)
// The latch pin (L) can be any pin but pin 10 (Arduino) or 53 (Mega)
// MUST BE AN OUTPUT!
int latchPin = 10;
HL1606stripPWM strip = HL1606stripPWM(32, latchPin); 


void setup() {
  Serial.begin(9600);
  Serial.println("hello!");

  // You can customize/control the pulse width modulation and color 
  // resolution by setting how many bits of PWM you want per LED
  // For example, 3 bits is 8 different PWM values per LED and 9 bits, 512
  // values for full color. 4 bits is 16 PWM per LED, 12 bit color with
  // 4096 different colors available.
  // Increasing the PWMbits by 1 means you need *TWICE* as much CPU !!!
  // We suggest starting with 3 and tweaking the other variables to get
  // the fastest SPI and maximum CPU. Then try upping this to 4. For short
  // strips (like 1 meter) that are ok with SPIdiv of 16, you can try 5
  strip.setPWMbits(3);
  
  // We use the built-in hardware SPI module. We can change the speed
  // of the module to push data out faster. In theory, HL1606's should work with
  // the SPI divider set to 16 but we found that this makes some strips
  // spaz out. Start with 32 and once it works try reducing it to 16
  // If you're lucky, you can even try 8 
  // Valid divider values are: 2, 4, 8, 16, 32, 64, and 128, dont try others!
  strip.setSPIdivider(32);

  // all the hard work of running the strip is done in an interrupt
  // we can configure the interrupt so that we spend more or less
  // time running the strip, letting you do other stuff like sensors
  // or an LED or whatever. Set it between 0 and 100, where 100 means
  // higher quality colorstrip display but no time for anything else.
  strip.setCPUmax(70);    // 70% is what we start at

  // For Arduinos, pin 10 MUST be an output before you begin!
  // (if using a Mega, pin 53 must be an output (instead of 10) )
  // We suggest using pin 10 for the latchPin
  strip.begin();

  // print out what the timing is (for debugging)
  double time = strip.numLEDs();    // each LED
  time *= 8;              // 8 bits of data for each LED;
  time *= strip.getSPIdivider();  // larger dividers = more time per bit
  time *= 1000;           // time in milliseconds
  time /= F_CPU;          // multiplied by how long it takes for one instruction (nverse of cpu)
  
  Serial.print("Time per LED strip write: "); 
  Serial.print(time); Serial.println(" millis");
  
  time *= 100;
  time /= strip.getCPUmax();
  
  Serial.print("Time allowed per interrupt ");
  Serial.print(time);
  Serial.println(" millis");
  
  // get that in Hz
  time = 1000 / time;
  Serial.print("Interrupt freq: "); 
  Serial.print(time);
  Serial.println(" Hz");

  // Now find the final 'color cycle' frequency
  Serial.print("Color cycle freq: "); 
  Serial.print(time / (1 << strip.getPWMbits()));
  Serial.println(" Hz");
  // Try to keep the color frequency above 60 Hz (flickery). 100 Hz or higher looks good to most people
}


uint8_t j=0;
void loop() {
 
   for (uint8_t i=0; i< strip.numLEDs() ; i++) {
     uint16_t c = Wheel((i+j) % 96);
     // the 16 bit color we get from Wheel is actually made of 5 bits RGB, we can use bitwise notation to get it out and
     // convert it to 8 bit
     strip.setLEDcolorPWM(i, (c & 0x1F) << 3, ((c>>10) & 0x1F) << 3, ((c>>5) & 0x1F) << 3);
  }
  
  j++;
  // there's only 96 colors in the 'wheel' so wrap around
  if (j > 96) { j = 0; }
  delay(100);
}

/************************** Pixel routine */
// this code is from http://www.bliptronics.com Ben Moyes's example code for LEDs, check them out!

// Create a 15 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}


//Input a value 0 to 127 to get a color value.
//The colours are a transition r - g -b - back to r
unsigned int Wheel(byte WheelPos)
{
  byte r,g,b;
  switch(WheelPos >> 5)
  {
    case 0:
      r=31- WheelPos % 32;   //Red down
      g=WheelPos % 32;      // Green up
      b=0;                  //blue off
      break; 
    case 1:
      g=31- WheelPos % 32;  //green down
      b=WheelPos % 32;      //blue up
      r=0;                  //red off
      break; 
    case 2:
      b=31- WheelPos % 32;  //blue down 
      r=WheelPos % 32;      //red up
      g=0;                  //green off
      break; 
  }
  return(Color(r,g,b));
}


