/*
* Arduino Learning Board Project - MPU6050 - Gyro Example
*
* ALB_MPU6050 - This sketch demonstrates how to read the Yaw, Pitch and Roll
* data from the MPU6050 Gyro.
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*
* This example is based largely on the demonstration ARduino sketch by
* Jeff Rowberg <jeff@rowberg.net>, written 6/21/2012
*
* Note, Serial Port is defined as 115200 Baud in this example
*/

// First DEFINE the components of the library we're going to use for this sketch
// Define #USE_ALB_MPU6050 to include the MPU6050 (Gyro) functions of the ArduinoLearningBoard Library
// (Must do this before including ArduinoLearningBoard.h)
#define USE_ALB_MPU6050

// NOW include the main ArduinoLearningBoard library
// Based on the defines above, the appropriate modules will be added to the project
#include "ArduinoLearningBoard.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

/*
VCC to 5V
GND to GND
SDA to A4
SCL to A5
INT to D2
*/

#define LED_PIN 13
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
	mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup()
{
	// join I2C bus (I2Cdev library doesn't do this automatically)
	Wire.begin();
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

	// initialize serial communication
	// (115200 chosen because it is required for Teapot Demo output, but it's
	// really up to you depending on your project)
	Serial.begin(115200);

	// initialize device
	Serial.println(F("Initializing I2C devices..."));
	mpu.initialize();

	// verify connection
	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

	// wait for ready
	Serial.println(F("\nSend any character to begin DMP programming and demo: "));
	while (Serial.available() && Serial.read()); // empty buffer
	while (!Serial.available());                 // wait for data
	while (Serial.available() && Serial.read()); // empty buffer again

	// load and configure the DMP
	Serial.println(F("Initializing DMP..."));
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, use the output of ALB_MPU650_Calibrate
	// since each gyro is a little different from the factory
	mpu.setXAccelOffset(-3058);
	mpu.setYAccelOffset(2105);
	mpu.setZAccelOffset(1313);
	mpu.setXGyroOffset(8);
	mpu.setYGyroOffset(-11);
	mpu.setZGyroOffset(11);

	// make sure it worked (returns 0 if so)
	if (devStatus == 0)
	{
		// turn on the DMP, now that it's ready
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
		attachInterrupt(0, dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else
	{
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}

	// configure LED for output
	pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop()
{
	// if programming failed, don't try to do anything
	if (!dmpReady) return;

	// wait for MPU interrupt or extra packet(s) available
	while (!mpuInterrupt && fifoCount < packetSize)
	{
		// other program behavior stuff here
		// .
		// .
		// .
		// if you are really paranoid you can frequently test in between other
		// stuff to see if mpuInterrupt is true, and if so, "break;" from the
		// while() loop to immediately process the MPU data
		// .
		// .
		// .
	}

	// reset interrupt flag and get INT_STATUS byte
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();

	// get current FIFO count
	fifoCount = mpu.getFIFOCount();

	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024)
	{
		// reset so we can continue cleanly
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));

		// otherwise, check for DMP data ready interrupt (this should happen frequently)
	}
	else if (mpuIntStatus & 0x02)
	{
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);

		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;

		// display Euler angles in degrees
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		Serial.print("ypr\t");
		Serial.print(ypr[0] * 180 / M_PI);
		Serial.print("\t");
		Serial.print(ypr[1] * 180 / M_PI);
		Serial.print("\t");
		Serial.println(ypr[2] * 180 / M_PI);

		// blink LED to indicate activity
		blinkState = !blinkState;
		digitalWrite(LED_PIN, blinkState);
	}
}