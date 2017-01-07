/************************************************************************************************************/
/*      Inovatic-ICT d.o.o						                                                    			            */
/*												                                                                                  */
/*      EMoRo Example:		  tone Melody                                                                     */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This example shows how to use the tone() command to generate notes.                           */  
/*            The notes will be played on the builtin buzzer.                                               */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/

#include "pitches.h"          // include external file "pitches.h"

int melody[] = {              // notes in the melody
  NOTE_C5, NOTE_G4,NOTE_G4, NOTE_A4, NOTE_G4,0, NOTE_B4, NOTE_C5};

int noteDurations[] = {       // note durations: 4 = quarter note, 8 = eighth note, etc
  4, 8, 8, 4, 4, 4, 4, 4 };

void setup() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {      // iterate over the notes of the melody:

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];      // note duration calculation
    
    tone(BUZ_BUILTIN, melody[thisNote], noteDuration);    // generate tone on builting buzzer

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;          // calculate pause between notes
    delay(pauseBetweenNotes);                             // delay for calculated pause
   
    noTone(BUZ_BUILTIN);                                  // stop the tone playing
  }
}

void loop() {
  // no need to repeat the melody.
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

