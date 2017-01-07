#include "pitches.h"

static char lcd[60];        //Temp. lcd string to print variables on lcd.
static int magdir=0;        //Compass direction in falling detect mode.
static int currentMode=0;   //currentMode is used to select current RC/ autonomus mode. It can be selected via lcd or bluetooth(some modes only over bluetooth).
static int btConnected=0;   //Used to check if bluetooth is connected.
static int Lfwd=1000;       //Servo values for each motor and each direction. They are changed if speed is changed over BT.
static int Rfwd=2000;
static int Lbck=2000;
static int Rbck=1000;
static int reversed=1;      //Reverses the forward direction of EMoRo in RC mode.

int magDir90=0;             //Compass direction in 90deg turns mode.
int magDirLf=0;             //Compass direction in line following mode.
int magGetDirLf=0;          //Current compass direction in line following mode. Is updated at least 10 times a second times a second.
int magDirLfFwd=0;          //Compass direction when going straight forward in line following mode.
int magDir90Fwd=0;          //Compass direction when going straight forward in 90deg turns mode.
int magGetDir90=0;          //Current compass direction in 90deg turns mode. Is updated at least 10 times a second times a second.
int btMsg=0;                //Indicates if bluetooth disconnected was displayed on lcd when bluetooth was disconnected.

//This values are used to speed up program by updating sensor readings only as often as needed:
long lastUsTime=0;          //Time in ms when ultrasonic was value was last checked in line following mode.
long lastMag=0;             //Time in ms when compass value was last updated.
long lastAcc=0;             //Time in ms when accelerometer value was last updated.
long lastLCD=0;             //Time in ms when LCD was last updated
long delayTime;             //Time at the begining of while loop that checks if stop button was pressed or stop message was received over bluetooth.
char btChar;                //Used to check which action was triggered over bluetooth.
int btParam;               //Used to store parameter of servo control
int servoParam;

int startAccY;

void setup(){
  InitEmoro();  //Initializes all available inputs and outputs on EMoRo 2560.
  Ultrasonic.attach(GPP_0);  //Initiates ultrasonic sensor on GPP_1 port.
  pinMode(IO_0, INPUT_PULLUP);//Initiates infrared sensor on IO_1 port.
  pinMode(IO_1, INPUT_PULLUP);//Initiates infrared sensor on IO_2 port.
  EmoroServo.attach(SERVO_0);  //Initiates servo on SERVO_0 port.
  EmoroServo.attach(SERVO_1);  //Initiates servo on SERVO_1 port.
  EmoroServo.attach(SERVO_2);  //Initiates servo on SERVO_2 port.
  EmoroServo.write(SERVO_2,1000);
  Lcd.clear();  //Clears all characters from LCD.
  changeMode();  //Allows user to select initial mode.
}

