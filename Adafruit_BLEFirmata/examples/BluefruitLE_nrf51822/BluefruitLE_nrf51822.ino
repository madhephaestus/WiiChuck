#include <Servo.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BLE_Firmata.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

// Change this to whatever is the Serial console you want, either Serial or SerialUSB
#define FIRMATADEBUG    Serial
// Pause for Serial console before beginning?
#define WAITFORSERIAL   true
// Print all BLE interactions?
#define VERBOSE_MODE    false


/************************  CONFIGURATION SECTION ***********************************/
/*
Don't forget to also change the BluefruitConfig.h for the SPI or UART connection
and pinout you are using!

Then below, you can edit the list of pins that are available. Remove any pins
that are used for accessories or for talking to the BLE module!
*/

/************** For Bluefruit Micro or Feather 32u4 Bluefruit ************/
uint8_t boards_digitaliopins[] = {0,1,2,3,5,6,9,10,11,12,13,A0,A1,A2,A3,A4,A5};

/************** For UNO + nRF58122 SPI & shield ************/
//uint8_t boards_digitaliopins[] = {2, 3, 5, 6, 9, 10, A0, A1, A2, A3, A4, A5}; 

/************** For Bluefruit M0 Bluefruit ************/
//uint8_t boards_digitaliopins[] = {0,1,5,6,9,10,11,12,13,20,21,A0,A1,A2,A3,A4,A5};

#if defined(__AVR_ATmega328P__) 
  // Standard setup for UNO, no need to tweak
  uint8_t boards_analogiopins[] = {A0, A1, A2, A3, A4, A5};  // A0 == digital 14, etc
  uint8_t boards_pwmpins[] = {3, 5, 6, 9, 10, 11};
  uint8_t boards_servopins[] = {9, 10};
  uint8_t boards_i2cpins[] = {SDA, SCL};
#elif defined(__AVR_ATmega32U4__)
  uint8_t boards_analogiopins[] = {A0, A1, A2, A3, A4, A5};  // A0 == digital 14, etc
  uint8_t boards_pwmpins[] = {3, 5, 6, 9, 10, 11, 13};
  uint8_t boards_servopins[] = {9, 10};
  uint8_t boards_i2cpins[] = {SDA, SCL};
#elif defined(__SAMD21G18A__)
  #define SDA PIN_WIRE_SDA
  #define SCL PIN_WIRE_SCL
  uint8_t boards_analogiopins[] = {PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5,PIN_A6, PIN_A7};  // A0 == digital 14, etc
  uint8_t boards_pwmpins[] = {3,4,5,6,8,10,11,12,A0,A1,A2,A3,A4,A5};
  uint8_t boards_servopins[] = {9, 10};
  uint8_t boards_i2cpins[] = {SDA, SCL};
  #define NUM_DIGITAL_PINS 26
#endif


#define TOTAL_PINS     NUM_DIGITAL_PINS   /* highest number in boards_digitaliopins MEMEFIXME:automate */
#define TOTAL_PORTS    ((TOTAL_PINS + 7) / 8)


/***********************************************************/


#include "Adafruit_BLE_Firmata_Boards.h"

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"


// Create the bluefruit object, either software serial...uncomment these lines
/*
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART bluefruit(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
*/

/* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
// Adafruit_BluefruitLE_UART bluefruit(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);

/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI bluefruit(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

/* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
//Adafruit_BluefruitLE_SPI bluefruit(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
//                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
//                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);


#define AUTO_INPUT_PULLUPS true

// our current connection status
boolean lastBTLEstatus, BTLEstatus;

// make one instance for the user to use
Adafruit_BLE_FirmataClass BLE_Firmata = Adafruit_BLE_FirmataClass(bluefruit);

// A small helper
void error(const __FlashStringHelper*err) {
  FIRMATADEBUG.println(err);
  while (1);
}

/*==============================================================================
 * GLOBAL VARIABLES
 *============================================================================*/

/* analog inputs */
int analogInputsToReport = 0; // bitwise array to store pin reporting
int lastAnalogReads[NUM_ANALOG_INPUTS];

/* digital input ports */
byte reportPINs[TOTAL_PORTS];       // 1 = report this port, 0 = silence
byte previousPINs[TOTAL_PORTS];     // previous 8 bits sent

