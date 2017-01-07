/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  Serial RGB controller                                                           */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*              Reads a serial input string looking for three comma-separated integers with a newline at    */ 
/*              the end. Values should be between 0 and 255. The sketch uses those values to set the color  */
/*              of an RGB LED attached to pins PWM_0 - PWM_2.                                               */
/*              Because of common-anode: higher analogWrite level, lower brightness.                        */
/*      The circuit:                                                                                        */
/*             * Common-anode RGB LED cathodes attached to pins PWM_0, PWM_1 and PWM_2                      */
/*               (common-anode to +5V (PWM connector pin 9), red LED pin to PWM_0 (PWM connector pin 1),    */
/*               green LED pin to PWM_1 (PWM connector pin 2), blue LED pin to PWM_2 (PWM connector pin 3)).*/
/*               If it is necessary, use appropriate serial resistor with each LED.                         */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          RGB LED with common anode                                                       */
/*                          Male pin header connector 3x2 RM 2.54mm                                         */
/*                          Female header connector 3x2 RM 2.54mm                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/                    

/************************************************************************************************************/
/*                         Available PWM outputs for analogWrite() function:                                */
/*----------------------------------------------------------------------------------------------------------*/
/*   PORT        USING TIMER      PIN       FREQ          NOTE                                              */
/*----------------------------------------------------------------------------------------------------------*/
/*   PWM_0           T0           PG5       980 Hz        USED FOR SYSTEM TIMER                             */
/*   PWM_1           T0           PB7       980 Hz        USED FOR SYSTEM TIMER                             */
/*   PWM_2           T3           PE2       490 Hz        NOT USED                                          */
/*   PWM_3           T3           PE4       490 Hz        NOT USED                                          */
/*   PWM_4           T1           PB5       490 Hz        USED FOR EMoRo SERVO                              */
/*   PWM_5           T1           PB6       490 Hz        USED FOR EMoRo SERVO                              */
/*   LED_BUILTIN     T2           PB4       490 Hz        USED FOR ARDUINO TONE LIB                         */
/*   SERVO_3         T5           PL3       490 Hz        USED FOR ARDUINO SERVO LIB                        */
/*   SERVO_4         T5           PL4       490 Hz        USED FOR ARDUINO SERVO LIB                        */
/*   SERVO_5         T5           PL5       490 Hz        USED FOR ARDUINO SERVO LIB                        */
/*   EX_IO_7         T3           PE5       490 Hz        NOT USED                                          */
/*   GPP_1_B         T2           PH6       490 Hz        USED FOR ARDUINO TONE LIB                         */
/*   GPP_2_B         T4           PH5       490 Hz        USED FOR EMoRo ULTRASONIC                         */
/*   GPP_3_B         T4           PH4       490 Hz        USED FOR EMoRo ULTRASONIC                         */
/*   GPP_4_B         T4           PH3       490 Hz        USED FOR EMoRo ULTRASONIC                         */
/************************************************************************************************************/

String inString = "";                               // initialize string to hold input
int currentColor = 0;                               // initialize variable
int red, green, blue = 0;                           // initialize variable

void setup() {
  Serial.begin(9600);                               // open serial communications and wait for port to open

  // send an intro:
  Serial.println("\n\nString toInt() RGB:");        // print two new line characters (\n\n) and a constant string
  Serial.println();                                 // go to new line
  // set LED cathode pins as outputs:
  pinMode(PWM_0, OUTPUT);                           // set PWM_0 pin as OUTPUT
  pinMode(PWM_1, OUTPUT);                           // set PWM_1 pin as OUTPUT
  pinMode(PWM_2, OUTPUT);                           // set PWM_2 pin as OUTPUT
  pinMode(IO_0, OUTPUT);                            // set IO_0 pin as OUTPUT
  digitalWrite(IO_0, HIGH);                         // turn on IO_0 signal LED
}

void loop() {
  int inChar;                                       // initialize inChar variable

  if (Serial.available() > 0) {                     // if there is data available on the serial port
    inChar = Serial.read();                         // read serial input
  } 

  if (isDigit(inChar)) {                            // if recieved character is a digit
    // convert the incoming byte to a char 
    // and add it to the string:
    inString += (char)inChar;
  }

  // if you get a comma, convert to a number,
  // set the appropriate color, and increment
  // the color counter:
  if (inChar == ',') {                              // when comma is recieved
    // do something different for each value of currentColor:
    switch (currentColor) {
    case 0:    // 0 = red
      red = inString.toInt();                       // decode red value
      inString = "";                                // clear the string for new input
      break;                                        // break case
    case 1:    // 1 = green:
      green = inString.toInt();                     // decode green value
      inString = "";                                // clear the string for new input
      break;                                        // break case
    }
    currentColor++;                                 // increment current color
  }
  // if you get a newline, you know you've got
  // the last color, i.e. blue:
  if (inChar == '\n') {                             // when newline is recieved
    blue = inString.toInt();                        // decode blue color

    // set the levels of the LED.
    // subtract value from 255 because a higher
    // analogWrite level means a dimmer LED, since
    // you're raising the level on the anode:
    analogWrite(PWM_0,  255 - red);                 // dimm single color of LED to appropriate level
    analogWrite(PWM_1, 255 - green);                // dimm single color of LED to appropriate level
    analogWrite(PWM_2, 255 - blue);                 // dimm single color of LED to appropriate level

    // print the colors:
    Serial.print("Red: ");                          // print constant string
    Serial.print(red);                              // print detected red value
    Serial.print(", Green: ");                      // print constant string  
    Serial.print(green);                            // print detected green value
    Serial.print(", Blue: ");                       // print constant string  
    Serial.println(blue);                           // print detected blue value
    
    inString = "";                                  // clear the string for new input
    currentColor = 0;                               // reset the color counter
  } 
}


