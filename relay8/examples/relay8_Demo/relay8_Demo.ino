// relay8_Demo 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of most of the library functions.
//

// Include our library :)
#include <relay8.h>

// Declare what pins our relays are connected to.
// relay8 supports from 1 to 8 relays.
relay8  relay(2, 3, 4, 5, 6, 7, 8, 9);

void setup()
{ 
  // Nothing special is needed here
}

void loop()
{
  // Switch the first relay on and off a couple of times
  for (int t=0; t<3; t++)
  {
    relay.on(1);
    delay(500);
    relay.off(1);
    delay(500);
  }

  // Switch the last relay on and off a couple of times
  for (int t=0; t<3; t++)
  {
    relay.on(relay.numberOfRelays());
    delay(500);
    relay.off(relay.numberOfRelays());
    delay(500);
  }

  // Switch all relays on and off a couple of times
  for (int t=0; t<3; t++)
  {
    relay.allOn();
    delay(500);
    relay.allOff();
    delay(500);
  }
  
  // Cycle through all relays slowly from the first to the last
  // then a little faster back down again
  for (int t=0; t<3; t++)
  {
    relay.cycleUp();
    relay.cycleDown(100);
  }
  
  // Switch on all relays in sequence then switch then all off
  // again a little faster
  for (int t=0; t<3; t++)
  {
    relay.chaseUpOn(500);
    relay.chaseUpOff();
  }
  
  // Switch on all relays in sequence then switch then all off
  // again a little faster. This time in the opposite direction
  for (int t=0; t<3; t++)
  {
    relay.chaseDownOn();
    relay.chaseDownOff(100);
  }
}
