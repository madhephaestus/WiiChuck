# AD5241/2 Arduino driver

Created & maintained by Max Chan.

## Introduction

This driver interfaces the microcontroller with Analog Devices AD5241/2 digital
potentiometers.

The AD5280/2 series are protocol compatible with AD5241/2, so the same library
can be used.

## Device addressing

The devices are addressed using the I2C address assigned using the AD1/0 pins.
Individual potentiometers inside AD5242 are addressed as devices 0 and 1.

The two digital output pins can be accessed using `digitalWrite`.

## Shutdown Mode and Reset

Those digi-pots can enter shut down mode. To enter this mode, write
`ADC_SHUTDOWN` into the pot. Writing any other value will take the gate out of
shutdown mode as long as it is not inhibited by the hardware shutdown line.

To reset the pot, write `ADC_RESET`

## Using the Driver

The driver uses a similar syntax as the default PWM outputs, albeit with an
object.

```Arduino
#include <Wire.h>
#include <AD5241.h>

AD5241 dac(Wire, 0x2c);

void setup(void)
{
  dac.begin();
}

void loop(void)
{
  dac.analogWrite(0, millis());
}
```

## License

This library is licensed under the 3-clause BSD license.
