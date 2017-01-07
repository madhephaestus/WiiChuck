#include <UTFT.h>
#include <UTouch.h>
#include <TFT_Extension.h>
// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

UTFT myGLCD(ITDB32S,A1,A2,A0,A3,A5); //myGLCD(RS,WR,CS,RST,ALE,mode);
UTouch  myTouch(13,10,11,12,A4); //myTouch(TCLK,TCS,DIN,DOUT,IRQ);
TFT_Extension myTFT(&myGLCD, &myTouch);

struct {
  byte Score;
  byte LastScore;
  word RGB;
} 
Player1, Player2;

int newRow = 0;

boolean Player = 0, Host = 0;
byte gameMode = 0;

char PlayerMsg[20];
char WinnerMsg[20];
char ScoreMsg[25];
char oldchip = -1;

byte MAP[7][9], MapEmpty[7][9];
byte WINNER = 0;
boolean MapFull[9];

byte Remaining;

void setup()
{
  Serial1.begin(9600);
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.setFont(BigFont);
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_LOW);
  myTFT.ExtSetup();
  
  MainScr();
}

void MainScr()
{
  Player1.Score = 0;
  Player2.Score = 0;
  WINNER = 0;
  StartScr();
  set_Gamemode();
  StartScr();
  FP_Choose_Color();
  StartScr();
  drawJig();
  emptyJig();
}

void loop()
{
  myGLCD.setColor(YELLOW);
  myGLCD.setFont(BigFont);

  sprintf(PlayerMsg, "Player %d's turn", Player + 1);
  myGLCD.print(PlayerMsg, CENTER, 20);

  if (Remaining != 0 && WINNER == 0)
  {
    switch (gameMode)
    {
      case 0:
        PlayComputer();
        break;
      case 1:
        PlayPerson();
        break;
      case 2:
        Bluetooth_Play();
        break;
    }

    if (Player1.Score != Player1.LastScore || Player2.Score != Player2.LastScore)
    {
      sprintf(ScoreMsg, "P1:%d        P2:%d", Player1.Score, Player2.Score);
      myGLCD.print(ScoreMsg, CENTER, 220);
      Player1.LastScore = Player1.Score;
      Player2.LastScore = Player2.Score;
    }
  }
  else
  {
    delay(1000);
    StartScr();
    if (WINNER)
    {
      sprintf(WinnerMsg, "Player %d has won!", WINNER);
      myGLCD.print(WinnerMsg, CENTER, 100);

      if (WINNER == 1) Player1.Score++;
      else Player2.Score++;

      PlayAgain(true);
      //showMap();
    }
    else
    {
      myGLCD.print("Both Players Lost", CENTER, 100);
      PlayAgain(true);
    }
  }
}

void StartScr()
{
  myGLCD.fillScr(BLUE);
  myGLCD.setColor(YELLOW);
  myGLCD.setBackColor(BLUE);
  myGLCD.print("CONNECT FOUR", CENTER, 5);
}

void set_Gamemode()
{
  myGLCD.setColor(LIGHT_BLUE);
  myGLCD.setBackColor(LIGHT_BLUE);

  myGLCD.fillRoundRect(60, 70, 260, 100);
  myGLCD.fillRoundRect(60, 130, 260, 160);
  myGLCD.fillRoundRect(60, 190, 260, 220);
  while ( true )
  {
    if (myTFT.TextButton("One Player", Big, 60, 70, 260, 100, YELLOW))
    {
      gameMode = 0;
      break;
    }
    else if (myTFT.TextButton("Two Players", Big, 60, 130, 260, 160, YELLOW))
    {
      gameMode = 1;
      break;
    }
    else if (myTFT.TextButton("BlueTooth", Big, 60, 190, 260, 220, YELLOW))
    {
      gameMode = 2;
      SetHost();
      break;
    }
  }
}

