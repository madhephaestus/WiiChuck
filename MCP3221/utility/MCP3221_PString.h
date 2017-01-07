/*
MCP3221_PString.h - Lightweight printable string class
Original Code by: Mikal Hart (http://arduiniana.org/libraries/PString/)
*/

#if 1
__asm volatile ("nop");
#endif

#ifndef MCP3221PString_h
#define MCP3221_PString_h

#include "Print.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

class MCP3221_PString : public Print {
    private:
        char *_buf, *_cur;
        size_t _size;
    public:
        #if defined(ARDUINO) && ARDUINO >= 100
            virtual size_t write(uint8_t);
        #else
            virtual void write(uint8_t);
        #endif

        // Basic constructor requires a preallocated buffer
        MCP3221_PString(char *buf, size_t size) : _buf(buf), _size(size) {
            begin();
        }
    
        // templated constructors allow inline renderings of this type: MCP3221_PString(buf, size, myfloat[, modifier]);
        template<class T> MCP3221_PString(char *buf, size_t size, T arg) : _buf(buf), _size(size) {
            begin();
            print(arg);
        }

        template<class T> MCP3221_PString(char *buf, size_t size, T arg, int modifier) : _buf(buf), _size(size) {
            begin();
            print(arg, modifier);
        }

        // returns the length of the current string, not counting the 0 terminator
        inline const size_t length() {
            return _cur - _buf;
        }

        // returns the capacity of the string
        inline const size_t capacity() {
            return _size;
        }

        // gives access to the internal string
        inline operator const char *() {
            return _buf;
        }

        // compare to another string
        bool operator==(const char *str) {
            return _size > 0 && !strcmp(_buf, str);
        }

        // call this to re-use an existing string
        void begin();

        // This function allows assignment to an arbitrary scalar value like str = myfloat;
        template<class T> inline MCP3221_PString &operator =(T arg) {
            begin();
            print(arg);
            return *this;
        }

        // Concatenation str += myfloat;
        template<class T> inline MCP3221_PString &operator +=(T arg) {
            print(arg);
            return *this;
        }

        // Safe access to sprintf-like formatting, e.g. str.format("Hi, my name is %s and I'm %d years old", name, age);
        int format(char *str, ...);
};

#endif
