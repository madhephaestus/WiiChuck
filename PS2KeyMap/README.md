# PS2KeyMap
Arduino PS2 keyboard International Keyboard mapping from PS2KeyAdvanced and return as UTF-8

##This library requires PS2KeyAdvanced as well to work

First Public Release Version V1.0.2

Smaller footprint than any others found and more functionality.

###Platforms

    Arduino AVR - tested on Uno and Mega 2560
    Arduino SAM - tested on DUE

###Test Environment

    All platforms versions of test R3 Hardware
    Arduino V1.6.7
    DUE Board Manager package V1.6.6

##Important Hardware Notes

1 Make sure you have data and clock wires connected correctly. Clock wire MUST be connected to an interrupt pin.

  -  Do NOT use Pin 13 for either clock or data wires
  -  PS2KeyRaw requires both pins specified for begin() as in keyboard.begin( data_pin, irq_pin );
  -  Valid irq pins:
      -  Arduino Uno: 2, 3
      -  Arduino Due: All pins, except 13 (LED)
      -  Arduino Mega: 2, 3, 18, 19, 20, 21
      -  Teensy 2.0: All pins, except 13 (LED)
      -  Teensy 2.0: 5, 6, 7, 8
      -  Teensy 1.0: 0, 1, 2, 3, 4, 6, 7, 16
      -  Teensy++ 2.0: 0, 1, 2, 3, 18, 19, 36, 37
      -  Teensy++ 1.0: 0, 1, 2, 3, 18, 19, 36, 37
      -  Sanguino: 2, 10, 11

2 When using DUE or other boards with 3V3 I/O you MUST use a level translator FET or IC like Texas Instruments TXS0102 or similar as most keyboards not only operate at 5V but the two wire communications are pulled up by a resistor to 5V at the keyboard end.

###Introduction

After looking round for suitable libraries I found most were lacking in functionality and high in code and data footprint, so I created a series of PS2 Keyboard libraries. This is the third which extends the second one PS2KeyAdvanced to support different Country keyboard mapping. More countries and special mappings can be added.

The size of the keyboard mappings is smaller than other libraries as some countries can be not used, and existing mappings are only the keys that are different from US layout, so only change data is stored (in Flash).

The library takes the 16 bit integer from PS2KeyAdvanced and converts to UTF-8/ASCII honouring SHIFT, CAPS and as long as NOT a functional key like F1 to F24 or Page Up, will convert to UTF-8 equivalent, otherwise same code is returned.

Current Country mappings included (other contributions welcomed)
   * US 
   * UK/GB
   * DE - German
   * FR - French
    
You can select which map to use from your programme.

###Installation

Performed by standard zip file library inclusion into Arduino IDE

###Examples

This library has TWO examples, from simplest to most complec -

  - Internationl that uses the serial port to output the converted codes received and allow changing of keyboard mapping on the fly.
  - KeyToLCD - Example that will allow you to display converted keyboard characters on LCD connected to Arduino and allow cursor movements to move the cursor on LCD, whilst also displaying strings for keys like ESC, TAB, F1 to F12
   
Note on LCDs and some terminal emulators not all characters may be supported.

##Euro Currency Symbol

As the Euro came about *AFTER* the standardisation of UTF-8, the Euro Symbol is a supported keycode but *NOT* a conversion to UTF-8. This is just not possible.

###Contributor and Author Details

Author Paul Carpenter, PC Services

Web Site http://www.pcserviceselectronics.co.uk