void SetHost()
{
  StartScr();
  myGLCD.setColor(GREEN);
  myGLCD.setBackColor(BLUE);
  myGLCD.print("Host or Guest", CENTER, 50);

  myGLCD.setColor(LIGHT_BLUE);
  myGLCD.setBackColor(LIGHT_BLUE);

  myGLCD.fillRoundRect(120, 90, 200, 120);
  myGLCD.fillRoundRect(120, 150, 200, 180);
  while ( true )
  {
    if (myTFT.TextButton("Host", Big, 120, 90, 200, 120, YELLOW))
    {
      Player = 0;
      Host = !Player;
      break;
    }
    else if (myTFT.TextButton("Guest", Big, 120, 150, 200, 180, YELLOW))
    {
      Player = 1;
      Host = !Player;
      break;
    }
  }
  StartScr();
  if (!Player)
    myGLCD.print("You are Player 1", CENTER, 120);
  else
    myGLCD.print("You are Player 2", CENTER, 120);

  Player = 0; // Player 1 goes first.
  delay(2000);
}

void FP_Choose_Color()
{
  if (gameMode != 2)
  {
    myGLCD.setColor(GREEN);
    myGLCD.setBackColor(BLUE);
    myGLCD.print("P1 Choose Color", CENTER, 80);

    myGLCD.setColor(WHITE);
    myGLCD.fillCircle(100, 130, 30);
    myGLCD.setColor(BLACK);
    myGLCD.fillCircle(100, 130, 30 - 3);

    myGLCD.setColor(WHITE);
    myGLCD.fillCircle(220, 130, 30);
    myGLCD.setColor(RED);
    myGLCD.fillCircle(220, 130, 30 - 3);
    while ( true )
    {
      if (myTFT.TouchCircle(100, 130, 30))
      {
        PlayerOneColor(BLACK);
        PlayerTwoColor(RED);
        break;
      }
      else if (myTFT.TouchCircle(220, 130, 30))
      {
        PlayerOneColor(RED);
        PlayerTwoColor(BLACK);
        break;
      }
    }
  }
  else
  {
    if (Host == 1)
    {
      myGLCD.setColor(GREEN);
      myGLCD.setBackColor(BLUE);
      myGLCD.print("P1 Choose Color", CENTER, 80);

      myGLCD.setColor(WHITE);
      myGLCD.fillCircle(100, 130, 30);
      myGLCD.setColor(BLACK);
      myGLCD.fillCircle(100, 130, 30 - 3);

      myGLCD.setColor(WHITE);
      myGLCD.fillCircle(220, 130, 30);
      myGLCD.setColor(RED);
      myGLCD.fillCircle(220, 130, 30 - 3);
      while ( true )
      {
        if (myTFT.TouchCircle(100, 130, 30))
        {
          PlayerOneColor(BLACK); // 0
          PlayerTwoColor(RED); // 1
          Serial1.print('1');
          break;
        }
        else if (myTFT.TouchCircle(220, 130, 30))
        {
          PlayerOneColor(RED);
          PlayerTwoColor(BLACK);
          Serial1.print('0');
          break;
        }
      }
    }
    else
    {
      myGLCD.print("Waiting for Host", CENTER, 80);
      while (1)
      {
        if (Serial1.available() > 0)
        {
          boolean color = Serial1.read() - '0';
          if (color == 0)
          {
            PlayerTwoColor(BLACK);
            myGLCD.print("You are Black", CENTER, 120);
            break;
          }
          else
          {
            PlayerTwoColor(RED);
            myGLCD.print("You are Red", CENTER, 120);
            break;
          }
        }
      }
    }
  }
}

void PlayerOneColor( word rgb)
{
  Player1.RGB = rgb;
}

void PlayerTwoColor( word rgb)
{
  Player2.RGB = rgb;
}

void drawJig()
{
  myGLCD.setColor(YELLOW);
  myGLCD.fillRect(40, 50, 280, 200);

  myGLCD.setColor(LIGHT_BLUE);
  myGLCD.fillRect(35, 45, 40, 220);

  myGLCD.setColor(LIGHT_BLUE);
  myGLCD.fillRect(280, 45, 285, 220);

  myGLCD.setColor(LIGHT_BLUE);
  myGLCD.fillRect(15, 215, 305, 220);
}

