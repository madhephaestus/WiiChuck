CC3000 Multicast DNS
====================

Version 1.1

This is a simple implementation of multicast DNS query support for an Arduino
and CC3000 WiFI chip.  Only support for resolving address queries is currently
implemented.

Requirements
------------
- [Adafruit CC3000 Library](https://github.com/adafruit/Adafruit_CC3000_Library)
- MDNS support in your operating system/client machines:
  - For Mac OSX support is built in through Bonjour already.
  - For Linux, install [Avahi](http://avahi.org/).
  - For Windows, install [Bonjour](http://www.apple.com/support/bonjour/).

Usage
-----
1. Download this repository as a zip (button on the right) and follow [these instructions to install into Arduino](http://arduino.cc/en/Guide/Libraries).
2. Include the CC3000 Multicast DNS library in the sketch.
3. Create an instance of the MDNSResponder class.
4. Call the begin method in the sketch's setup and provide a domain name (without
   the '.local' suffix, i.e. just provide 'foo' to resolve 'foo.local'), and the 
   Adafruit CC3000 class instance.  Optionally provide a time to live (in seconds) 
   for the DNS record--the default is 1 hour.
5. Call the update method in each iteration of the sketch's loop function.

See the included MDNS echo server example for a full example (make sure you have the latest
version of Adafruit's CC3000 library with server support).

License
-------
Copyright (c) 2013 Tony DiCola (tony@tonydicola.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.