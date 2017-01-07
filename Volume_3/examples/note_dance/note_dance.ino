/*
   Volume3 Music Example
   by Connor Nishijima

   Picks a random note from a C Major scale to play,
   switches between volume-envelope and normal tone()
   behavior every 20 notes played.
*/

#include "Volume3.h" // Import the magic juju

#define speakerPin 9 // Connect a speaker from Pin 9 to GND

bool useVol = true; // Are we using volume envelopes?
byte noteCount = 0; // Keeps track of notes played so far

unsigned int scale[9] = {131, 165, 196, 262, 330, 392, 523, 659, 784}; // C Major scale in 3 octaves
byte lastNote = 255; // Stores the last note played

void setup() {
  // Nothing needed here!
}

void loop() {
  byte note = random(0, 9); // Pick a random note from the scale array

  while (note == lastNote) { // We don't want repeat notes, try again until we find a new one
    note = random(0, 9);
  }

  int frequency = scale[note]; // Set tone frequency based on the note we picked
  int volume = 1023; // Start volume at full 10-bit 1023 value

  if (useVol == true) {
    // Play the note, and fade it out quickly
    while (volume > 0) {
      vol.tone(speakerPin, frequency, volume);
      volume -= 10;
      delay(2);
    }
  }
  else {
    vol.tone(speakerPin, frequency, 1023); // Just use max volume here
    delay(110);
    vol.noTone();
    delay(110);
  }

  noteCount++;
  if (noteCount % 20 == 0) { // If we've played 20 notes, switch between tone modes
    useVol = !useVol;
  }

  lastNote = note; // Remember the note we picked for next run
}