void emptyJig()
{
  Player1.LastScore = 255;
  Player2.LastScore = 255;
  for (byte col = 0; col < 9; col++)
  {
    for (byte row = 0; row < 7; row++)
    {
      myGLCD.setColor(BLUE);
      myGLCD.fillCircle(60 + col * 25 , 60 + row * 20, 8);
      MAP[row][col] = 0;
      MapFull[col] = 0;
    }
  }
  WINNER = 0;
  Remaining = (9 * 7);
  delay(1);
}

#if 0
void Debug(int X, int Y)
{
  myGLCD.setBackColor(0, 0, 0);//background of text is black
  myGLCD.setColor(255,255,255); // test color is white

  myGLCD.print("X: ", 0,0, 0);
  if(X < 100){
    myGLCD.print("  ", 29,0, 0);
    myGLCD.printNumI(X, 24, 0);
  }
  else myGLCD.printNumI(X, 24, 0);

  myGLCD.print("Y: ", 55,0,0);
  if(Y < 100){
    myGLCD.print("  ", 84,0, 0); 
    myGLCD.printNumI(Y, 79, 0);
  }
  else myGLCD.printNumI(Y, 79, 0);
}
#endif

void PlayPerson()//
{
  for (int col = 0; col < 9; col++)
  {
    if (myTFT.TouchButton(45 + col * 25, 50, 72 + col * 25, 195))
    {
      newRow = CheckMap(col);
      if (newRow >= 0)
      {
        if (!MapFull[col])
        {
          Remaining--;
          addToMap(col, newRow, Player);
          dropChipAnimation(col, newRow);
          //myGLCD.fillCircle(60 + col*25, 60 + newRow*20, 8);
          if (newRow == 0) MapFull[col] = true;
          if (!ScanHorizontal() && !ScanVertical() && !ScanDiag())
            Player = !Player;
          else
          {
            WINNER = Player + 1;
            break;
          }
        }
        else
        {
          myGLCD.setFont(SmallFont);
          myGLCD.setColor(RED);
          myGLCD.print("Pick another column", CENTER, 37);
          delay(500);
          myGLCD.print("                   ", CENTER, 37);
        }
      }
    }
  }
}

void PlayComputer()//
{
  int newRow;

  for (int col = 1; col < 10; col++)
  {
    if ( Player ? col = SmartAIResponse() : myTFT.TouchButton(45 + (col - 1) * 25, 50, 72 + (col - 1) * 25, 195)) // Get col
    {
      if (!MapFull[col - 1])
      {
        Remaining--;
        newRow = CheckMap(col - 1);
        if (newRow >= 0)
        {
          addToMap(col - 1, (newRow), Player);
          dropChipAnimation(col - 1, newRow); // col - 1!!

          if (newRow == 0) MapFull[col - 1] = true;
          if (!ScanHorizontal() && !ScanVertical() && !ScanDiag())
            Player = !Player;
          else
          {
            WINNER = Player + 1;
            break;
          }
          delay(500);
        }
      }
      else
      {
        if (!Player) //Human
        {
          myGLCD.setFont(SmallFont);
          myGLCD.setColor(RED);
          myGLCD.print("Pick another column", CENTER, 37);
          delay(500);
          myGLCD.print("                   ", CENTER, 37);
        }
        else col = random(random(0, 4), random(5, 9));
      }
    }
  }
}

