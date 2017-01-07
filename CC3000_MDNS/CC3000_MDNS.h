/*

CC3000 Multicast DNS 
Version 1.1
Copyright (c) 2013 Tony DiCola (tony@tonydicola.com)

This is a simple implementation of multicast DNS query support for an Arduino
and CC3000 WiFI chip.  Only support for resolving address queries is currently
implemented.

Requirements:
- Adafruit CC3000 Library: https://github.com/adafruit/Adafruit_CC3000_Library

Usage:
- Include the CC3000 Multicast DNS library in the sketch.
- Create an instance of the MDNSResponder class.
- Call the begin method in the sketch's setup and provide a domain name (without
  the '.local' suffix, i.e. just provide 'foo' to resolve 'foo.local'), and the 
  Adafruit CC3000 class instance.  Optionally provide a time to live (in seconds) 
  for the DNS record--the default is 1 hour.
- Call the update method in each iteration of the sketch's loop function.

License (MIT license):
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

*/
#ifndef CC3000_MDNS_H
#define CC3000_MDNS_H

#include "Adafruit_CC3000.h"
#include "utility/socket.h"

class MDNSResponder {
public:
  MDNSResponder();
  ~MDNSResponder();
  bool begin(const char* domain, Adafruit_CC3000& cc3000, uint32_t ttlSeconds = 3600);
  void update();

private:
  // Expected query values
  static uint8_t _queryHeader[];
  uint8_t* _expected;
  int _expectedLen;
  // Current parsing state
  int _index;
  // Response data
  uint8_t* _response;
  int _responseLen;
  // Socket for MDNS communication
  static int _mdnsSocket;
};

#endif