/*
Here's a Processing sketch that will draw a color wheel and send a serial
 string with the color you click on:
 
 // Subtractive Color Wheel with Serial
 // Based on a Processing example by Ira Greenberg. 
 // Serial output added by Tom Igoe
 // 
 // The primaries are red, yellow, and blue. The secondaries are green, 
 // purple, and orange. The tertiaries are  yellow-orange, red-orange, 
 // red-purple, blue-purple, blue-green, and yellow-green.
 // 
 // Create a shade or tint of the subtractive color wheel using
 // SHADE or TINT parameters.
 
 // Updated 29 November 2010.
 
 
 
 import processing.serial.*;
 
 int segs = 12;
 int steps = 6;
 float rotAdjust = TWO_PI / segs / 2;
 float radius;
 float segWidth;
 float interval = TWO_PI / segs;
 
 Serial myPort;
 
 void setup() {
 size(200, 200);
 background(127);
 smooth();
 ellipseMode(RADIUS);
 noStroke();
 // make the diameter 90% of the sketch area
 radius = min(width, height) * 0.45;
 segWidth = radius / steps;
 
 // swap which line is commented out to draw the other version
 // drawTintWheel();
 drawShadeWheel();
 // open the first serial port in your computer's list
 myPort = new Serial(this, Serial.list()[0], 9600);
 }
 
 
 void drawShadeWheel() {
 for (int j = 0; j < steps; j++) {
 color[] cols = { 
 color(255-(255/steps)*j, 255-(255/steps)*j, 0), 
 color(255-(255/steps)*j, (255/1.5)-((255/1.5)/steps)*j, 0), 
 color(255-(255/steps)*j, (255/2)-((255/2)/steps)*j, 0), 
 color(255-(255/steps)*j, (255/2.5)-((255/2.5)/steps)*j, 0), 
 color(255-(255/steps)*j, 0, 0), 
 color(255-(255/steps)*j, 0, (255/2)-((255/2)/steps)*j), 
 color(255-(255/steps)*j, 0, 255-(255/steps)*j), 
 color((255/2)-((255/2)/steps)*j, 0, 255-(255/steps)*j), 
 color(0, 0, 255-(255/steps)*j),
 color(0, 255-(255/steps)*j, (255/2.5)-((255/2.5)/steps)*j), 
 color(0, 255-(255/steps)*j, 0), 
 color((255/2)-((255/2)/steps)*j, 255-(255/steps)*j, 0)
 };
 for (int i = 0; i < segs; i++) {
 fill(cols[i]);
 arc(width/2, height/2, radius, radius, 
 interval*i+rotAdjust, interval*(i+1)+rotAdjust);
 }
 radius -= segWidth;
 }
 }
 
 
 void drawTintWheel() {
 for (int j = 0; j < steps; j++) {
 color[] cols = { 
 color((255/steps)*j, (255/steps)*j, 0), 
 color((255/steps)*j, ((255/1.5)/steps)*j, 0), 
 color((255/steps)*j, ((255/2)/steps)*j, 0), 
 color((255/steps)*j, ((255/2.5)/steps)*j, 0), 
 color((255/steps)*j, 0, 0), 
 color((255/steps)*j, 0, ((255/2)/steps)*j), 
 color((255/steps)*j, 0, (255/steps)*j), 
 color(((255/2)/steps)*j, 0, (255/steps)*j), 
 color(0, 0, (255/steps)*j),
 color(0, (255/steps)*j, ((255/2.5)/steps)*j), 
 color(0, (255/steps)*j, 0), 
 color(((255/2)/steps)*j, (255/steps)*j, 0)
 };
 for (int i = 0; i < segs; i++) {
 fill(cols[i]);
 arc(width/2, height/2, radius, radius, 
 interval*i+rotAdjust, interval*(i+1)+rotAdjust);
 }
 radius -= segWidth;
 }
 }
 
 void draw() {
 // nothing happens here
 }
 
 void mouseReleased() {
 // get the color of the mouse position's pixel:
 color targetColor = get(mouseX, mouseY);
 // get the component values:
 int r = int(red(targetColor));
 int g = int(green(targetColor));
 int b = int(blue(targetColor));
 // make a comma-separated string:
 String colorString = r + "," + g + "," + b + "\n";
 // send it out the serial port:
 myPort.write(colorString );
 }
 
*/

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/










