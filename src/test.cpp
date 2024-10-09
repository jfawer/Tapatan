//The DIY Life
//16 June 2021
//Michael Klements
#include <Arduino.h>

int gameBoard [3][3] = {{0,0,0},{0,0,0},{0,0,0}};           //Array to store the state of each sqaure on the board, 3 columns and 3 rows
int greenLEDs [3][3] = {{10,9,8},{4,3,2},{14,15,16}};       //Arrays to store pin numbers for LEDs and input buttons
int blueLEDs [3][3] = {{13,12,11},{7,6,5},{42,40,38}};
int buttons [3][3] = {{53,43,35},{51,41,33},{49,39,31}};
int startButton = 50;
int statusLED [3] = {19,18,17};
int turn = 1;                                               //Counter to keep track of which player's turn it is
int mode = 0;                                               //Variable to store game mode - 0 is hard (red), 1 is easy (green), 2 is 2 player (blue)
bool firstMove = true;                                      //Tracks when the AI is playing it's first move, then doesn't check through all possible moves
int turnCount = 0;                                          //Tracks number of plays for easy mode, first two plays are random

void setup() 
{
  Serial.begin(9600);
  randomSeed(analogRead(0));                                //Randomly choose a starting point for the random function, otherwise player turn pattern is predictable
  turn = random(1,3);                                       //Randomly generate the starting player's turn
  for (int i=0; i<=2 ; i++)                                 //Define the pin modes for buttons and LEDs
  {
    for (int j=0; j<=2; j++)
    {
      pinMode(greenLEDs[i][j], OUTPUT);
      pinMode(blueLEDs[i][j], OUTPUT);
      pinMode(buttons[i][j], INPUT);
    }
    pinMode(statusLED[i], OUTPUT);
  }
  pinMode(startButton, INPUT);
  updateDisplay ();                                          //Clear display board
  selectGameMode ();
  startGameAni ();
  updateDisplay ();                                          //Reset player turn indicator
}

void loop() 
{
  if (turn == 1)                                             //If it is player 1's turn to play
  {
    boolean playerInput = false;
    while (!playerInput)                                     //Loop until player 1 has made their play
    {
      for (int i=0; i<=2 ; i++)                              //Check the state of all buttons to see if the player has chosen a position
        for (int j=0; j<=2; j++)
          if (digitalRead(buttons[i][j]) == HIGH)            //If a button is pushed
          {
            if (gameBoard [i][j] == 0)                       //Check that that position is unoccupied
            {
              gameBoard[i][j] = 1;                           //Add the position to the gameboard and change the player's turn
              turn = 2;
              playerInput = true;
            }
          }
    }
    displayBoard ();
    Serial.println("");
  }
  else                                                       //Else it it player 2's turn
  {
    if (mode == 0)                                           //Mode 0 - AI's turn to play a strategic move
    {
      delay(300);                                            //Delay so that AI's response isn't immediate
      aITurn ();
    }
    else if (mode == 1)                                      //Mode 1 - AI's turn to play a random move
    {
      delay(500);                                            //Delay so that AI's response isn't immediate
      randomTurn();                                          //Generate a random move
    }
    else                                                     //Mode 2 - Two player mode, second play to make their move
    {
      boolean playerInput = false;
      while (!playerInput)                                   //Loop until player 2 has made their move
      {
        for (int i=0; i<=2 ; i++)                            //Check the state of all buttons to see if the player has chosen a position
          for (int j=0; j<=2; j++)
            if (digitalRead(buttons[i][j]) == HIGH)          //If a button is pushed
            {
              if (gameBoard [i][j] == 0)                     //Check that that position is unoccupied
              {
                gameBoard[i][j] = 2;                         //Add the position to the gameboard and change the player's turn
                turn = 1;
                playerInput = true;
              }
            }
      }
    }
    displayBoard ();
    Serial.println("");
  }
  updateDisplay ();                                          //Update the LEDs to reflect the latest position played
  int winner = evaluate (gameBoard);                         //Check if there has been a winner
  if (winner == 10 || winner == -10)                         //If the score is 10 or -10 then a player has won
  {
    flashWin () ;                                            //Flash the winning player
    resetBoard ();
    startGameAni ();
    updateDisplay ();
  }
  if (!checkMovesLeft(gameBoard))                            //If there are no more spaces on the board and no-one has won then it is a draw
  {
    flashDraw();
    resetBoard ();
    startGameAni ();
    updateDisplay ();
  }
}

