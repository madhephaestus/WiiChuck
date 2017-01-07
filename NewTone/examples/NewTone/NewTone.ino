#include <NewTone.h>

#define TONE_PIN 2 // Pin you have speaker/piezo connected to (be sure to include a 100 ohm resistor).

// Melody (liberated from the toneMelody Arduino example sketch by Tom Igoe).
int melody[] = { 262, 196, 196, 220, 196, 0, 247, 262 };
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {} // Nothing to setup, just start playing!

void loop() {
  for (unsigned long freq = 125; freq <= 15000; freq += 10) {  
    NewTone(TONE_PIN, freq); // Play the frequency (125 Hz to 15 kHz sweep in 10 Hz steps).
    delay(1); // Wait 1 ms so you can hear it.
  }
  noNewTone(TONE_PIN); // Turn off the tone.

  delay(1000); // Wait a second.

  for (int thisNote = 0; thisNote < 8; thisNote++) { // Loop through the notes in the array.
    int noteDuration = 1000/noteDurations[thisNote];
    NewTone(TONE_PIN, melody[thisNote], noteDuration); // Play thisNote for noteDuration.
    delay(noteDuration * 4 / 3); // Wait while the tone plays in the background, plus another 33% delay between notes.
  }

  while(1); // Stop (so it doesn't repeat forever driving you crazy--you're welcome).
}