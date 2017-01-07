/* Boards.h - Hardware Abstraction Layer for Firmata library */

#ifndef BLE_Firmata_Boards_h
#define BLE_Firmata_Boards_h

#include <inttypes.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"	// for digitalRead, digitalWrite, etc
#else
#include "WProgram.h"
#endif

// Normally Servo.h must be included before Firmata.h (which then includes
// this file).  If Servo.h wasn't included, this allows the code to still
// compile, but without support for any Servos.  Hopefully that's what the
// user intended by not including Servo.h
#ifndef MAX_SERVOS
#define MAX_SERVOS 0
#endif

/*
    Firmata Hardware Abstraction Layer

Firmata is built on top of the hardware abstraction functions of Arduino,
specifically digitalWrite, digitalRead, analogWrite, analogRead, and 
pinMode.  While these functions offer simple integer pin numbers, Firmata
needs more information than is provided by Arduino.  This file provides
all other hardware specific details.  To make Firmata support a new board,
only this file should require editing.

The key concept is every "pin" implemented by Firmata may be mapped to
any pin as implemented by Arduino.  Usually a simple 1-to-1 mapping is
best, but such mapping should not be assumed.  This hardware abstraction
layer allows Firmata to implement any number of pins which map onto the
Arduino implemented pins in almost any arbitrary way.


General Constants:

These constants provide basic information Firmata requires.

TOTAL_PINS: The total number of pins Firmata implemented by Firmata.
    Usually this will match the number of pins the Arduino functions
    implement, including any pins pins capable of analog or digital.
    However, Firmata may implement any number of pins.  For example,
    on Arduino Mini with 8 analog inputs, 6 of these may be used
    for digital functions, and 2 are analog only.  On such boards,
    Firmata can implement more pins than Arduino's pinMode()
    function, in order to accommodate those special pins.  The
    Firmata protocol supports a maximum of 128 pins, so this
    constant must not exceed 128.

TOTAL_ANALOG_PINS: The total number of analog input pins implemented.
    The Firmata protocol allows up to 16 analog inputs, accessed
    using offsets 0 to 15.  Because Firmata presents the analog
    inputs using different offsets than the actual pin numbers
    (a legacy of Arduino's analogRead function, and the way the
    analog input capable pins are physically labeled on all
    Arduino boards), the total number of analog input signals
    must be specified.  16 is the maximum.

VERSION_BLINK_PIN: When Firmata starts up, it will blink the version
    number.  This constant is the Arduino pin number where a
    LED is connected.


Pin Mapping Macros:

These macros provide the mapping between pins as implemented by
Firmata protocol and the actual pin numbers used by the Arduino
functions.  Even though such mappings are often simple, pin
numbers received by Firmata protocol should always be used as
input to these macros, and the result of the macro should be
used with with any Arduino function.

When Firmata is extended to support a new pin mode or feature,
a pair of macros should be added and used for all hardware
access.  For simple 1:1 mapping, these macros add no actual
overhead, yet their consistent use allows source code which
uses them consistently to be easily adapted to all other boards
with different requirements.

IS_PIN_XXXX(pin): The IS_PIN macros resolve to true or non-zero
    if a pin as implemented by Firmata corresponds to a pin
    that actually implements the named feature.

PIN_TO_XXXX(pin): The PIN_TO macros translate pin numbers as
    implemented by Firmata to the pin numbers needed as inputs
    to the Arduino functions.  The corresponding IS_PIN macro
    should always be tested before using a PIN_TO macro, so
    these macros only need to handle valid Firmata pin
    numbers for the named feature.


Port Access Inline Funtions:

For efficiency, Firmata protocol provides access to digital
input and output pins grouped by 8 bit ports.  When these
groups of 8 correspond to actual 8 bit ports as implemented
by the hardware, these inline functions can provide high
speed direct port access.  Otherwise, a default implementation
using 8 calls to digitalWrite or digitalRead is used.

When porting Firmata to a new board, it is recommended to
use the default functions first and focus only on the constants
and macros above.  When those are working, if optimized port
access is desired, these inline functions may be extended.
The recommended approach defines a symbol indicating which
optimization to use, and then conditional complication is
used within these functions.

readPort(port, bitmask):  Read an 8 bit port, returning the value.
   port:    The port number, Firmata pins port*8 to port*8+7
   bitmask: The actual pins to read, indicated by 1 bits.

writePort(port, value, bitmask):  Write an 8 bit port.
   port:    The port number, Firmata pins port*8 to port*8+7
   value:   The 8 bit value to write
   bitmask: The actual pins to write, indicated by 1 bits.
*/

/*==============================================================================
 * Board Specific Configuration
 *============================================================================*/


#define VERSION_BLINK_PIN       99
#define ARDUINO_PINOUT_OPTIMIZE 0




#endif /* BLE_Firmata_Boards_h */