void Bluetooth_Play()
{
  boolean PLAY = false;
  int col;
  if (Player == Host) // guest
  {
    if (Serial1.available() > 0)
    {
      col = Serial1.read() - '0';
      newRow = CheckMap(col);
      PLAY = true;
    }
    else
    {
      myGLCD.setFont(SmallFont);
      myGLCD.print("Waiting", CENTER, 37);
      myGLCD.setFont(BigFont);
    }
  }
  else
  {
    myGLCD.setFont(SmallFont);
    myGLCD.print("       ", CENTER, 37);
    myGLCD.setFont(BigFont);

    for (col = 0; col < 9; col++)
    {
      if (myTFT.TouchButton(45 + col * 25, 50, 72 + col * 25, 195))
      {
        newRow = CheckMap(col);
        Serial1.print(col);
        PLAY = true;
        break;
      }
    }
  }

  if (PLAY)
  {
    if (!MapFull[col])
    {
      Remaining--;
      addToMap(col, (newRow), Player);
      dropChipAnimation(col, newRow);
      if (newRow == 0) MapFull[col] = true;
      if (!ScanHorizontal() && !ScanVertical() && !ScanDiag())
        Player = !Player;
      else
        WINNER = Player + 1;
    }
    else
    {
      myGLCD.setFont(SmallFont);
      myGLCD.setColor(RED);
      myGLCD.print("Pick another column", CENTER, 37);
      delay(500);
      myGLCD.print("                   ", CENTER, 37);
    }
    PLAY = false;
  }
}

int SmartAIResponse()
{
  byte P1countH = 0, P2countH = 0, HC = 0, SC = 0;
  byte P1countV = 0, P2countV = 0, VC = 0;
  byte P1countDdown = 0, P2countDdown = 0, P1countDup = 0, P2countDup = 0;
  byte EmptySpace = 0;

  //horizontal
  for ( byte HR = 0; HR < 7; HR++)
  {
    for ( HC = 0; HC < 6; HC++)
    {
      for ( SC = 0; SC < 4; SC++)
      {
        if (MAP[HR][HC + SC] == 1)
        {
          P1countH++;
          //myGLCD.printNumI(P1countH, 300,0);
          MapEmpty[HR][HC + SC] = 0;
        }
        else if (MAP[HR][HC + SC] == 2)
        {
          P2countH++;
          //myGLCD.printNumI(P2countH, 300,20);
          P1countH = 0;
          MapEmpty[HR][HC + SC] = 0;
        }
        else
        {
          EmptySpace++;
          MapEmpty[HR][HC + SC] = 1;
        }
      }

      if (EmptySpace)
      {
        if (P1countH > 2 || P2countH > 2)
        {
          for (byte Zeros = 0; Zeros < 4; Zeros++)
          {
            if (MapEmpty[HR][HC + Zeros])
            {
              int tmp1;
              if (MapEmpty[HR][HC + Zeros] && MapEmpty[HR - 1][HC + Zeros] && (HR - 1) == 0)
                return random(random(1, 4), random(5, 10));
              if (P1countH > P2countH)
                tmp1 = CheckMap(HC + (P1countH));
              else
                tmp1 = CheckMap(HC + (P2countH));
              //myGLCD.printNumI(HC + Zeros, 0,0);
              if (tmp1 != 0)
              {
                //myGLCD.printNumI(HC + Zeros, 0,0);
                return (HC + (Zeros + 1));
              }
              else break;//return random(random(0,4), random(5,9));
            }
          }
        }
        EmptySpace = 0;
      }
      P1countH = 0;
      P2countH = 0;
    }
  }

  //vertical
  for (  byte VR = 0; VR < 4; VR++)
  {
    for (  byte VC = 0; VC < 9; VC++)
    {
      for (  byte SR = 0; SR < 4; SR++)
      {
        if (MAP[VR + SR][VC] == 1)
        {
          P1countV++;
          MapEmpty[VR + SR][VC] = 0;
        }
        else if (MAP[VR + SR][VC] == 2)
        {
          P2countV++;
          P1countV = 0;
          MapEmpty[VR + SR][VC] = 0;
        }
        else
        {
          EmptySpace++;
          MapEmpty[VR + SR][VC] = 1;
        }
      }
      if (EmptySpace)
      {
        if (P1countV > 2 || P2countV > 2)
        {
          for (  byte Zeros = 0; Zeros < 4; Zeros++)
          {
            if (MapEmpty[VR + Zeros][VC])
            {
              if (CheckMap(VC) != 0)
              {
                //myGLCD.printNumI(VC, 0,0);
                return (VC + (Zeros + 1));
              }
              else break;//return random(1,10);
            }
          }
        }
        EmptySpace = 0;
      }
      P1countV = 0;
      P2countV = 0;
    }
  }

  //Diagonal
  for (byte R = 0; R < 4; R++)
  {
    for (byte C = 0; C < 6; C++)
    {
      for (byte SR = 0; SR < 4; SR++)
      {
        if (MAP[R + SR][C + SR] == 1) //diagonal down
        {
          //myGLCD.printNumI(P1countDdown, 300,0);
          P1countDdown++;
          MapEmpty[R + SR][C + SR] = 0;
        }
        else if (MAP[R + SR][C + SR] == 2)
        {
          //myGLCD.printNumI(P2countDdown, 300,20);
          P2countDdown++;
          P1countDdown = 0;
          MapEmpty[R + SR][C + SR] = 0;
        }
        else
        {
          EmptySpace++;
          MapEmpty[R + SR][C + SR] = 1;
        }

        if (MAP[(3 - R) + SR][C + (3 - SR)] == 1) //diagonal up
        {
          //myGLCD.printNumI(P1countDup, 300,40);
          P1countDup++;
          MapEmpty[(3 - R) + SR][C + (3 - SR)] = 0;
        }
        else if (MAP[(3 - R) + SR][C + (3 - SR)] == 2)
        {
          //myGLCD.printNumI(P2countDup, 300,60);
          P2countDup++;
          P1countDup = 0;
          MapEmpty[(3 - R) + SR][C + (3 - SR)] = 0;
        }
        else
        {
          EmptySpace++;
          MapEmpty[(3 - R) + SR][C + (3 - SR)] = 1;
        }
        if (EmptySpace)
        {
          if ((P1countDdown > 2 || P2countDdown > 2) || (P1countDup > 2 || P2countDup > 2))
          {
            for (byte Zeros = 0; Zeros < 4; Zeros++)
            {
              if (MapEmpty[(3 - R) + SR][C + (3 - SR)]) // up
              {
                if (CheckMap(C + (3 - SR)) != 0)
                {
                  //myGLCD.printNumI(C, 0,0);
                  return (C + (3 - SR) + (Zeros + 1));
                }
                //return random(1,10);
              }
              else
              {
                if (MapEmpty[R + SR][C + SR]) // down
                {
                  if (CheckMap(C + SR) != 0)
                  {
                    //myGLCD.printNumI(C + SR, 0,20);
                    return ((C + SR) + (Zeros + 1));
                  }
                  //else break;
                }
              }
            }
          }
          EmptySpace = 0;
        }
      }
      P1countDdown = 0;
      P2countDdown = 0;
      P1countDup = 0;
      P2countDup = 0;
    }
  }
  return random(1, 10);
}

