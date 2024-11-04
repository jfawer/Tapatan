#ifndef TICTACTOE_H
#define TICTACTOE_H

// Einbinden der Bibliotheken
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Einbinden der Header-Dateien
#include "struct.h"
#include "game_logic.h"
#include "display.h"
#include "input.h"

const int PlayerVsComputer = 1;                                         // Konstante für den Spielmodus Spieler gegen Computer
const int PlayerVsPlayer = 2;                                           // Konstante für den Spielmodus Spieler gegen Spieler
const int Player1 = 1;                                                  // Konstante für Spieler 1
const int Player2 = 2;                                                  // Konstante für Spieler 2
const int Computer = 2;                                                 // Konstante für den Computer
const int Einfach = 1;                                                     // Konstante für den Schwierigkeitsgrad Einfach
const int Mittel = 2;                                                   // Konstante für den Schwierigkeitsgrad Mittel
const int Schwer = 3;                                                     // Konstante für den Schwierigkeitsgrad Schwer

void TicTacToePlayerVsPlayer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  bool firstDisplay = true;
  bool turnOver = false;
  int turn = 0;

  while (true) {
    if (firstDisplay) {
      displayGameScreen(lcd, gameSettings, Board, currentPlayer);
      firstDisplay = false;
    }

    updateBoard(Board, potPins);
    turnOver = playerMove(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);

    if (turnOver) {
      turn++;
      int evaluation = evaluate(Board);
      if (evaluation == 10) {
        displayWinner(lcd, gameSettings, Player1);
        break;
      } else if (evaluation == -10) {
        displayWinner(lcd, gameSettings, Player2);
        break;
      } else if (!checkFieldsLeft(Board)) {
        displayDraw(lcd);
        break;
      }
      copyBoard(Board, BoardMemory);
      switchPlayer(currentPlayer);
      displayPlayer(lcd, gameSettings, currentPlayer);
      turnOver = false;
    }
  }
}


// Funktion für das Spiel Tic Tac Toe Spieler gegen Computer
void TicTacToePlayerVsComputer(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  bool firstDisplay = true;
  bool turnOver = false;
  int turn = 0;
  
  // Spiellogik für Tic Tac Toe
  while (true) {
    if (firstDisplay) {
      displayGameScreen(lcd, gameSettings, Board, currentPlayer);
      firstDisplay = false;
    }

    updateBoard(Board, potPins);

    if (currentPlayer == Player1) {                                                                               // Überprüfen, ob Spieler 1 am Zug ist       
      turnOver = playerMove(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                        // Spielerzug                                         
    } else if (currentPlayer == Computer) {
      // Computerzug
      int BoardDisplay[3][3];
      copyBoard(Board, BoardDisplay);

      switch (gameSettings.difficulty) {
        case Einfach:
          makeRandomMove(BoardDisplay);                                                                            // Zufälligen Zug für den Computer bestimmen
          break;
        case Mittel:
          if (turn < 3) {
            makeRandomMove(BoardDisplay);                                                                          // Zufälligen Zug für den Computer bestimmen
          } else {
            makeBestMove(BoardDisplay);                                                                         // Besten Zug für den Computer bestimmen
          }                                                                                                         // Besten Zug für den Computer bestimmen
          break;
        case Schwer:
          makeBestMove(BoardDisplay);                                                                           // Besten Zug für den Computer bestimmen
          break;
      }                                                                                                           // Besten Zug für den Computer bestimmen
      delay(400);
      displayGameScreen(lcd, gameSettings, BoardDisplay, currentPlayer);                                          // Spielfeld anzeigen
      waitUntilMoveIsUndone(Board, BoardDisplay, potPins);                                                        // Warten, bis der Computerzug gemacht wurde
      turnOver = true;                                                                                            // Zug beenden
    }

    if (turnOver) {
      turn++;
      int evaluation = evaluate(Board);
      if (evaluation == 10) {
        displayWinner(lcd, gameSettings, Player1);
        break;
      } else if (evaluation == -10) {
        displayWinner(lcd, gameSettings, Player2);
        break;
      } else if (!checkFieldsLeft(Board)) {
        displayDraw(lcd);
        break;
      }
      copyBoard(Board, BoardMemory);
      switchPlayer(currentPlayer);
      displayPlayer(lcd, gameSettings, currentPlayer);
      turnOver = false;
    }
  }
}

// Funktion für das Spiel Tic Tac Toe
void playTicTacToe(LiquidCrystal_I2C &lcd, GameSettings gameSettings, int Board [3][3], int BoardMemory[3][3], int currentPlayer, const int potPins[]) {
  // Spiellogik für Tic Tac Toe
  if (gameSettings.mode == PlayerVsComputer) {                                         
    TicTacToePlayerVsComputer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                     // Spieler gegen Computer
  } else if (gameSettings.mode == PlayerVsPlayer) {                                                     
    TicTacToePlayerVsPlayer(lcd, gameSettings, Board, BoardMemory, currentPlayer, potPins);                       // Spieler gegen Spieler
  }
}


#endif