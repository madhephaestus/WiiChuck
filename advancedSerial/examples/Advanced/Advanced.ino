/*
   advancedSerial Library - Advanced Example
   https://github.com/klenov/advancedSerial
*/

#include <advancedSerial.h>

void setup() {
  Serial.begin(9600);

  aSerial.setPrinter(Serial);
  aSerial.setFilter(Level::vv);
  /* Uncomment the following line to disable the output. By defalut the ouput is on. */
  // aSerial.off();
}

void loop() {
  // This message will be printed
  aSerial.level(Level::v).println("Level::v is less verbose than the filtering threshold");
  // This message also will be printed
  aSerial.level(Level::vv).println("Level::vv is equal to the filtering threshold");

  // This message won't be printed
  aSerial.level(Level::vvv).println("Level::vvv is more verbose than the filtering threshold");
  // This message won't be printed
  aSerial.level(Level::vvvv).println("Level::vvvv is more verbose than the filtering threshold");

  /* The same using short method names. */
  aSerial.l(Level::v).pln("This message will be printed");
  aSerial.l(Level::vv).pln("This message also will be printed");

  aSerial.l(Level::vvv).pln("This message won't be printed");
  aSerial.l(Level::vvvv).pln("This message won't be printed");

  aSerial.l(Level::v).p("Look! ").pln("This message will be printed");
  
  // Consider storing long strings on flash memory rather than RAM using F() macro
  aSerial.l(Level::v).pln(F("This string will be stored in flash memory"));

  // Two empty lines
  aSerial.l(Level::v).println().pln();

  delay(3000);
}