/* pins configuration */
byte pinConfig[TOTAL_PINS];         // configuration of every pin
byte portConfigInputs[TOTAL_PORTS]; // each bit: 1 = pin in INPUT, 0 = anything else
int pinState[TOTAL_PINS];           // any value that has been written

/* timer variables */
unsigned long currentMillis;        // store the current value from millis()
unsigned long previousMillis;       // for comparison with currentMillis
int samplingInterval = 200;          // how often to run the main loop (in ms)
#define MINIMUM_SAMPLE_DELAY 150
#define ANALOG_SAMPLE_DELAY 50


/* i2c data */
struct i2c_device_info {
  byte addr;
  byte reg;
  byte bytes;
};

/* for i2c read continuous more */
i2c_device_info query[MAX_QUERIES];

byte i2cRxData[32];
boolean isI2CEnabled = false;
signed char queryIndex = -1;
unsigned int i2cReadDelayTime = 0;  // default delay time between i2c read request and Wire.requestFrom()

Servo servos[MAX_SERVOS];
/*==============================================================================
 * FUNCTIONS
 *============================================================================*/

void readAndReportData(byte address, int theRegister, byte numBytes) {
  // allow I2C requests that don't require a register read
  // for example, some devices using an interrupt pin to signify new data available
  // do not always require the register read so upon interrupt you call Wire.requestFrom()  
  if (theRegister != REGISTER_NOT_SPECIFIED) {
    Wire.beginTransmission(address);
    #if ARDUINO >= 100
    Wire.write((byte)theRegister);
    #else
    Wire.send((byte)theRegister);
    #endif
    Wire.endTransmission();
    delayMicroseconds(i2cReadDelayTime);  // delay is necessary for some devices such as WiiNunchuck
  } else {
    theRegister = 0;  // fill the register with a dummy value
  }

  Wire.requestFrom(address, numBytes);  // all bytes are returned in requestFrom

  // check to be sure correct number of bytes were returned by slave
  if(numBytes == Wire.available()) {
    i2cRxData[0] = address;
    i2cRxData[1] = theRegister;
    for (int i = 0; i < numBytes; i++) {
      #if ARDUINO >= 100
      i2cRxData[2 + i] = Wire.read();
      #else
      i2cRxData[2 + i] = Wire.receive();
      #endif
    }
  }
  else {
    if(numBytes > Wire.available()) {
      BLE_Firmata.sendString("I2C Read Error: Too many bytes received");
    } else {
      BLE_Firmata.sendString("I2C Read Error: Too few bytes received"); 
    }
  }

  // send slave address, register and received bytes
  BLE_Firmata.sendSysex(SYSEX_I2C_REPLY, numBytes + 2, i2cRxData);
}

void outputPort(byte portNumber, byte portValue, byte forceSend)
{
  // pins not configured as INPUT are cleared to zeros
  portValue = portValue & portConfigInputs[portNumber];
  // only send if the value is different than previously sent
  if(forceSend || previousPINs[portNumber] != portValue) {
    //FIRMATADEBUG.print(F("Sending update for port ")); FIRMATADEBUG.print(portNumber); FIRMATADEBUG.print(" = 0x"); FIRMATADEBUG.println(portValue, HEX);
    BLE_Firmata.sendDigitalPort(portNumber, portValue);
    previousPINs[portNumber] = portValue;
  }
}

/* -----------------------------------------------------------------------------
 * check all the active digital inputs for change of state, then add any events
 * to the Serial output queue using () */
void checkDigitalInputs(boolean forceSend = false)
{
  /* Using non-looping code allows constants to be given to readPort().
   * The compiler will apply substantial optimizations if the inputs
   * to readPort() are compile-time constants. */
  for (uint8_t i=0; i<TOTAL_PORTS; i++) {
    if (reportPINs[i]) {
     // FIRMATADEBUG.print("Reporting on port "); FIRMATADEBUG.print(i); FIRMATADEBUG.print(" mask 0x"); FIRMATADEBUG.println(portConfigInputs[i], HEX);
      uint8_t x = BLE_Firmata.readPort(i, portConfigInputs[i]);
     // FIRMATADEBUG.print("Read 0x"); FIRMATADEBUG.println(x, HEX);
      outputPort(i, x, forceSend);
    }
  }
}

