This library allows you to restart an Arduino from software using the Watchdog Timer (as described in the avr-gcc manual FAQ). As long as you are using a recent version of the Optiboot bootloader there should be no issues with this. 

Older versions of Optiboot had some problems related to handling of the Watchdog Timer which would cause the Mega328 to go into an infinite reset loop

To reset the processor from software, simply call the `soft_restart()` function.
