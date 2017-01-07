#include <VoltageReference.h>

VoltageReference vRef;

void setup() {
	Serial.begin(9600);
	while (!Serial);
	Serial.println("Calibrating voltage reference");
	vRef.begin();
}

void loop() {
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);
	delay(500);

	int analog = analogRead(A0);
	int vcc = vRef.readVcc();
	Serial.print("Input voltage is ");
	Serial.print(vcc);
	Serial.print("mV, analog pin voltage is ");
	Serial.print(analog / 1024 * vcc);
	Serial.print("mV");
	delay(50);

	// For input voltages below 1V
	//analogReference(INTERNAL);
	//int precise = analogRead(A1);
	//Serial.print("Precise analog pin voltage is ");
	//Serial.print(analog / 1024 * vRef.internalValue());
	//Serial.print("mV");
}