void loop(){
  //Check if bluetooth was disconnected and updates btConnected value. Also disables RC mode if it is enabled at this time.
  if(!Bluetooth.connection()&&btConnected==1){
      btConnected=0;
    if(currentMode==4||currentMode==8)
      currentMode=0;
  }
  
  //interrupts the program and changes the mode if something was received over bluetooth.
  if(Serial1.available()&&(currentMode!=4&&currentMode!=8)){
    btModeSelect();
    goto startedall;
  }
  
  startedall:  //Bailout tag if bluetooth message interrupts the program.
  
  //Bailout tag if SW_4 interrupts the program.
  if(2==1){
  stopedall:
  stopAll();
  }
  
  //Allows user to change mode on lcd if SW_1 is pressed.
  if(ReadSwitch(SW_1))
    changeMode();
    
  //Stops the program if SW_4 is pressed.
  if(ReadSwitch(SW_4))
    stopAll();
  
  //This part of program is used to check which mode is selected and execute it.
  switch(currentMode){
    
    case 0:  //Do nothing
      EmoroServo.write(SERVO_0,1500);
      EmoroServo.write(SERVO_1,1500);
    break;
    
    case 1:  //line following
            
      if(digitalRead(IO_0)==1 && digitalRead(IO_1)==1){  //Both sensros are on the white, go forward.
        EmoroServo.write(SERVO_0,Lbck);
        EmoroServo.write(SERVO_1,Rbck);
      }else if(digitalRead(IO_0)==1 && digitalRead(IO_1)==1){  //Both sensors are on the black, still go forward.
        EmoroServo.write(SERVO_0,Lbck);
        EmoroServo.write(SERVO_1,Rbck);
      }else if(digitalRead(IO_0)==0){  //Only left sensor is on the black, turn right.
        EmoroServo.write(SERVO_0,Lfwd);
        EmoroServo.write(SERVO_1,Rbck);
        delay(20);
      }else if(digitalRead(IO_1)==0){  //Only right sensor is on the black, turn left.
        EmoroServo.write(SERVO_0,Lbck);
        EmoroServo.write(SERVO_1,Rfwd);
        delay(20);
      }     
      
      if(millis()>=lastUsTime+100){  //Checks the ultrasonic value every 100ms.
      lastUsTime=millis();           
      if(Ultrasonic.read(GPP_0)<15){  //If something is in front of the robot, aviod it by going around it.
        
        EmoroServo.write(SERVO_0,1500);
        EmoroServo.write(SERVO_1,1500); 
        
        while(millis()<delayTime+500){
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
        }
        magGetDirLf=Mag.readDirection(); 
        
        magDirLfFwd=magGetDirLf+90; 
        
        
         //Turn left 90deg
         
          EmoroServo.write(SERVO_0,Lbck);
          EmoroServo.write(SERVO_1,Rfwd);
              
              
             delayTime=millis();
       while(millis()<delayTime+500){
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
        }    
                 
        
        //Go forward
        EmoroServo.write(SERVO_0,Lbck);
        EmoroServo.write(SERVO_1,Rbck);
        
        delayTime=millis();
          while(millis()<delayTime+1000){
            if(millis()>lastMag+100){
              lastMag=millis();
            magGetDirLf=Mag.readDirection();Mag.readDirection();}
            if(magDirLfFwd>180){
            if(magGetDirLf<180) 
              magGetDirLf+=360;
            }else if(magDirLfFwd<180){
              if(magGetDirLf>180)
              magGetDirLf-=360;
            }
            if(magGetDirLf>=magDirLfFwd+5){
              EmoroServo.write(SERVO_0,1500);
              EmoroServo.write(SERVO_1,Rbck);
              while(MagGetDirectionLF(magDirLfFwd)>magDirLfFwd+1);  
            }else if(magGetDirLf<=magDirLfFwd-5){
              EmoroServo.write(SERVO_0,Lbck);
              EmoroServo.write(SERVO_1,1500);
              while(MagGetDirectionLF(magDirLfFwd)<magDirLfFwd-1); 
            }else{
              EmoroServo.write(SERVO_0,Lbck);
              EmoroServo.write(SERVO_1,Rbck);
            }
        
            if(ReadSwitch(SW_4))
              goto stopedall;
            if(Serial1.available()&&currentMode!=4){
              btModeSelect();
              goto startedall;
            }
        }
        
        
        
        //Turn right 90deg
        
        
          EmoroServo.write(SERVO_0,Lfwd);
          EmoroServo.write(SERVO_1,Rbck);
              
              
             delayTime=millis();
       while(millis()<delayTime+500){
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
        }    
        
        magDirLfFwd-=90;
        //Go forward
        EmoroServo.write(SERVO_0,Lbck);
        EmoroServo.write(SERVO_1,Rbck);
        
        delayTime=millis();
          while(millis()<delayTime+1600){
            if(millis()>lastMag+100){
              lastMag=millis();
            magGetDirLf=Mag.readDirection();}
            if(magDirLfFwd>300){
            if(magGetDirLf<60) 
              magGetDirLf+=360;
            }else if(magDirLfFwd<60){
              if(magGetDirLf>300)
              magGetDirLf-=360;
            }
            if(magGetDirLf>=magDirLfFwd+5){
              EmoroServo.write(SERVO_0,1500);
              EmoroServo.write(SERVO_1,Rbck);
              while(MagGetDirectionLF(magDirLfFwd)>magDirLfFwd+1);  
            }else if(magGetDirLf<=magDirLfFwd-5){
              EmoroServo.write(SERVO_0,Lbck);
              EmoroServo.write(SERVO_1,1500);
              while(MagGetDirectionLF(magDirLfFwd)<magDirLfFwd-1); 
            }else{
              EmoroServo.write(SERVO_0,Lbck);
              EmoroServo.write(SERVO_1,Rbck);
            }
        
            if(ReadSwitch(SW_4))
              goto stopedall;
            if(Serial1.available()&&currentMode!=4){
              btModeSelect();
              goto startedall;
            }
        }

        
        
                //Turn right 90deg
        
             EmoroServo.write(SERVO_0,Lfwd);
          EmoroServo.write(SERVO_1,Rbck);
              
              
             delayTime=millis();
       while(millis()<delayTime+300){
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
        }    
        magDirLfFwd-=90;
        
                //Go forward
        EmoroServo.write(SERVO_0,Lbck);
        EmoroServo.write(SERVO_1,Rbck);
        while(digitalRead(IO_0)==digitalRead(IO_1)){
           if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
          
        }
        delay(200);
       
      }
      }
      
      
    break;
    
    case 2:  //90deg mode
      
      //Go forward for 5 seconds. Correct the direction whille driving if robot is not driving straight.
      EmoroServo.write(SERVO_0,Lfwd);    
      EmoroServo.write(SERVO_1,Rfwd);
      magDir90Fwd=Mag.readDirection();
      delayTime=millis();
      while(millis()<delayTime+5000){
        if(millis()>lastMag+100){
          lastMag=millis();
        magGetDir90=Mag.readDirection();}
        if(magDir90Fwd>300){
          if(magGetDir90<60) 
          magGetDir90+=360;
        } else if(magDir90Fwd<60){
          if(magGetDir90>300)
          magGetDir90-=360;
        }
         if(magGetDir90>=magDir90Fwd+5){
          EmoroServo.write(SERVO_0,Lfwd);
          EmoroServo.write(SERVO_1,1500);
          while(MagGetDirectionLF(magDir90Fwd)>magDir90Fwd+1);  
        }else if(magGetDir90<=magDir90Fwd-5){
          EmoroServo.write(SERVO_0,1500);
          EmoroServo.write(SERVO_1,Rfwd);
          while(MagGetDirectionLF(magDir90Fwd)<magDir90Fwd-1); 
        }else{
          EmoroServo.write(SERVO_0,Lfwd);
          EmoroServo.write(SERVO_1,Rfwd);
        }
        //Stop the program if SW_4 is pressed or bluetooth message received.
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
        }
        
        
      //Go backwards for 5 seconds. Correct the dirrection if robot is not driving straight.
      EmoroServo.write(SERVO_0,Lbck);
      EmoroServo.write(SERVO_1,Rbck);
      delayTime=millis();
      while(millis()<delayTime+5000){
        if(millis()>lastMag+100){
          lastMag=millis();
          magGetDir90=Mag.readDirection();}
        if(magDir90Fwd>300){
          if(magGetDir90<60) 
          magGetDir90+=360;
        } else if(magDir90Fwd<60){
          if(magGetDir90>300)
          magGetDir90-=360;
        }
        if(magGetDir90>=magDir90Fwd+5){
          EmoroServo.write(SERVO_0,1500);
          EmoroServo.write(SERVO_1,Rbck);    
          while(MagGetDirectionLF(magDir90Fwd)>magDir90Fwd+1); 
        }else if(magGetDir90<=magDir90Fwd-5){
          EmoroServo.write(SERVO_0,Lbck);
          EmoroServo.write(SERVO_1,1500);
          while(MagGetDirectionLF(magDir90Fwd)<magDir90Fwd-1); 
        }else{
          EmoroServo.write(SERVO_0,Lbck);
          EmoroServo.write(SERVO_1,Rbck);
        }
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
         btModeSelect();
        goto startedall;
        }
        }
        
      EmoroServo.write(SERVO_0,1500);
      EmoroServo.write(SERVO_1,1500);
      delayTime=millis();
    while(millis()<delayTime+1000){
        if(ReadSwitch(SW_4))
          goto stopedall;
          if(Serial1.available()&&currentMode!=4){
          btModeSelect();
          goto startedall;
        }
        }
        
      EmoroServo.write(SERVO_0,Lbck);
      EmoroServo.write(SERVO_1,Rfwd);
      
      magDir90=magDir90Fwd;
        EmoroServo.write(SERVO_0,1550);
        EmoroServo.write(SERVO_1,1550);  
        
        magDir90+=90;
        if(magDir90>360){
          while(Mag.readDirection()>120){
            delay(1);            
            if(ReadSwitch(SW_4))
               goto stopedall;  
            if(Serial1.available()&&currentMode!=4){
              btModeSelect();
              goto startedall;
            }    
        }
          while(Mag.readDirection()>120){
            delay(1);            
            if(ReadSwitch(SW_4))
              goto stopedall;      
              if(Serial1.available()&&currentMode!=4){
              btModeSelect();
              goto startedall;
            }
        }
          magDir90-=360;
          delayTime=millis();
          while(millis()<delayTime+400){
        if(ReadSwitch(SW_4))
          goto stopedall;
        if(Serial1.available()&&currentMode!=4){
            btModeSelect();
            goto startedall;
        }
        }
          error190:
          while(Mag.readDirection()<=magDir90-1){
          delay(1);
            if(ReadSwitch(SW_4))
              goto stopedall;
            if(Serial1.available()&&currentMode!=4){
              btModeSelect();
              goto startedall;
            }
        }
          delay(5);
          if(Mag.readDirection()<=magDir90-1) goto error190;
        }else{
          error290:
          while(Mag.readDirection()<=magDir90-1){
            delay(1);
            if(ReadSwitch(SW_4))
              goto stopedall; 
            if(Serial1.available()&&currentMode!=4){
              btModeSelect();
              goto startedall;
            }  
        }
          delay(5);
          if(Mag.readDirection()<=magDir90-1) goto error290;
        }
        EmoroServo.write(SERVO_0,1500);
        EmoroServo.write(SERVO_1,1500);
      
      
      
      magDir90Fwd+=90;
    
    break;
    
    case 3:  //falling detect
              startAccY=Acc.readY();
              
              EmoroServo.write(SERVO_0,1380);
              EmoroServo.write(SERVO_1,1610);
 
              //while(1){
              
              Lcd.locate(0,0);
              sprintf(lcd, "%4d, %4d", Acc.readX(), Acc.readY());
              Lcd.print(lcd);
              
              if(Gyr.testConnection()){				/* ako je žiroskop dostupan			    */
                Lcd.locate(1,0);
                sprintf(lcd, "%4d, %4d", Gyr.readX(), Gyr.readY());
                Lcd.print(lcd);
              }
              //delay(100);
              //}
              if(millis()>lastAcc+10){
                if(Acc.readY()>startAccY+5){
                  EmoroServo.write(SERVO_0,1610);
                  EmoroServo.write(SERVO_1,1380);
                  delayTime=millis();
                  while(millis()<delayTime+5000){
                    if(ReadSwitch(SW_4))
                      goto stopedall;
                    if(Serial1.available()&&currentMode!=4){
                      btModeSelect();
                      goto startedall;
                    }
                  }
                  EmoroServo.write(SERVO_0,1500);
                  EmoroServo.write(SERVO_1,1500);
                  delayTime=millis();
                  while(millis()<delayTime+500){
                    if(ReadSwitch(SW_4))
                      goto stopedall;
                    if(Serial1.available()&&currentMode!=4){
                      btModeSelect();
                      goto startedall;
                    } 
                  }
        
                  EmoroServo.write(SERVO_0,1610);
                  EmoroServo.write(SERVO_1,1610);  
        
                  magdir+=180;
                  if(magdir>360){
                    while(Mag.readDirection()>90){
                      delay(1);            
                      if(ReadSwitch(SW_4))
                        goto stopedall;   
                      if(Serial1.available()&&currentMode!=4){
                        btModeSelect();
                        goto startedall;
                      }   
                    }
                    while(Mag.readDirection()>90){
                      delay(1);            
                      if(ReadSwitch(SW_4))
                        goto stopedall;   
                      if(Serial1.available()&&currentMode!=4){
                        btModeSelect();
                        goto startedall;
                      }   
                    }
                    magdir-=360;
                    delayTime=millis();
                    while(millis()<delayTime+500){
                      if(ReadSwitch(SW_4))
                        goto stopedall;
                      if(Serial1.available()&&currentMode!=4){
                        btModeSelect();
                        goto startedall;
                      }
                    }
error1:
                    while(Mag.readDirection()<=magdir-1){
                      delay(1);
                      if(ReadSwitch(SW_4))
                        goto stopedall;
                      if(Serial1.available()&&currentMode!=4){
                        btModeSelect();
                        goto startedall;
                      }
                    }
                    delay(5);
                    if(Mag.readDirection()<=magdir-1) 
                      goto error1;
                    }
                    else{
error2:
                      while(Mag.readDirection()<=magdir-1){
                        delay(1);
                        if(ReadSwitch(SW_4))
                          goto stopedall; 
                        if(Serial1.available()&&currentMode!=4){
                          btModeSelect();
                          goto startedall;
                        }
                      }
                      delay(5);
                      if(Mag.readDirection()<=magdir-1) 
                        goto error2;
                    }
                    EmoroServo.write(SERVO_0,1500);
                    EmoroServo.write(SERVO_1,1500);
                    delayTime=millis();
                    while(millis()<delayTime+500){
                      if(ReadSwitch(SW_4))
                        goto stopedall;
                      if(Serial1.available()&&currentMode!=4){
                        btModeSelect();
                        goto startedall;
                      }
                    }
                  }
                  else{
                    lastAcc=millis();  
                  }
                }
              break;
    
  case 4:  //bluetooth
  if(!Bluetooth.connection()&&btMsg==0){
      btMsg=1;
      Lcd.locate(0,0);
      Lcd.print("BT disconnected!"); 
    }else if(Bluetooth.connection()&&btMsg==1){
      btMsg=0;
      Lcd.locate(0,0);
      Lcd.print("Mode selected: 4");
    }
    if(Serial1.available()){
      btChar=(char)Serial1.read();   //   (char)Serial1.read(); 
   switch(btChar){
     case '0':  //Stop motors
       EmoroServo.write(SERVO_0,1500);  //Left
       EmoroServo.write(SERVO_1,1500);  //Right
     break;
     
     case '1':  //FWD
     if(reversed==0){
       EmoroServo.write(SERVO_0,Lfwd);
       EmoroServo.write(SERVO_1,Rfwd);
     }else{
       EmoroServo.write(SERVO_0,Lbck);
       EmoroServo.write(SERVO_1,Rbck);
     }
     break;
     
     case '2':  //BCK
     if(reversed==0){
       EmoroServo.write(SERVO_0,Lbck);
       EmoroServo.write(SERVO_1,Rbck);
     }else{
       EmoroServo.write(SERVO_0,Lfwd);
       EmoroServo.write(SERVO_1,Rfwd);
     }
     break;
     
     case '3':  //LEFT
       EmoroServo.write(SERVO_0,Lbck);
       EmoroServo.write(SERVO_1,Rfwd);     
     break;
     
     case '4':  //RIGHT     
       EmoroServo.write(SERVO_0,Lfwd);
       EmoroServo.write(SERVO_1,Rbck);     
     break;
         
     case '5':  //Speed 4
       Lfwd=500;
       Rfwd=2500;
       Lbck=2500;
       Rbck=500;
     break;
     
     case '6':  //Speed 3
       Lfwd=1000;
       Rfwd=2000;
       Lbck=2000;
       Rbck=1000;
     break;
     
     case '7':  //Speed 2
       Lfwd=1200;
       Rfwd=1800;
       Lbck=1800;
       Rbck=1200;
     break;
     
     case '8':  //Speed 1
       Lfwd=1350;
       Rfwd=1650;
       Lbck=1650;
       Rbck=1350;
     break;
     
     case '9':  //Reverse
       if(reversed==0) reversed=1;
       else reversed=0;
     break;

     case 's':  //Set servo on slider
      
      btParam=(unsigned char)Serial1.read(); 
        servoParam=1500+btParam*4;
        
        delay(20);
         EmoroServo.write(SERVO_2, servoParam); 
     break;

     
     case 'a':
       currentMode=0;
     break;
     
     case 'b':
       currentMode=1;
     break;
     
     case 'c':
       currentMode=2;
       magDir90Fwd=Mag.readDirection();
     break;
     
     case 'd':
       currentMode=3;
       magdir=Mag.readDirection();
     break;
     
     case 'e':
       currentMode=4;
     break;
     
     case 'z': //Test Ultrasonic
       currentMode=5;
     break;
     
     case 'y': //Test Compass
       currentMode=6;
     break;
     
     case 'x': //Test IR
       currentMode=7;
     break;
          
     default:
       EmoroServo.write(SERVO_0,1500);
       EmoroServo.write(SERVO_1,1500);
     break;
   }
 }  
  break; 
  
  case 5:
    Lcd.locate(0,0);
    sprintf(lcd,"Distance: %4d     ",Ultrasonic.read(GPP_1));    
    Lcd.print(lcd);
  break;
  
  case 6:
    Lcd.locate(0,0);
    sprintf(lcd,"Compass: %4d      ", Mag.readDirection());    
    Lcd.print(lcd);
  break;
  
  case 7:
    Lcd.locate(0,0);
    sprintf(lcd,"IR1: %d, IR2: %d  ",digitalRead(IO_1),digitalRead(IO_2));    
    Lcd.print(lcd);
  break;
  
  case 8:
  
  if(!Bluetooth.connection()&&btMsg==0){
      btMsg=1;
      Lcd.locate(0,0);
      Lcd.print("BT disconnected!"); 
    }else if(Bluetooth.connection()&&btMsg==1){
      btMsg=0;
      Lcd.locate(0,0);
      Lcd.print("Mode selected: 8");
    }
    if(Serial1.available()){
      btChar=(char)Serial1.read(); 
   switch(btChar){
     case 'j':  //C1
     tone(BUZ_BUILTIN, NOTE_C1,5);
     break;
     case 'k':  //D1
     tone(BUZ_BUILTIN, NOTE_D1,5);
     break;
     case 'l':  //E1
     tone(BUZ_BUILTIN, NOTE_E1,5);
     break;
     case 'm':  //F1
     tone(BUZ_BUILTIN, NOTE_F1,5);
     break;
     case 'n':  //G1
     tone(BUZ_BUILTIN, NOTE_G1,5);
     break;
     case 'o':  //A1
     tone(BUZ_BUILTIN, NOTE_A1,5);
     break;
     case 'p':  //B1
     tone(BUZ_BUILTIN, NOTE_B1,5);
     break;
     
     case 'J':  //C2
     tone(BUZ_BUILTIN, NOTE_C2,5);
     break;
     case 'K':  //D2
     tone(BUZ_BUILTIN, NOTE_D2,5);
     break;
     case 'L':  //E2
     tone(BUZ_BUILTIN, NOTE_E2,5);
     break;
     case 'M':  //F2
     tone(BUZ_BUILTIN, NOTE_F2,5);
     break;
     case 'N':  //G2
     tone(BUZ_BUILTIN, NOTE_G2,5);
     break;
     case 'O':  //A2
     tone(BUZ_BUILTIN, NOTE_A2,5);
     break;
     case 'P':  //B2
     tone(BUZ_BUILTIN, NOTE_B2,5);
     break;
    
   }
   }
  break;
  
  default:
      EmoroServo.write(SERVO_0,1500);
      EmoroServo.write(SERVO_1,1500);
  break;
    }
  }