void selectGameMode ()                                //Function to select the game mode, game continues in this mode until reset
{
  bool confirmed = false;
  while (!confirmed)                                  //Keep looping until a mode is selected
  {
    if (digitalRead(buttons[0][1]) == HIGH)           //Increment the game mode up or loop back down to mode 0
    {
      if (mode < 2)
        mode++;
      else
        mode=0;
    }
    if (digitalRead(buttons[2][1]) == HIGH)           //Increment the game mode down or loop back up to mode 2
    {
      if (mode > 0)
        mode--;
      else
        mode=2;
    }
    if (digitalRead(startButton) == HIGH)             //When start game button is pressed, mode is confirmed
      confirmed = true;
    if (mode == 0)                                    //Set status LED to show which game mode is selected
    {
      digitalWrite(statusLED [0],HIGH);
      digitalWrite(statusLED [1],LOW);
      digitalWrite(statusLED [2],LOW);
    }
    else if (mode == 1)
    {
      digitalWrite(statusLED [0],LOW);
      digitalWrite(statusLED [1],HIGH);
      digitalWrite(statusLED [2],LOW);
    }
    else
    {
      digitalWrite(statusLED [0],LOW);
      digitalWrite(statusLED [1],LOW);
      digitalWrite(statusLED [2],HIGH);
    }
    delay(200);
  }
}

void updateDisplay ()                                 //Function to update display LEDs to reflect the game positions
{
  for (int i=0; i<=2 ; i++)                           //Loop through all 9 board LEDs - i row, j column
  {
    for (int j=0; j<=2; j++)
    {
      if (gameBoard[i][j] == 1)                       //Turn on green LED if 1
        digitalWrite(greenLEDs[i][j],HIGH);
      else if (gameBoard[i][j] == 2)
        digitalWrite(blueLEDs[i][j],HIGH);            //Turn on blue LED if 2
      else
      {
        digitalWrite(greenLEDs[i][j],LOW);            //Turn off both LEDs if 0
        digitalWrite(blueLEDs[i][j],LOW);
      }
    }
  }
  if(turn == 1)                                       //Set status LED to show player 1's turn
  {
    digitalWrite(statusLED[0],LOW);
    digitalWrite(statusLED[1],HIGH);
    digitalWrite(statusLED[2],LOW);
  }
  else if (turn == 2)                                 //Set status LED to show player 2's turn
  {
    digitalWrite(statusLED[0],LOW);
    digitalWrite(statusLED[1],LOW);
    digitalWrite(statusLED[2],HIGH);
  }
}

void turnOffAll ()                                    //Function to turn off all gameboard LEDs
{
  for (int i=0; i<=2 ; i++)
    for (int j=0; j<=2; j++)
    {
      digitalWrite(greenLEDs[i][j],LOW);              //Turn off both LEDs
      digitalWrite(blueLEDs[i][j],LOW);
    }
}