// -----------------------------------------------------------------------------
/* sets the pin mode to the correct state and sets the relevant bits in the
 * two bit-arrays that track Digital I/O and PWM status
 */
void setPinModeCallback(byte pin, int mode)
{
  //FIRMATADEBUG.print("Setting pin #"); FIRMATADEBUG.print(pin); FIRMATADEBUG.print(" to "); FIRMATADEBUG.println(mode);
  if ((pinConfig[pin] == I2C) && (isI2CEnabled) && (mode != I2C)) {
    // disable i2c so pins can be used for other functions
    // the following if statements should reconfigure the pins properly
    disableI2CPins();
  }
  if (BLE_Firmata.IS_PIN_SERVO(pin) && mode != SERVO && servos[BLE_Firmata.PIN_TO_SERVO(pin)].attached()) {
    servos[BLE_Firmata.PIN_TO_SERVO(pin)].detach();
  }
  if (BLE_Firmata.IS_PIN_ANALOG(pin)) {
    reportAnalogCallback(BLE_Firmata.PIN_TO_ANALOG(pin), mode == ANALOG ? 1 : 0); // turn on/off reporting
  }
  if (BLE_Firmata.IS_PIN_DIGITAL(pin)) {
    if (mode == INPUT) {
      portConfigInputs[pin/8] |= (1 << (pin & 7));
    } else {
      portConfigInputs[pin/8] &= ~(1 << (pin & 7));
    }
   // FIRMATADEBUG.print(F("Setting pin #")); FIRMATADEBUG.print(pin); FIRMATADEBUG.print(F(" port config mask to = 0x")); 
   // FIRMATADEBUG.println(portConfigInputs[pin/8], HEX);
  }
  pinState[pin] = 0;
  switch(mode) {
  case ANALOG:
    if (BLE_Firmata.IS_PIN_ANALOG(pin)) {
      FIRMATADEBUG.print(F("Set pin #")); FIRMATADEBUG.print(pin); FIRMATADEBUG.println(F(" to analog"));
      if (BLE_Firmata.IS_PIN_DIGITAL(pin)) {
        pinMode(BLE_Firmata.PIN_TO_DIGITAL(pin), INPUT); // disable output driver
      }
      pinConfig[pin] = ANALOG;
      lastAnalogReads[BLE_Firmata.PIN_TO_ANALOG(pin)] = -1;
    }
    break;
  case INPUT:
    if (BLE_Firmata.IS_PIN_DIGITAL(pin)) {
      FIRMATADEBUG.print(F("Set pin #")); FIRMATADEBUG.print(pin); FIRMATADEBUG.println(F(" to input"));
      
      if (AUTO_INPUT_PULLUPS) {
        pinMode(BLE_Firmata.PIN_TO_DIGITAL(pin), INPUT_PULLUP); // disable output driver
      } else {
        pinMode(BLE_Firmata.PIN_TO_DIGITAL(pin), INPUT); // disable output driver
      }
      pinConfig[pin] = INPUT;
      
      // force sending state immediately
      //delay(10);
      //checkDigitalInputs(true);  
    }
    break;
  case OUTPUT:
    if (BLE_Firmata.IS_PIN_DIGITAL(pin)) {
      FIRMATADEBUG.print(F("Set pin #")); FIRMATADEBUG.print(pin); FIRMATADEBUG.println(F(" to output"));
      digitalWrite(BLE_Firmata.PIN_TO_DIGITAL(pin), LOW); // disable PWM
      pinMode(BLE_Firmata.PIN_TO_DIGITAL(pin), OUTPUT);
      pinConfig[pin] = OUTPUT;
    }
    break;
  case PWM:
    if (BLE_Firmata.IS_PIN_PWM(pin)) {
      FIRMATADEBUG.print(F("Set pin #")); FIRMATADEBUG.print(pin); FIRMATADEBUG.println(F(" to PWM"));
      pinMode(BLE_Firmata.PIN_TO_PWM(pin), OUTPUT);
      analogWrite(BLE_Firmata.PIN_TO_PWM(pin), 0);
      pinConfig[pin] = PWM;
    }
    break;
  case SERVO:
    if (BLE_Firmata.IS_PIN_SERVO(pin)) {
      pinConfig[pin] = SERVO;
      if (!servos[BLE_Firmata.PIN_TO_SERVO(pin)].attached()) {
          servos[BLE_Firmata.PIN_TO_SERVO(pin)].attach(BLE_Firmata.PIN_TO_DIGITAL(pin));
      }
    }
    break;
  case I2C:
    if (BLE_Firmata.IS_PIN_I2C(pin)) {
      // mark the pin as i2c
      // the user must call I2C_CONFIG to enable I2C for a device
      pinConfig[pin] = I2C;
    }
    break;
  default:
    FIRMATADEBUG.print(F("Unknown pin mode")); // TODO: put error msgs in EEPROM
  }
  // TODO: save status to EEPROM here, if changed
}

