/************************************************************************************************************/
/*      Inovatic-ICT d.o.o			                                                     						            */
/*												                                                                                  */
/*      EMoRo Example:		  ControlAsterisk             			                                              */
/*      Board name: 		    EMoRo 2560 v3.2						                                                      */
/*      MCU name:		        ATMEGA2560-16					                                                          */
/*      Clock:			        16 MHz						                                                              */
/*      File description:                                                                                   */
/*           Use switches SW_1 to SW_4 to control asterisk position printed on LCD. SW_1 – up, left;        */
/*           SW_2 - up, right; SW_3 - down, left; SW_4 - down, right                                        */
/*      The circuit:                                                                                        */
/*            * Attach GLAM module to EMoRo 2560 controller                                                 */
/*                                                                                                          */
/*      Necessary equipment:                                                                                */
/*                          EMoRo 2560 controller                                                           */
/*                          GLAM module with switches SW_1 to SW_4 and LCD 2x16 characters                  */
/*      More info: www.emoro.eu                                                                             */
/************************************************************************************************************/
unsigned char lastState_sw1, lastState_sw2, lastState_sw3, lastState_sw4; // store state of last state of each switch
unsigned char charRowPos, charColPos;                         // current asterisk position 
unsigned char newRowPos, newColPos;                           // new asterisk position 

void setup(){				

  Serial.begin(9600);			                                    // initialize serial communication		
  Serial.println("Example: ControlAsterisk");	                // send example name to console  
  
  
  if( (ReadEmoroHardware() & SW_AVAILABLE) && (ReadEmoroHardware() & LCD_AVAILABLE) ) // if switches and LCD are available
    Serial.println("Example can be run.");		                // print constant string
  else{
    Serial.println("Switches and LCD should be available");	  // print constant string
    tone(BUZ_BUILTIN, 800, 1000);                             // play tone on internal buzzer
    while(1){}                                                // stop the example
  }  
  
  lastState_sw1=0; lastState_sw2=0; lastState_sw3=0; lastState_sw4=0; // set last state to 0
  charRowPos=0; charColPos=0,                                 // set current character position to 0, 0
  newRowPos=0, newColPos=8;                                   // set new character position to 0, 8 
}

void loop(){					
  if(ReadSwitch(SW_1)==1 && ReadSwitch(SW_1)!=lastState_sw1){ // if switch 1 was pressed
    if(charRowPos==1)                                         // as first change row up
      newRowPos=0;                                            // set new row position
    else if(charColPos>0)                                     // if asterisk already is in upper row
      newColPos-=1;                                           // move asterisk left
    else
      tone(BUZ_BUILTIN, 1000, 300);                           // play beep if there is no new possible location 
      
    lastState_sw1=ReadSwitch(SW_1);                           // set current state of switch 1 as last state
  }
  else if(ReadSwitch(SW_2)==1 && ReadSwitch(SW_2)!=lastState_sw2){ // if switch 2 was pressed
    if(charRowPos==1)                                         // as first change row up
      newRowPos=0;                                            // set new row position
    else if(charColPos<15)                                    // if asterisk already is in upper row
      newColPos+=1;                                           // move asterisk right
    else
      tone(BUZ_BUILTIN, 1000, 300);                           // play beep if there is no new possible location 
      
    lastState_sw2=ReadSwitch(SW_2);                           // set current state of switch 2 as last state
  }
  else if(ReadSwitch(SW_3)==1 && ReadSwitch(SW_3)!=lastState_sw3){ // if switch 3 was pressed
    if(charRowPos==0)                                         // as first change row down
      newRowPos=1;                                            // set new row position
    else if(charColPos>0)                                     // if you already in lower row
      newColPos-=1;                                           // move asterisk left
    else
      tone(BUZ_BUILTIN, 1000, 300);                           // play beep if there is no new possible location 
      
    lastState_sw3=ReadSwitch(SW_3);                           // set current state of switch 3 as last state
  }
  else if(ReadSwitch(SW_4)==1 && ReadSwitch(SW_4)!=lastState_sw4){ // if switch 4 was pressed
    if(charRowPos==0)                                         // as first change row down
      newRowPos=1;                                            // set new row position
    else if(charColPos<15)                                    // if you already in lower row
      newColPos+=1;                                           // move asterisk right
    else
      tone(BUZ_BUILTIN, 1000, 300);                           // play beep if there is no new possible location
      
    lastState_sw4=ReadSwitch(SW_4);   // play beep if there is no new possible location 
  }
  else if(ReadSwitch(SW_1)==0 && ReadSwitch(SW_2)==0 && ReadSwitch(SW_3)==0 && ReadSwitch(SW_4)==0) { // if nothing is pressed
    lastState_sw1=0; lastState_sw2=0; lastState_sw3=0; lastState_sw4=0; 
  }
  // reprint (clear on current and print on the new position) character on new position:
  if(charRowPos!=newRowPos  || charColPos!=newColPos){        // if old and new positions are different
    Lcd.locate(charRowPos, charColPos);                       // set cursor to position (charRowPos, charColPos)
    Lcd.printChar(' ');                                       // clear previous asterisk
    Lcd.locate(newRowPos, newColPos);                         // set cursor to new position
    Lcd.printChar('*');                                       // print asterisk
    charRowPos=newRowPos; charColPos=newColPos;               // update new position
    delay(100);                                               // wait for 100ms (0.1 sec)
  }
}

/************************************************************************************************************/
/*                                              end of file                                                 */
/************************************************************************************************************/