void flashWin ()                                                                         //Function to flash the wining line
{
  for (int i=0; i<=2 ; i++)                                                             //Loop through all rows and check for a winning line
    if(gameBoard[i][0] == gameBoard[i][1] && gameBoard[i][1] == gameBoard[i][2])
    {
      if (gameBoard[i][0] == 1)
        for (int k=0; k<=3 ; k++)
        {
          digitalWrite(greenLEDs[i][0],LOW);
          digitalWrite(greenLEDs[i][1],LOW);
          digitalWrite(greenLEDs[i][2],LOW);
          delay (500);
          digitalWrite(greenLEDs[i][0],HIGH);
          digitalWrite(greenLEDs[i][1],HIGH);
          digitalWrite(greenLEDs[i][2],HIGH);
          delay (500);
        }
      else if (gameBoard[i][0] == 2)
        for (int k=0; k<=3 ; k++)
        {
          digitalWrite(blueLEDs[i][0],LOW);
          digitalWrite(blueLEDs[i][1],LOW);
          digitalWrite(blueLEDs[i][2],LOW);
          delay (500);
          digitalWrite(blueLEDs[i][0],HIGH);
          digitalWrite(blueLEDs[i][1],HIGH);
          digitalWrite(blueLEDs[i][2],HIGH);
          delay (500);
        }
    }

  for (int j=0; j<=2; j++)                                                              //Loop through all columns and check for a winning line
    if(gameBoard[0][j] == gameBoard[1][j] && gameBoard[1][j] == gameBoard[2][j])
    {
      if (gameBoard[0][j] == 1)
        for (int k=0; k<=3 ; k++)
        {
          digitalWrite(greenLEDs[0][j],LOW);
          digitalWrite(greenLEDs[1][j],LOW);
          digitalWrite(greenLEDs[2][j],LOW);
          delay (500);
          digitalWrite(greenLEDs[0][j],HIGH);
          digitalWrite(greenLEDs[1][j],HIGH);
          digitalWrite(greenLEDs[2][j],HIGH);
          delay (500);
        }
      else if (gameBoard[0][j] == 2)
        for (int k=0; k<=3 ; k++)
        {
          digitalWrite(blueLEDs[0][j],LOW);
          digitalWrite(blueLEDs[1][j],LOW);
          digitalWrite(blueLEDs[2][j],LOW);
          delay (500);
          digitalWrite(blueLEDs[0][j],HIGH);
          digitalWrite(blueLEDs[1][j],HIGH);
          digitalWrite(blueLEDs[2][j],HIGH);
          delay (500);
        }
    }

  if(gameBoard[0][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][2])          //Check first diagonal for a winning line
  {
    if (gameBoard[0][0] == 1)
    {
      for (int k=0; k<=3 ; k++)
      {
        digitalWrite(greenLEDs[0][0],LOW);
        digitalWrite(greenLEDs[1][1],LOW);
        digitalWrite(greenLEDs[2][2],LOW);
        delay (500);
        digitalWrite(greenLEDs[0][0],HIGH);
        digitalWrite(greenLEDs[1][1],HIGH);
        digitalWrite(greenLEDs[2][2],HIGH);
        delay (500);
      }
    }
    else if (gameBoard[0][0] == 2)
    {
      for (int k=0; k<=3 ; k++)
      {
        digitalWrite(blueLEDs[0][0],LOW);
        digitalWrite(blueLEDs[1][1],LOW);
        digitalWrite(blueLEDs[2][2],LOW);
        delay (500);
        digitalWrite(blueLEDs[0][0],HIGH);
        digitalWrite(blueLEDs[1][1],HIGH);
        digitalWrite(blueLEDs[2][2],HIGH);
        delay (500);
      }
    }
  }

  if(gameBoard[0][2] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][0])          //Check second diagonal for a winning line
  {
    if (gameBoard[0][2] == 1)
    {
      for (int k=0; k<=3 ; k++)
      {
        digitalWrite(greenLEDs[0][2],LOW);
        digitalWrite(greenLEDs[1][1],LOW);
        digitalWrite(greenLEDs[2][0],LOW);
        delay (500);
        digitalWrite(greenLEDs[0][2],HIGH);
        digitalWrite(greenLEDs[1][1],HIGH);
        digitalWrite(greenLEDs[2][0],HIGH);
        delay (500);
      }
    }
    else if (gameBoard[0][2] == 2)
    {
      for (int k=0; k<=3 ; k++)
      {
        digitalWrite(blueLEDs[0][2],LOW);
        digitalWrite(blueLEDs[1][1],LOW);
        digitalWrite(blueLEDs[2][0],LOW);
        delay (500);
        digitalWrite(blueLEDs[0][2],HIGH);
        digitalWrite(blueLEDs[1][1],HIGH);
        digitalWrite(blueLEDs[2][0],HIGH);
        delay (500);
      }
    }
  }

  turnOffAll ();
  if(turn == 1)                                             //Change the turn counter to the winner
    turn = 2;
  else
    turn = 1;
  for (int i=0; i<=2 ; i++)                                 //Set whole board to winner colour
    for (int j=0; j<=2; j++)
      gameBoard [i][j] = turn;
  updateDisplay ();
  delay (1000);                                             //Show winning colour for 1 second
  resetBoard ();                                            //Reset gameboard
}