void analogWriteCallback(byte pin, int value)
{
  if (pin < TOTAL_PINS) {
    switch(pinConfig[pin]) {
    case SERVO:
      if (BLE_Firmata.IS_PIN_SERVO(pin))
        servos[BLE_Firmata.PIN_TO_SERVO(pin)].write(value);
        pinState[pin] = value;
      break;
    case PWM:
      if (BLE_Firmata.IS_PIN_PWM(pin))
        analogWrite(BLE_Firmata.PIN_TO_PWM(pin), value);
        FIRMATADEBUG.print("pwm("); FIRMATADEBUG.print(BLE_Firmata.PIN_TO_PWM(pin)); FIRMATADEBUG.print(","); FIRMATADEBUG.print(value); FIRMATADEBUG.println(")");
        pinState[pin] = value;
      break;
    }
  }
}

void digitalWriteCallback(byte port, int value)
{
  //FIRMATADEBUG.print("DWCx"); FIRMATADEBUG.print(port, HEX); FIRMATADEBUG.print(" "); FIRMATADEBUG.println(value);
  byte pin, lastPin, mask=1, pinWriteMask=0;

  if (port < TOTAL_PORTS) {
    // create a mask of the pins on this port that are writable.
    lastPin = port*8+8;
    if (lastPin > TOTAL_PINS) lastPin = TOTAL_PINS;
    for (pin=port*8; pin < lastPin; pin++) {
      // do not disturb non-digital pins (eg, Rx & Tx)
      if (BLE_Firmata.IS_PIN_DIGITAL(pin)) {
        // only write to OUTPUT 
        // do not touch pins in PWM, ANALOG, SERVO or other modes
        if (pinConfig[pin] == OUTPUT) {
          pinWriteMask |= mask;
          pinState[pin] = ((byte)value & mask) ? 1 : 0;
        }
      }
      mask = mask << 1;
    }
    FIRMATADEBUG.print(F("Write digital port #")); FIRMATADEBUG.print(port); 
    FIRMATADEBUG.print(F(" = 0x")); FIRMATADEBUG.print(value, HEX);
    FIRMATADEBUG.print(F(" mask = 0x")); FIRMATADEBUG.println(pinWriteMask, HEX);
    BLE_Firmata.writePort(port, (byte)value, pinWriteMask);
  }
}


// -----------------------------------------------------------------------------
/* sets bits in a bit array (int) to toggle the reporting of the analogIns
 */
//void FirmataClass::setAnalogPinReporting(byte pin, byte state) {
//}
void reportAnalogCallback(byte analogPin, int value)
{
  if (analogPin < BLE_Firmata._num_analogiopins) {
    if(value == 0) {
      analogInputsToReport = analogInputsToReport &~ (1 << analogPin);
      FIRMATADEBUG.print(F("Stop reporting analog pin #")); FIRMATADEBUG.println(analogPin);
    } else {
      analogInputsToReport |= (1 << analogPin);
      FIRMATADEBUG.print(F("Will report analog pin #")); FIRMATADEBUG.println(analogPin);
    }
  }
  // TODO: save status to EEPROM here, if changed
}

void reportDigitalCallback(byte port, int value)
{
  if (port < TOTAL_PORTS) {
    //FIRMATADEBUG.print(F("Will report 0x")); FIRMATADEBUG.print(value, HEX); FIRMATADEBUG.print(F(" digital mask on port ")); FIRMATADEBUG.println(port);
    reportPINs[port] = (byte)value;
  }
  // do not disable analog reporting on these 8 pins, to allow some
  // pins used for digital, others analog.  Instead, allow both types
  // of reporting to be enabled, but check if the pin is configured
  // as analog when sampling the analog inputs.  Likewise, while
  // scanning digital pins, portConfigInputs will mask off values from any
  // pins configured as analog
}