void changeMode(){
    EmoroServo.write(SERVO_0,1500);
    EmoroServo.write(SERVO_1,1500);
   // if(currentMode==4&&Bluetooth.connection()) UartPutString1("c");
    Lcd.locate(0,0);
    
    Lcd.print("Change mode(SW1)");
        Lcd.locate(1,0);
    Lcd.print("Select mode(SW2)");
    delay(500);
    while(!ReadSwitch(SW_1)){
     if(Serial1.available()&&currentMode!=4){
      btModeSelect();
      goto modeselected;
     }
    }   
    
    tryagainmode:
    Lcd.locate(0,0);
    Lcd.print("Select mode:    ");
    Lcd.locate(1,0);
    Lcd.print("0-Stop all.     ");
    delay(500);
    while(!ReadSwitch(SW_1)){
      if(ReadSwitch(SW_2)){
        currentMode=0;
        goto modeselected;
      }
    }
    
    Lcd.locate(1,0);
    Lcd.print("1-Line Following");
    delay(500);
    while(!ReadSwitch(SW_1)){
      if(ReadSwitch(SW_2)){
        currentMode=1;
        goto modeselected;
      }
    }
    
    Lcd.locate(1,0);
    Lcd.print("2-90deg turns   ");
    delay(500);
    while(!ReadSwitch(SW_1)){
      if(ReadSwitch(SW_2)){
        currentMode=2;
        magDir90Fwd=Mag.readDirection();
        goto modeselected;
      }
    }
    
    Lcd.locate(1,0);
    Lcd.print("3-falling detect");
    delay(500);
    while(!ReadSwitch(SW_1)){
      if(ReadSwitch(SW_2)){
        currentMode=3;
        magdir=Mag.readDirection();
        goto modeselected;
      }
    }
    
  
  
  
    Lcd.locate(1,0);
    Lcd.print("4-BT mode.      ");
    delay(500);
    while(!ReadSwitch(SW_1)){
      if(ReadSwitch(SW_2)){
        currentMode=4;
        goto modeselected;
      }
    }
    goto tryagainmode;
  
  
  modeselected:
  
  if(currentMode==4&&Bluetooth.connection()==0){
    Lcd.locate(0,0);
    Lcd.print("No BT connected!");
    Lcd.locate(1,0);
    Lcd.print("Select mode(SW1)");
    delay(500);
    while(!ReadSwitch(SW_1));
    goto tryagainmode;
  } //else if(currentMode == 4) UartPutString1("b"); 
  
  if(currentMode == 0){
    Lcd.locate(0,0);
    Lcd.print("Stopped. Select ");
    Lcd.locate(1,0);
    Lcd.print("mode: SW1.      "); 
  }else{
    Lcd.locate(0,0);
    sprintf(lcd,"Mode selected: %d",currentMode);
    Lcd.print(lcd);
    Lcd.locate(1,0);
    Lcd.print("Stop: SW4.      ");
    delay(1000);
  }
 
}


