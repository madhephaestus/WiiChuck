/*
   advancedSerial Library
   https://github.com/klenov/advancedSerial
*/

#ifndef ADVANCED_SERIAL
#define ADVANCED_SERIAL

#include <stdarg.h>
#include "Arduino.h"

class advancedSerial {
public:
    enum class Level { v, vv, vvv, vvvv };
private:
    bool _output_enabled;

    Level _filter_level;
    const Level default_filter_level = Level::v;

    Level _message_level;
    const Level default_message_level = Level::v;

    Print*  _printer;

    inline bool shouldBePrinted(void) { return _output_enabled && ( _message_level <= _filter_level ); }
public:
    /*! * default Constructor */
    advancedSerial()
      : _output_enabled(true),
        _filter_level(default_filter_level),
        _message_level(default_message_level),
        _printer(nullptr) {}

    void setPrinter(Print &printer) {
      _printer = &printer;
    }

    void setFilter(Level f_level) {
      _filter_level = f_level;
    }

    void on(void)  { _output_enabled = true;  }
    void off(void) { _output_enabled = false; }

    advancedSerial& level(Level m_level) {
      _message_level = m_level;
      return *this;
    }
    inline advancedSerial& l(Level m_level) { return level(m_level); }

    template <typename Type>
    advancedSerial& print(Type tX) {
      if ( shouldBePrinted() ) {
        _printer->print(tX);
      }
      return *this;
    }

    template <typename Type>
    advancedSerial& p(Type tX) {
      return print(tX);
    }

    template <typename Type>
    advancedSerial& println(Type tX) {
      if( shouldBePrinted() ) {
        print(tX);
        _printer->println();
      }
      return *this;
    }

    template <typename Type>
    advancedSerial& pln(Type tX) {
      return println(tX);
    }

    advancedSerial& print(long n, int base) {
      if ( shouldBePrinted() ) {
        _printer->print(n, base);
      }
      return *this;
    }
    inline advancedSerial& p(long n, int base) { return print(n, base); }

    advancedSerial& println(long n, int base) {
      if ( shouldBePrinted() ) {
        print(n, base);
        _printer->println();
      }
      return *this;
    }
    inline advancedSerial& pln(long n, int base) { return println(n, base); }

    advancedSerial& println(void) {
      if ( shouldBePrinted() ) {
        _printer->println();
      }
      return *this;
    }
    inline advancedSerial& pln(void) { return println(); }
};

extern advancedSerial aSerial;
typedef advancedSerial::Level Level;
#endif