/*==============================================================================
 * SYSEX-BASED commands
 *============================================================================*/

void sysexCallback(byte command, byte argc, byte *argv)
{
  byte mode;
  byte slaveAddress;
  byte slaveRegister;
  byte data;
  unsigned int delayTime; 
  
  FIRMATADEBUG.println("********** Sysex callback");
  switch(command) {
  case I2C_REQUEST:
    mode = argv[1] & I2C_READ_WRITE_MODE_MASK;
    if (argv[1] & I2C_10BIT_ADDRESS_MODE_MASK) {
      //BLE_Firmata.sendString("10-bit addressing mode is not yet supported");
      FIRMATADEBUG.println(F("10-bit addressing mode is not yet supported"));
      return;
    }
    else {
      slaveAddress = argv[0];
    }

    switch(mode) {
    case I2C_WRITE:
      Wire.beginTransmission(slaveAddress);
      for (byte i = 2; i < argc; i += 2) {
        data = argv[i] + (argv[i + 1] << 7);
        #if ARDUINO >= 100
        Wire.write(data);
        #else
        Wire.send(data);
        #endif
      }
      Wire.endTransmission();
      delayMicroseconds(70);
      break;
    case I2C_READ:
      if (argc == 6) {
        // a slave register is specified
        slaveRegister = argv[2] + (argv[3] << 7);
        data = argv[4] + (argv[5] << 7);  // bytes to read
        readAndReportData(slaveAddress, (int)slaveRegister, data);
      }
      else {
        // a slave register is NOT specified
        data = argv[2] + (argv[3] << 7);  // bytes to read
        readAndReportData(slaveAddress, (int)REGISTER_NOT_SPECIFIED, data);
      }
      break;
    case I2C_READ_CONTINUOUSLY:
      if ((queryIndex + 1) >= MAX_QUERIES) {
        // too many queries, just ignore
        BLE_Firmata.sendString("too many queries");
        break;
      }
      queryIndex++;
      query[queryIndex].addr = slaveAddress;
      query[queryIndex].reg = argv[2] + (argv[3] << 7);
      query[queryIndex].bytes = argv[4] + (argv[5] << 7);
      break;
    case I2C_STOP_READING:
	  byte queryIndexToSkip;      
      // if read continuous mode is enabled for only 1 i2c device, disable
      // read continuous reporting for that device
      if (queryIndex <= 0) {
        queryIndex = -1;        
      } else {
        // if read continuous mode is enabled for multiple devices,
        // determine which device to stop reading and remove it's data from
        // the array, shifiting other array data to fill the space
        for (byte i = 0; i < queryIndex + 1; i++) {
          if (query[i].addr = slaveAddress) {
            queryIndexToSkip = i;
            break;
          }
        }
        
        for (byte i = queryIndexToSkip; i<queryIndex + 1; i++) {
          if (i < MAX_QUERIES) {
            query[i].addr = query[i+1].addr;
            query[i].reg = query[i+1].addr;
            query[i].bytes = query[i+1].bytes; 
          }
        }
        queryIndex--;
      }
      break;
    default:
      break;
    }
    break;
  case I2C_CONFIG:
    delayTime = (argv[0] + (argv[1] << 7));

    if(delayTime > 0) {
      i2cReadDelayTime = delayTime;
    }

    if (!isI2CEnabled) {
      enableI2CPins();
    }
    
    break;
  case SERVO_CONFIG:
    if(argc > 4) {
      // these vars are here for clarity, they'll optimized away by the compiler
      byte pin = argv[0];
      int minPulse = argv[1] + (argv[2] << 7);
      int maxPulse = argv[3] + (argv[4] << 7);

      if (BLE_Firmata.IS_PIN_SERVO(pin)) {
        if (servos[BLE_Firmata.PIN_TO_SERVO(pin)].attached())
          servos[BLE_Firmata.PIN_TO_SERVO(pin)].detach();
        servos[BLE_Firmata.PIN_TO_SERVO(pin)].attach(BLE_Firmata.PIN_TO_DIGITAL(pin), minPulse, maxPulse);
        setPinModeCallback(pin, SERVO);
      }
    }
    break;
  case SAMPLING_INTERVAL:
    if (argc > 1) {
      samplingInterval = argv[0] + (argv[1] << 7);
      if (samplingInterval < MINIMUM_SAMPLING_INTERVAL) {
        samplingInterval = MINIMUM_SAMPLING_INTERVAL;
      }      
    } else {
      //BLE_Firmata.sendString("Not enough data");
    }
    break;
  case EXTENDED_ANALOG:
    if (argc > 1) {
      int val = argv[1];
      if (argc > 2) val |= (argv[2] << 7);
      if (argc > 3) val |= (argv[3] << 14);
      analogWriteCallback(argv[0], val);
    }
    break;
  case CAPABILITY_QUERY:
    bluefruit.write(START_SYSEX);
    bluefruit.write(CAPABILITY_RESPONSE);

    //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(START_SYSEX, HEX); FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(CAPABILITY_RESPONSE, HEX);
    delay(10);
    for (byte pin=0; pin < TOTAL_PINS; pin++) {
      //FIRMATADEBUG.print("\t#"); FIRMATADEBUG.println(pin);
      if (BLE_Firmata.IS_PIN_DIGITAL(pin)) {
        bluefruit.write((byte)INPUT);
        bluefruit.write(1);
        bluefruit.write((byte)OUTPUT);
        bluefruit.write(1);

        /*
        FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(INPUT, HEX);
        FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(1, HEX);
        FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(OUTPUT, HEX);
        FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(1, HEX);
        */
        delay(20);
      } else {
        bluefruit.write(127);
        //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(127, HEX);
        delay(20);
        continue;
      }
      if (BLE_Firmata.IS_PIN_ANALOG(pin)) {
        bluefruit.write(ANALOG);
        bluefruit.write(10);
        
        //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(ANALOG, HEX);  FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(10, HEX);
        delay(20);
      }
      if (BLE_Firmata.IS_PIN_PWM(pin)) {
        bluefruit.write(PWM);
        bluefruit.write(8);

        //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(PWM, HEX); FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(8, HEX);
        delay(20);
      }
      if (BLE_Firmata.IS_PIN_SERVO(pin)) {
        bluefruit.write(SERVO);
        bluefruit.write(14);

        //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.print(SERVO, HEX);FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(14, HEX);
        delay(20);
      }
      if (BLE_Firmata.IS_PIN_I2C(pin)) {
        bluefruit.write(I2C);
        bluefruit.write(1);  // to do: determine appropriate value 
        delay(20);
      }
      bluefruit.write(127);
      //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(127, HEX);
    }
    bluefruit.write(END_SYSEX);
    //FIRMATADEBUG.print(" 0x"); FIRMATADEBUG.println(END_SYSEX, HEX);
    break;
  case PIN_STATE_QUERY:
    if (argc > 0) {
      byte pin=argv[0];
      bluefruit.write(START_SYSEX);
      bluefruit.write(PIN_STATE_RESPONSE);
      bluefruit.write(pin);
      if (pin < TOTAL_PINS) {
        bluefruit.write((byte)pinConfig[pin]);
	bluefruit.write((byte)pinState[pin] & 0x7F);
	if (pinState[pin] & 0xFF80) bluefruit.write((byte)(pinState[pin] >> 7) & 0x7F);
	if (pinState[pin] & 0xC000) bluefruit.write((byte)(pinState[pin] >> 14) & 0x7F);
      }
      bluefruit.write(END_SYSEX);
    }
    break;
  case ANALOG_MAPPING_QUERY:
    FIRMATADEBUG.println("Analog mapping query");
    bluefruit.write(START_SYSEX);
    bluefruit.write(ANALOG_MAPPING_RESPONSE);
    for (byte pin=0; pin < TOTAL_PINS; pin++) {
      bluefruit.write(BLE_Firmata.IS_PIN_ANALOG(pin) ? BLE_Firmata.PIN_TO_ANALOG(pin) : 127);
    }
    bluefruit.write(END_SYSEX);
    break;
  }
}