void flashDraw ()
{
  for (int k=0; k<=3 ; k++)
  {
    turnOffAll ();
    delay(500);
    updateDisplay ();
    delay(500);
  }
}

void startGameAni ()                                  //Simple start game animation
{
  turnOffAll ();
  for (int i=2; i>=0; i--)
  {
    digitalWrite(greenLEDs[i][0],HIGH);
    digitalWrite(greenLEDs[i][1],HIGH);
    digitalWrite(greenLEDs[i][2],HIGH);
    delay (150);
  }
  for (int i=2; i>=0; i--)
  {
    digitalWrite(greenLEDs[i][0],LOW);
    digitalWrite(greenLEDs[i][1],LOW);
    digitalWrite(greenLEDs[i][2],LOW);
    delay (150);
  }
}

bool checkMovesLeft (int gBoard[3][3])                //Function to check if there are still empty spaces on the board
{
  for (int i=0; i<=2 ; i++)                           //Loop through all 9 board positions - i row, j column
    for (int j=0; j<=2; j++)
      if (gBoard[i][j] == 0)
        return true;                                  //If an empty space is found, return true
  return false;
}

int evaluate (int gBoard[3][3])                                         //Function to evaluate the current board value
{
  for (int i=0; i<=2; i++)                                              //Loop through all rows and check for a win
  {
    if(gBoard[i][0] == gBoard[i][1] && gBoard[i][1] == gBoard[i][2])
    {
      if (gBoard[i][0] == 1)
        return 10;
      else if (gBoard[i][0] == 2)
        return -10;
    }
  }
  for (int j=0; j<=2; j++)                                              //Loop through all columns and check for a win
  {
    if(gBoard[0][j] == gBoard[1][j] && gBoard[1][j] == gBoard[2][j])
    {
      if (gBoard[0][j] == 1)
        return 10;
      else if (gBoard[0][j] == 2)
        return -10;
    }
  }
  if(gBoard[0][0] == gBoard[1][1] && gBoard[1][1] == gBoard[2][2])      //Check first diagonal for a win
  {
    if (gBoard[0][0] == 1)
      return 10;
    else if (gBoard[0][0] == 2)
      return -10;
  }
  if(gBoard[0][2] == gBoard[1][1] && gBoard[1][1] == gBoard[2][0])      //Check second diagonal for a win
  {
    if (gBoard[0][2] == 1)
      return 10;
    else if (gBoard[0][2] == 2)
      return -10;
  }
  return 0;
}

int minimax (int gBoard [3][3], int depth, bool isMax)            //Considers the possible plays and returns a value for the current board position
{
  int score = evaluate (gBoard);

  if (score == 10 || score == -10)                                //If a player has won the game
    return score;

  if (checkMovesLeft (gBoard) == false)                           //If there are no moves left then it must be a draw
    return 0;

  if (isMax)                                                      //If this is the maximizer's turn
  {
    int best = -1000;
    for (int i=0; i<=2; i++)                                      //Go through each cell
      for (int j=0; j<=2; j++)
        if (gBoard[i][j] == 0)                                    //If the space hasn't been played
        {
          gBoard[i][j] = 1;                                       //Play the move
          best = max (best, minimax(gBoard, depth+1, !isMax));
          gBoard[i][j] = 0;                                       //Undo the move
        }
    return best;
  }
  else                                                            //If this is the minimizer's turn
  {
    int best = 1000;
    for (int i=0; i<=2 ; i++)                                     //Go through each cell
      for (int j=0; j<=2; j++)
        if (gBoard[i][j] == 0)                                    //If the space hasn't been played
        {
          gBoard[i][j] = 2;                                       //Play the move
          best = min (best, minimax(gBoard, depth+1, !isMax));
          gBoard[i][j] = 0;                                       //Undo the move
        }
    return best;
  }
}

