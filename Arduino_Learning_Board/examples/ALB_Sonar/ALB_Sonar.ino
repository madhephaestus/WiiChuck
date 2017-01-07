/* 
 * Arduino Learning Board Project - HC-SR04 Ultrasonic Sensor Example
 *
 * Please visit http://www.ArduinoLearningBoard.com for more information
 *
 * Last modified August 2016 by Jeff Shapiro <Jeff@ArduinoLearningBoard.com>
 */

/*
VCC  -> 5V
Trig -> D2
Echo -> D4
GND  -> GND
*/

#define trigPin 2 // Which digital (output) pin will we trigger the pulse on
#define echoPin 4 // Which digital (input) pin will we listen for echo on

long duration; 		// Duration used to calculate distance
float inches, cm;

void setup()
{
	Serial.begin(9600);
	pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
}

void loop()
{
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);

	// Call pulseIn function to wait for High pulse
	// result will be time in microseconds until pulse is detected
	duration = pulseIn(echoPin, HIGH);

	if (duration < 100000)
	{
		inches = microSecondsToInches(duration);
		cm = microSecondsToCentimeters(duration);

		Serial.print("Ping Time: ");
		Serial.print(duration);
		Serial.print("uS, ");
		Serial.print(cm);
		Serial.print("cm, ");
		Serial.print(inches);
		Serial.println("in");
	}
	delay(100);	
}


float microSecondsToInches(long microseconds)
{
	// Sound travels at 1125 feet per second
	// or 74.642 microseconds per inch
	// This gives the distance travelled by the ping, outbound
	// and return, so we divide by 2 to get the distance of the obstacle.
	return microseconds / 74.642 / 2.0;
}

float microSecondsToCentimeters(long microseconds)
{
	// Sound travels at 1125 feet per second
	// or 29.386 microseconds per centimeter
	// This gives the distance travelled by the ping, outbound
	// and return, so we divide by 2 to get the distance of the obstacle.
	return microseconds / 29.387 / 2.0;
}