void dropChipAnimation(byte col, byte newRow)
{
  for (char chip = 0, oldchip = 0; chip <= newRow; chip++)
  {
    if (oldchip != -1)
    {
      myGLCD.setColor(BLUE);
      myGLCD.fillCircle(60 + (col) * 25, 60 + oldchip * 20, 8);
    }
    if (!Player)
      myGLCD.setColor(Player1.RGB);
    else
      myGLCD.setColor(Player2.RGB);
    oldchip = chip;
    myGLCD.fillCircle(60 + (col) * 25, 60 + chip * 20, 8);
  }
}


int CheckMap(int Col)
{
  for (int Row = 6; Row >= 0; Row--)
  {
    if (MAP[Row][Col] == 0)
      return Row;
  }
  return -1;
}

void addToMap(byte Col, byte Row, boolean Player)
{
  MAP[Row][Col] = byte(Player + 1);
  MapEmpty[Row][Col] = 0;
}

boolean ScanHorizontal()
{
  myGLCD.setColor(WHITE);
  byte tmpcountH = 1, SC = 0;
  for ( byte R = 0; R < 7; R++)
  {
    for ( byte C = 0; C < 6; C++)
    {
      for (SC = 0; SC < 4; SC++)
      {
        tmpcountH *= MAP[R][C + SC];
      }
      if (tmpcountH == 1)
      {
        myGLCD.fillRect(60 + C * 25, 59 + R * 20, 60 + (C + (SC - 1)) * 25, 61 + R * 20);
        WINNER = 1;
        return true;
      }
      else if (tmpcountH == 16)
      {
        myGLCD.fillRect(60 + C * 25, 59 + R * 20, 60 + (C + (SC - 1)) * 25, 61 + R * 20);
        WINNER = 2;
        return true;
      }
      else tmpcountH = 1;
    }
  }
  return false;
}

