/*
* Arduino Learning Board Project - MPU6050 - Gyro Calibration Example
*
* ALB_MPU6050_Calibrate - This sketch demonstrates how to generate calibration constants
* to use in the ALB_MPU6050 sketch
*
* Please visit http://www.ArduinoLearningBoard.com for more information
*
* Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
*
* This example is based largely on the demonstration Arduino sketch by
* Jeff Rowberg <jeff@rowberg.net> 6/21/2012 and
* Luis Ródenas <luisrodenaslorda@gmail.com> 1/31/2014
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

///////////////////////////////////   CONFIGURATION   /////////////////////////////
//Change this 3 variables if you want to fine tune the skecth to your needs.
int buffersize = 1000;   //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone = 8;   //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone = 1;   //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

// default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
// MPU6050 mpu(0x69); // <-- use for AD0 high

int16_t ax, ay, az, gx, gy, gz;

int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

///////////////////////////////////   SETUP   ////////////////////////////////////
void setup()
{
	// join I2C bus (I2Cdev library doesn't do this automatically)
	Wire.begin();
	// COMMENT NEXT LINE IF YOU ARE USING ARDUINO DUE
	TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz). Leonardo measured 250kHz.

	// initialize serial communication
	Serial.begin(115200);

	// initialize device
	mpu.initialize();

	// wait for ready
	while (Serial.available() && Serial.read()); // empty buffer
	while (!Serial.available())
	{
		Serial.println(F("Send any character to start sketch.\n"));
		delay(1500);
	}
	while (Serial.available() && Serial.read()); // empty buffer again

	// start message
	Serial.println("\nMPU6050 Calibration Sketch");
	delay(2000);
	Serial.println("\nYour MPU6050 should be placed in horizontal position, with package letters facing up. \nDon't touch it until you see a finish message.\n");
	delay(3000);
	// verify connection
	Serial.println(mpu.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
	delay(1000);
	// reset offsets
	mpu.setXAccelOffset(0);
	mpu.setYAccelOffset(0);
	mpu.setZAccelOffset(0);
	mpu.setXGyroOffset(0);
	mpu.setYGyroOffset(0);
	mpu.setZGyroOffset(0);
}

///////////////////////////////////   LOOP   ////////////////////////////////////
void loop()
{
	if (state == 0)
	{
		Serial.println("\nReading sensors for first time...");
		meansensors();
		state++;
		delay(1000);
	}

	if (state == 1)
	{
		Serial.println("\nCalculating offsets...");
		calibration();
		state++;
		delay(1000);
	}

	if (state == 2)
	{
		meansensors();
		Serial.println("\nFINISHED!");
		Serial.print("\nSensor readings with these offsets:\t");
		Serial.print(mean_ax);
		Serial.print("\t");
		Serial.print(mean_ay);
		Serial.print("\t");
		Serial.print(mean_az);
		Serial.print("\t");
		Serial.print(mean_gx);
		Serial.print("\t");
		Serial.print(mean_gy);
		Serial.print("\t");
		Serial.println(mean_gz);
		Serial.print("Your offsets:\t");
		Serial.print(ax_offset);
		Serial.print("\t");
		Serial.print(ay_offset);
		Serial.print("\t");
		Serial.print(az_offset);
		Serial.print("\t");
		Serial.print(gx_offset);
		Serial.print("\t");
		Serial.print(gy_offset);
		Serial.print("\t");
		Serial.println(gz_offset);
		Serial.println("Data is printed as: acelX acelY acelZ giroX giroY giroZ\n");
		Serial.println("Check that your sensor readings are close to 0 0 16384 0 0 0");
		Serial.println("If calibration was succesful write down your offsets so you can set them");
		Serial.println("in your projects using something similar to:");
		Serial.println("");
		Serial.print("mpu.setXAccelOffset(");
		Serial.print(ax_offset);
		Serial.println(");");
		Serial.print("mpu.setYAccelOffset(");
		Serial.print(ay_offset);
		Serial.println(");");
		Serial.print("mpu.setZAccelOffset(");
		Serial.print(az_offset);
		Serial.println(");");
		Serial.print("mpu.setXGyroOffset(");
		Serial.print(gx_offset);
		Serial.println(");");
		Serial.print("mpu.setYGyroOffset(");
		Serial.print(gy_offset);
		Serial.println(");");
		Serial.print("mpu.setZGyroOffset(");
		Serial.print(gz_offset);
		Serial.println(");");

		while (1);
	}
}

///////////////////////////////////   FUNCTIONS   ////////////////////////////////////
void meansensors()
{
	long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;

	while (i < (buffersize + 101))
	{
		// read raw accel/gyro measurements from device
		mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

		if (i > 100 && i <= (buffersize + 100)) //First 100 measures are discarded
		{
			buff_ax = buff_ax + ax;
			buff_ay = buff_ay + ay;
			buff_az = buff_az + az;
			buff_gx = buff_gx + gx;
			buff_gy = buff_gy + gy;
			buff_gz = buff_gz + gz;
		}
		if (i == (buffersize + 100))
		{
			mean_ax = buff_ax / buffersize;
			mean_ay = buff_ay / buffersize;
			mean_az = buff_az / buffersize;
			mean_gx = buff_gx / buffersize;
			mean_gy = buff_gy / buffersize;
			mean_gz = buff_gz / buffersize;
		}
		i++;
		delay(2); //Needed so we don't get repeated measures
	}
}

void calibration()
{
	ax_offset = -mean_ax / 8;
	ay_offset = -mean_ay / 8;
	az_offset = (16384 - mean_az) / 8;

	gx_offset = -mean_gx / 4;
	gy_offset = -mean_gy / 4;
	gz_offset = -mean_gz / 4;

	while (1) 
	{
		int ready = 0;
		mpu.setXAccelOffset(ax_offset);
		mpu.setYAccelOffset(ay_offset);
		mpu.setZAccelOffset(az_offset);

		mpu.setXGyroOffset(gx_offset);
		mpu.setYGyroOffset(gy_offset);
		mpu.setZGyroOffset(gz_offset);

		meansensors();

		// Serial.println("...");
		/*
		Serial.print("ax: ");
		Serial.print(mean_ax);
		Serial.print("\tay: ");
		Serial.print(mean_ay);
		Serial.print("\taz: ");
		Serial.print(mean_az);
		Serial.print("\tgx: ");
		Serial.print(mean_gx);
		Serial.print("\tgy: ");
		Serial.print(mean_gy);
		Serial.print("\tgz: ");
		Serial.print(mean_gz);
		*/

		if (abs(mean_ax) <= acel_deadzone) ready++;
		else ax_offset = ax_offset - mean_ax / acel_deadzone;

		if (abs(mean_ay) <= acel_deadzone) ready++;
		else ay_offset = ay_offset - mean_ay / acel_deadzone;

		if (abs(16384 - mean_az) <= acel_deadzone) ready++;
		else az_offset = az_offset + (16384 - mean_az) / acel_deadzone;

		if (abs(mean_gx) <= giro_deadzone) ready++;
		else gx_offset = gx_offset - mean_gx / (giro_deadzone + 1);

		if (abs(mean_gy) <= giro_deadzone) ready++;
		else gy_offset = gy_offset - mean_gy / (giro_deadzone + 1);

		if (abs(mean_gz) <= giro_deadzone) ready++;
		else gz_offset = gz_offset - mean_gz / (giro_deadzone + 1);

		Serial.print(ready);
		Serial.println(" of 6 offsets calculated.");

		if (ready == 6) break;
	}
}