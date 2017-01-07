![#Arduino Volume Library](http://i.imgur.com/SJ35VTh.jpg)

** **
**Arduino can sing now - with no extra components.**

# [VIDEO DEMONSTRATION](https://www.youtube.com/watch?v=4wkMY6DDPDw)

Libraries like [Mozzi](http://sensorium.github.io/Mozzi/) already exist for generating beautiful tones with an Arduino. However, no other library can generate these tones without extra components or specific recording environments! By using Pulse Width Modulation at a frequency higher than most speakers (or your eardrums) can work with, we're left with an analog average of the input signal!

This means that if you feed a 25% duty cycle PWM wave into a speaker at 65,200Hz it will only react as if it were fed 25% of the input voltage.

Using this technique, we can produce many waveforms like sines, triangles and sawtooths - at any volume we want!

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

~~**With Arduino Library Manager:**~~ (Coming soon!)

~~1. Open *Sketch > Include Library > Manage Libraries* in the Arduino IDE.~~
~~2. Search for "Volume", (look for "Connor Nishijima") and select the latest version.~~
~~3. Click the Install button and Arduino will prepare the library and examples for you!~~

**Manual Install:**

1. Click "Clone or Download" above to get an "arduino-volume-2-master.zip" file.
2. Extract it's contents to the libraries folder in your sketchbook.
3. Rename the folder from "arduino-volume-2-master" to "Volume2".

----------
# Usage

Using this library's `vol.tone()` function looks very similar to the Arduino `tone()`, but the function arguments are very different:

**Arduino:**
 - **tone**(unsigned int **pin**, unsigned int **frequency**);

**Volume:**
 - vol.**tone**(unsigned int **frequency**, byte **type**, byte **volume**);

Volume2 audio is limited to certain pins. See [Supported Pins](#supported-pins).

----------
Here is what you need to get started with the bare minimum:

    #include "volume2.h" // Include the Volume2 library

    Volume vol; // Plug your speaker into the default pin for your board type:
    // https://github.com/connornishijima/arduino-volume-2#supported-pins
    
    void setup() {
      vol.tone(440, SAWTOOTH_HIGH, 64); // Sawtooth at 440 Hz, 25% Volume
    }
    void loop() {}

Of course, you can set the volume to any value between 0 and 255 you'd like, for full 8-bit control.

----------
# Functions

**Volume vol**;

This initializes the Volume2 library after import. "vol" can be any word you want, as long as it's reflected in the rest of your code.

**vol.tone**(unsigned int **frequency**, byte **type**, byte **volume**);

*This is where the magic happens.* At the **frequency** you specify, your Arduino will analogWrite(**volume**) to the speaker with a PWM frequency of 62.5KHz, to produce the waveform **type** of your choice:

- `SQUARE` | Your friendly neighborhood square wave
- `PWM_12` | PWM "square" at 12.5% duty cycle
- `PWM_25` | PWM "square" at 25%. The NES used these PWM voices!
- `SAWTOOTH` / `SAWTOOTH_HIGH` | Sawtooth wave, low and high quality
- `TRIANGLE` / `TRIANGLE_HIGH` | Triangle wave, low and high quality
- `SINE` / `SINE_HIGH` | Sine wave, low and high quality
- `CUSTOM` | Custom 32-byte array defined by the user for custom voices
- `NOISE` | Ignores **frequency**, returns volume controlled RNG

This high-speed PWM is beyond your range of hearing, (and probably the functioning range of your speaker) so it will just sound like a quieter or louder version of the input frequency.

**vol.noTone**();

This is identical in function to the standard `noTone()` function. `vol.noTone()` stops any currently playing tones, and resets Timer0 to it's default functionality.

**vol.delay**();

This replaces the standard `delay()` Arduino function, and is designed to compensate for the changes in the Timer0 clock divisor. See [Limitations](#limitations).

**vol.setCustomWave**(byte **arr[]**);

Allows the user to pass their own array of samples to be used for tone production. Array must be 32 8-bit signed values. For example, this is an array for generating several square wave octaves:

    byte wave[32] = {
        255,255,192,192,192,192,128,128,
        192,192,128,128,128,128,64,64,
        192,192,128,128,128,128,64,64,
        128,128,64,64,64,64,0,0,
    };
    vol.setCustomWave(wave);
    
With this array, setting the frequency to 220 Hz will generate 220, 440, 880, and 1760 Hz simultaneously!

**vol.getCustomSample**(byte **pos**);

This is useful for reading out the sample data for a custom waveform you've set.

----------
# Supported Pins

By default, the library uses DEFAULT_PIN for the speaker, *(changes from board to board due to Timer0 channels)* but if you need this pin for digitalWrite's, you can call *vol.alternatePin(**true**)* to use ALTERNATE_PIN instead.

| Board                           | DEFAULT_PIN | ALTERNATE_PIN | Tested |
|---------------------------------|-------------|---------------|--------|
| (**Uno**) ATmega168/328(pb)     | 5           | 6             | YES    |
| (**Mega**) ATmega1280/2560      | 4           | 13            | YES    |
| (**Leo/Micro**) ATmega16u2/32u4 | 9           | 10            | YES*   |

*I recently killed my only ATmega32u4 board while stripping it for low-power usage and don't have one to test current releases of the library. If anyone who has a working one wants to report compatibility back to me, please do so as I've only tested the initial release!

----------
# Limitations
Unfortunately, cheating the Arduino's normal functions in this way means we'll lose some of them. This is also still a proof-of-concept library at this point, so it may break more functionality than I'm aware of. Sorry!

**ATmega*** **Only:**

I don't know if I'll have this working on ATTiny*5 boards any time soon, though it's theoretically possible on any AVR with > 2 timers and ultrasonic PWM speeds. For now, it's only confirmed working on Arduino Uno (ATMega168/328) and Mega. (ATMega1280/2560)

**Volume2 is limited to certain pins:**

This is because only two pins are driven by Timer0, *which can do PWM at a frequency higher than your hearing range!* This is the main trick behind the volume function. It also means that while you're using Volume2, normal `analogWrite()` use probably won't work on these two pins.

*Now that the Mega168, 328, 1280, 2560, 16u2/32u4 and more are now supported, the supported pins differs from board to board. See [Supported Pins](#supported-pins) section.*

**Volume2 alters Timer0 for 62.5KHz PWM:**

Speaking of Timer0 - it's normally used for the `delay()`, `delayMicroseconds()`, `millis()` and `micros()` functions. Normally with Timer0 set with a divisor of 64, `delay(1000)` would wait for 1 second - but because Volume2 sets Timer0 with a divisor of 1, `delay(1000)` will now only wait for *15.625ms*! The only alternative as of now is `vol.delay()`, which works a little math to fix this.

**Volume2 does not yet offer fixed millis() or micros() functions:**

I haven't gotten around to toying with these yet. If you need to use `millis()` or `micros()` BETWEEN playing sounds, just use a `vol.noTone()` to reset Timer0 to it's default function, and `vol.tone()` to use it for Volume2 again after you're done.

**Volume2 has frequency limitations:**

If you're using `SQUARE`, `PWM_12` or `PWM_25`, the sound will be crisp anywhere from 1-3,000 Hz. With `SAWTOOTH`, `TRIANGLE` and `SINE` however, lower frequencies will sound a bit dodgy. For frequencies under 400 Hz, I reccomend using `[type]_HIGH` instead, as these voices use a 64-byte lookup table for generation, opposed to the default 16-byte ones.

----------
# Contributing
Any advice or pull requests are welcome. :)

----------
# License and Credits
**Developed by Connor Nishijima (2016)**

**Released under the [GPLv3 license](http://www.gnu.org/licenses/gpl-3.0.en.html).**