void enableI2CPins()
{
  byte i;
  // is there a faster way to do this? would probaby require importing 
  // Arduino.h to get SCL and SDA pins
  for (i=0; i < TOTAL_PINS; i++) {
    if(BLE_Firmata.IS_PIN_I2C(i)) {
      // mark pins as i2c so they are ignore in non i2c data requests
      setPinModeCallback(i, I2C);
    } 
  }
   
  isI2CEnabled = true; 
  
  // is there enough time before the first I2C request to call this here?
  Wire.begin();
}

/* disable the i2c pins so they can be used for other functions */
void disableI2CPins() {
    isI2CEnabled = false;
    // disable read continuous mode for all devices
    queryIndex = -1;
    // uncomment the following if or when the end() method is added to Wire library
    // Wire.end();
}

/*==============================================================================
 * SETUP()
 *============================================================================*/

void systemResetCallback()
{
  // initialize a defalt state
  FIRMATADEBUG.println(F("***RESET***"));
  // TODO: option to load config from EEPROM instead of default
  if (isI2CEnabled) {
  	disableI2CPins();
  }
  for (byte i=0; i < TOTAL_PORTS; i++) {
    reportPINs[i] = false;      // by default, reporting off
    portConfigInputs[i] = 0;	// until activated
    previousPINs[i] = 0;
  }
  // pins with analog capability default to analog input
  // otherwise, pins default to digital output
  for (byte i=0; i < TOTAL_PINS; i++) {
    if (BLE_Firmata.IS_PIN_ANALOG(i)) {
      // turns off pullup, configures everything
      setPinModeCallback(i, ANALOG);
    } else {
      // sets the output to 0, configures portConfigInputs
      setPinModeCallback(i, INPUT);
    }
  }
  // by default, do not report any analog inputs
  analogInputsToReport = 0;

  /* send digital inputs to set the initial state on the host computer,
   * since once in the loop(), this firmware will only send on change */
  /*
  TODO: this can never execute, since no pins default to digital input
        but it will be needed when/if we support EEPROM stored config
  for (byte i=0; i < TOTAL_PORTS; i++) {
    outputPort(i, readPort(i, portConfigInputs[i]), true);
  }
  */
}