boolean ScanVertical()
{
  myGLCD.setColor(WHITE);
  byte tmpcountV = 1, SR = 0;
  for (  byte R = 0; R < 4; R++)
  {
    for (  byte C = 0; C < 9; C++)
    {
      for (SR = 0; SR < 4; SR++)
      {
        tmpcountV *= MAP[R + SR][C];
      }
      if (tmpcountV == 1)
      {
        myGLCD.fillRect(59 + C * 25, 60 + R * 20, 61 + C * 25, 60 + (R + (SR - 1)) * 20);
        WINNER = 1;
        return true;
      }
      else if (tmpcountV == 16)
      {
        myGLCD.fillRect(59 + C * 25, 60 + R * 20, 61 + C * 25, 60 + (R + (SR - 1)) * 20);
        WINNER = 2;
        return true;
      }
      else tmpcountV = 1;
    }
  }
  return false;
}

boolean ScanDiag()
{
  myGLCD.setColor(WHITE);
  byte tmpcountD = 1, tmpcountU = 1 , SR = 0;
  for (  byte R = 0; R < 4; R++)
  {
    for (  byte C = 0; C < 6; C++)
    {
      for (SR = 0; SR < 4; SR++)
      {
        tmpcountD *= MAP[R + SR][C + SR];
        tmpcountU *= MAP[(3 - R) + SR][C + (3 - SR)];
      }
      if (tmpcountD == 1)
      {
        myGLCD.drawLine(60 + C * 25, 60 + R * 20, 60 + (C + (SR - 1)) * 25, 60 + (R + (SR - 1)) * 20);
        WINNER = 1;
        return true;
      }
      else if (tmpcountU == 1)
      {
        myGLCD.drawLine(60 + C * 25, 60 + (6 - R) * 20, 60 + (C + (SR - 1)) * 25, 60 + ((6 - R) + (SR - 7)) * 20);
        WINNER = 1;
        return true;
      }
      else if (tmpcountD == 16)
      {
        myGLCD.drawLine(60 + C * 25, 60 + R * 20, 60 + (C + (SR - 1)) * 25, 60 + (R + (SR - 1)) * 20);
        WINNER = 2;
        return true;
      }
      else if (tmpcountU == 16)
      {
        myGLCD.drawLine(60 + C * 25, 60 + (6 - R) * 20, 60 + (C + (SR - 1)) * 25, 60 + ((6 - R) + (SR - 7)) * 20);
        WINNER = 2;
        return true;
      }
      else
      {
        tmpcountD = 1;
        tmpcountU = 1;
      }
    }
  }
  return false;
}

void showMap()
{
  StartScr();
  for (byte row = 0; row < 7; row++)
  {
    for (byte col = 0; col < 9; col++)
    {
      myGLCD.printNumI(MAP[row][col], (30 + 30 * col), (30 + 30 * row));
    }
  }
  while (1);
}

void makeButton()
{
  myGLCD.drawRect(80, 159, 145, 189); //yes
  myGLCD.drawRect(170, 159, 235, 189); //no
}

void PlayAgain(boolean playAgain)
{
  while (playAgain)
  {
    myGLCD.setColor(YELLOW);
    myGLCD.print("Play Again?", CENTER, 131);
    makeButton();
    if (myTFT.TextButton("Yes", Big, 80, 159, 145, 189, GREEN))
    {
      StartScr();
      drawJig();
      emptyJig();
      playAgain = false;
      break;
    }

    else if (myTFT.TextButton("No", Big, 170, 159, 235, 189, RED))
    {
      myGLCD.fillScr(BLUE);
      myGLCD.setColor(YELLOW);
      myGLCD.print("Thanks for playing", CENTER, 131);
      delay(1000);
      playAgain = false;
      MainScr();
    }
  }
}
