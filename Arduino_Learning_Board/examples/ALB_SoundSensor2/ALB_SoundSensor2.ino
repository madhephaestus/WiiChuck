/* Sound Sensor - Analog Example */

int led = 13;
int analogPin = A0;

int analogVal = 0;
int analogTrigger = 530;
int prevVal = 0;
int lamp = 0;

void setup()
{
	pinMode(led, OUTPUT);
	digitalWrite(led, lamp);
	pinMode(analogPin, INPUT);

	Serial.begin(115200);
	Serial.println("Sound Sensor Test");

	analogVal = analogRead(analogPin);
	Serial.println(analogVal);
	prevVal = analogVal;
}

void loop()
{
	analogVal = analogRead(analogPin);
	if ((analogVal > analogTrigger) && (prevVal < analogTrigger))
	{
		Serial.println(analogVal);
		lamp = !lamp;
		digitalWrite(led, lamp);
		delay(500);
		analogVal = analogRead(analogPin);
		Serial.println(analogVal);
		prevVal = analogVal;
	}
}