void setup() 
{
  if (WAITFORSERIAL) {
    while (!FIRMATADEBUG) delay(1);
  }
  
  FIRMATADEBUG.begin(9600);
  FIRMATADEBUG.println(F("Adafruit Bluefruit LE Firmata test"));
  
  FIRMATADEBUG.print("Total pins: "); FIRMATADEBUG.println(NUM_DIGITAL_PINS);
  FIRMATADEBUG.print("Analog pins: "); FIRMATADEBUG.println(sizeof(boards_analogiopins));
  //for (uint8_t i=0; i<sizeof(boards_analogiopins); i++) {
  //  FIRMATADEBUG.println(boards_analogiopins[i]);
  //}
  
  BLE_Firmata.setUsablePins(boards_digitaliopins, sizeof(boards_digitaliopins), 
    boards_analogiopins, sizeof(boards_analogiopins),
    boards_pwmpins, sizeof(boards_pwmpins),
    boards_servopins, sizeof(boards_servopins), SDA, SCL);

  /* Initialise the module */
  FIRMATADEBUG.print(F("Initialising the Bluefruit LE module: "));

  if ( !bluefruit.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  
  FIRMATADEBUG.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  FIRMATADEBUG.println(F("Performing a factory reset: "));
  if (! bluefruit.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  bluefruit.echo(false);

  FIRMATADEBUG.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  bluefruit.info();
  
  FIRMATADEBUG.println("Setting name to BLE Firmata");
  bluefruit.println("AT+GAPDEVNAME=BLE_Firmata");
  
  BTLEstatus = false;
}

void firmataInit() {
  FIRMATADEBUG.println(F("Init firmata"));
  //BLE_Firmata.setFirmwareVersion(FIRMATA_MAJOR_VERSION, FIRMATA_MINOR_VERSION);
  //FIRMATADEBUG.println(F("firmata analog"));
  BLE_Firmata.attach(ANALOG_MESSAGE, analogWriteCallback);
  //FIRMATADEBUG.println(F("firmata digital"));
  BLE_Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
  //FIRMATADEBUG.println(F("firmata analog report"));
  BLE_Firmata.attach(REPORT_ANALOG, reportAnalogCallback);
  //FIRMATADEBUG.println(F("firmata digital report"));
  BLE_Firmata.attach(REPORT_DIGITAL, reportDigitalCallback);
  //FIRMATADEBUG.println(F("firmata pinmode"));
  BLE_Firmata.attach(SET_PIN_MODE, setPinModeCallback);
  //FIRMATADEBUG.println(F("firmata sysex"));
  BLE_Firmata.attach(START_SYSEX, sysexCallback);
  //FIRMATADEBUG.println(F("firmata reset"));
  BLE_Firmata.attach(SYSTEM_RESET, systemResetCallback);

  FIRMATADEBUG.println(F("Begin firmata"));
  BLE_Firmata.begin();
  systemResetCallback();  // reset to default config 
}
/*==============================================================================
 * LOOP()
 *============================================================================*/

void loop() 
{
  delay(100);
  // Link status check
  if (!  BTLEstatus) {
    bluefruit.setMode(BLUEFRUIT_MODE_COMMAND);
    BTLEstatus = bluefruit.isConnected();
    bluefruit.setMode(BLUEFRUIT_MODE_DATA);
  }
  
  // Check if something has changed
  if (BTLEstatus != lastBTLEstatus) {
    // print it out!
    if (BTLEstatus == true) {
        FIRMATADEBUG.println(F("* Connected!"));
        // initialize Firmata cleanly
        bluefruit.setMode(BLUEFRUIT_MODE_DATA);
        firmataInit();
    }
    if (BTLEstatus == false) {
        FIRMATADEBUG.println(F("* Disconnected or advertising timed out"));
    }
    // OK set the last status change to this one
    lastBTLEstatus = BTLEstatus;
  }
    
  // if not connected... bail
  if (! BTLEstatus) {
    delay(100);
    return;
  }
  
  // For debugging, see if there's data on the serial console, we would forwad it to BTLE
  if (FIRMATADEBUG.available()) {
    bluefruit.write(FIRMATADEBUG.read());
  }
  
  // Onto the Firmata main loop
  
  byte pin, analogPin;
  
  /* DIGITALREAD - as fast as possible, check for changes and output them to the
   * BTLE buffer using FIRMATADEBUG.print()  */
  checkDigitalInputs();  

  /* SERIALREAD - processing incoming messagse as soon as possible, while still
   * checking digital inputs.  */
  while(BLE_Firmata.available()) {
    // FIRMATADEBUG.println(F("*data available*"));
    BLE_Firmata.processInput();
  }
  /* SEND FTDI WRITE BUFFER - make sure that the FTDI buffer doesn't go over
   * 60 bytes. use a timer to sending an event character every 4 ms to
   * trigger the buffer to dump. */

  // make the sampling interval longer if we have more analog inputs!
  uint8_t analogreportnums = 0;
  for(uint8_t a=0; a<8; a++) {
    if (analogInputsToReport & (1 << a)) {
      analogreportnums++;
    }
  }

  samplingInterval = (uint16_t)MINIMUM_SAMPLE_DELAY  + (uint16_t)ANALOG_SAMPLE_DELAY * (1+analogreportnums); 
  
  currentMillis = millis();
  if (currentMillis - previousMillis > samplingInterval) {
    previousMillis += samplingInterval;
    /* ANALOGREAD - do all analogReads() at the configured sampling interval */

    for(pin=0; pin<TOTAL_PINS; pin++) {
      // FIRMATADEBUG.print("pin #"); FIRMATADEBUG.print(pin); FIRMATADEBUG.print(" config = "); FIRMATADEBUG.println(pinConfig[pin]);
      if (BLE_Firmata.IS_PIN_ANALOG(pin) && (pinConfig[pin] == ANALOG)) {
        analogPin = BLE_Firmata.PIN_TO_ANALOG(pin);

        if (analogInputsToReport & (1 << analogPin)) {
          int currentRead = analogRead(analogPin);
          
          if ((lastAnalogReads[analogPin] == -1) || (lastAnalogReads[analogPin] != currentRead)) {
            //FIRMATADEBUG.print(F("Analog")); FIRMATADEBUG.print(analogPin); FIRMATADEBUG.print(F(" = ")); FIRMATADEBUG.println(currentRead);
            BLE_Firmata.sendAnalog(analogPin, currentRead);
            lastAnalogReads[analogPin] = currentRead;
          }
        }
      }
    }
    // report i2c data for all device with read continuous mode enabled
    if (queryIndex > -1) {
      for (byte i = 0; i < queryIndex + 1; i++) {
        readAndReportData(query[i].addr, query[i].reg, query[i].bytes);
      }
    }
  }
}
