Arduino Library for 20T202DA2JA SPI VFD (vacuum fluorescent display) 

To install, click DOWNLOAD SOURCE in the top right corner, and see our tutorial at http://www.ladyada.net/library/arduino/libraries.html on Arduino Library installation

Modified Aug 17 2011 by J.P. McGlinn github user jpbot 
with help from Stephen Paine github user smpaine
 -Thanks to smpaine for pointing out some parameter testing I didn't perform in my first go.
 -Thanks to smpaine for implementing brightness in the begin method (My first thought was
  constructor/init method)
 -added brightness control (25%, 50%, 75%, 100%)
 -initial brightness can be set at the constructor or begin method.
 -begin method is called by the constructor for a 2x20 display making a call from setup() optional.
 -added example Dimmer to show off brightness control
 -removed references to LCD and LiquidCrystal so this library can be used with the
  LiquidCrystal library
 -lots of little fixes:
   --chispselect is really strobe, and is required, so all the checking for is removed
   --removed 5x10 mode because the datasheet doesn't suggest it's an option
   --assume always 8bit, removed 4bit checking
   --strobe and clock are HIGH at idle and were not set initially HIGH so first command
     was ignored
   --cleaned up some duplicate code in the begin method

Unfortunately removing the 5x10 dotmode breaks backwards compatibility with the begin method.
This wasn't used by many, but is something to be aware of. It also might make switching from
a LCD less trivial. But this only affects the begin method. Bounds checking is done on the
brightness parameter which replaced the dotmode parameter.