void aITurn ()
{
  int bestRow = 0;
  int bestCol = 0;
  if (firstMove)
  {
    bool played = false;                                       //Check for whether AI has found an unoccupied corner
    int temp = random(4);
    if (temp == 0)
    {
      if (gameBoard[0][0] == 0)
      {
        bestRow = 0;
        bestCol = 0;
        played = true;
      }
    }
    else if (temp == 1)
    {
      if (gameBoard[0][2] == 0)
      {
        bestRow = 0;
        bestCol = 2;
        played = true;
      }
    }
    else if (temp == 2)
    {
      if (gameBoard[2][0] == 0)
      {
        bestRow = 2;
        bestCol = 0;
        played = true;
      }
    }
    else
    {
      if (gameBoard[2][2] == 0)
      {
        bestRow = 2;
        bestCol = 2;
        played = true;
      }
    }
    if (!played)
    {
      bestRow = 1;
      bestCol = 1;
    }
    firstMove = false;
  }
  else
  {
    int bestVal = +1000;
    for (int i=0; i<=2; i++)                                    //Go through each cell
      for (int j=0; j<=2; j++)
        if (gameBoard[i][j] == 0)                               //If the space hasn't been played
        {
          gameBoard[i][j] = 2;                                  //Play the move
          displayBoard ();
          int moveVal = minimax(gameBoard, 0, true);
          gameBoard[i][j] = 0;                                  //Undo the move
          Serial.println(moveVal);                              //Display on serial monitor for debugging
          Serial.println("");
          if (moveVal < bestVal)
          {
            bestRow = i;
            bestCol = j;
            bestVal = moveVal;
          }
        }
  }
  gameBoard[bestRow][bestCol] = 2;                            //Once all combinations have been explored, play the best move
  turn = 1;                                                   //Set back to player's turn
}

void displayBoard ()                                          //Function to display the gameboard
{
  for (int i=0; i<=2; i++)                                    //Go through each cell
  {
    for (int j=0; j<=2; j++)
    {
      Serial.print(gameBoard[i][j]);                          //Display the cell value
      Serial.print(" ");
    }
    Serial.println("");
  }
}

void randomTurn ()
{
  bool played = false;                                         //Check to see that a move has been played
  if (turnCount < 2)
  {
    while (!played)
    {
      int randomPos = random(9);                               //Generate a random board position
      switch (randomPos)                                       //Check position is unoccupied then play the random position
      {
        case 1:
          if (gameBoard[0][0] == 0)
          {
            gameBoard[0][0] = 2;
            played = true;
          }
          break;
        case 2:
          if (gameBoard[0][1] == 0)
          {
            gameBoard[0][1] = 2;
            played = true;
          }
          break;
        case 3:
          if (gameBoard[0][2] == 0)
          {
            gameBoard[0][2] = 2;
            played = true;
          }
          break;
        case 4:
          if (gameBoard[1][0] == 0)
          {
            gameBoard[1][0] = 2;
            played = true;
          }
          break;
        case 5:
          if (gameBoard[1][1] == 0)
          {
            gameBoard[1][1] = 2;
            played = true;
          }
          break;
        case 6:
          if (gameBoard[1][2] == 0)
          {
            gameBoard[1][2] = 2;
            played = true;
          }
          break;
        case 7:
          if (gameBoard[2][0] == 0)
          {
            gameBoard[2][0] = 2;
            played = true;
          }
          break;
        case 8:
          if (gameBoard[2][1] == 0)
          {
            gameBoard[2][1] = 2;
            played = true;
          }
          break;
        case 9:
          if (gameBoard[2][2] == 0)
          {
            gameBoard[2][2] = 2;
            played = true;
          }
          break;
      }
    }
    turnCount++;
  }
  else
  {
    aITurn ();                                            //AI takes over after two turns so that it has a chance of winning and will complete "almost" lines
  }
  turn = 1;
}

void resetBoard ()                                        //Function to reset the gameboard for the next game
{
  turn = random(1,3);                                     //Reset turn counter
  firstMove = true;                                       //Reset first move
  turnCount = 0;                                          //Reset number of turns counter
  for (int i=0; i<=2; i++)                                //Clear play board
    for (int j=0; j<=2; j++)
    {
      gameBoard [i][j] = 0;
    }
  Serial.println("Board Reset");                          //Display results on serial monitor for debugging
  displayBoard ();
  Serial.print("Turn: ");
  Serial.print(turn);
}
