/*
   advancedSerial Library - Basic Example
   https://github.com/klenov/advancedSerial
*/

#include <advancedSerial.h>

int counter = 0;

void setup() {
  Serial.begin(9600);

  aSerial.setPrinter(Serial);
  /* Uncomment the following line to disable the output. By defalut the ouput is on. */
  // aSerial.off();
}

void loop() {
  /* Printing "hello, world". */
  aSerial.print("hello, ");
  aSerial.println("world");

  // you can chain print() and println() methods
  aSerial.print("counter = ").println(counter);

  // also short method names are available
  aSerial.p("counter = ").pln(counter);

  counter++;
  delay(3000);
}
