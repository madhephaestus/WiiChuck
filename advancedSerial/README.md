
[![Build Status](https://travis-ci.org/klenov/advancedSerial.svg?branch=master)](https://travis-ci.org/klenov/advancedSerial)
# advancedSerial

This library provides some additions to vanilla `Serial.print()`:

**1. Chainable `print()` and `println()` methods:**

```Arduino

// you can chain print() and println() methods
aSerial.print("x = ").print(x).print("y = ").println(y);

// short method names are also available
aSerial.p("x = ").p(x).p("y = ").pln(y);
```
You cand find a complete example [here](https://github.com/klenov/advancedSerial/blob/master/examples/Basic/Basic.ino).

**2. Verbosity levels:**
There are four verbosity levels. The order in terms of verbosity, from least to most is `v, vv, vvv, vvvv`. You can choose at wich verbosity level a message will be printed and also set a filtering threshold. Only the masseges less or equally verbose to the treshold level will be printed. 
It may be easier to see this in the example:
```Arduino
void setup() {
  Serial.begin(9600);

  aSerial.setPrinter(Serial);
  aSerial.setFilter(Level::vv); // The filtering threshold is set to Level::vv
}

void loop() {
  // This message will be printed
  aSerial.level(Level::v).println("Level::v is less verbose than the filtering threshold");
  // This message also will be printed
  aSerial.level(Level::vv).println("Level::vv is equal to the filtering threshold"); 
  
  // This message won't be printed
  aSerial.level(Level::vvv).println("Level:vvv is more verbose than the filtering threshold");
  // This message won't be printed
  aSerial.level(Level::vvvv).println("Level::vvvv is more verbose than the filtering threshold");
  
  // also short method names are available
  aSerial.l(Level::vvvv).p("Oops! ").pln("This message won't be printed");
}
```

**Consider storing long strings on flash memory rather than RAM using F() macro**
```Arduino
aSerial.println(F("This string will be stored in flash memory"));

```

You can find a complete example [here](https://github.com/klenov/advancedSerial/blob/master/examples/Advanced/Advanced.ino).

You should have Arduino IDE 1.5.8 or newer to use this library or you can [manually enable C++11 in the configuration files](https://arduino.land/FAQ/content/2/49/en/can-c11-be-used-with-arduino.html).

# Public Methods
| Method               | Short alias | Description                                                   |
|----------------------|-------------|---------------------------------------------------------------|
| `setPrinter(Print)`  |             | `Print` could be a hardware or software serial                |
| `setFilter(Level)`   |             | `Level` could be one of `Level::v`, `Level::vv`, ..., `Level::vvvv`|
| `off()`              |             | Disables the output                                           |
| `on()`               |             | Enables the output                                            |
| `level(Level)`       | `l(Level)`  | Sets the message verbosity level                              |
| `print(x)`            |    `p(x)`    | The same functionality as `Serial.print()`                    |
| `println(x)`          |    `pln(x)`  | The same functionality as `Serial.println()`                  |


## Tested with
* Arduino Uno
* Arduino Nano
* Arduino Pro Micro
* Arduino Mega 2560
* Teensy 3.1
* NodeMCU 1.0
* Arduino Due

## Similar libraries
* http://playground.arduino.cc/Code/SerialDebugger
* https://github.com/felixstorm/AvrArdLogging
* https://github.com/dhylands/Arduino-logging-library
* https://github.com/AMPWorks/ArduinoLibs/tree/master/Debug