void stopAll(){
  //if(currentMode==4&&Bluetooth.connection()) UartPutString1("c");
  EmoroServo.write(SERVO_0,1500);
  EmoroServo.write(SERVO_1,1500);
  currentMode=0;
  Lcd.locate(0,0);
  Lcd.print("Stopped. Select ");
  Lcd.locate(1,0);
  Lcd.print("mode: SW1.      ");
  delay(1000);
}

void btModeSelect(){
  EmoroServo.write(SERVO_0,1500);
  EmoroServo.write(SERVO_1,1500);
  char btCharacter;
  btCharacter=(char)Serial1.read();
  switch(btCharacter){
     case 'a':
     currentMode=0;
     break;
     
     case 'b':
     currentMode=1;
     break;
     
     case 'c':
     currentMode=2;
     magDir90Fwd=Mag.readDirection();
     break;
     
     case 'd':
     currentMode=3;
     magdir=Mag.readDirection();
     break;
     
     case 'e':
     currentMode=4;
     break;
     
     case 'z':
     currentMode=5;
     break;
     
     case 'y':
     currentMode=6;
     break;
     
     case 'x':
     currentMode=7;
     break;
     
     case 'w':
     currentMode=8;
     break;
     
     default:
       currentMode=4;
     break; 
  }
  
    Lcd.locate(0,0);
    Lcd.print("BT mode select.");
    Lcd.locate(1,0);
    sprintf(lcd,"Mode selected: %d",currentMode);
    Lcd.print(lcd);
  
}


int MagGetDirectionLF(int mGDLF){
  int mGDL;
  mGDL=Mag.readDirection();
  if(mGDLF>180){
    if(mGDL<180) 
      mGDL+=360;
  }else if(mGDLF<180){
    if(mGDL>180)
      mGDL-=360;
 }  
return mGDL;
}
