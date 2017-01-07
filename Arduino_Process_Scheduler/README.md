# ArduinoProcessScheduler
An Cooperative Arduino Object Oriented Cooperative Process Scheduler to Replace Them All

## Features
- Fine Grained Control Over How Often a Process Runs (Periodically, Iterations, or as Often as Possible)
- Interrupt safe (add, disable, destroy, etc.. processes from interrupt routines)
- Process concurrency protection (Guarantees your process to be in a valid state when performing actions on it)
- Dynamically Add/Remove and Enable/Disable Processes
- Easily spawn new processes from within running processes
- Automatic Process Monitoring Statistics (Automatically calculates % CPU time for process)
- Truly object oriented (a Process is no longer just a callback function like other libraries, but its own object)
- Exception Handling (wait what?!)
- Scheduler can automatically terminate stuck processes

## Important Notes
- This is not a preemptive scheduler! However by not being one, processes get to use the entire stack when they run.
- This library was inspired from this [TaskScheduler](https://github.com/arkhipenko/TaskScheduler) library.

## Supported Platfroms
- AVR
- ESP8266 (No exception handling or process timeouts)


## Usage 
See [Wiki](https://github.com/wizard97/ArduinoProcessScheduler/wiki)


## License
MIT.
