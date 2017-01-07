![#Arduino Volume Library](http://i.imgur.com/muq8u3K.jpg)

** **
**Arduino tone() just got 10-bit volume control!***

**with no extra components!*

Ever needed a project to play a tone through a speaker or piezo that *wasn't* blisteringly loud? You can bring the volume down with a resistor, but what if you needed a loud alert beep at some point as well?

**Now it's no longer an issue.** :) Using ultra-fast PWM, the Volume library allows the speaker itself to act as an RC filter to produce *smooth* (**10-bit**) analog-like volume control.

----------
# Contents
- [Installation](#installation)
- [Usage](#usage)
- [Functions](#functions)
- [Supported Pins](#supported-pins)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [License and credits](#license-and-credits)

----------
# Installation

~~**With Arduino Library Manager:**~~ *COMING SOON!*

~~1. Open *Sketch > Include Library > Manage Libraries* in the Arduino IDE.~~
~~2. Search for "Volume3", (look for "Connor Nishijima") and select the latest version.~~
~~3. Click the Install button and Arduino will prepare the library and examples for you!~~

**Manual Install:**

1. Click "Clone or Download" above to get an "arduino-volume3-master.zip" file.
2. Extract it's contents to the libraries folder in your sketchbook.
3. Rename the folder from "arduino-volume3-master" to "Volume3".

----------
# Usage

Using the volume-controlled `vol.tone()` function is very similar to the Arduino `tone()`:

**Arduino:**
 - **tone**(unsigned int **pin**, unsigned int **frequency**);

**Volume3:**
 - vol.**tone**(byte **pin**, unsigned int **frequency**, unsigned int **volume**);

Volume control is limited to certain pins. See [Supported Pins](#supported-pins).

----------
Here is what you need to get started with the bare minimum:

    #include "Volume3.h" // Include the Volume library
    #define speakerPin 9

    void setup() {
      // Nothing here!
    }
    void loop() {
      uint16_t volume = 1023;
      uint16_t frequency = 440;
      while(volume > 0){
        vol.tone(speakerPin,frequency,volume);
        volume--;
        delay(10);
      }
    }

----------
# Functions

**vol.tone**(byte **pin**, unsigned int **frequency**, unsigned int **volume**);

*This is where the magic happens.* At the frequency you specify, your Arduino will analogWrite(**volume**) to the speaker with a PWM frequency of 100,000 Hz, for half the duration of a single period of the **frequency** before pulling it `LOW`. This high-speed PWM is beyond your range of hearing, (and probably the functioning range of your speaker) so it will just sound like a quieter or louder version of the input frequency!

**vol.noTone**();

This is identical in function to the standard `noTone()` function, this stops any currently playing tones.

----------
# Supported Pins

| Board                           | Pin1        | Pin2          | Tested |
|---------------------------------|-------------|---------------|--------|
| (**Uno**) ATmega168/328(pb)     | 9           | 10            | YES    |
| (**Mega**) ATmega1280/2560      | 11          | 12            | NO     |
| (**Leo/Micro**) ATmega16u2/32u4 | 9           | 10            | NO     |

----------
# Limitations
Unfortunately, cheating the Arduino's normal functions in this way means we'll lose some of them. This is also still a proof-of-concept library at this point, so it may break more functionality than I'm aware of. Sorry!

**ATmega* Only:**

I don't know if I'll have this working on ATTiny*5 boards any time soon, though it's theoretically possible on any AVR supported by the TimerOne. For now, it's only confirmed working on Arduino Uno (ATMega168/328) and Mega. (ATMega1280/2560)

**Volume is limited to certain pins:**

This is because on AVRs with Timer1 - only 2 pins are in that timer's control. It also means that while you're using Volume3, normal `analogWrite()` use probably won't work on these two pins.

----------
# Contributing
Any advice or pull requests are welcome. :)

----------
# License and Credits
**Developed by Connor Nishijima (2016)**

**Released under the [GPLv3 license](http://www.gnu.org/licenses/gpl-3.0.en.html).**
