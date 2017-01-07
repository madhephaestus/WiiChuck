#include <PulsePosition.h>

// Two wire output is intended to connect to a 74HCT164 shift register chip
//
// 74HCT164 pinout:
//
//  pins   Function       Recommended Connection
//  ----   --------       ----------------------
//  1,2    Serial Input   Connect to framePin
//    8    Clock Input    Connect to txPin
//    9    Clear Input    Connect to pin 2 and a 10K resistor to ground
//  3-6    Outputs        
// 10-13   Outputs
//    7    Ground
//   14    +5V            VIN or VUSB on Teensy 3.1
//
// The clear pin (pin 9) on the 74HCT164 can be used to assure proper startup
// without false pulses.  Connect a 10K resistor to ground, so the pin will
// be low (holding all the outputs low) until Teensy begins sending updates.
// If startup behavior is not a concern, pin 9 may be +3.3V or +5V.
//
// For 16 outputs, two 74HCT164 chips may be used.  Connect pin 13 from the
// first chip to pins 1+2 on the second chip.  Both should receive the same
// Clock and Clear signals.

PulsePositionOutput myOut;
// You can create up to 8 PulsePositionOutput objects, each sending 16 signals,
// for a total of 128 servo motors controlled.

void setup() {
  myOut.begin(5, 7);  // txPin = 5, framePin = 7
  pinMode(2, OUTPUT);
  pinMode(2, HIGH);   // pin 2 holds 74HCT164 outputs low until we're ready
  myOut.write(1, 800.0);
  myOut.write(2, 1721);
  myOut.write(3, 912.12);
}

void loop() {
}

