#include "volume2.h";

Volume vol;

void setup() {
  Serial.begin(115200);

  // Custom waveform shape, 4 octaves of squares
  byte wave[32] = {
    255,255,192,192,192,192,128,128,
    192,192,128,128,128,128,64,64,
    192,192,128,128,128,128,64,64,
    128,128,64,64,64,64,0,0,
  };
  
  Serial.println("Volume Library Voice Demo\n");
  Serial.print("Setting custom wave samples: ");
  
  vol.setCustomWave(wave); // use our custom waveform above

  // print the custom waveform
  for (byte i = 0; i < 32; i++) {
    Serial.print(vol.getCustomSample(i));
    Serial.print(" ");
  }

  // Play tune with all waveform types:
  Serial.println("\nContacting aliens...\n");
}

void loop() {
  Serial.println("Type: PWM (12.5% Duty)");
  closeEncounter(PWM_12);

  Serial.println("Type: PWM (25% Duty)");
  closeEncounter(PWM_25);

  Serial.println("Type: Square Wave");
  closeEncounter(SQUARE);

  Serial.println("Type: Sawtooth Wave");
  closeEncounter(SAWTOOTH);

  Serial.println("Type: Sawtooth Wave (HQ)");
  closeEncounter(SAWTOOTH_HIGH);

  Serial.println("Type: Triangle Wave");
  closeEncounter(TRIANGLE);

  Serial.println("Type: Triangle Wave (HQ)");
  closeEncounter(TRIANGLE_HIGH);

  Serial.println("Type: Sine Wave");
  closeEncounter(SINE);

  Serial.println("Type: Sine Wave (HQ)");
  closeEncounter(SINE_HIGH);

  Serial.println("Type: Custom Wave");
  closeEncounter(CUSTOM);
}

// Plays the famous "Close Encounters of The Third Kind motif
// with the waveform of your choice
void closeEncounter(byte type){
  int frequencies[5] = {392, 440, 349, 175, 262}; // Notes: G4, A4, F4, F3, C4
  
  for (int lev = 255; lev > 0; lev -= 55) { // Reduce volume by ~20% each time we loop
    Serial.print("Volume: ");
    Serial.print((float(lev)/float(255))*100);
    Serial.println("%");
    for (byte freq = 0; freq < 5; freq++) { // Play through the notes
      vol.tone(frequencies[freq], type, lev); // Start the voice at [freq] Hz, waveform [type], at volume [lev]
      vol.delay(200); // use Volume delay during sound
    }
    vol.delay(100);
    vol.noTone(); // end voice
    delay(100);
  }
  Serial.println();
}
