/************************************************************************************************************/
/*      Inovatic-ICT d.o.o									                                                                */
/*												                                                                                  */
/*      EMoRo Example:		  MIDI note player                                                                */
/*      Board name: 		    EMoRo 2560 v3.2	                                          					            */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*            This sketch shows how to use the serial transmit pin (GPP_6_B) to send MIDI note data.If this */
/*            circuit is connected to a MIDI synth, it will play the notes F#-0 (0x1E) to F#-5 (0x5A) in    */
/*            sequence.                                                                                     */
/*      The circuit:                                                                                        */
/*            digital in GPP_6_B connected to MIDI jack pin 5                                               */
/*            MIDI jack pin 2 connected to ground                                                           */
/*            MIDI jack pin 4 connected to +5V through 220-ohm resistor                                     */
/*      Attach a MIDI cable to the jack, then to a MIDI synth, and play music.                              */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          MIDI male conenctor                                                             */
/*                          MIDI device                                                                     */  
/*                          Wires                                                                           */
/*                          Resistor 220 Ohm                                                                */
/*                          Female header connector 4x1 RM 2.54mm                                           */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
/* 
 http://www.arduino.cc/en/Tutorial/Midi
*/

/* note Serial connection on the EMoRo 2560 controller:                                                     */
/* Serial_0 Tx    Not Available               (Used for USB CDC)                                            */
/* Serial_0 Rx    Not Available               (Used for USB CDC)                                            */
/* Serial_1 Tx    PD3              EX_IO_3    (Used for External board)                                     */
/* Serial_1 Rx    PD2              EX_IO_2    (Used for External board)                                     */
/* Serial_2 Tx    PH1              GPP_6_B    (Used for GPP_6)                                              */
/* Serial_2 Rx    PH0              GPP_7_B    (Used for GPP_7)                                              */
/* Serial_3 Tx    PJ1              GPP_2_A    (Used for GPP_2)                                              */
/* Serial_3 Rx    PJ0              GPP_1_A    (Used fom GPP_1)                                              */

void setup() {
  Serial2.begin(31250);                                     //  Set MIDI baud rate
}

void loop() {
  for (int note = 0x1E; note < 0x5A; note ++) {             // play notes from F#-0 (0x1E) to F#-5 (0x5A)
    noteOn(0x90, note, 0x45);                               // note on channel 1 (0x90), some note value (note), middle velocity (0x45)
    delay(100);                                             // delay 100 ms
    noteOn(0x90, note, 0x00);                               // note on channel 1 (0x90), some note value (note), silent velocity (0x00):
    delay(100);                                             // delay 100 ms
  }
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial2.write(cmd);
  Serial2.write(pitch);
  Serial2.write(velocity);
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

